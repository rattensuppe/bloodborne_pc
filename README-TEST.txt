Bloodborne Windows 1.9.7 - SDK 2.4 prototype.2
Combined test build, 2 November 2026

Source branch: test/all-features
Release: https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.7-sdk24-prototype.2
BUILD-INFO.json records the exact base, source commit and seven topic heads.

Included
- SDK 2.4 FSR 4.1.2: AMD Vulkan and AMD DX12.
- SDK 2.4 ML-FG 4.0.2 through the existing DX12 bridge.
- FSR 4.1.1 DX12/fsr4vk, FG 3.1 and ML-FG 4.0.1 retained.
- RX 6000 object-motion SSBO fixes and shared launcher/overlay settings.
- Optional motion-vector cache with LRU replacement and deferred GPU release.
- Combined RCAS/output and scaled motion-vector debug view.
- Latest review corrections, native PC-menu selection/retry and package checks.

Setup
Extract into a new folder and start bbport-windows/Bloodborne.exe. Use copied
test saves and a separate shader-cache directory. Launcher preferences are
shared with other bbport copies on the same Windows account.

Please test
1. Official 1.9.7 versus this build: FSR 3.1, cache OFF, FG OFF.
2. Cache OFF/ON with object motion vectors ON; restart after changing cache.
3. Available FSR 4.1 methods: startup, live switching, camera movement and HUD.
4. FG 3.1 and supported ML-FG versions: image quality, stability and latency.
5. Preset/resolution changes, area transitions and launcher/overlay persistence.
6. Optional motion-vector colour view: movement response, HUD, return to normal.

For performance comparisons use identical settings, VSync/FPS caps OFF, and
the same save/camera. The warehouse above the sewers near Eileen is useful
for cache comparisons. Keep the loaded camera unchanged when stationary.
Stationary: twice 10 s warmup + 25 s capture per method. For movement, walk
warehouse -> lantern -> directly back, without using the lantern or pausing.
Keep stationary and movement results separate. Report average FPS, 1%/0.1%
lows if captured, and VRAM. With FG, separate game FPS from output FPS.

Availability and known scope
- FSR 4.1 Settings > Details shows GPU/driver/library checks per method.
- ML-FG 4.0.1/4.0.2 requires supported RX 9000 hardware; 2x, SDR.
- Motion-vector cache remains optional and OFF by default.
- Original SDK Vulkan validation findings remain open. Use AMD DX12 if AMD
  Vulkan causes issues. This pre-release does not close the SDK conformance work.
- The earlier combined package passed a basic RX 6800 XT gameplay check.
  This update has fresh isolated RX 9070 and packaged-launcher checks; no new
  gameplay, HDR-display, performance or broad cross-device certification.

Reply with build, CPU/GPU, driver, resolution/preset, exact FSR method, cache/FG
state, results and reproduction steps. Attach the BB Report ZIP from Help or
BB-Report-Tool.bat. The fsr4vk source companion is beside bbport-windows.
