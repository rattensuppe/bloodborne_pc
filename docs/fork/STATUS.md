# Fork status

Updated November 13, 2026. [Workflow](WORKFLOW.md) · [Review](REVIEW.md) ·
[Build history](RELEASES.md) · [Fork overview](../../README.md).

## Source and downloads

| Role | Current state |
| --- | --- |
| Official mirror `windows` | Windows **1.9.8.5**, exact upstream [`1a4ba07`](https://github.com/rattensuppe/bloodborne_pc/commit/1a4ba07ad63abe793832f7f5188ca0bedcc88485) |
| Combined source | `integration/windows`; upstream merge [`791ece2`](https://github.com/rattensuppe/bloodborne_pc/commit/791ece2173d23f8b38a312b2b5d377129ea2c0a5), followed by fork documentation |
| Runtime source | Identical to official 1.9.8.5; the fork adds navigation/status documents |
| Published fork test download | [Windows 1.9.7 SDK24 prototype.2](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.7-sdk24-prototype.2) |
| `test/all-features` | Historical published tester source, unchanged |
| New 1.9.8.5 ZIP | Local verification package only |

## Upstream adoption and retained topic history

Official 1.9.8.5 contains all six submitted topics and SDK ML-FG 4.0.2. Their
implementation was incorporated in the release commit; the individual PRs
remain formally open (#146 Draft). No PR was closed or updated by this sync.
All seven previous heads below remain in the integration history. Their code
was compared before merging and was not applied a second time.

| ID | Component | Topic | Included head | Submission |
| --- | --- | --- | --- | --- |
| L | Launcher runtime/package checks | [`fix/launcher-runtime-checks`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/launcher-runtime-checks) | [`9f1d51a`](https://github.com/rattensuppe/bloodborne_pc/commit/9f1d51a0daea17885fffd4716bf8cd658d4477a7) | Open [#140](https://github.com/Supermedo/bloodborne_pc/pull/140) |
| S | RX 6000 object-motion SSBO | [`fix/rx6000-object-motion`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/rx6000-object-motion) | [`4adbf2f`](https://github.com/rattensuppe/bloodborne_pc/commit/4adbf2fae3b6bb3d3013f967a02161a681ecd445) | Open [#141](https://github.com/Supermedo/bloodborne_pc/pull/141) |
| V | Scaled motion-vector view | [`fix/scaled-motion-view`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/scaled-motion-view) | [`336a908`](https://github.com/rattensuppe/bloodborne_pc/commit/336a90842d66b019aa2c4c525ea191840f0ea8e6) | Open [#142](https://github.com/Supermedo/bloodborne_pc/pull/142) |
| R | Combined RCAS/output | [`perf/fsr41-output`](https://github.com/rattensuppe/bloodborne_pc/tree/perf/fsr41-output) | [`b6c8dbf`](https://github.com/rattensuppe/bloodborne_pc/commit/b6c8dbfd15e5889aaba99b7fa97528f12c77b956) | Open [#143](https://github.com/Supermedo/bloodborne_pc/pull/143) |
| C | Motion-target LRU/deferred release | [`perf/motion-target-cache`](https://github.com/rattensuppe/bloodborne_pc/tree/perf/motion-target-cache) | [`e0a24f7`](https://github.com/rattensuppe/bloodborne_pc/commit/e0a24f7b915ba0f6889e12c8cbf80bfe5c1e3e7a) | Open [#144](https://github.com/Supermedo/bloodborne_pc/pull/144) |
| U | SDK 2.4 upscaling | [`feat/fsr-sdk24-upscaling`](https://github.com/rattensuppe/bloodborne_pc/tree/feat/fsr-sdk24-upscaling) | [`a58e2eb`](https://github.com/rattensuppe/bloodborne_pc/commit/a58e2eb2405a7b09db97df949ff635350a6321c7) | Draft [#146](https://github.com/Supermedo/bloodborne_pc/pull/146) |
| F | SDK 2.4 ML-FG 4.0.2 | [`feat/mlfg-sdk24`](https://github.com/rattensuppe/bloodborne_pc/tree/feat/mlfg-sdk24) | [`94e16c6`](https://github.com/rattensuppe/bloodborne_pc/commit/94e16c69a7e10b985151aa10c877ad57d781ccb2) | Prototype; no PR |


## Fresh combined validation

- Native Windows product build and **26 CPU programs** passed,
  including the new upstream command-buffer, hang-snapshot, frame-rate, flip-tail,
  GCN float-rule and render-target guard checks.
- **563 of 564 normal Python tests** passed; **86 optimized
  gate/manifest/TLS repeats** passed. One unchanged upstream adapter-list test
  fails on this host: DXGI exposes an additional Microsoft Basic Render Driver
  without its software flag. This is recorded separately, not counted as a pass.
- **93 isolated GPU cases** cover the four FSR methods, both ML-FG versions,
  BDA/SSBO motion storage, cache lifetimes, scaled visualization and RCAS/HUD.
- The standard Windows ZIP is checked for manifest/CRC, original pinned
  runtimes and licenses, native executable identity, and actual frozen-launcher
  imports. Relocated UI/support/preferences checks cover the 13 FSR languages;
  upstream Korean launcher/menu regression tests are also included.

## Validation and remaining limits

The original SDK Vulkan shaders retain the documented validation findings.
No new validation IDs or synchronization hazards were found in the exercised
paths. Functional output is not Vulkan conformance.
[Technical scope](../development/FSR24_UPSCALING.md).

This synchronization starts no game and provides no new gameplay, FPS/lows,
HDR-monitor, D3D12-debug-layer or RX 6000 hardware result. Forced SSBO fixtures
on the available RX 9070 do not certify all RX 6000 cards. Earlier RX 6800 XT
feedback remains basic functionality; see [player notes](../RX6000_OBJECT_MOTION.md).
SDK ML-FG is now also upstream code; isolated checks do not replace its separate
presentation/gameplay review.

The normal engine is built and packaged. The optional separate Seamless-co-op
engine/data are not built or bundled in this check. The private shaderfix, Lite
experiment and further renderer work remain separate. Existing releases,
individual topic branches and personal installations are unchanged.
