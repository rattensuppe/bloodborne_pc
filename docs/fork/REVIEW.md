# Topic review and submission order

Reviewed October 31, 2026 against official Windows 1.9.7 `b480526`.
This is the source/coverage review for the seven extracted topics, separate from
the combined experimental download. Five topic PRs are now open; the table
below records their exact reviewed heads and coverage. SDK SR and FG remain
separate topics for later submission.

## Sources and dependencies

| ID | Topic | Reviewed head | Required foundation | Submission state |
| --- | --- | --- | --- | --- |
| L | [`fix/launcher-runtime-checks`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/launcher-runtime-checks) | [`b06b235`](https://github.com/rattensuppe/bloodborne_pc/commit/b06b235e6c7c49ebe69fdb1f24ce3acf331dbaa0) | None | Open [#140](https://github.com/Supermedo/bloodborne_pc/pull/140) |
| S | [`fix/rx6000-object-motion`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/rx6000-object-motion) | [`24159ad`](https://github.com/rattensuppe/bloodborne_pc/commit/24159adbd3bed1704fc3d4d492286ab80e4f619e) | None; official cache lifecycle | Open [#141](https://github.com/Supermedo/bloodborne_pc/pull/141); hardware limits documented |
| V | [`fix/scaled-motion-view`](https://github.com/rattensuppe/bloodborne_pc/tree/fix/scaled-motion-view) | [`d926e6f`](https://github.com/rattensuppe/bloodborne_pc/commit/d926e6f5fc69aedbb8d3df22c03a62e0abedbe4d) | None | Open [#142](https://github.com/Supermedo/bloodborne_pc/pull/142) |
| R | [`perf/fsr41-output`](https://github.com/rattensuppe/bloodborne_pc/tree/perf/fsr41-output) | [`140b593`](https://github.com/rattensuppe/bloodborne_pc/commit/140b593035dcb297f2fdd8e68bba9e9dc36979a1) | None | Open [#143](https://github.com/Supermedo/bloodborne_pc/pull/143) |
| C | [`perf/motion-target-cache`](https://github.com/rattensuppe/bloodborne_pc/tree/perf/motion-target-cache) | [`a1d32a4`](https://github.com/rattensuppe/bloodborne_pc/commit/a1d32a4571da89e96d6211b7dfaf91b0f7c1d8d6) | None; combine explicitly with S | Open [#144](https://github.com/Supermedo/bloodborne_pc/pull/144) |
| U | [`feat/fsr-sdk24-upscaling`](https://github.com/rattensuppe/bloodborne_pc/tree/feat/fsr-sdk24-upscaling) | [`ba4dc22`](https://github.com/rattensuppe/bloodborne_pc/commit/ba4dc221419dd82b213b56271e7f6e2b6d6073ab) | Shared SDK foundation `238137e` | Prepared for Draft; overlay-test follow-up passed; AMD Vulkan/gameplay checks open |
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

All 21 unordered topic pairs were checked. Ten merge automatically; eleven
need explicit resolutions. Codes: **D** documentation-copy union in package.sh,
**T** independent test targets/helpers, **O** SSBO private owner declarations,
**I** SDK capability includes. A checkmark means an automatic pairwise merge,
not an independent hardware or gameplay certification.

| | L | S | V | R | C | U | F |
| --- | --- | --- | --- | --- | --- | --- | --- |
| L | — | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| S | | — | D, T | D | O, T | D, I | D |
| V | | | — | D | ✓ | D | D |
| R | | | | — | ✓ | D | D |
| C | | | | | — | ✓ | ✓ |
| U | | | | | | — | D |
| F | | | | | | | — |

The private cumulative merge was built from all seven reviewed heads:

- Preserve both motion-storage and scaled-view CMake test targets.
- Preserve the SSBO constructor/friend declarations and the cache lifetime
  policy. Union the SSBO owner fixture with the cache regression helpers.
- In the SDK/SSBO capability helper, retain the public SDK includes and SSBO
  metadata; exclude the obsolete `driver_provider.h` route.
- Keep every topic's documentation in the standard package exactly once.

Runtime sources for the launcher, SR/FG, public SDK, motion shaders and shader
compiler match the existing integration. The deliberate differences are the
excluded unfinished target diagnostics, preservation of upstream's constant
far-plane input, and a comment encoding correction. The local combination
does not replace the published integration runtime or download.

## Coverage and limits

| Scope | Standalone evidence | Fresh cumulative evidence |
| --- | --- | --- |
| L | 34 Python tests; actual console/windowed freezes; Unicode path without development PATH | Real standard package, frozen imports, 27 TLS tests including local certificate failures |
| S | RX6000 routing/bounds/history and BDA/forced-SSBO checks; one RX6800XT/26.10.1 gameplay sample of the prior integration | Policy test plus SSBO/cache owner combination; no new RX6000 machine or vertex-draw certification |
| V | 20 complete pixel/alpha checks | 10 complete checks under active Core/Sync validation |
| R | 220 exact comparisons; unchanged fallback shader bytes | 110 exact comparisons, including fresh outputs from all four SR methods |
| C | 76 cache cases and 24 private SSBO combination cases | 48 BDA/SSBO cases: direct/threaded recording, cache off/on, retirement pressure, LRU, teardown and late reads |
| U | Nine CPU programs, 151 Python tests, 31 runtime cases, actual package/picker | All four methods and fresh RCAS replays; original SDK Vulkan findings reproduced |
| F | Six CPU programs, 58 Python tests, 31 runtime cases, actual package/picker | Both DLL load orders/versions, 1080p/4K, serial/overlap, three slots/256 frames, reset/switch/fallback and HUD |

The cumulative build passes 13 native CPU programs and 181 Python tests in
23 fresh processes. Its 78 GPU processes pass their functional assertions;
the validation sentinel deliberately reports one invalid buffer. Apart from
that sentinel and the known original SDK Vulkan findings, the exercised
Core/Synchronization paths report no VUIDs or hazards. This is not an overall
clean SDK Vulkan result, a D3D12 debug-layer result, or new FPS/low-percentile
or gameplay-quality evidence. The standard ZIP and relocated actual frozen
launcher pass pins/manifest/CRC, 13-language picker/preferences and separate
real packaged support probes. Cached DLSS binaries were retained; no NVIDIA
hardware was tested.

The SDK SR topic now includes the separately reviewed overlay-test follow-up
`6047f4e`: a PC-menu `runtime_restart` test stub and stable `###settings` window
lookup. Its documentation follow-up is `ba4dc22`. Both repairs already existed
in the old integration. The fresh build passes ten CPU programs, with assertions
enabled in the actual overlay test, and 151 Python tests. A fresh SR-only ZIP
and relocated 13-language frozen launcher pass; the previous ZIP is retained.

Production runtime sources and SDK pins remain identical to `580d935`, so its
31 GPU cases and known AMD findings are reused, not rerun or reclassified as
clean. Six merge previews against the updated U head retain the matrix above.
The original cumulative-runtime results still apply to its unchanged runtime;
the corrected test matches the validated private combination. The Draft text
is prepared locally; SDK PRs have not been submitted.

## Submission sequence and remaining checks

1. **First wave submitted: L [#140](https://github.com/Supermedo/bloodborne_pc/pull/140)
   and S [#141](https://github.com/Supermedo/bloodborne_pc/pull/141)**, each with
   its own changes and checks. S states which RX6000 sample exists and that the newly
   extracted candidate was not freshly played on RX6000. Request a targeted
   RX6800XT/cache-off-on check; do not call this whole-series certification.
2. **Second wave submitted: V [#142](https://github.com/Supermedo/bloodborne_pc/pull/142),
   R [#143](https://github.com/Supermedo/bloodborne_pc/pull/143) and
   C [#144](https://github.com/Supermedo/bloodborne_pc/pull/144)**. Preserve the
   reviewed merge resolutions as upstream advances. A short current-game check of debug view, sharpening,
   movement and resolution changes remains useful before promoting a new
   combined player build. No new performance gain is inferred from these tests.
3. **SDK topics separately**: first review the common API/package/headless
   foundation. F can progress independently of U's Vulkan findings, after a
   fresh current-build FG switch/movement/HUD check. U remains Draft while its
   original AMD shader conformance findings and current gameplay coverage are
   open. Its test-only follow-up is now published and checked.
4. Before each submission, refresh upstream, inspect existing PRs/equivalent
   patches and compare the resulting delta. Do not treat an automatic merge
   or matching ancestry as proof of coverage.
5. Add reliable native build/package CI as its own work package. The existing
   read-only documentation CI remains useful and is not a renderer gate.

Historical branches/tags, all 13 releases/assets, personal installation,
settings and saves remain intact. The local cumulative package is an audit
artifact, not a new test release. See [status](STATUS.md) and [workflow](WORKFLOW.md).
