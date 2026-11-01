# Topic review and submission order

Reviewed October 31, 2026; follow-ups through November 1, 2026.
Base: official Windows 1.9.7 `b480526`.
This is the source/coverage review for the seven extracted topics, separate from
the combined experimental download. Five ready topic PRs and SDK SR Draft
[#146](https://github.com/Supermedo/bloodborne_pc/pull/146) are open; the table
below records their exact reviewed heads and coverage. SDK FG remains separate
for later submission after its current-game/presentation check.

## Sources and dependencies

| ID | Topic | Reviewed head | Required foundation | Submission state |
| --- | --- | --- | --- | --- |
| L | [`fix/launcher-runtime-checks`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/launcher-runtime-checks) | [`63a37f9`](https://github.com/rattensuppe/bloodborne_pc/commit/63a37f9fcafe39be2f4213d63a95eba27cb55d47) | None | Open [#140](https://github.com/Supermedo/bloodborne_pc/pull/140) |
| S | [`fix/rx6000-object-motion`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/rx6000-object-motion) | [`b3bd49d`](https://github.com/rattensuppe/bloodborne_pc/commit/b3bd49d72175c2027e37b85f24e5b55af1df1286) | None; official cache lifecycle | Open [#141](https://github.com/Supermedo/bloodborne_pc/pull/141); hardware limits documented |
| V | [`fix/scaled-motion-view`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/scaled-motion-view) | [`ef9e1ef`](https://github.com/rattensuppe/bloodborne_pc/commit/ef9e1efa6348b3e69850857f88e5fc5343053216) | None | Open [#142](https://github.com/Supermedo/bloodborne_pc/pull/142) |
| R | [`perf/fsr41-output`](https://github.com/rattensuppe/bloodborne_pc/tree/perf/fsr41-output) | [`89b5215`](https://github.com/rattensuppe/bloodborne_pc/commit/89b5215528392e1071f5e67c2b7955208c525732) | None | Open [#143](https://github.com/Supermedo/bloodborne_pc/pull/143) |
| C | [`perf/motion-target-cache`](https://github.com/rattensuppe/bloodborne_pc/tree/perf/motion-target-cache) | [`775d816`](https://github.com/rattensuppe/bloodborne_pc/commit/775d8167fe5bc045a6b3d5dc610b6d5155477243) | None; combine explicitly with S | Open [#144](https://github.com/Supermedo/bloodborne_pc/pull/144) |
| U | [`feat/fsr-sdk24-upscaling`](https://github.com/rattensuppe/bloodborne_pc/tree/feat/fsr-sdk24-upscaling) | [`2f63b4b`](https://github.com/rattensuppe/bloodborne_pc/commit/2f63b4be8c85016a03256b1671780327df7c3337) | Shared SDK foundation `238137e` | Draft [#146](https://github.com/Supermedo/bloodborne_pc/pull/146); AMD Vulkan findings open; standalone RX9070 game comparison checked |
| F | [`feat/mlfg-sdk24`](https://github.com/rattensuppe/bloodborne_pc/tree/feat/mlfg-sdk24) | [`e2426a9`](https://github.com/rattensuppe/bloodborne_pc/commit/e2426a91b2ec7ef77842ef6981dbf6c5c3863ac4) | Shared SDK foundation `238137e`; no dependency on U | Later wave: fresh FG/presentation check before ready status |

The SDK topics share `2943038` (unchanged public API headers, verified fetching,
nested-runtime manifests) and `238137e` (headless-device extension fix).
That base contains neither SDK SR nor SDK FG integration. The nested-manifest
change also appears in L; it is the same change, not a dependency on the full
launcher-runtime topic. Review the foundation once and account for it when the
second SDK topic is refreshed after an upstream merge. SDK FG fetches its own
pin directly through the shared helper and does not import the SR downloader.

V does not require R. S does not require C. Each was built and checked on its
own documented base. C and S share ownership/test code, so their combination
needs the explicit resolution below.

## Merge matrix

All 21 unordered topic pairs were checked; pairs involving S, V, R, C and U were
refreshed after their November 1 follow-ups. Nineteen merge automatically;
two need explicit resolutions. Codes: **D** documentation-copy union in package.sh,
**T** independent test targets/helpers. The S/C header now merges automatically.
A checkmark means an automatic pairwise merge,
not an independent hardware or gameplay certification.

| | L | S | V | R | C | U | F |
| --- | --- | --- | --- | --- | --- | --- | --- |
| L | — | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| S | | — | ✓ | ✓ | T | ✓ | ✓ |
| V | | | — | ✓ | ✓ | ✓ | ✓ |
| R | | | | — | ✓ | ✓ | ✓ |
| C | | | | | — | ✓ | ✓ |
| U | | | | | | — | D |
| F | | | | | | | — |

The earlier private cumulative merge used the original S head `24159ad`,
original V head `d926e6f`, original R head `140b593`, original C head `a1d32a4`
and the original U runtime before its November 1 corrections. Updated S, V, R, C
and U are checked independently; the cumulative runtime
has not been rebuilt. Its original resolutions were:

- Preserve both motion-storage and scaled-view CMake test targets.
- Preserve the SSBO constructor/friend declarations and the cache lifetime
  policy. Union the SSBO owner fixture with the cache regression helpers.
- In the SDK/SSBO capability helper, retain the public SDK includes and SSBO
  metadata; exclude the obsolete `driver_provider.h` route.
- Keep every topic's documentation in the standard package exactly once.

In that earlier cumulative build, runtime sources for the launcher, SR/FG,
public SDK, motion shaders and shader compiler match the existing integration. The deliberate differences are the
excluded unfinished target diagnostics, preservation of upstream's constant
far-plane input, and a comment encoding correction. The local combination
does not replace the published integration runtime or download.

## Coverage and limits

| Scope | Standalone evidence | Fresh cumulative evidence |
| --- | --- | --- |
| L | 44 Python tests in default/-O/-OO; actual/optimized freezes and 31 negative controls; standard ZIP and Unicode path without development PATH | Real standard package, frozen imports, 27 TLS tests including local certificate failures |
| S | 75 location cases, 56 bounds cases, 48 frames, four indirect rebind cases, byte-identical shaders and four cache readbacks on forced SSBO; 12 Python tests per mode and standard ZIP; earlier combined RX6800XT sample only | Policy test plus SSBO/cache owner combination; no new RX6000 machine or vertex-draw certification |
| V | 39 pixel/alpha readbacks, 12 current-frame owner sequences, unchanged SPIR-V, standard ZIP and RX9070 scaled/legacy HUD checks with output/FSR/FG changes | 10 complete checks under active Core/Sync validation on original V |
| R | 440 dual-reference RGBA8 checks, ten explicit-GPU checks, 90 direct/copy cases, ten NDEBUG negative controls, standard ZIP and 14 live phases with user FG/HUD sight checks | 110 exact comparisons on original R, including outputs from all four SR methods |
| C | 78 cache cases, four safe negative controls, 26 private SSBO cases, full build/three CPU programs and standard ZIP | 48 BDA/SSBO cases on original C: direct/threaded recording, cache off/on, retirement pressure, LRU, teardown and late reads |
| U | 11 CPU programs, 162 Python tests, ABI controls, 31 fresh runtime cases, standard ZIP/13-language frozen UI; current RX9070 Warehouse/movement checks | Original U runtime: all four methods and RCAS replays; original SDK Vulkan findings reproduced |
| F | Six CPU programs, 58 Python tests, 31 runtime cases, actual package/picker | Both DLL load orders/versions, 1080p/4K, serial/overlap, three slots/256 frames, reset/switch/fallback and HUD |

That earlier cumulative build passes 13 native CPU programs and 181 Python tests in
23 fresh processes. Its 78 GPU processes pass their functional assertions;
the validation sentinel deliberately reports one invalid buffer. Apart from
that sentinel and the known original SDK Vulkan findings, the exercised
Core/Synchronization paths report no VUIDs or hazards. This is not an overall
clean SDK Vulkan result, a D3D12 debug-layer result, or new FPS/low-percentile
or gameplay-quality evidence. The standard ZIP and relocated actual frozen
launcher pass pins/manifest/CRC, 13-language picker/preferences and separate
real packaged support probes. Cached DLSS binaries were retained; no NVIDIA
hardware was tested.

The earlier SDK SR test-only follow-up `6047f4e` repaired the PC-menu restart
stub and stable overlay-window lookup; `ba4dc22` recorded that scope. At that
stage the runtime still matched `580d935`: ten CPU programs, 151 Python tests,
standard ZIP/frozen checks and reused runtime evidence. The November 1 SDK
corrections and fresh runtime/game evidence are recorded below. The original
cumulative-runtime evidence retains its original scope. FG is not submitted.

## PR140 packaging-gate follow-up

L now includes a separate hardening commit after the cumulative review. It
changes only the build gate, its invocation and regression tests. Explicit
failures remain active with Python optimization; TLS output describes context
configuration. Pillow/Tk/Tcl coverage includes the actual windowed bundle and
both filesystem and DLL-embedded Tcl libraries. Full native dependencies and
Tcl/Tk data are compared before the actual check, which has a 30-second timeout.

The standard standalone ZIP, actual relocated launcher, optimized frozen gate
and 31 negative controls pass. A real packaging-failure control preserves an
existing stage and ZIP. The manifest/shared-SDK patch is unchanged. Dependency
pinning is a separately validated local build-policy change, not part of #140.
Six fresh merge previews against the other topic heads remain clean. Cumulative
runtime evidence retains its original scope; the integration runtime and
published binary releases have not been rebuilt or replaced.

## PR141 hardware-independent follow-up

S now includes three follow-up commits for indirect descriptor binding, actual
varying-location checks, shared 13-language reasons and separated player/developer
documentation. The 75 CPU location cases cover remapping, constant defaults,
passthrough, unused declarations and clip-distance shifts. GPU fixtures add an
indirect zero-record draw after incompatible set0 layouts in all four recorder/
descriptor combinations. Eight emitted shader modules are byte-identical; no
additional validation findings or synchronization hazards were reported beyond
the inherited headless VUID, also reproduced by the BDA control.

The complete native build, 12 Python tests per mode, standard ZIP and actual
frozen-launcher check pass. The ZIP retains player help and excludes the technical
report. Existing motion-pipeline preload exclusion, GPU selection, BDA/SSBO access,
atomics and cache lifetime are preserved. A fresh RX6000 gameplay run is pending;
there is no whole-series reliability or performance claim. No automatic device-loss
policy changes the saved preference. Five merge previews are automatic; only the
existing S/C owner/test-helper combination requires resolution. Remaining D conflicts
between other topics retain their documented scope.

## PR142 current-frame coverage and visibility follow-up

V adds two focused commits: checked current-frame/size-matched object coverage
with regression tests, and separate player/developer guidance. Red/green retain
their render-pixel magnitude mapping; blue does not certify depth acceptance.
Both shader modules are byte-identical. The full native build, three CPU tests,
39 pixel/alpha readbacks, 12 owner sequences, standard ZIP and actual frozen
launcher pass. Core/Sync reports no additional findings beyond the inherited
headless-device VUID, also reproduced by the BDA control.

RX9070 game checks confirm actual scaled/legacy dispatches, 4K/WQHD/1080p
changes, FSR3.1/TAA/FSR4.1.2 Vulkan/DX12 and FG3.1/ML-FG4.0.2 activation.
The user confirms motion-dependent colors and normally visible HUD in both
routes. Debug-off dispatch is absent. These functional checks do not certify
general upscaler/FG image quality or display HDR. Private control hooks are
excluded from the branch and ZIP. All six V merge previews are automatic;
remaining D conflicts are between other topics. Integration runtime and
published downloads are unchanged.

## PR143 diagnostics and test follow-up

R adds three focused commits for route/dump diagnostics, robust dual-reference
tests and separate player/developer documentation. The historical direct shader
is retained alongside the current generated reference; arithmetic and existing
shader bytes remain unchanged. Explicit failure checks survive NDEBUG, the
fixture prefers a capable discrete GPU, and an explicit GPU selector is checked.
The build, three CPU programs, 440 main comparisons, ten explicit-selection
comparisons, 90 direct/copy cases, ten negative controls and standard ZIP pass.
The RCAS Core/Sync fixture reports no errors or synchronization hazards.

Actual game composition confirms all four methods, off/zero guards, output
changes, TAA/FSR3.1 and FG3.1/ML-FG4.0.2. Both new dump outputs are checked;
the user reports no image errors in camera/movement/HUD checks with each FG
provider. Regular exit and original data preservation are verified. Private
control hooks are excluded from the branch/package. RX7900XT and display HDR
remain untested; this adds no new performance result. All six R merge previews
are automatic; only the S/C owner/test-helper and U/F documentation-copy
combinations retain explicit resolutions. Integration runtime/downloads remain.

## PR144 lifetime and test follow-up

C adds an outgoing-target submission bound for Attach -> upload submission ->
actual rendering. Existing late-read tracking remains. A local helper exchanges
all image ownership fields; global UniqueImage moves and image rules are unchanged.
The NDEBUG fixture tests the split submission before unsafe eviction, then checks
a gated real clear/readback and retirement. Conservative retention is reflected
in count-pressure expectations; pending count/byte limits still hold.

The full build, three CPU programs, 78 GPU cases, four controlled negative cases,
26 private SSBO-owner cases and standard ZIP/frozen launcher pass. Only the
known headless-base VUID remains; RX6000 hardware is unavailable. Five of six
current topic merges are automatic. S/C requires one test-helper union; its
runtime/header are automatic and the resolution is exercised. Technical notes
are excluded from the player package. No game, performance or real-OOM result
is added, and the combined integration runtime/downloads remain unchanged.

## PR146 SDK safeguards and current-game follow-up

U adds five focused follow-up commits for neutral localized hints, compile-time
SDK ABI guards and pre-dispatch context verification cleanup, explicit native
PC-menu Retry FSR with its existing settling filter, initial-wrapper reuse,
and separate player/developer notes. Original SDK headers, pins, FG and render
math/shaders remain unchanged. No bridge, depth-direction or max-size refactor
is folded into this correction.

The full build, 11 CPU programs, 162 Python tests, ABI negative controls,
31 fresh runtime cases and standard ZIP/actual frozen UI pass. The RX9070
same-compiler official 1.9.7 comparison verifies the identical saved camera:
Vulkan 98.01 -> 98.06 FPS; DX12 97.05 -> 97.01 FPS at 4K/Quality, cache on,
FG/cap/VSync off. Each method has two 10+25 s captures. The means are effectively
unchanged; five-frame pooled 0.1% tails do not establish movement smoothness.
The user reports unremarkable short movement/camera/HUD checks on both methods.
Initial wrapper construction is two versus one; startup time is unmeasured.

Original AMD Vulkan findings are reproduced on byte-identical SDK shaders;
the cause remains open and #146 stays Draft. Five current topic merges are
automatic; U/F retains only the package-documentation union. Integration runtime,
all releases and personal data remain unchanged. [Detailed scope/results](https://github.com/rattensuppe/bloodborne_pc/blob/2f63b4be8c85016a03256b1671780327df7c3337/docs/development/FSR24_UPSCALING.md).

## Submission sequence and remaining checks

1. **First wave submitted: L [#140](https://github.com/Supermedo/bloodborne_pc/pull/140)
   and S [#141](https://github.com/Supermedo/bloodborne_pc/pull/141)**, each with
   its own changes and checks. S states which RX6000 sample exists and that the newly
   extracted candidate was not freshly played on RX6000. Request a targeted
   RX6800XT/cache-off-on check; do not call this whole-series certification.
2. **Second wave submitted: V [#142](https://github.com/Supermedo/bloodborne_pc/pull/142),
   R [#143](https://github.com/Supermedo/bloodborne_pc/pull/143) and
   C [#144](https://github.com/Supermedo/bloodborne_pc/pull/144)**. Preserve the
   reviewed merge resolutions as upstream advances. V now has a current-game
   visibility check, and R has current composition/FG/HUD checks. The full
   updated combination still needs appropriate checks before a combined build. No new
   performance gain is inferred from these tests.
3. **SDK SR submitted as Draft [#146](https://github.com/Supermedo/bloodborne_pc/pull/146)**:
   first review the common API/package/headless
   foundation. F can progress independently of U's Vulkan findings, after a
   fresh current-build FG switch/movement/HUD check. U now has a standalone
   RX9070 game comparison and short movement/HUD checks. It remains Draft while
   original AMD shader conformance findings are open; broader coverage stays separate.
4. Before each submission, refresh upstream, inspect existing PRs/equivalent
   patches and compare the resulting delta. Do not treat an automatic merge
   or matching ancestry as proof of coverage.
5. Add reliable native build/package CI as its own work package. The existing
   read-only documentation CI remains useful and is not a renderer gate.

Historical branches/tags, all 13 releases/assets, personal installation,
settings and saves remain intact. The local cumulative package is an audit
artifact, not a new test release. See [status](STATUS.md) and [workflow](WORKFLOW.md).
