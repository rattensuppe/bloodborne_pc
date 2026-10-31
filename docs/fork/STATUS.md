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

## Integrated components and review topics

Six topics have independent fork branches on official `b480526`.
The remaining names define the planned split. No new upstream PR is open.

| Component | Planned topic | Implementation / readiness |
| --- | --- | --- |
| Frozen-launcher runtime and package checks | [`fix/launcher-runtime-checks`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/launcher-runtime-checks) | Prepared at [`b06b235`](https://github.com/rattensuppe/bloodborne_pc/commit/b06b235e6c7c49ebe69fdb1f24ce3acf331dbaa0); two focused commits, 34 Python tests and real frozen-runtime checks |
| RX 6000 bounded object-motion SSBO | [`fix/rx6000-object-motion`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/rx6000-object-motion) | Prepared at [`24159ad`](https://github.com/rattensuppe/bloodborne_pc/commit/24159adbd3bed1704fc3d4d492286ab80e4f619e); native build/readbacks and standard ZIP checked on the original cache lifecycle |
| Scaled SDR motion-vector view | [`fix/scaled-motion-view`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/scaled-motion-view) | Prepared at [`d926e6f`](https://github.com/rattensuppe/bloodborne_pc/commit/d926e6f5fc69aedbb8d3df22c03a62e0abedbe4d); standalone build, 20 pixel/alpha readbacks and standard ZIP checked |
| Combined RCAS/output | [`perf/fsr41-output`](https://github.com/rattensuppe/bloodborne_pc/tree/perf/fsr41-output) | Prepared at [`140b593`](https://github.com/rattensuppe/bloodborne_pc/commit/140b593035dcb297f2fdd8e68bba9e9dc36979a1); standalone build, 220 exact output comparisons and standard ZIP checked |
| Motion-target LRU and deferred release | [`perf/motion-target-cache`](https://github.com/rattensuppe/bloodborne_pc/tree/perf/motion-target-cache) | Prepared at [`a1d32a4`](https://github.com/rattensuppe/bloodborne_pc/commit/a1d32a4571da89e96d6211b7dfaf91b0f7c1d8d6); three focused commits, 76 standalone GPU cases and standard ZIP checked |
| SDK 2.4 FSR 4.1.2 upscaling | [`feat/fsr-sdk24-upscaling`](https://github.com/rattensuppe/bloodborne_pc/tree/feat/fsr-sdk24-upscaling) | Prepared at [`580d935`](https://github.com/rattensuppe/bloodborne_pc/commit/580d93515fbe48cf1e0af86b7dfb8d1951570e00); shared foundation and SR-only runtime checked; original AMD Vulkan findings remain open |
| SDK 2.4 ML-FG 4.0.2 | `feat/mlfg-sdk24` | Integrated; [independent FG runtime](../SDK24_ML_FG.md); existing DX12 bridge, 2x SDR |

SDK upscaling records a small common foundation at `238137e`: public headers,
file verification, nested-runtime manifests and the separate headless fix. The
SDK FG topic will reuse that base without the SR runtime change. Effect pins and
tests remain separate. SSBO/cache and scaled-view/output also touch common
renderer code; preserve their documented merge resolutions and coverage.

### First two standalone candidates

The launcher branch adds the console TLS/Pillow/Tk preflight and recursive
package-manifest coverage. Its actual windowed launcher also executes the
import check, including from a relocated Unicode path without development tools
on PATH. There are no renderer or SDK changes in that branch.

The SSBO branch resolves three extraction conflicts while preserving the
official cache lifecycle and provider paths. On RX 9070, private SSBO fixtures
pass 56 bounds cases and 48 history frames across direct/threaded recording and
normal/push descriptor layouts. Eight emitted SPIR-V modules validate; four
BDA/SSBO cache-off/on readbacks match. The standard local Windows ZIP passes
manifest/CRC and real frozen-launcher checks. The validation package omits the
optional separately built DLSS bridge; existing published packages are unchanged.

One existing headless-device finding remains in this standalone candidate:
`VUID-vkCreateDevice-ppEnabledExtensionNames-01387`. Unchanged upstream instance
code enables a swapchain extension without a surface; a BDA-only control
reproduces it. No additional SSBO validation findings or synchronization hazards
were reported. The combined branch already has a separate headless correction;
it is not folded into the SSBO topic. A fresh RX 6000 gameplay run of the
extracted branch is still useful; earlier combined-prototype feedback is
identified separately in its notes.

### Standalone scaled motion view

The view branch keeps upstream sharpening, providers and target ownership
unchanged. Its shader and visualization function match the combined prototype;
the tests add smaller object targets and repeated output-size changes. Twenty
pixel/alpha readbacks pass, including WQHD/4K, absent/mismatched targets and
Native-AA/Performance sizes. Core and Synchronization checks are confirmed active;
only the existing headless-device VUID described above is reported. The actual
embedded SPIR-V validates, three relevant CPU tests pass, and the standard ZIP
passes manifest/CRC and frozen-launcher checks. Cached DLSS files are retained;
there is no new NVIDIA or gameplay test.

See the [candidate's scope and validation notes](https://github.com/rattensuppe/bloodborne_pc/blob/d926e6f5fc69aedbb8d3df22c03a62e0abedbe4d/docs/SCALED_MOTION_VIEW.md).
No SDK, SSBO, RCAS/cache optimisation or ghosting experiment is included in this
independent topic. The combined integration runtime remains unchanged.

### Standalone RCAS/output fusion

The RCAS branch combines FSR 4.1 host sharpening and SDR output while preserving
FP16 rounding. The original off/zero and other-format paths remain; providers,
SDK, motion vectors, cache ownership and settings are unchanged. Three CPU tests
pass. All 220 GPU comparisons match exactly, including fresh outputs of the four
Vulkan/DX12 methods. The three existing shader variants compile byte-identically
to the official base. Core/Synchronization validation is confirmed active in the
standalone RCAS fixture, with no errors or hazards; the standard ZIP and actual
frozen launcher also pass their checks.

The [candidate's validation notes](https://github.com/rattensuppe/bloodborne_pc/blob/140b593035dcb297f2fdd8e68bba9e9dc36979a1/docs/FSR41_FUSED_OUTPUT.md)
separate shader/provider-output tests from the complete in-game composition
wrapper, HUD and FG, which were not run anew. The unrelated upstream overlay-test
`runtime_restart` link failure remains documented and is not counted as a pass.
No new gameplay benchmark or performance gain is claimed for this extracted
branch. The combined integration runtime remains unchanged.

### Standalone motion-target cache

The cache branch separates LRU/bounded retirement, late image/view reads and test
hardening into three commits. The default stays off. It retains four reusable
targets and at most two pending retired images / 128 MiB of allocation bytes.
Shaders, providers and saved settings remain unchanged. Seventy-six isolated
GPU cases pass in direct/threaded recording, including 60 identical full pixel
readbacks and 16 lifetime/pressure checks. Core/Synchronization validation reports
only the known headless-base VUID described above. The pixel fixture's own
transfer read now has an explicit barrier. The standard ZIP, manifest/CRC and
actual frozen launcher pass their checks.

Merge previews are clean with the launcher, scaled-view and RCAS topics. Combining
with SSBO requires resolving private declarations and fixture code in two files;
the renderer implementation merges automatically. A private resolved combination
passes 24 additional SSBO-owner/cache readback and lifetime cases, with identical
cache-function bodies. Both standalone branches remain separate. This is no new
RX 6000 hardware or vertex-shader gameplay test.

The unchanged launcher-cache suite has a stale shortcut mock: seven original
tests pass and one errors on unchanged base files too. Eight pass with a corrected
private mock; that unrelated correction is not shipped. The
[candidate's notes](https://github.com/rattensuppe/bloodborne_pc/blob/a1d32a4571da89e96d6211b7dfaf91b0f7c1d8d6/docs/OBJECT_MOTION_CACHE.md#standalone-review-on-windows-197)
preserve these limits. No new gameplay/FPS result or forced out-of-memory test is
claimed. The combined integration runtime remains unchanged.

### Standalone SDK 2.4 upscaling

The SDK branch has three commits: shared API/file/manifest foundation
[`2943038`](https://github.com/rattensuppe/bloodborne_pc/commit/29430384d1449da4ef7e168ed492c6e7fd9923fa),
headless-device fix
[`238137e`](https://github.com/rattensuppe/bloodborne_pc/commit/238137e44071a2050825e245b91daff20fc9c1c9),
then the public Vulkan/DX12 upscaling integration. A future FG branch can start
at the second commit; that base contains neither SDK SR nor SDK FG integration.
This SR package retains the official FG sources, headers and runtime files and
adds only two SDK upscaling DLLs. Camera, cache, shaders and RCAS remain official.

Native build, nine CPU programs and 151 Python tests pass. Thirty-one isolated
runtime cases cover all four SR methods, switching, failure/repair boundaries,
DLL loading orders and both retained FG support queries. The standard ZIP and
actual relocated frozen launcher pass manifest/pin/runtime checks; its 13-language
CPU picker fixture is separate from the real packaged four-method support probe.

Fresh Core/Synchronization validation and captured original AMD submissions
reproduce the SDK Vulkan findings: 30 modules, 12 separate SPIR-V failures and
14 `CooperativeMatrixConversionsNV` declarations. Both DX12 paths and fsr4vk have
no Vulkan errors in these cases. This is not a clean overall SDK validation pass,
D3D12 debug-layer result or new gameplay/performance evidence. Keep the planned
SDK upstream submission a Draft while those review questions remain open.

Runner/readiness conflicts preserve current Windows diagnostics and exclude the
obsolete driver ABI. Merge previews with launcher/cache are clean. Scaled-view,
RCAS and SSBO need documentation-copy unions; SSBO also needs the public-SDK
includes retained in its capability helper. Private shell/C++ syntax checks pass;
other branches remain separate and no new combined GPU run is claimed. See the
[standalone scope and validation notes](https://github.com/rattensuppe/bloodborne_pc/blob/580d93515fbe48cf1e0af86b7dfb8d1951570e00/docs/FSR24_UPSCALING.md#standalone-review-on-windows-197).
The combined integration runtime remains unchanged.

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

1. Review the prepared standalone diffs and their documented coverage before
   opening focused upstream PRs; preserve the headless and hardware limits.
2. Extract the remaining topics with explicit shared foundations and document
   standalone checks separately from combined integration checks.
3. Keep SDK SR, SDK FG and shared/headless helper changes independently reviewable.
4. Add reliable build/package automation before requiring CI status checks.

## Repository organisation

`integration/windows` is the fork default. `windows` remains the official mirror.
Rules block deletion and forced history updates on both primary branches, freeze
the seven historical branches and protect version tags from updates/deletion.
Normal integration updates and new topic branches remain available.

The hosted **Fork documentation** workflow checks local overview links on the
integration commit. It has read-only repository access and uses a pinned checkout
action. It is not a renderer build, does not execute public PRs on the NUC and is
not a required merge check. All 13 existing releases and their assets are retained.
