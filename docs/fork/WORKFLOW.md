# Fork workflow

This fork keeps the official source, combined experiments and upstream review
topics distinct. [Status](STATUS.md) is the source of current component and
validation information; [build history](RELEASES.md) maps published test packages.

## Branch roles and names

| Branch | Role |
| --- | --- |
| `windows` | Exact official Windows mirror; controlled fast-forward updates only |
| `integration/windows` | Combined experimental source and fork documentation |
| `fix/<topic>` | Focused correction for upstream review |
| `perf/<topic>` | Focused optimisation and its correctness/performance evidence |
| `feat/<topic>` | Feature or API migration with its required tests |

**Do not put the changing bbport base version in new branch names.** Record it
in release tags, manifests and the status page. `sdk24` identifies a specific
SDK migration; `fsr41` and `rx6000` identify technical scope. They are independent
of the bbport version.

`codex/prototype-195-sdk24` is the historical predecessor of
`integration/windows`. Keep it and the merged-PR/image branches at their existing
heads. Old documents and PRs contain branch-based links; raw file URLs are not
automatically redirected by a rename.

The fork default is `integration/windows`. Active rules protect the two primary
branches from deletion/forced history updates, freeze the historical branches
and prevent version-tag updates/deletion. New review topics remain writable.

## Updating upstream

1. Check the remote heads, working tree and parallel work first.
2. Fetch the official `windows`; confirm the existing mirror is its ancestor.
3. Fast-forward the fork's `windows` to that exact official commit. Keep personal
   changes out of the mirror; investigate divergence instead of force-pushing.
4. Integrate the official update into `integration/windows`, resolving semantics
   as well as text conflicts. Record the base commit and changes to tested paths.
5. Run checks appropriate to the actual delta. Update the status page without
   relabelling an older package as a new build.

GitHub's generic “Sync fork” action is not the integration procedure. The fork's
default branch may show combined changes; an upstream PR must explicitly target
`Supermedo/bloodborne_pc:windows`.

After upstream accepts a topic, compare the final patch, update the mirror and
integrate upstream. Squash merges can change commit identities. Do not reapply
equivalent code just because the old topic head is not an ancestor.

## Focused changes and reviews

- Base review topics on the current official source. If a genuine dependency
  exists, keep it small and make its PR/compare link explicit.
- Preserve authorship and provenance. Separate functional commits from fork-only
  navigation, release links and administration.
- SSBO and cache changes share resource ownership; test the fix on the original
  lifecycle and the combined result on the optimised lifecycle.
- The scaled motion view and combined output share the upscaler. Protect late
  GPU reads in each independent version, not only in the combined branch.
- SDK SR and FG share packaging utilities and headers. Isolate a common base
  rather than importing the whole other feature or duplicating SDK headers.
- Build and test the exact extracted topic. A combined prototype test is not a
  substitute. Include only evidence relevant to the submitted change.
- Start with at most two small, ready PRs. SDK SR remains a Draft while relevant
  validation/gameplay questions are unresolved.

PR descriptions explain the trigger/problem, resulting behaviour, meaningful
validation and material limits. Keep short linked commit hashes, dependencies
and comparison links. Detailed measurements belong in the feature documents;
enumerating unrelated upstream changes obscures the actual review.

## Tests and measurements

Use independent test profiles and copied saves. Never point a test at an active
player save, installation INI or shared launcher settings. Verify package runtime
modules as well as Python source. Classify missing files, unavailable hardware/
driver providers and runtime failures separately; a session fallback must not
overwrite the user's saved preference.

Record the exact source/base, dependency pins, GPU/driver, settings, method and
limits for each test. Existing upstream failures need a pristine-base comparison.
Keep development input checking and recording disabled for normal image and
performance comparisons.

Performance comparisons use the same copied Warehouse save and its **unchanged
loaded camera**, identical settings, FPS cap/VSync off, normally 4K/Quality.
Stationary runs use 10 seconds warmup plus 25 seconds capture, twice per case,
with automatic method switching. Separate game FPS from output FPS with FG.

The movement route is Warehouse → lantern → directly back, without using the
lantern, stopping or fighting. Repeat as needed and record warm/cold cache state
and run order. Report mean FPS, 1% and 0.1% lows with the calculation method;
very short stationary runs have limited tail samples. User visual checks cover
movement, HUD, ghosting, edges and crashes separately.

Documentation/branch-role changes do not require rerunning GPU benchmarks when
the complete runtime source is demonstrably unchanged.

## Releases

Keep all published tags, assets and historical downloads. Names retain the
bbport base, for example `v1.9.7-sdk24-prototype.2` and
`Bloodborne-Windows-1.9.7-SDK24-FSR412-FGML402-prototype.2.zip`.

1. Fix the source and packaging commit, submodules and dependency pins.
2. Build/test that source; package through the standard supported path.
3. Extract the ZIP and verify the real frozen launcher, including a relocated
   Unicode path. Check manifests, licenses, bundled runtime files and hashes.
4. Include matching sources and document any packaging-only additions. Check
   that every local README link points to an included file.
5. Upload as Draft; verify the tag, asset list, sizes and server SHA-256 digests.
6. Publish a Pre-Release with concise features, checks and known limitations;
   verify public downloads and links afterwards.

Do not replace a published ZIP or move its tag: corrections get a new prototype
number. Consider immutable releases for future publications after validating
the Draft procedure. Existing releases are preserved. Only one session publishes
at a time; a mirror push never automatically publishes an experimental build.

## Automation and safeguards

Protect mirror/integration branches and release tags against deletion/force
push while permitting the intended normal updates. Upstream has merge commits;
do not require linear history on the mirror or integration. Avoid mandatory
outside approvals when no reviewer is available.

Require status checks only after they reliably check the **actual topic SHA**.
An integration-only green workflow does not validate a different topic commit.
Hosted Windows runners can build and check CPU/frozen/package paths. GPU and
gameplay checks remain separately documented.

The initial hosted `Fork documentation` check only verifies overview links and
anchors on the integration SHA. It has read-only permissions, a pinned checkout
action and no public-PR trigger. It is intentionally not a required status check;
native build/package automation is a later step.

Never automatically execute untrusted public-PR code on a personal NUC runner.
Keep credentials out of logs and build artifacts. Fork-only administration/CI
must not accidentally become part of an upstream functional PR.

## Coordination and history

Use one working copy per task, compare expected remote heads before pushes and
avoid changing another session's checkout or shared configuration. Keep current
builds and research on fast local storage; archived raw data can be restored
from verified archives when needed.

Before removing any historical branch, audit its use in PRs, releases, images
and docs; preserve reachable commits and a verified backup, then check links
again. Existing release tags and downloads stay. No cleanup is a reason to
rewrite public history.

Local camera/depth ghosting probes, F10 captures, Lite experiments and deferred
B2 work remain outside the integrated release unless explicitly reviewed and
accepted as separate changes.
