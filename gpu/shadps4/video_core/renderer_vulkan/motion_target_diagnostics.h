// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <utility>

namespace Vulkan {
// CPU attachment metadata only. Earlier attachment is a candidate, not proof
// that useful object vectors were drawn or that a missing vector caused ghosting.
struct MotionTargetDiagnostics {
    struct Counts {
        uint64_t frames{}, attachments{}, clears{}, reads{}, valid_reads{};
        uint64_t size_mismatches{}, earlier_matching_attachment{}, untracked_sizes{};
    } counts;
    std::array<std::pair<uint32_t, uint32_t>, 8> sizes{};
    unsigned used{};

    void BeginFrame() {
        used = 0;
        ++counts.frames;
    }

    void Attach(uint32_t width, uint32_t height, bool clear) {
        ++counts.attachments;
        if (clear) {
            ++counts.clears;
        }
        const auto size = std::pair{width, height};
        if (std::find(sizes.begin(), sizes.begin() + used, size) != sizes.begin() + used) {
            return;
        }
        if (used < sizes.size()) {
            sizes[used++] = size;
        } else {
            ++counts.untracked_sizes;
        }
    }

    void Read(uint32_t width, uint32_t height, uint32_t active_width, uint32_t active_height,
              bool valid) {
        ++counts.reads;
        if (valid) {
            ++counts.valid_reads;
        }
        if (width != active_width || height != active_height) {
            ++counts.size_mismatches;
            if (std::find(sizes.begin(), sizes.begin() + used, std::pair{width, height}) !=
                sizes.begin() + used) {
                ++counts.earlier_matching_attachment;
            }
        }
    }
};
} // namespace Vulkan
