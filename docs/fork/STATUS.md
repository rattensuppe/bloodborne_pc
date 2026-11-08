# Fork status

Updated November 8, 2026. [Workflow](WORKFLOW.md) · [Review](REVIEW.md) ·
[Build history](RELEASES.md) · [Fork overview](../../README.md).

## Source and downloads

| Role | Current state |
| --- | --- |
| Official mirror `windows` | Windows **1.9.8.4**, exact upstream [`115a029`](https://github.com/rattensuppe/bloodborne_pc/commit/115a0299aa0f7132e98305b74855bf5840495ba1) |
| Combined source | `integration/windows`; reviewed runtime merge [`50306c8`](https://github.com/rattensuppe/bloodborne_pc/commit/50306c87fbca389de25c7b1e8cc51fffc3fb2f17), followed by integration-only test/documentation updates |
| Current published test download | [Windows 1.9.7 SDK24 prototype.2](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.7-sdk24-prototype.2), source `b1ca96e` |
| `test/all-features` | Retains that earlier published tester source; unchanged by this integration |
| New 1.9.8.4 ZIP | Local verification package only; no release uploaded |

## Integrated components and review topics

All seven exact heads below are ancestors of the current integration. Their
individual public branches and PRs were not modified. Only SDK upscaling was
already refreshed to 1.9.8.4; the other independent topics retain their reviewed
bases. A combined test does not replace a standalone-topic hardware test.

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

- Windows native product build and **18 CPU programs** passed,
  including native SR/FG menu policies, SDK contracts, motion storage,
  composition, preferences and upstream runtime/file-position tests.
- **262 Python tests**, plus **86 repeated gate/manifest/TLS
  checks under `-O` and `-OO`, passed in private profiles.
- **93 isolated GPU cases** cover BDA/SSBO bounds and history, indirect
  descriptor rebinding, cache off/on, LRU/retirement/lifetime, current-frame
  scaled visualization, RCAS pixel/reference checks, all four SR methods,
  both ML-FG versions and DLL-order/switch/reset/fallback/HUD controls.
- Standard Windows ZIP checks cover manifest/CRC, pinned runtimes/licenses,
  native identity, actual frozen-launcher imports and relocated 13-language
  UI/support/preferences. The package is retained locally, not published.
- Shared text generation and all 21 current topic pairs were rechecked.
  Nineteen pairs merge automatically; the two explicit resolutions are in
  [Review](REVIEW.md#merge-resolution).

The integration-only TLS test correction excludes the Linux-only client from
the Windows/shared routing lint. That client uses its own default verifying
SSL context and its existing tests also pass. No network runtime changed.

## Validation and remaining limits

The original SDK Vulkan shaders retain their documented validation findings.
The sentinel deliberately emits one invalid-buffer VUID; exercised other paths
report no new validation findings or synchronization hazards. Functional output
is not Vulkan conformance. [Technical scope](../development/FSR24_UPSCALING.md).

This round starts no game and provides no new FPS/lows, gameplay-quality,
HDR-monitor or D3D12-debug-layer result. Forced SSBO fixtures on the available
GPU do not certify all RX 6000 hardware. The earlier RX 6800 XT feedback is
basic combined-build functionality only; see [player notes](../RX6000_OBJECT_MOTION.md).
SDK FG remains a prototype pending its separate presentation/gameplay review.

The separate Seamless-co-op engine and data are optional packager inputs and
are not built or bundled by this normal-engine check. Upstream source changes
are retained; fork changes do not automatically modify the separate engine.

The private shaderfix, Lite experiment, UI-view cleanup and further renderer
work are not included in this synchronization. PRs, published downloads and
personal installations remain unchanged.
