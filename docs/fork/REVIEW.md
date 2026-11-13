# Topic review and integration

Updated November 13, 2026. Official base: Windows 1.9.8.5 [`1a4ba07`](https://github.com/rattensuppe/bloodborne_pc/commit/1a4ba07ad63abe793832f7f5188ca0bedcc88485).
[Status](STATUS.md) · [Workflow](WORKFLOW.md) · [Published builds](RELEASES.md).

## Included code and history

The [seven retained topic heads](STATUS.md#upstream-adoption-and-retained-topic-history)
are all ancestors of merge [`791ece2`](https://github.com/rattensuppe/bloodborne_pc/commit/791ece2173d23f8b38a312b2b5d377129ea2c0a5). Official 1.9.8.5 already contains their
implementation, including the independent SDK ML-FG prototype. PRs #140–144
remain open and #146 remains Draft; adoption in the release is not an individual
GitHub PR merge.

## Merge resolution

Four files needed explicit resolution against the previous integration:

- `vk_pipeline_cache.cpp`: retain the upstream shader-HLE include and new pipeline fixes.
- `bbport_launcher_win.py`: retain new borderless and language-module imports.
- `packaging/windows/package.sh`: retain the Offline Chalice Glyphs catalog and license.
- `scripts/pc_menu.py`: retain Korean texts alongside FSR selection/retry and ML-FG options.

All four resolved files exactly match upstream. A complete tree comparison
shows no runtime, test or packaging difference from official 1.9.8.5. Only fork
navigation, status and the clearly labelled historical tester note differ.
No topic was reapplied and no public history was rewritten.

The earlier 21 topic-pair checks belong to the November 8 review (19 automatic,
two explicit S/C and U/F resolutions). This sync uses the already adopted
upstream implementation, not another reconstruction of those pairwise merges.

## Current coverage

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

## Remaining review boundaries

- Original SDK Vulkan validation findings remain open; private shader changes are excluded.
- No new gameplay, HDR display, D3D12 debug-layer or RX 6000 machine check is claimed.
- One stock Windows DXGI test has the host-specific failure recorded in Status.
- SDK FG presentation review and UI-view cleanup remain separate work.
- Published test downloads and their tags/assets retain their earlier source scopes.

Keep the independent topic branches as provenance. Before further topic work,
compare actual upstream code rather than reapplying equivalent patches by hash.
