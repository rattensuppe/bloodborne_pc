# Fork status

Updated October 31, 2026. This page tracks integration and review readiness.
[Workflow](WORKFLOW.md) · [Build history](RELEASES.md) · [Fork overview](../../README.md)

## Source and downloads

| Role | Reference |
| --- | --- |
| Official mirror | [`windows`](https://github.com/rattensuppe/bloodborne_pc/tree/windows), Windows 1.9.7, upstream [`b480526`](https://github.com/Supermedo/bloodborne_pc/commit/b48052617a919fc4d6b10ae3395e209c9e94e17a) |
| Combined fork | [`integration/windows`](https://github.com/rattensuppe/bloodborne_pc/tree/integration/windows) |
| Integration starting point | [`2a8ec7e`](https://github.com/rattensuppe/bloodborne_pc/commit/2a8ec7e435f3b22e031237a6a201fb572c83a411); subsequent reorganisation changes are documentation only |
| Current test download | [Windows 1.9.7 SDK 2.4 prototype.1](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.7-sdk24-prototype.1) |
| Tested runtime and release tag | [`2180512`](https://github.com/rattensuppe/bloodborne_pc/commit/21805126c2f7b93d853e17bb5535cb0111a5c4f7) |

The release ZIP adds a prototype README and the RX 6000 notes to the tested
program files. Commit `2a8ec7e` records the download link and adds the RX 6000
document to future standard packages; its runtime is identical to `2180512`.
Reorganising the fork does not publish a new binary build.

## Integrated components awaiting separate review

The topic names below define the intended split. They are **planned**, not
claims that each standalone branch has already passed its checks or has a PR.

| Component | Planned topic | Implementation / readiness |
| --- | --- | --- |
| Frozen-launcher runtime and package checks | `fix/launcher-runtime-checks` | Integrated; extract the checks and verify a standalone package |
| RX 6000 bounded object-motion SSBO | `fix/rx6000-object-motion` | Integrated; [implementation and RX 6800 XT feedback](../RX6000_OBJECT_MOTION.md); separate from the cache optimisation |
| Scaled SDR motion-vector view | `fix/scaled-motion-view` | Integrated; WQHD/4K pixel, alpha and resize checks passed; verify independent target lifetime |
| Combined RCAS/output | `perf/fsr41-output` | Integrated; [math, rounding and checks](../FSR41_FUSED_OUTPUT.md); Vulkan and DX12 |
| Motion-target LRU and deferred release | `perf/motion-target-cache` | Integrated; [cache and lifetime details](../OBJECT_MOTION_CACHE.md); include late image/view reads |
| SDK 2.4 FSR 4.1.2 upscaling | `feat/fsr-sdk24-upscaling` | Integrated; [public API implementation](../FSR24_UPSCALING.md); Vulkan findings and current gameplay coverage remain open |
| SDK 2.4 ML-FG 4.0.2 | `feat/mlfg-sdk24` | Integrated; [independent FG runtime](../SDK24_ML_FG.md); existing DX12 bridge, 2x SDR |

SDK upscaling and FG share headers and packaging utilities. Their runtime pins
remain separate. Before separate PRs, isolate the required common foundation
and test each feature on its own source base. The SSBO/cache and scaled-view/
output changes also touch common renderer code; copying commits alone does not
establish independence.

## Already upstream

| PR | Scope |
| --- | --- |
| [#76](https://github.com/Supermedo/bloodborne_pc/pull/76) | FSR 4.1.1 methods, initial cache and associated launcher/overlay work |
| [#94](https://github.com/Supermedo/bloodborne_pc/pull/94) | ML-FG 4.0.1 and related integration |
| [#128](https://github.com/Supermedo/bloodborne_pc/pull/128) | FSR 4.1.2 / ML-FG 4.0.2 before the SDK 2.4 migration |
| [#133](https://github.com/Supermedo/bloodborne_pc/pull/133) | Projection jitter correction for bottom/right border artifacts |

Their historical head branches are retained. New PRs describe only the current
delta against upstream, accounting for squash merges or equivalent patches.

## Validation and remaining limits

The 1.9.7 runtime passed native build, focused CPU/launcher groups, isolated GPU
cases for the four upscaling methods, both ML-FG support queries, SSBO and cache
paths, scaled-view pixel/alpha tests and actual frozen-package checks. The
packaged launcher was checked in 13 languages, with relocated Unicode paths,
runtime pins, manifests and ZIP hashes. These results belong to the cited
runtime; each extracted topic still needs its own validation.

- Original AMD SDK Vulkan shaders produce validation findings on RX 9070 /
  Adrenalin 26.10.1. The [captured-module report](../FSR24_UPSCALING.md#local-validation-findings-october-30-2026)
  distinguishes them from the validated SSBO and motion-view shaders.
- A fresh gameplay/performance comparison on the 1.9.7 source remains open.
  Isolated output checks do not establish gameplay image quality.
- Static-floor ghosting is reported in both official and fork builds. A local
  camera/depth investigation is separate; this branch does not contain a
  claimed fix. Brief freezes on teleport/first attack remain observations to test.
- A stale offline-text assertion also fails on pristine upstream. Broad UI
  batches showed window-state interference; focused relevant groups were
  checked in fresh processes. These are not blanket all-tests-passed results.
- RX 6800 XT / 26.10.1 short gameplay feedback covers Vulkan/DX12 with cache
  off/on. It is one hardware/driver sample, not general RX 6000 certification
  or an uncapped performance benchmark.

Motion cache stays optional and off by default. User sharpening preferences and
existing hardware restrictions are preserved. ML-FG is still the DX12 bridge;
native Vulkan FG, new presentation pacing and FSR Lite are separate topics.

## Next review steps

1. Extract launcher/package checks and the RX 6000 fix onto current upstream.
2. Verify their independent builds and relevant tests before opening small PRs.
3. Extract the remaining topics with explicit shared foundations and document
   standalone checks separately from combined integration checks.
4. Add reliable commit-specific automation before requiring CI status checks.
