# FSR SDK 2.4 upscaling on Windows

For development only, `BB_FSR24_DEBUG=1` enables AMD's SDK input checker on
both 4.1.2 methods. It is off by default; leave it off for performance and
image-quality comparisons. Both methods log SDK messages and query the
SDK's total/aliasable GPU memory once after each context creation. These
figures describe the SDK effect, not total process VRAM. An unsupported
memory query is logged and does not disable upscaling.

On AMD devices with the SDK Vulkan DLL present, device creation enables the
advertised Cooperative Matrix and Float8 features plus `computeFullSubgroups`.
Float8 requires both Float8 feature bits and Cooperative Matrix support.
Absent features remain disabled: they are optional accelerators, not a new
RX-series restriction. Enabled bits are logged, without claiming which ML
model the SDK uses. Vulkan validation and real GPU comparisons remain required.

FSR 4.1.2 uses the public FidelityFX API from AMD's signed SDK 2.4 effect
libraries for both AMD Vulkan and AMD DX12. The older 4.1.1 methods remain.
The DX12 method still uses the existing Vulkan/DX12 texture and fence bridge.
Image preparation, color-space flags, motion vectors, jitter, host RCAS and
motion-cache behavior are unchanged by this integration.

`bin/fsr24/amd_fidelityfx_upscaler_vk.dll` and
`bin/fsr24/amd_fidelityfx_upscaler_dx12.dll` are pinned to AMD SDK 2.4.0,
commit `7539ae8b6568ad29ed78a753404351daf3b5cca7`. Packaging checks their exact
size and SHA256; `packaging/windows/fetch_fsr24_upscaling.py` restores the
original files. The SDK license is included in the Windows package.

The effect DLLs export the five public FFX functions. We call these exports
from their explicit module handles instead of using the central loader:
AMD's central loader first reuses a DLL of that name already in the process,
which might be the retained 4.1.1 DX12 runtime. Different folders alone would
not prevent that collision. The requested module path is checked after loading.
No internal driver exports, memory patches, model overrides or shader rewrites
are used by these 4.1.2 paths. The existing frame-generation path is separate.

Support checks enumerate the versions on the selected adapter. Creation uses
the exact queried provider ID and verifies the active context version and ID;
FSR 3 is never silently accepted as FSR 4.1.2. Missing package files, failed
runtime loading and unavailable GPU/driver providers remain distinct failures.
Launcher and overlay keep the four existing choices and saved preferences.

The upscaling API headers are scoped to its translation units. The separate
[SDK ML-FG change](SDK24_ML_FG.md) retains the same upscaling code and adds the
current FG runtime independently. FSR Lite and new pacing are not integrated.

For local comparisons retain the original 1.9.5+RCAS/cache build and use the
same copied save, camera and settings. Isolated tests establish API operation,
handoffs, switching and finite output; they do not establish movement quality,
absence of ghosting, or a game-performance improvement.
