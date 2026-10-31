# Combined RCAS and SDR output

For FSR 4.1.1/4.1.2 through Vulkan or AMD DX12, positive host sharpening writes from
the provider's RGBA16F output into the RGBA8 UI target. This replaces the separate
FP16 sharpen target and nearest blit. The shader explicitly rounds through FP16
before RGBA8 conversion, retaining the prior two-step pixel result. Upscaler
history, motion vectors and subsequent HUD composition are unchanged.

Other providers, formats, sharpening off/zero, mismatched sizes and aliased
input/output retain the existing path. The method does not change settings or
the AMD provider. The dedicated GPU test compares every output component with
the pinned original direct-RCAS shader followed by a real nearest Vulkan blit,
including small/odd sizes, finite boundary/random values and five strengths.
An optional captured RGBA16F input can also be supplied to the test.

Build and run in a configured Windows CLANG64 environment:

```sh
ninja -C out/gpu bb-probe fsr41-fused-output-test
out/gpu/fsr41-fused-output-test.exe
```

The reference header pins the old direct-RCAS SPIR-V from before fusion; it is
test-only and must remain equal to the non-fused shader compiled by this build.

## Standalone review scope

This topic is extracted from `bd23156` onto official Windows 1.9.7 (`b480526`).
The fusion function and shader are preserved. SDK 2.4, SSBO routing, motion-cache
optimisation and the scaled debug view are separate changes. User defaults,
RCAS strength mapping and the existing sharpening-off/zero paths are retained.

On RX9070 / Adrenalin26.10.1, the native renderer/provider tools and three
relevant CPU tests build and pass. The three existing non-fused shader variants
are byte-identical to newly compiled pristine-base shaders, and the pinned
reference matches the current direct-RCAS module. The fused SPIR-V validates.

There are 220 exact RGBA8 comparisons with unchanged FP16 inputs: 90 synthetic
cases normally, 90 with Core/Synchronization validation confirmed active and
40 replays of fresh GPU outputs from all four FSR4.1 methods at 1080p/4K.
The synthetic set covers tiny/odd/WQHD/4K sizes, finite boundary/random values
and strengths 0, 0.25, 0.5, 1 and 2. No validation errors or synchronization
hazards were reported by the standalone RCAS test. The backend-output checks
do not certify AMD's complete shader implementation.

The original DirectSharpen/ExtraSharpen functions are unchanged and the fusion
guards were reviewed separately. These isolated provider/replay tests do not
execute the complete in-game composition wrapper, HUD or FG. No fresh gameplay
or performance benchmark of this extracted branch is claimed.

The unrelated `shared-graphics-overlay-test` still has its pristine-base
`runtime_restart` link defect. It is recorded as a failure, not counted as a
passing check or repaired in this performance change. The GPU fixture routes
its own assertion failures to stderr rather than a modal CRT dialog; a missing
input file is verified to fail with a captured assertion.

The normal local Windows ZIP is checked separately, including its program
manifest, CRC and real frozen launcher. Cached DLSS files are retained without
a new NVIDIA hardware test.

## Earlier measurements

An earlier local 4K/Quality Warehouse comparison on RX9070 measured about 0.8%
more game FPS. Earlier beta checks also covered synthetic/captured inputs and
backend-output replays. Those historical results are not new measurements of
this 1.9.7 branch or claims for FG, HDR or other GPUs.
