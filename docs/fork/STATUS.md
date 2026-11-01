# Fork status

Updated November 1, 2026. This page tracks integration and review readiness.
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

All seven topics have independent fork branches based on official `b480526`.
The SDK topics share the explicit foundation below. Five ready PRs and one
SDK SR Draft are open:
the first wave [#140](https://github.com/Supermedo/bloodborne_pc/pull/140) /
[#141](https://github.com/Supermedo/bloodborne_pc/pull/141), followed by
[#142](https://github.com/Supermedo/bloodborne_pc/pull/142),
[#143](https://github.com/Supermedo/bloodborne_pc/pull/143) and
[#144](https://github.com/Supermedo/bloodborne_pc/pull/144), plus
[Draft #146](https://github.com/Supermedo/bloodborne_pc/pull/146).

| Component | Topic | Implementation / readiness |
| --- | --- | --- |
| Frozen-launcher runtime and package checks | [`fix/launcher-runtime-checks`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/launcher-runtime-checks) | [PR #140 open](https://github.com/Supermedo/bloodborne_pc/pull/140) at [`63a37f9`](https://github.com/rattensuppe/bloodborne_pc/commit/63a37f9fcafe39be2f4213d63a95eba27cb55d47); three focused commits, 44 Python tests in default/-O/-OO and real frozen-runtime checks |
| RX 6000 bounded object-motion SSBO | [`fix/rx6000-object-motion`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/rx6000-object-motion) | [PR #141 open](https://github.com/Supermedo/bloodborne_pc/pull/141) at [`b3bd49d`](https://github.com/rattensuppe/bloodborne_pc/commit/b3bd49d72175c2027e37b85f24e5b55af1df1286); indirect binding, varying-location checks and 13-language reasons verified; original cache lifecycle retained |
| Scaled RGBA8 motion-vector view | [`fix/scaled-motion-view`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/scaled-motion-view) | [PR #142 open](https://github.com/Supermedo/bloodborne_pc/pull/142) at [`ef9e1ef`](https://github.com/rattensuppe/bloodborne_pc/commit/ef9e1efa6348b3e69850857f88e5fc5343053216); current-frame coverage, 39 pixel/alpha readbacks, 12 owner sequences and live scaled/legacy HUD checks |
| Combined RCAS/output | [`perf/fsr41-output`](https://github.com/rattensuppe/bloodborne_pc/tree/perf/fsr41-output) | [PR #143 open](https://github.com/Supermedo/bloodborne_pc/pull/143) at [`89b5215`](https://github.com/rattensuppe/bloodborne_pc/commit/89b5215528392e1071f5e67c2b7955208c525732); three follow-up commits; dual-reference pixel checks, NDEBUG controls, standard ZIP and RX9070 live composition/HUD checks |
| Motion-target LRU and deferred release | [`perf/motion-target-cache`](https://github.com/rattensuppe/bloodborne_pc/tree/perf/motion-target-cache) | [PR #144 open](https://github.com/Supermedo/bloodborne_pc/pull/144) at [`775d816`](https://github.com/rattensuppe/bloodborne_pc/commit/775d8167fe5bc045a6b3d5dc610b6d5155477243); two follow-up commits; split-submission lifetime guard, 78 GPU cases, four negative controls, 26 private SSBO cases and standard ZIP checked |
| SDK 2.4 FSR 4.1.2 upscaling | [`feat/fsr-sdk24-upscaling`](https://github.com/rattensuppe/bloodborne_pc/tree/feat/fsr-sdk24-upscaling) | [Draft PR #146](https://github.com/Supermedo/bloodborne_pc/pull/146) at [`ba4dc22`](https://github.com/rattensuppe/bloodborne_pc/commit/ba4dc221419dd82b213b56271e7f6e2b6d6073ab); shared foundation and SR-only package checked; original AMD Vulkan findings and current gameplay checks remain open |
| SDK 2.4 ML-FG 4.0.2 | [`feat/mlfg-sdk24`](https://github.com/rattensuppe/bloodborne_pc/tree/feat/mlfg-sdk24) | Prepared at [`e2426a9`](https://github.com/rattensuppe/bloodborne_pc/commit/e2426a91b2ec7ef77842ef6981dbf6c5c3863ac4); independent FG effect, retained SR paths, bridge/stress and standard ZIP checked |

SDK upscaling records a small common foundation at `238137e`: public headers,
file verification, nested-runtime manifests and the separate headless fix. The
SDK FG topic reuses that base without the SR runtime change. Effect pins and
tests remain separate. SSBO/cache and scaled-view/output also touch common
renderer code; preserve their documented merge resolutions and coverage.

### First two standalone candidates

The launcher branch checks TLS-context settings, Pillow image/font operations,
Tk imports and Tcl initialization without removable assertions. It compares
native dependencies and Tcl/Tk script data before invoking the actual windowed
launcher through its existing `--script` role with a 30-second timeout. Tcl 9's
DLL-embedded script library is checked as well as Tcl 8's filesystem layout.

The follow-up in PR #140 passes 44 focused tests in each of default, `-O` and
`-OO` modes, a fresh complete native build and standard Windows ZIP. Normal
and optimized frozen gates reject 31 controlled native/import/Tcl/TLS faults;
the actual relocated windowed launcher passes with only Windows tools on PATH.
A deliberately failing packaging gate preserves the previous staging folder
and ZIP. The gate remains build-only and is not shipped to players.

Recursive manifest coverage first benefits the standard SDK layout at
`bin/fsr24/` in #146, whose manifest patch remains identical. Direct dependency
pinning was validated separately and remains outside this PR. Renderer, SDK,
game startup settings and the integration runtime are unchanged.

The SSBO follow-up binds descriptors on indirect motion draws and checks actual
varying locations, including fragment remapping and clip-distance emulation.
Optional logs describe the final choice; launcher/overlay reasons share 13
translations. Existing device policy, memory access and cache lifetime remain.

The complete Windows build, 12 Python tests in each default/-O/-OO mode, 75
location cases, 56 GPU bounds/data cases, 48 history frames and four indirect
descriptor-rebind cases pass. Private RX9070 fixtures cover direct/threaded
recording and normal/push set0 layouts. Eight emitted SPIR-V modules remain
byte-identical; four BDA/SSBO cache-off/on readbacks match. The standard ZIP
passes manifest/CRC and actual frozen-launcher checks, retaining cached DLSS.
Only concise player help is packaged; technical validation stays in the repo.

Five of S's six topic merge previews are now automatic. Package-copy, SDK
include-order and test-target insertion conflicts are removed; combining S/C
still requires the documented owner/test-helper resolution. Other topics are
unchanged. The integration runtime and published downloads remain unchanged.

One existing headless-device finding remains in this standalone candidate:
`VUID-vkCreateDevice-ppEnabledExtensionNames-01387`. Unchanged upstream instance
code enables a swapchain extension without a surface; a BDA-only control
reproduces it. No additional SSBO validation findings or synchronization hazards
were reported. The combined branch already has a separate headless correction;
it is not folded into the SSBO topic. A fresh RX 6000 gameplay run of the
extracted branch is still useful; earlier combined-prototype feedback is
identified separately in its notes.

### Standalone scaled motion view

The view branch now accepts blue coverage only from a current-frame target at
the matching render size. Motion generation and the original target lifecycle
are preserved. Both visualization SPIR-V modules remain byte-identical.
Thirteen pixel/alpha cases across normal and validated cache-off/on runs give
39 readbacks; 12 real-owner sequences check stale coverage and size changes.
The full native build, three CPU programs, standard ZIP and actual frozen
launcher pass. Only the inherited headless-device VUID above remains.

RX9070 live checks cover FSR3.1, TAA, FSR4.1.2 Vulkan/DX12, the scaled and legacy
1080p routes, output changes, FG3.1 and ML-FG4.0.2. The user confirms
movement-dependent colors and normally visible HUD in both routes. Disabling
the view skips its dispatch. These are functional visibility checks; display
HDR and general image-quality certification are outside this evidence.

Player guidance is in the packaged FSR guide; the
[technical validation](https://github.com/rattensuppe/bloodborne_pc/blob/ef9e1efa6348b3e69850857f88e5fc5343053216/docs/development/SCALED_MOTION_VIEW.md)
stays in the repository. All six topic merge previews are automatic. SDK,
SSBO, RCAS/cache optimisation and ghosting experiments remain separate;
the integration runtime and downloads are unchanged.

### Standalone RCAS/output fusion

The RCAS branch combines FSR4.1 host sharpening and RGBA8 UI output with
explicit FP16 rounding. Diagnostic follow-ups identify the executed route on
transitions and distinguish optional provider `output` from composed pre-HUD
`output_sdr`. Existing shaders, guards, providers and motion/cache settings remain.

The complete native build and three CPU programs pass. Historical and current
direct references match in 440 full RGBA8 comparisons across synthetic inputs
and fresh outputs of all four methods, plus ten explicit-GPU comparisons.
The existing 90-case direct/copy regression passes. Actual NDEBUG checks reject
ten malformed captures/selectors. Core/Sync and SPIR-V checks pass; all three
pre-existing shader variants remain byte-identical. Standard ZIP, manifest/CRC
and actual frozen-launcher checks pass; technical notes stay outside the package.

RX9070 game checks confirm 14 phases covering methods, off/zero sharpening,
4K/WQHD/1080p, TAA/FSR3.1 and FG3.1/ML-FG4.0.2, plus the new diagnostic outputs.
The user reports no image errors in requested camera/movement/HUD checks with
both FG providers. The game exits regularly. RX7900XT and display HDR checks
remain open; no new performance measurement is claimed.

See the [technical validation](https://github.com/rattensuppe/bloodborne_pc/blob/89b5215528392e1071f5e67c2b7955208c525732/docs/development/FSR41_FUSED_OUTPUT.md).
All six R topic merge previews are automatic. The integration runtime and
published downloads remain unchanged.

### Standalone motion-target cache

The cache branch retains LRU/bounded retirement and late reads, then adds two
follow-up commits for the outgoing submission bound, regression robustness and
separate player/developer guidance. The stash tick covers a submission between
attachment setup and the actual draw. The default remains off, with four reusable
targets and at most two retired images / 128 MiB of actual allocation bytes.
Shaders, providers, clear/validity rules and saved settings remain unchanged.

The complete native build, three CPU programs and standard ZIP/frozen launcher
pass. On RX9070, 78 direct/threaded GPU cases include 60 complete pixel/validity
readbacks and 18 lifetime/pressure cases. Two safe mutation controls detect the
old bound before eviction; two injected watchdog failures exit normally under
NDEBUG. Core/Sync checks retain only the documented headless-base VUID, with
no additional cache findings or hazards. Real out-of-memory is not injected.

All six topic merge previews are refreshed. Five merge automatically; with
current SSBO head `b3bd49d`, only the test-helper union needs manual resolution.
The production implementation and header merge automatically. The checked
private combination passes 26 forced SSBO-owner cache cases on RX9070; RX6000
hardware and new vertex-shader gameplay checks remain open. Standalone source
and renderer are restored afterward; no combined runtime is published.

The packaged cache guide is concise; the [technical validation](https://github.com/rattensuppe/bloodborne_pc/blob/775d8167fe5bc045a6b3d5dc610b6d5155477243/docs/development/OBJECT_MOTION_CACHE.md)
stays in the repository and records the inherited launcher-test mock limitation.
No new gameplay/performance result is claimed. Integration runtime and
downloads remain unchanged.

### Standalone SDK 2.4 upscaling

The SDK branch starts with three implementation commits: shared API/file/manifest foundation
[`2943038`](https://github.com/rattensuppe/bloodborne_pc/commit/29430384d1449da4ef7e168ed492c6e7fd9923fa),
headless-device fix
[`238137e`](https://github.com/rattensuppe/bloodborne_pc/commit/238137e44071a2050825e245b91daff20fc9c1c9),
then the public Vulkan/DX12 upscaling integration. The independent FG branch starts
at the second commit; that base contains neither SDK SR nor SDK FG integration.
This SR package retains the official FG sources, headers and runtime files and
adds only two SDK upscaling DLLs. Camera, cache, shaders and RCAS remain official.

The latest native build, ten CPU programs (including the repaired overlay test)
and 151 Python tests pass. The existing 31 isolated runtime cases cover all four SR methods, switching, failure/repair boundaries,
DLL loading orders and both retained FG support queries. The standard ZIP and
actual relocated frozen launcher pass manifest/pin/runtime checks; its 13-language
CPU picker fixture is separate from the real packaged four-method support probe.

Recorded Core/Synchronization validation and captured original AMD submissions
reproduce the SDK Vulkan findings: 30 modules, 12 separate SPIR-V failures and
14 `CooperativeMatrixConversionsNV` declarations. Both DX12 paths and fsr4vk have
no Vulkan errors in these cases. This is not a clean overall SDK validation pass,
D3D12 debug-layer result or new gameplay/performance evidence. Keep SDK [PR #146](https://github.com/Supermedo/bloodborne_pc/pull/146)
a Draft while those review questions remain open.

Runner/readiness conflicts preserve current Windows diagnostics and exclude the
obsolete driver ABI. Merge previews with launcher/cache are clean. Scaled-view,
RCAS and SSBO need documentation-copy unions; SSBO also needs the public-SDK
includes retained in its capability helper. Private shell/C++ syntax checks pass;
other branches remain separate and no new combined GPU run is claimed. See the
[standalone scope and validation notes](https://github.com/rattensuppe/bloodborne_pc/blob/ba4dc221419dd82b213b56271e7f6e2b6d6073ab/docs/FSR24_UPSCALING.md#standalone-review-on-windows-197).
The combined integration runtime remains unchanged.

### SDK SR test-only follow-up

The existing SDK SR topic now adds
[`6047f4e`](https://github.com/rattensuppe/bloodborne_pc/commit/6047f4e7382300235f4d8c70928775f9fd3b8d43)
for the missing PC-menu restart test stub and stable `###settings` window lookup,
then [`ba4dc22`](https://github.com/rattensuppe/bloodborne_pc/commit/ba4dc221419dd82b213b56271e7f6e2b6d6073ab)
for scope notes. Assertions are explicitly enabled and the actual overlay
selection/storage/restart test passes. Changes from `580d935` are confined to
that test and its documentation.

A separate worktree preserves the earlier ZIP. Its fresh standard SR-only
package and relocated actual frozen UI pass manifest/CRC/pins, 13 languages
and independent real four-method support probes. The previous 31 GPU cases
and original AMD findings are reused against identical runtime sources and
SDK pins; no new GPU dispatch or gameplay test is claimed. The topic is now
submitted as [Draft #146](https://github.com/Supermedo/bloodborne_pc/pull/146).
No new binary release has been created.

The foundation is reconfirmed: six public API headers, verified file fetching,
license and nested manifests, plus the separate headless fix. Its manifest
matches #140; it contains no SR/FG effect integration. SDK FG still starts at
`238137e`. All six SDK-SR merge previews retain the existing conflict classes;
the repaired test matches the already validated cumulative combination.

### Standalone SDK 2.4 frame generation

The FG branch shares foundation `238137e` with SDK SR, then adds two commits:
[`a8e571e`](https://github.com/rattensuppe/bloodborne_pc/commit/a8e571e21786baec94d66927438bad5ff2f317d2)
for the independent 4.0.2 effect and
[`e2426a9`](https://github.com/rattensuppe/bloodborne_pc/commit/e2426a91b2ec7ef77842ef6981dbf6c5c3863ac4)
for optional input checks and nonfatal memory diagnostics. Public FG headers
and the signed DLL match the AMD pins. Its downloader uses the shared verifier
directly and does not depend on SDK SR. The package adds one SDK FG DLL;
legacy 4.0.1 and the four official upscaling paths remain. The DX12 interop
bridge, SDR 2x output, HUD, camera, shaders, cache and user settings are unchanged.

Native build, six CPU programs and 58 Python tests pass. Thirty-one isolated
runtime cases cover both DLL load orders and ML-FG versions, moving input,
1080p/4K output, serial/overlap operation, 256-frame three-slot stress,
reset/version switches, missing SDK fallback, independent 4.0.1 availability,
invalid adapter, optional diagnostics, HUD and all four retained SR methods.
Core/Synchronization validation is active and clean in the FG bridge, stress,
switch and HUD cases. This is not a D3D12 debug-layer or real presentation test.
The separate SDK SR Vulkan findings above remain open.

The standard ZIP passes manifest/CRC/pin checks. The real relocated frozen
launcher passes imports and 13-language picker/preference checks; real packaged
capability probes independently verify SR and FG, including a missing FG DLL.
No game, new visual-quality test or performance measurement was run.

Merge previews are runtime-clean against the other six topics. SSBO, scaled
motion view, RCAS and SDK SR each need a documentation-copy union in package.sh;
the private resolutions pass shell syntax checks. Other branches remain separate;
this is not a fresh combined-runtime GPU result. See the
[standalone scope](https://github.com/rattensuppe/bloodborne_pc/blob/e2426a91b2ec7ef77842ef6981dbf6c5c3863ac4/docs/SDK24_ML_FG.md#standalone-review-on-windows-197).
The combined integration runtime remains unchanged.

### Final dependency and coverage review

The [review matrix and submission order](REVIEW.md) records all seven exact
topic heads, the common SDK foundation, all 21 pairwise merge previews and
the explicit resolutions used in a private cumulative build. The published
integration runtime and the seven topic heads are unchanged.

The cumulative build passes 13 native CPU programs, 181 Python tests and
78 isolated functional GPU cases, including 48 BDA/SSBO cache cases,
10 scaled-view readbacks, 110 exact RCAS comparisons and both FG versions.
The standard package and actual relocated 13-language frozen launcher pass.
Known original AMD SDK Vulkan findings remain; there is no blanket clean
validation, new gameplay, frame-pacing or performance claim.

The overlay-test restart/window-ID repairs are now published in the SDK SR
topic as a separate test-only commit, followed by scope documentation. They
match the previously validated local follow-up; product runtime sources are unchanged. The first wave is submitted: [#140](https://github.com/Supermedo/bloodborne_pc/pull/140)
for launcher/package checks and [#141](https://github.com/Supermedo/bloodborne_pc/pull/141)
for the RX6000 SSBO fix. Both target `windows` on `b480526` at the reviewed heads.
All 33 existing open PRs were checked for overlap. Fresh checks passed 34
launcher/TLS/package tests, five SSBO launcher guards, two CPU programs and
frozen-runtime checks; the reviewed package hashes and sources are unchanged.
SDK SR is now submitted as Draft #146; fresh current-build gameplay and native
build/package CI remain separate work items. See the matrix for per-topic limits.

### Second-wave submissions

The independent motion-view [#142](https://github.com/Supermedo/bloodborne_pc/pull/142),
RCAS/output [#143](https://github.com/Supermedo/bloodborne_pc/pull/143) and target-cache
[#144](https://github.com/Supermedo/bloodborne_pc/pull/144) topics are submitted
against official `windows` at `b480526`. Exact bodies, heads and file scopes
were read back after creation; GitHub reports clean merges at this base.

All 35 existing open PRs were checked for file overlap, reusing patch reads
only when their head hashes were unchanged. Nine CPU-program runs and six
actual frozen/runtime checks pass. Standalone and cumulative build/GPU/pixel
results are reused against unchanged source heads and verified ZIP hashes;
no new game or GPU run is claimed. The descriptions preserve the headless
test finding, original harness/mock limits and current gameplay boundaries.
The SSBO/cache merge resolution remains explicit in #144 and the review matrix.

The first wave is still open with no review feedback at this check. SDK topics
were unsubmitted at this second-wave check; SR was subsequently submitted as
Draft #146 below. All topic sources and the integration runtime,
personal installation and all existing releases are unchanged.

## Already upstream

| PR | Scope |
| --- | --- |
| [#76](https://github.com/Supermedo/bloodborne_pc/pull/76) | FSR 4.1.1 methods, initial cache and associated launcher/overlay work |
| [#94](https://github.com/Supermedo/bloodborne_pc/pull/94) | ML-FG 4.0.1 and related integration |
| [#128](https://github.com/Supermedo/bloodborne_pc/pull/128) | FSR 4.1.2 / ML-FG 4.0.2 before the SDK 2.4 migration |
| [#133](https://github.com/Supermedo/bloodborne_pc/pull/133) | Projection jitter correction for bottom/right border artifacts |

Their historical head branches are retained. New PRs describe only the current
delta against upstream, accounting for squash merges or equivalent patches.

## SDK SR Draft submission

[PR #146](https://github.com/Supermedo/bloodborne_pc/pull/146), **Use SDK 2.4 for
FSR 4.1.2 Vulkan and DX12**, targets official `windows` at `b480526`. The exact
description, five commits and 62-file delta at `ba4dc22` were read back after
creation; GitHub reports a clean merge. It remains a Draft because the original
AMD Vulkan conformance findings and current gameplay/composition checks are open.

All 39 previously open PRs were checked for file overlap. The new launcher
PR #145 concerns automatic settings for new installations; its launcher hunks
apply cleanly to this SDK head and do not replace the SDK integration. Existing
reviewed overlap resolutions are retained. The ten CPU programs pass again;
the exact-head native build, 151 Python tests, 31 isolated runtime cases and
standard ZIP/frozen checks are retained after source, pin and package verification.
No new GPU dispatch, game or performance measurement is claimed.

The shared foundation remains independently reviewable at `238137e`; its nested
manifest change matches #140. SDK FG stays in its separate branch with its own
current-game/presentation check pending. #140–#144, all topic heads, integration
runtime, personal data and all 13 releases/assets are unchanged.

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

1. Follow reviews of the five ready PRs and SDK SR Draft above; inspect the final upstream
   content before updating the mirror or later topics.
2. Follow the [review order and coverage limits](REVIEW.md); retain the tested
   merge resolutions when updating later topics.
3. Keep SDK SR, SDK FG and shared/headless helpers independently reviewable;
   keep SDK SR #146 in Draft while its conformance/gameplay checks remain open.
4. Complete the applicable current-game checks and add reliable native
   build/package automation before making those CI checks mandatory.

## Repository organisation

`integration/windows` is the fork default. `windows` remains the official mirror.
Rules block deletion and forced history updates on both primary branches, freeze
the seven historical branches and protect version tags from updates/deletion.
Normal integration updates and new topic branches remain available.

The hosted **Fork documentation** workflow checks local overview links on the
integration commit. It has read-only repository access and uses a pinned checkout
action. It is not a renderer build, does not execute public PRs on the NUC and is
not a required merge check. All 13 existing releases and their assets are retained.
