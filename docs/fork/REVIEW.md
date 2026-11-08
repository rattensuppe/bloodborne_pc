# Topic review and integration

Updated November 8, 2026. Integration base: Windows 1.9.8.4 [`115a029`](https://github.com/rattensuppe/bloodborne_pc/commit/115a0299aa0f7132e98305b74855bf5840495ba1).
[Status](STATUS.md) · [Workflow](WORKFLOW.md) · [Published builds](RELEASES.md).

## Exact included sources

| ID | Component | Topic | Included head | Submission |
| --- | --- | --- | --- | --- |
| L | Launcher runtime/package checks | [`fix/launcher-runtime-checks`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/launcher-runtime-checks) | [`9f1d51a`](https://github.com/rattensuppe/bloodborne_pc/commit/9f1d51a0daea17885fffd4716bf8cd658d4477a7) | Open [#140](https://github.com/Supermedo/bloodborne_pc/pull/140) |
| S | RX 6000 object-motion SSBO | [`fix/rx6000-object-motion`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/rx6000-object-motion) | [`4adbf2f`](https://github.com/rattensuppe/bloodborne_pc/commit/4adbf2fae3b6bb3d3013f967a02161a681ecd445) | Open [#141](https://github.com/Supermedo/bloodborne_pc/pull/141) |
| V | Scaled motion-vector view | [`fix/scaled-motion-view`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/scaled-motion-view) | [`336a908`](https://github.com/rattensuppe/bloodborne_pc/commit/336a90842d66b019aa2c4c525ea191840f0ea8e6) | Open [#142](https://github.com/Supermedo/bloodborne_pc/pull/142) |
| R | Combined RCAS/output | [`perf/fsr41-output`](https://github.com/rattensuppe/bloodborne_pc/tree/perf/fsr41-output) | [`b6c8dbf`](https://github.com/rattensuppe/bloodborne_pc/commit/b6c8dbfd15e5889aaba99b7fa97528f12c77b956) | Open [#143](https://github.com/Supermedo/bloodborne_pc/pull/143) |
| C | Motion-target LRU/deferred release | [`perf/motion-target-cache`](https://github.com/rattensuppe/bloodborne_pc/tree/perf/motion-target-cache) | [`e0a24f7`](https://github.com/rattensuppe/bloodborne_pc/commit/e0a24f7b915ba0f6889e12c8cbf80bfe5c1e3e7a) | Open [#144](https://github.com/Supermedo/bloodborne_pc/pull/144) |
| U | SDK 2.4 upscaling | [`feat/fsr-sdk24-upscaling`](https://github.com/rattensuppe/bloodborne_pc/tree/feat/fsr-sdk24-upscaling) | [`a58e2eb`](https://github.com/rattensuppe/bloodborne_pc/commit/a58e2eb2405a7b09db97df949ff635350a6321c7) | Draft [#146](https://github.com/Supermedo/bloodborne_pc/pull/146) |
| F | SDK 2.4 ML-FG 4.0.2 | [`feat/mlfg-sdk24`](https://github.com/rattensuppe/bloodborne_pc/tree/feat/mlfg-sdk24) | [`94e16c6`](https://github.com/rattensuppe/bloodborne_pc/commit/94e16c69a7e10b985151aa10c877ad57d781ccb2) | Prototype; no PR |

The current combined runtime is [`50306c8`](https://github.com/rattensuppe/bloodborne_pc/commit/50306c87fbca389de25c7b1e8cc51fffc3fb2f17). Every listed head and the official
base are in its ancestry. Older duplicated prototype copies were reconciled
to the already reviewed seven-topic union, refreshed with the current SDK SR
head. Fork navigation, history and link checks are retained separately.
Individual topic refs, PR bodies/states and historical tester branches remain
unchanged. SDK headers, effect pins and original AMD shaders are retained.

## Merge resolution

All **21 unordered pairs** of the exact heads above were checked again:
**19 automatic**, two explicit combinations:

- **S/C:** `tests/test_object_motion_cache.cpp` combines the SSBO owner fixture
  with the optimized-cache regression helpers. Preserve descriptor ownership,
  parameter/history lifetime and split-submission coverage.
- **U/F:** `packaging/windows/package.sh` retains both independently pinned SDK
  effects and both player documents. The shared foundation (`2943038` and
  `238137e`) is present once; neither topic requires the whole other topic.

The 1.9.8.4 SDK SR sync already reconciled the common low-latency include and
`presentation && LowLatency::Wanted(GetVendorID())`. The combined tree retains
that headless guard, upstream vendor-aware Auto policy and optional checkpoints.

The reviewed union also preserves current-frame motion coverage, the SSBO
varying-location/descriptor fixes, fused output ordering and deferred cache
lifetimes. Old unfinished motion-target diagnostics are excluded, as in the
independent topics. No private shader/model repair or new rendering algorithm
was introduced by resolving the older duplicate copies.

## Current coverage

Fresh combined checks: native build, 18 CPU programs,
262 normal Python tests and 86 optimized repeats,
93 isolated GPU cases, standard ZIP and actual relocated frozen
launcher in 13 languages. See [Status](STATUS.md#fresh-combined-validation).
The Linux-only TLS lint correction changes tests only, with the Linux client
covered by its existing independent suite.

These combined results cover interactions; they do not relabel each earlier
standalone test as a 1.9.8.4 test. Earlier live SR, motion-view and RCAS/HUD
checks retain their original source/settings scope. The RX 6800 XT feedback
remains basic functionality, not a series-wide stability or performance result.

## Remaining review boundaries

- SDK SR remains Draft: original SDK Vulkan conformance findings remain open.
- SDK FG has no PR; a current real-game/presentation check remains separate.
- No new HDR display, D3D12 debug-layer, NVIDIA or RX 6000 machine run here.
- `test/all-features` and published 1.9.7 downloads remain historical tested
  packages; the new 1.9.8.4 package is local only.
- UI-view cleanup and other renderer candidates remain future separate work.

Published topic history is retained. If upstream later squash-merges a topic,
compare actual code and shared foundations before refreshing the next topic;
do not reapply equivalent changes based only on different commit identities.
