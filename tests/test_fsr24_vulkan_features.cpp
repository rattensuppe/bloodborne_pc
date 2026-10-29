// SPDX-License-Identifier: GPL-2.0-or-later
#include "sdk24_vulkan_features.h"
#include <cassert>
#include <cstdio>
int main() {
    unsigned cases = 0;
    for (unsigned mask = 0; mask < 128; ++mask) {
        const bool requested=mask&1, ce=mask&2, c=mask&4, fe=mask&8,
            f=mask&16, fc=mask&32, full=mask&64;
        const auto result=FsrSdk24::ChooseVulkanFeatures(requested,ce,c,fe,f,fc,full);
        assert(result.cooperative_matrix==(requested&&ce&&c));
        assert(result.float8==(requested&&ce&&c&&fe&&f&&fc));
        assert(result.full_subgroups==(requested&&full));
        ++cases;
    }
    const auto rdna23=FsrSdk24::ChooseVulkanFeatures(true,false,false,false,false,false,true);
    assert(!rdna23.cooperative_matrix&&!rdna23.float8&&rdna23.full_subgroups);
    const auto rdna4=FsrSdk24::ChooseVulkanFeatures(true,true,true,true,true,true,true);
    assert(rdna4.cooperative_matrix&&rdna4.float8&&rdna4.full_subgroups);
    std::printf("PASS: %u feature combinations; optional accelerators do not gate INT8 GPUs\n",cases);
}
