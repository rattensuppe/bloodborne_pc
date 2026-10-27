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
The earlier local 4K/Quality Warehouse comparison on an RX 9070 measured about
0.8% more game FPS. This is not a new measurement of the 1.9.5 beta build or a
performance claim for the beta, FG, HDR, other GPUs or other methods.

On the 1.9.5 beta port, 110 synthetic/captured-image comparisons and 40 replays
of fresh production outputs were pixel-exact. The production wrapper and GPU
consumer were checked separately for all four FSR 4.1 methods at 1080p and 4K.
These isolated checks do not replace a moving-game or frame-generation test.
