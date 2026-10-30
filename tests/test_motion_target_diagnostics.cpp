// SPDX-License-Identifier: GPL-2.0-or-later
#include "video_core/renderer_vulkan/motion_target_diagnostics.h"
#include <cassert>
#include <cstdio>
int main() {
    Vulkan::MotionTargetDiagnostics d;
    d.BeginFrame();
    d.Attach(2560,1440,true);d.Attach(1280,720,true);
    d.Read(2560,1440,1280,720,false);
    assert(d.counts.size_mismatches==1 && d.counts.earlier_matching_attachment==1);
    d.Read(1280,720,1280,720,true);
    assert(d.counts.reads==2 && d.counts.valid_reads==1);
    d.BeginFrame(); // A match in a previous frame is not evidence for this one.
    d.Attach(1280,720,false);d.Read(2560,1440,1280,720,false);
    assert(d.counts.size_mismatches==2 && d.counts.earlier_matching_attachment==1);
    for (unsigned i=0;i<12;++i) d.Attach(i+1,i+1,false);
    assert(d.used==8 && d.counts.untracked_sizes==5);
    assert(d.counts.frames==2 && d.counts.attachments==15 && d.counts.clears==2);
    std::puts("PASS: size mismatch/earlier candidate, per-frame reset and bounded diagnostic storage");
}
