// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
namespace FsrSdk24 {
struct VulkanFeatures {
    bool cooperative_matrix{};
    bool float8{};
    bool full_subgroups{};
};
// Optional accelerators, never an FSR 4.1.2 hardware allow-list. Older AMD GPUs
// remain eligible for the SDK's INT8 implementation without Float8 support.
inline constexpr VulkanFeatures ChooseVulkanFeatures(bool requested,
    bool cooperative_extension, bool cooperative_supported,
    bool float8_extension, bool float8_supported, bool float8_cooperative_supported,
    bool full_subgroups_supported) {
    const bool cooperative = requested && cooperative_extension && cooperative_supported;
    return {cooperative,
        cooperative && float8_extension && float8_supported && float8_cooperative_supported,
        requested && full_subgroups_supported};
}
} // namespace FsrSdk24
