// SPDX-License-Identifier: GPL-2.0-or-later
#include "sdk24_fg.h"
#include <cassert>
#include <cstdio>
#include <cstring>

namespace {
int mode{};
ffxReturnCode_t Query(ffxContext* context, ffxQueryDescHeader* desc) {
    if (mode == 6) return FFX_API_RETURN_ERROR;
    if (context) {
        auto* value = reinterpret_cast<ffxQueryGetProviderVersion*>(desc);
        value->versionId = mode == 3 ? 800 : 702;
        value->versionName = mode == 4 ? "4.0.1" : "4.0.2";
        return FFX_API_RETURN_OK;
    }
    auto* value = reinterpret_cast<ffxQueryDescGetVersions*>(desc);
    assert(value->createDescType == FFX_API_CREATE_CONTEXT_DESC_TYPE_FRAMEGENERATION);
    if (mode == 5) { *value->outputCount = 65; return FFX_API_RETURN_OK; }
    if (!value->versionIds) { *value->outputCount = 2; return FFX_API_RETURN_OK; }
    value->versionIds[0] = 501; value->versionNames[0] = "4.0.1";
    value->versionIds[1] = 702; value->versionNames[1] = mode == 1 ? "3.1.6" : "4.0.2";
    if (mode == 2) *value->outputCount = 3;
    return FFX_API_RETURN_OK;
}
template<class F> void Reject(F operation) {
    bool rejected = false;
    try { operation(); } catch (const FgSdk24::Error&) { rejected = true; }
    assert(rejected);
}
}
int main(int argc, char** argv) {
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    if (argc == 3) {
        HMODULE legacy{};
        const bool legacy_first = std::strcmp(argv[1], "legacy-first") == 0;
        const auto path = std::filesystem::absolute(argv[2]);
        if (legacy_first) legacy = LoadLibraryExW(path.c_str(), nullptr, LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
        const auto& sdk = FgSdk24::Runtime();
        if (!legacy_first) legacy = LoadLibraryExW(path.c_str(), nullptr, LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
        assert(legacy && sdk.module != legacy);
        assert(GetProcAddress(legacy, "ffxCreateContext") != reinterpret_cast<FARPROC>(sdk.functions.CreateContext));
        FreeLibrary(legacy);
        std::puts("PASS: SDK 4.0.2 and legacy 4.0.1 use independent modules");
        return 0;
    }
    FgSdk24::Api api{}; api.functions.Query = Query;
    assert(FgSdk24::Is402("4.0.2") && FgSdk24::Is402("4.0.2 *"));
    assert(!FgSdk24::Is402(nullptr) && !FgSdk24::Is402("4.0.20") && !FgSdk24::Is402("4.0.1"));
    const auto selected = FgSdk24::Select(api, nullptr);
    assert(selected.id == 702 && selected.name == "4.0.2");
    ffxContext context = reinterpret_cast<void*>(1);
    FgSdk24::Verify(api, &context, selected);
    for (mode = 1; mode <= 2; ++mode) Reject([&]{ FgSdk24::Select(api, nullptr); });
    for (mode = 3; mode <= 4; ++mode) Reject([&]{ FgSdk24::Verify(api, &context, selected); });
    for (mode = 5; mode <= 6; ++mode) Reject([&]{ FgSdk24::Select(api, nullptr); });
    std::puts("PASS: SDK FG enumeration, bounded counts, exact identity and rejection of other versions");
}
