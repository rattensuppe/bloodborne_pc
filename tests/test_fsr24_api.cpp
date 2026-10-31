// SPDX-License-Identifier: GPL-2.0-or-later
#include "sdk24_upscaling.h"
#include <cassert>
#include <cstring>
#include <cstdio>

namespace {
int mode{};
ffxReturnCode_t Query(ffxContext* context, ffxQueryDescHeader* desc) {
    if (context) {
        if (desc->type == FFX_API_QUERY_DESC_TYPE_UPSCALE_GPU_MEMORY_USAGE) {
            if (mode == 6) return FFX_API_RETURN_ERROR;
            auto* value = reinterpret_cast<ffxQueryDescUpscaleGetGPUMemoryUsage*>(desc);
            value->gpuMemoryUsageUpscaler->totalUsageInBytes = 123456;
            value->gpuMemoryUsageUpscaler->aliasableUsageInBytes = 2048;
            return FFX_API_RETURN_OK;
        }
        auto* value = reinterpret_cast<ffxQueryGetProviderVersion*>(desc);
        value->versionId = mode == 3 ? 900 : 701;
        value->versionName = mode == 4 ? "3.1.6" : "4.1.2 *";
        return FFX_API_RETURN_OK;
    }
    auto* value = reinterpret_cast<ffxQueryDescGetVersions*>(desc);
    if (mode == 5) { *value->outputCount = 65; return FFX_API_RETURN_OK; }
    if (!value->versionIds) { *value->outputCount = 2; return FFX_API_RETURN_OK; }
    value->versionIds[0] = 501; value->versionNames[0] = "3.1.6";
    value->versionIds[1] = 701; value->versionNames[1] = mode == 1 ? "4.1.1" : "4.1.2 *";
    if (mode == 2) *value->outputCount = 3;
    return FFX_API_RETURN_OK;
}
template<class F> void Reject(F operation) {
    bool rejected = false;
    try { operation(); } catch (const FsrSdk24::Error&) { rejected = true; }
    assert(rejected);
}
}
int main(int argc, char** argv) {
#ifdef _WIN32
    _set_error_mode(_OUT_TO_STDERR);
    _set_abort_behavior(0, _CALL_REPORTFAULT);
#endif
    if (argc == 3) {
        const bool legacy_first = std::strcmp(argv[1], "legacy-first") == 0;
        const auto legacy_path = std::filesystem::absolute(argv[2]);
        HMODULE legacy{};
        const FsrSdk24::Api* sdk{};
        if (legacy_first) legacy = LoadLibraryExW(legacy_path.c_str(), nullptr, LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
        sdk = &FsrSdk24::Runtime(true);
        if (!legacy_first) legacy = LoadLibraryExW(legacy_path.c_str(), nullptr, LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
        assert(legacy && sdk->module != legacy);
        assert(GetProcAddress(legacy, "ffxCreateContext") != reinterpret_cast<FARPROC>(sdk->functions.CreateContext));
        FreeLibrary(legacy);
        std::puts("PASS: SDK 2.4 and legacy 4.1.1 resolve to independent modules in both loading orders");
        return 0;
    }
    FsrSdk24::Api api{}; api.functions.Query = Query;
    _putenv_s("BB_FSR24_DEBUG", "");
    ffxCreateContextDescUpscale create{};
    create.flags = FFX_UPSCALE_ENABLE_AUTO_EXPOSURE;
    FsrSdk24::ConfigureDiagnostics(create);
    assert(create.fpMessage && create.flags == FFX_UPSCALE_ENABLE_AUTO_EXPOSURE);
    _putenv_s("BB_FSR24_DEBUG", "1");
    FsrSdk24::ConfigureDiagnostics(create);
    assert(create.flags & FFX_UPSCALE_ENABLE_DEBUG_CHECKING);
    assert(create.flags & FFX_UPSCALE_ENABLE_AUTO_EXPOSURE);
    _putenv_s("BB_FSR24_DEBUG", "0");
    assert(!FsrSdk24::DebugChecking());
    _putenv_s("BB_FSR24_DEBUG", "");
    assert(FsrSdk24::Is412("4.1.2 *") && FsrSdk24::Is412("4.1.2"));
    assert(!FsrSdk24::Is412(nullptr) && !FsrSdk24::Is412("4.1.20") && !FsrSdk24::Is412("3.1.6"));
    const auto selected = FsrSdk24::Select(api, nullptr);
    assert(selected.id == 701 && selected.name == "4.1.2 *");
    ffxContext context = reinterpret_cast<void*>(1);
    assert(FsrSdk24::Verify(api, &context, selected).id == 701);
    FsrSdk24::LogMemory(api, &context, "CPU fixture");
    mode = 6; FsrSdk24::LogMemory(api, &context, "CPU fixture unsupported");
    for (mode = 1; mode <= 2; ++mode) Reject([&]{ FsrSdk24::Select(api, nullptr); });
    for (mode = 3; mode <= 4; ++mode) Reject([&]{ FsrSdk24::Verify(api, &context, selected); });
    mode = 5; Reject([&]{ FsrSdk24::Select(api, nullptr); });
    std::puts("PASS: SDK provider IDs, names, bounded enumeration and context verification");
}
