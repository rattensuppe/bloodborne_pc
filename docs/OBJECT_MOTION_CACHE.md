# Experimental motion-vector cache

In the Windows launcher's **Graphics** page, **Motion-vector cache
(experimental)** enables reuse of object-motion render targets. It is **off by
default**, including for existing profiles. With it off, the renderer uses the
existing single-target allocation path.

The checkbox is available with **FSR 3.1, FSR 4 or FSR 4.1 (4.1.1/4.1.2)** and
**Object motion vectors** enabled. It works with both Vulkan and DX12 methods. Its saved choice
is retained while another upscaler or disabled object motion makes it unavailable.
Both normal PLAY and shortcuts apply the choice at the **next game start**.
Changing the in-game upscaler retains that process's startup cache mode.

The **Insert / L3+R3** in-game overlay provides the same experimental checkbox.
It retains its choice while unavailable and marks a changed startup mode for
**Apply and restart**. The launcher follows saved overlay changes while open.

The option and hint are translated into all 13 launcher languages. The boolean
`object_motion_cache=0|1` lives in `bbport.ini`, shared by the launcher and overlay.
Existing `settings.json` preferences migrate when that INI key is first absent;
an explicit INI value takes precedence. The launcher removes the legacy JSON
keys when saving. `run.py` refreshes the derived `BB_OBJECT_MOTION_CACHE=0|1`
at each launcher-managed start, including **Apply and restart**. The saved
preference remains available when another upscaler temporarily disables its use.

## Implementation and tradeoff

Some scenes alternate full- and half-resolution object-motion targets within a
frame. The tested sewer view alternated 2560x1440 and 1280x720. The original
single-target path finishes the GPU and reallocates on each size change.

The cache keeps an active target and up to three inactive sizes, with each
image, view, dimensions and layout. A revisited size reuses its target with the
existing clearing and validity rules. The motion-vector calculations and selected image-quality settings stay
the same. The renderer is shared by upscalers that use object motion; the
Windows checkbox exposes the experiment for FSR.

### Local prototype: LRU and bounded retirement (25 October 2026)

The local candidate replaces the least recently used inactive target, using a
serial that also orders uses within one frame. Replacing a target retains its
image and view until their last scheduler tick has completed. Completed targets
are collected at frame start and on subsequent eviction. No global GPU finish
is needed for an ordinary eviction; returning to a cached size still discards
its prior contents, preserving the existing clear/read behavior.

Retirement is limited to **two images and 128 MiB** of actual image allocations,
in addition to the four reusable targets. At either limit, the cache waits for
the oldest pending retirement. A single larger target waits for its own last
use instead of exceeding that limit. A failed new allocation first drains
pending retirements and retries; teardown retains its existing completion wait.
These are limits on temporary image allocation bytes, not on driver heap/block
reservation or the total game's VRAM use.

This common object-motion path covers all FSR versions/backends when object
motion and the startup cache mode are active. Cache off retains the single-target
path, and the saved preference, default, shaders and motion-history math stay the
same. The local change was checked with pixel readbacks, LRU reuse, a GPU submission
held by a timeline semaphore, count/byte pressure and pending-work teardown.
No new in-game FPS or stutter improvement has been measured yet.

Resident targets consume additional VRAM. The option remains experimental and
off by default while moving gameplay, area transitions, frame pacing and
memory-pressure comparisons are collected. For comparisons, restart at the same
saved position with identical settings, follow the same route/camera movements,
and record per-frame timing and VRAM along with FPS.

## Evidence and reproduction

The [FSR Windows report](FSR411_WINDOWS.md#cache-only-ab-in-the-sewer-separate-run)
contains the static-scene cache-only A/B and a separate four-method comparison
with the cache enabled. Those recorded numbers describe the experimental
enabled configuration. They come from the 16 October 2026 test integrations;
the checkbox build receives separate correctness and packaging checks.

For direct engine launches, `BB_OBJECT_MOTION_CACHE=1` enables bounded reuse.
Unset, `0` and other values select the existing allocation path. The setting is
read once when the object-motion renderer starts, and its startup log prints
`Object motion: target cache on` or `off`. Use the checkbox when starting through
the Windows launcher.

In a configured CLANG64 build:

```bash
ninja -C out/gpu bb-probe object-motion-cache-test motion-history-test
out/gpu/motion-history-test.exe
for mode in 0 1; do
    for scene in tiny game churn; do
        BB_OBJECT_MOTION_CACHE=$mode out/gpu/object-motion-cache-test.exe "$scene"
    done
done
python -m unittest discover -s tests -p test_object_motion_cache_launcher.py -v
```

The GPU test verifies every readback pixel and validity flag across repeated,
full/half-size and seven-size eviction sequences. VMA allocation counts verify
one live target with the cache off and up to four reusable targets with it on,
plus bounded targets pending GPU completion in the local candidate. Repeat with the
variable unset and with an invalid value to check the default path.

## Published test.2

[test.2](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.8.2-fsr411-test.2)
is the preserved earlier build with automatic caching. Its tag, downloads and
recorded results remain fixed. The published
[test.3](https://github.com/rattensuppe/bloodborne_pc/releases/tag/v1.8.2-fsr411-test.3)
contains the default-off comparison control on Windows 1.8.2. The current PR
incorporates upstream Windows 1.9 and keeps this control alongside the upstream
frame-generation settings.

## Local 1.9.5 beta port

The 27 October 2026 port retains these limits and existing validity rules.
A fresh Windows build and 36 isolated Vulkan cache cases passed, including
full readbacks, count/byte pressure, collection and pending-work destruction.
This common cache applies to FSR 4.1 Vulkan/DX12 when object motion and the
startup cache switch are active. No new gameplay or FPS claim is made here.

## Standalone review on Windows 1.9.7

The `perf/motion-target-cache` topic extracts the LRU/retirement policy from
`d868e1f` and the late-reader correction from `906ffa9` onto official `b480526`.
The latter records a new scheduler tick when a valid image or view is obtained
after a provider finishes earlier work. Retired images and views therefore stay
alive through their actual last read. The two changes remain separate commits.
Shaders, motion-history calculations, providers, launcher/overlay settings and
the default-off preference are unchanged. SDK migration, RX 6000 SSBO routing,
RCAS/output fusion and optional motion-target diagnostics are separate topics.

On RX 9070 / Adrenalin 26.10.1, the native renderer/capability build and three
relevant CPU tests pass. There are 76 isolated Vulkan cache cases in direct and
threaded recording modes: 60 full pixel/validity readbacks agree across cache
off/on, unset/invalid values and the FSR 3/4/4.1.1/4.1.2 selectors; 16 cases
cover LRU reuse, blocked GPU submissions, count/byte limits, oversized targets,
frame-start collection, teardown and late image/view reads. Allocation counts
return to their starting value after the lifetime tests. Selector coverage does
not dispatch the upscalers or frame generation.

Core and Synchronization validation are explicitly confirmed active. The
unchanged headless instance reports
`VUID-vkCreateDevice-ppEnabledExtensionNames-01387`: it enables a device swapchain
extension without the required instance surface extension. The cache-off control
reports the same finding; no additional validation errors or synchronization
hazards remain. The existing pixel fixture needed a transfer-read barrier after
`PrepareRead`, whose production contract exposes shader reads. That barrier and
the recording-mode coverage are test-only changes. Fixture assertion failures
are routed to stderr on Windows.

The unchanged launcher cache suite has one stale shortcut mock: it intercepts
the old FSR 4.1.1 probe, while startup now queries the whole FSR 4.1 family.
Seven original tests pass and that failure is reproduced with untouched base
launcher/test files. All eight pass with a corrected private mock; this unrelated
test correction is not shipped or counted as a pass of the original suite.

The standard local Windows ZIP is checked for manifest/CRC, matching cache notes
and actual frozen-launcher imports. Cached DLSS files are retained; no new
NVIDIA or RX 6000 hardware test is claimed. Allocation-failure recovery was
reviewed, without forcing a real out-of-memory condition. No new gameplay,
area-transition, frame-pacing or FPS benchmark of this extracted branch is
claimed. Earlier combined-prototype measurements retain their own source and
scene context.
