// SPDX-License-Identifier: GPL-2.0-or-later
// Full readback/validity regression across alternating sizes and cache eviction.
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include "bbport_settings.h"
#include "video_core/renderer_vulkan/vk_instance.h"
#include "video_core/renderer_vulkan/vk_scheduler.h"
#include "video_core/renderer_vulkan/vk_object_motion.h"
#include "shader_recompiler/runtime_info.h"
#include <vk_mem_alloc.h>

using namespace Vulkan;
#ifdef _WIN32
extern "C" void runtime_restart() { std::abort(); }
#endif

namespace Vulkan {
struct ObjectMotionCacheTest {
    static size_t Retired(const ObjectMotion& motion) { return motion.retired_images.size(); }
    static u64 RetiredBytes(const ObjectMotion& motion) { return motion.retired_bytes; }
    static void Collect(ObjectMotion& motion) { motion.CollectRetiredImages(); }
    static bool Has(const ObjectMotion& motion, u32 w, u32 h) {
        if (motion.image && motion.image_width == w && motion.image_height == h) return true;
        return std::any_of(motion.cached_images.begin(), motion.cached_images.end(),
            [=](const auto& c) { return c.image && c.width == w && c.height == h; });
    }
    static void Limits(const ObjectMotion& motion) {
        if (Retired(motion) > ObjectMotion::MaxRetiredImages ||
            RetiredBytes(motion) > ObjectMotion::MaxRetiredBytes)
            throw std::runtime_error("retirement exceeds its count/VRAM limit");
    }
};
} // namespace Vulkan

void Require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

u32 Allocations(const Instance& instance) {
    VmaTotalStatistics stats{};
    vmaCalculateStatistics(instance.GetAllocator(), &stats);
    return stats.total.statistics.allocationCount;
}

void AttachSize(ObjectMotion& motion, Scheduler& scheduler, u32 w, u32 h) {
    RenderState state{};
    state.width = w; state.height = h; state.num_layers = 1;
    motion.Attach(state, w, h);
    scheduler.BeginRendering(state);
    scheduler.EndRendering();
}

// A real queued submission stays blocked on a host-signalled timeline semaphore. The
// watchdog releases it even if a regression blocks the CPU, so failures cannot hang the GPU.
class Gate {
    vk::Device device;
    vk::UniqueSemaphore semaphore;
    std::atomic<bool> release{false};
    std::jthread watchdog;
public:
    explicit Gate(vk::Device device) : device{device} {
        const vk::SemaphoreTypeCreateInfo type{.semaphoreType=vk::SemaphoreType::eTimeline};
        auto [result, value] = device.createSemaphoreUnique({.pNext=&type});
        Require(result == vk::Result::eSuccess, "gate semaphore allocation failed");
        semaphore = std::move(value);
        watchdog = std::jthread([this] {
            const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(750);
            while (!release.load() && std::chrono::steady_clock::now() < deadline)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            Require(this->device.signalSemaphore({.semaphore=*semaphore, .value=1}) == vk::Result::eSuccess,
                    "gate signal failed");
        });
    }
    void Submit(Scheduler& scheduler) {
        SubmitInfo info{}; info.AddWait(*semaphore); scheduler.Flush(info);
    }
    void Release() { release.store(true); watchdog.join(); }
    ~Gate() { if (watchdog.joinable()) Release(); }
};

void SpecialCase(const std::string& kind, Instance& instance, Scheduler& scheduler) {
    const u32 before = Allocations(instance);
    {
        ObjectMotion motion(instance, scheduler);
        Require(motion.Enabled(), "object motion unavailable");
        const u32 initial = Allocations(instance);
        if (kind == "destruct" || kind == "drain") {
            for (auto [w,h] : std::vector<std::pair<u32,u32>>{{128,72},{96,54},{64,36},{48,27},{32,18},{16,9}})
                AttachSize(motion,scheduler,w,h);
            Require(ObjectMotionCacheTest::Retired(motion)==2, "expected two unsubmitted retirements");
            Require(Allocations(instance)==initial+6, "retained images missing before completion");
            if (kind == "drain") {
                scheduler.Finish();
                motion.OnFrameStart();
                Require(ObjectMotionCacheTest::Retired(motion)==0, "frame start did not collect retired images");
            }
            // 'destruct' leaves the current work unsubmitted: the destructor must finish it
            // before releasing both the resident targets and the retirement queue.
        } else if (kind == "lru") {
            for (auto [w,h] : std::vector<std::pair<u32,u32>>{{128,72},{96,54},{64,36},{48,27},{128,72},{32,18}})
                AttachSize(motion,scheduler,w,h);
            Require(ObjectMotionCacheTest::Has(motion,128,72), "recently revisited A was evicted");
            Require(!ObjectMotionCacheTest::Has(motion,96,54), "least recently used B was retained");
            Require(ObjectMotionCacheTest::Has(motion,64,36) && ObjectMotionCacheTest::Has(motion,48,27),
                    "a newer cache entry was evicted instead");
            scheduler.Finish(); ObjectMotionCacheTest::Collect(motion);
        } else {
            const bool bytes = kind == "budget";
            const bool oversized = kind == "oversized";
            AttachSize(motion,scheduler,128,72);
            AttachSize(motion,scheduler,oversized ? 4096 : bytes ? 2560 : 96,
                       oversized ? 2160 : bytes ? 1792 : 54);
            AttachSize(motion,scheduler,bytes ? 2688 : 64,bytes ? 1792 : 36);
            AttachSize(motion,scheduler,48,27);
            const u64 held = scheduler.CurrentTick();
            Gate gate(instance.GetDevice());
            gate.Submit(scheduler);
            Require(!scheduler.IsFree(held), "gate did not hold GPU work");
            // A gets a new use in the current submission; B is now the oldest inactive size.
            AttachSize(motion,scheduler,128,72);
            const u64 current = scheduler.CurrentTick();
            AttachSize(motion,scheduler,32,18);
            if (oversized) {
                Require(scheduler.IsFree(held), "oversized retirement did not await completion");
                Require(ObjectMotionCacheTest::Retired(motion)==0, "oversized image was deferred");
            } else {
                Require(!scheduler.IsFree(held), "first eviction waited for unfinished GPU work");
                Require(ObjectMotionCacheTest::Retired(motion)==1, "old B image was not retained");
                Require(Allocations(instance)==initial+5, "pending image was freed before its GPU tick");
                AttachSize(motion,scheduler,16,9);
                if (bytes) {
                    Require(scheduler.IsFree(held), "VRAM retirement limit failed to wait");
                    Require(ObjectMotionCacheTest::Retired(motion)==0, "completed images not released");
                } else {
                    Require(!scheduler.IsFree(held), "second eviction unexpectedly waited");
                    Require(ObjectMotionCacheTest::Retired(motion)==2, "two pending images expected");
                    Require(Allocations(instance)==initial+6, "pending allocation accounting differs");
                    AttachSize(motion,scheduler,8,4); // the count cap must await the older submission
                    Require(scheduler.IsFree(held), "retirement count cap did not wait");
                    Require(ObjectMotionCacheTest::Retired(motion)==0, "completed retirements leaked");
                }
            }
            // A full Finish would submit the current work too; the narrow wait must not.
            Require(scheduler.CurrentTick()==current, "eviction drained/submitted current GPU work");
            ObjectMotionCacheTest::Limits(motion);
            gate.Release();
            scheduler.Finish(); ObjectMotionCacheTest::Collect(motion);
        }
        if (kind != "destruct") {
            Require(ObjectMotionCacheTest::Retired(motion)==0, "retired images remain after GPU completion");
            Require(Allocations(instance)==initial+4, "resident target count differs from four");
        }
    }
    Require(Allocations(instance)==before, "object-motion teardown leaked VMA allocations");
    std::printf("PASS kind=%s LRU/deferred_lifetime/bounds/teardown checked\n",kind.c_str());
}

int main(int argc, char **argv) {
    try {
        const std::string kind = argc > 1 ? argv[1] : "tiny";
        const u32 steps = kind == "game" ? 12 : 64;
        std::vector<std::pair<u32, u32>> sizes;
        if (kind == "game")
            sizes = {{2560, 1440}, {1280, 720}, {2560, 1440}, {2560, 1440}};
        else if (kind == "churn")
            sizes = {{128, 72}, {96, 54}, {64, 36}, {48, 27}, {32, 18}, {16, 9}, {1, 1}};
        else
            sizes = {{128, 72}, {64, 36}, {128, 72}, {128, 72}, {64, 36}, {64, 36}};
        const u64 stride = u64(sizes[0].first) * sizes[0].second * 16;
        BbSettings::Get().object_motion = true;
        const int upscaler = argc > 2 ? std::atoi(argv[2]) : BbSettings::UpscalerFsr3;
        BbSettings::Get().upscaler = upscaler;
        const bool validation = argc > 3 && std::string(argv[3]) == "validation";
        Instance instance(0, validation);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(instance.GetInstance());
        VULKAN_HPP_DEFAULT_DISPATCHER.init(instance.GetDevice());
        const auto device = instance.GetDevice();
        Scheduler scheduler(instance);
        if (kind == "lru" || kind == "deferred" || kind == "budget" || kind == "oversized" ||
            kind == "drain" || kind == "destruct") {
            Require(std::getenv("BB_OBJECT_MOTION_CACHE") &&
                    std::string(std::getenv("BB_OBJECT_MOTION_CACHE"))=="1", "special cases need cache on");
            SpecialCase(kind,instance,scheduler);
            return 0;
        }
        VkBuffer staging{};
        VmaAllocation allocation{};
        VmaAllocationInfo allocation_info{};
        VkBufferCreateInfo buffer{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        buffer.size = stride * steps;
        buffer.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        VmaAllocationCreateInfo host{};
        host.usage = VMA_MEMORY_USAGE_AUTO;
        host.flags =
            VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
        if (vmaCreateBuffer(instance.GetAllocator(), &buffer, &host, &staging, &allocation,
                            &allocation_info) != VK_SUCCESS)
            throw std::runtime_error("readback allocation failed");
        std::vector<std::array<u32, 4>> expected;
        u64 fnv = 1469598103934665603ull;
        {
            ObjectMotion motion(instance, scheduler);
            if (!motion.Enabled())
                throw std::runtime_error("object motion unavailable");
            const char* cache_setting = std::getenv("BB_OBJECT_MOTION_CACHE");
            const bool cache = cache_setting && std::strcmp(cache_setting, "1") == 0;
            VmaTotalStatistics initial_stats{};
            vmaCalculateStatistics(instance.GetAllocator(), &initial_stats);
            std::set<std::pair<u32, u32>> observed_sizes;
            std::array<u32, 4> contents{};
            for (u32 i = 0; i < steps; ++i) {
                if (i % 4 == 0)
                    motion.OnFrameStart();
                const auto [w, h] = sizes[i % sizes.size()];
                RenderState state{};
                state.width = w;
                state.height = h;
                state.num_layers = 1;
                motion.Attach(state, w, h);
                observed_sizes.emplace(w, h);
                VmaTotalStatistics stats{};
                vmaCalculateStatistics(instance.GetAllocator(), &stats);
                const u32 targets = stats.total.statistics.allocationCount -
                                    initial_stats.total.statistics.allocationCount;
                const u32 expected_targets = cache ? std::min<size_t>(observed_sizes.size(), 4) : 1;
                ObjectMotionCacheTest::Limits(motion);
                if (targets != expected_targets + ObjectMotionCacheTest::Retired(motion))
                    throw std::runtime_error("target allocation count differs from selected cache mode");
                auto &attachment = state.color_attachments[Shader::MotionVectors::Output];
                if (attachment.is_clear) {
                    contents = {std::bit_cast<u32>(float(i + 1) / 128),
                                std::bit_cast<u32>(-float(i + 1) / 256), std::bit_cast<u32>(0.5f),
                                std::bit_cast<u32>(1.0f)};
                }
                attachment.clear_value = contents;
                scheduler.BeginRendering(state);
                scheduler.EndRendering();
                bool valid = false;
                auto cmd = scheduler.CommandBuffer();
                const u32 requested = i % 5 == 0 ? w + 1 : w;
                motion.PrepareRead(cmd, requested, h, valid);
                if (valid != (requested == w))
                    throw std::runtime_error("validity differs from legacy resize semantics");
                if (bool(motion.Image(requested, h)) != valid)
                    throw std::runtime_error("Image validity mismatch");
                const vk::BufferImageCopy copy{
                    .bufferOffset = i * stride,
                    .imageSubresource = {vk::ImageAspectFlagBits::eColor, 0, 0, 1},
                    .imageExtent = {w, h, 1}};
                cmd.copyImageToBuffer(motion.Image(w, h), vk::ImageLayout::eGeneral, staging, copy);
                expected.push_back(contents);
            }
            auto cmd = scheduler.CommandBuffer();
            const vk::MemoryBarrier2 host_barrier{
                .srcStageMask = vk::PipelineStageFlagBits2::eAllCommands,
                .srcAccessMask = vk::AccessFlagBits2::eMemoryWrite,
                .dstStageMask = vk::PipelineStageFlagBits2::eHost,
                .dstAccessMask = vk::AccessFlagBits2::eHostRead};
            cmd.pipelineBarrier2({.memoryBarrierCount = 1, .pMemoryBarriers = &host_barrier});
            scheduler.Finish();
            ObjectMotionCacheTest::Collect(motion);
            Require(ObjectMotionCacheTest::Retired(motion)==0, "retired images did not drain");
            const u32 expected_targets = cache ? std::min<size_t>(observed_sizes.size(),4) : 1;
            Require(Allocations(instance)==initial_stats.total.statistics.allocationCount+expected_targets,
                    "final resident allocation count differs");
            vmaInvalidateAllocation(instance.GetAllocator(), allocation, 0, VK_WHOLE_SIZE);
            for (u32 i = 0; i < steps; ++i) {
                const auto [w, h] = sizes[i % sizes.size()];
                const auto *values = reinterpret_cast<const u32 *>(
                    static_cast<const u8 *>(allocation_info.pMappedData) + i * stride);
                for (u64 p = 0; p < u64(w) * h * 4; ++p) {
                    if (values[p] != expected[i][p % 4]) {
                        std::printf("MISMATCH step=%u word=%llu actual=%08x expected=%08x\n", i,
                                    (unsigned long long)p, values[p], expected[i][p % 4]);
                        throw std::runtime_error("motion pixels changed");
                    }
                    fnv = (fnv ^ values[p]) * 1099511628211ull;
                }
            }
        }
        vmaDestroyBuffer(instance.GetAllocator(), staging, allocation);
        std::printf("PASS kind=%s upscaler=%d steps=%u all_pixels_identical_to_expected hash=%016llx\n",
                    kind.c_str(), upscaler, steps, (unsigned long long)fnv);
        return 0;
    } catch (const std::exception &e) {
        std::printf("FAIL: %s\n", e.what());
        return 1;
    }
}
