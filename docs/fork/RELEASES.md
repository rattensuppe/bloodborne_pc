# Test build history

[Status](STATUS.md) · [Workflow](WORKFLOW.md) · [All downloads](https://github.com/rattensuppe/bloodborne_pc/releases)

Updated October 31, 2026. All 13 existing Pre-Releases are retained.
Branch reorganisation creates no new binary package and changes none of
these tags or assets. Each linked release contains its original notes,
Windows ZIP, sources and checksums.

## Current package

[Windows 1.9.7 SDK 2.4 prototype.1](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.7-sdk24-prototype.1)
uses runtime `2180512` on official base `b480526`. It contains public SDK
FSR 4.1.2 Vulkan/DX12, SDK ML-FG 4.0.2, retained earlier methods, RCAS/
cache optimisations, RX 6000 SSBO and the scaled SDR vector view.

Build, isolated GPU and frozen-package checks passed. A fresh 1.9.7 game
comparison remains open; original SDK Vulkan validation findings remain.
[Component status and limits](STATUS.md#validation-and-remaining-limits).

The ZIP has an additional prototype README and RX 6000 notes. The later
`2a8ec7e` source commit records the download link and future standard
copying of those notes; runtime program files are unchanged.

## Published history

| Release | Official base version | Tagged source |
| --- | --- | --- |
| [v1.9.7-sdk24-prototype.1](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.7-sdk24-prototype.1) | 1.9.7 | [`2180512`](https://github.com/rattensuppe/bloodborne_pc/commit/21805126c2f7b93d853e17bb5535cb0111a5c4f7) |
| [v1.9.6-sdk24-prototype.3](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.6-sdk24-prototype.3) | 1.9.6 | [`3449119`](https://github.com/rattensuppe/bloodborne_pc/commit/3449119f07446085ce763a40f51cbfbfe8867861) |
| [v1.9.5-sdk24-prototype.2](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.5-sdk24-prototype.2) | 1.9.5 | [`bd28740`](https://github.com/rattensuppe/bloodborne_pc/commit/bd28740e5e664aeb134a4443a6442db2cf5ee245) |
| [v1.9.5-sdk24-prototype.1](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.5-sdk24-prototype.1) | 1.9.5 | [`591856f`](https://github.com/rattensuppe/bloodborne_pc/commit/591856f318223fdff0a760677d7aece1664329a4) |
| [v1.9.4-fsr412-fgml402-test.1](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.4-fsr412-fgml402-test.1) | 1.9.4 | [`ca30000`](https://github.com/rattensuppe/bloodborne_pc/commit/ca3000063470f2e81c185f903ae21674a3b309b6) |
| [v1.9.2-fsr411-fgml401-test.3](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.2-fsr411-fgml401-test.3) | 1.9.2 | [`ac2aa29`](https://github.com/rattensuppe/bloodborne_pc/commit/ac2aa290f0a4393d4a9cd8cd0f757af81a27c01f) |
| [v1.9.2-fsr411-fgml401-test.2](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.2-fsr411-fgml401-test.2) | 1.9.2 | [`fd60179`](https://github.com/rattensuppe/bloodborne_pc/commit/fd601791ca8404d57f4cf974122abacc697f846a) |
| [v1.9.2-fsr411-fgml401-test.1](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.2-fsr411-fgml401-test.1) | 1.9.2 | [`a1473da`](https://github.com/rattensuppe/bloodborne_pc/commit/a1473dad94ed604a32465d03bdb68983b5317342) |
| [v1.9.1-fsr411-fgml401-test.2](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.1-fsr411-fgml401-test.2) | 1.9.1 | [`1a7028a`](https://github.com/rattensuppe/bloodborne_pc/commit/1a7028a658931ce9f1b0fe75e92594d49d56a02b) |
| [v1.9.1-fsr411-fgml401-test.1](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.9.1-fsr411-fgml401-test.1) | 1.9.1 | [`58fa44f`](https://github.com/rattensuppe/bloodborne_pc/commit/58fa44f3264dff656cb04fe0129a70e8ab5feb23) |
| [v1.8.2-fsr411-test.3](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.8.2-fsr411-test.3) | 1.8.2 | [`a049abe`](https://github.com/rattensuppe/bloodborne_pc/commit/a049abeb22043712d0d0f731a187d2d3790b616d) |
| [v1.8.2-fsr411-test.2](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.8.2-fsr411-test.2) | 1.8.2 | [`7c0c2e0`](https://github.com/rattensuppe/bloodborne_pc/commit/7c0c2e0f57479f777a9843b3ffa93627703e4645) |
| [v1.8.2-fsr411-test.1](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.8.2-fsr411-test.1) | 1.8.2 | [`a0f53a1`](https://github.com/rattensuppe/bloodborne_pc/commit/a0f53a1de2d1deb66835c692e2540ea6d67cb6dc) |

Older releases describe their own implementation and validation level.
They do not inherit the SDK migration or test coverage of newer builds.
The release tag, source archive and manifest identify the published build;
the current integration head is a separate moving development reference.
