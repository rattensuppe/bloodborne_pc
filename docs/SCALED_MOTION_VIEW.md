# Scaled motion-vector debug view

The existing **Show motion vectors (debug)** overlay option also works on
scaled SDR output. The visualization runs after reconstruction/sharpening and
before the HUD, so upscaling no longer bypasses the debug branch.

Red/green represent the horizontal/vertical motion supplied to the upscaler;
8 pixels reach full intensity. Blue marks coverage in the object-motion
target. It does not certify that object vectors passed the separate depth
validity check. Alpha is retained. A missing or differently sized object target
does not contribute blue coverage.

This topic is extracted from the combined prototype's `2180512` onto official
Windows 1.9.7 (`b480526`). It preserves the existing upscaling providers,
sharpening and original motion-target cache lifecycle. SDK 2.4, SSBO routing,
LRU/deferred target release and camera/depth ghosting experiments are separate
changes. The original cache and UI resizing wait for GPU completion before
destroying their images/views; this view needs no cache-optimisation dependency.

The new pipeline is created lazily. With debug disabled, or outside its
RGBA8 SDR target, the function returns before pipeline/view allocation and
dispatch. Existing native/HDR rendering paths retain their behaviour.

## Regression checks

Build `scaled-motion-debug-test` explicitly; it is excluded from normal builds.
It executes the production shader and checks every pixel, mapped render-space
motion, object coverage and alpha. Cases cover odd dimensions, absent/larger/
smaller object targets, WQHD/4K, Performance/Native-AA input sizes and returning
to a small output. Run in private test profiles without loading a game.

The fixture checks shader output and isolated resource transitions; it does
not replace a gameplay/HUD sight check.

## Standalone validation, October 31, 2026

On RX9070 / Adrenalin26.10.1, the renderer/capability tools, debug target and
three relevant CPU tests build and pass. The actual embedded SPIR-V module
passes standalone validation. Ten pixel/alpha cases pass normally and again
with Khronos Core and Synchronization checks confirmed active: 20 readbacks.

Validation reports the existing headless-device
`VUID-vkCreateDevice-ppEnabledExtensionNames-01387`. The unchanged upstream
instance enables `VK_KHR_swapchain` without a surface extension; an unchanged
cache/BDA control reproduces it. No additional debug-view findings or
synchronization hazards were reported. The separate headless correction in
the combined prototype is not part of this topic.

The standard local Windows ZIP, program manifest, CRC and actual frozen
launcher imports pass. Cached DLSS bridge files are retained unchanged; this
is not a new NVIDIA hardware test. Earlier gameplay feedback belongs to the
combined view. No fresh gameplay/HUD test or performance benchmark was run
for this independently extracted branch.
