# Testing Guidelines

## Ownership And Case Boundaries

- Use the existing Engine-independent `Arcadia.Testing` runner; do not introduce an external test framework. Only test executables and testing-specific components may depend on it, never production libraries.
- Register test products with `BeginProduct(... test)`, explicit source/header entries, PRIVATE dependencies and `EndProduct`. Enable the directory from its owning parent. PDLC fixture generators and the standalone discovery fixture are build/infrastructure tools, not missing runner suites.
- CMake-script regressions also use `BeginProduct(... test)` / `EndProduct`, never module-level `add_test`. Set `.TestScript` to the script, `.TestArguments` to its `-D` arguments and `.TestDependencies` to required build targets. The macros create a buildable script target and register CMake -P with the same metadata policy, without a dummy executable. Script tests manage their own private fixtures and remain serial; they do not link libraries or use case discovery/TestIsolated.
- Case names describe behavior or regression intent. Use executable `--list` as the authoritative inventory rather than maintaining a second CMake case-name table. Removed selectors have no duplicate workload aliases; no arguments remains the whole-suite invocation.
- Split only independently bounded callbacks. Preserve fixture loops, ordering, ARMS lifetime handling, `safeExecute`, process/jump-target ownership and expected-error recovery. Lifecycle-coupled cases such as PDLC LineMap/SyntacticalAnalysis and ARMS lifetime tests must not be mechanically split.
- Every case must exercise meaningful behavior. Remove duplicate/no-op workloads only after checking setup, state, assertions and unique regression coverage. Renaming helpers must not silently change their test content.

## Assertions And Expected Errors

- Keep simple Ring1 assertions available. Opt into `Arcadia_Tests_assertEqualInteger64`, `Natural64`, `Real64`, `Size`, `Status`, `CxxString` or `Bytes` when compared values help diagnose failures; their declarations/contracts are in Ring1 `Tests.h`.
- Choose the appropriate signed/unsigned domain; Real64 is exact equality, not a tolerance helper. Bytes compares explicit lengths including embedded NULs; CxxString compares NUL-terminated strings. Failure reports include the selected case and source location.
- Catch expected jumps inside the callback, capture the actual status before clearing it, and assert the intended error rather than any failure. Preserve raised values and roots until inspection; verify caller-stack restoration, including depths above 255 where relevant. `safeExecute` alone is not an expected-error oracle.
- PDLC diagnostic negatives use `PDLC/Tests/Support` and reviewed static `NegativeCases.i` contracts. See `PDLC/AGENTS.md` and that support library's README. Do not derive expected results automatically from unreviewed current output.
- Shared checkers need deliberate mismatch regressions as well as passing examples. Where practical, disable the guarded check temporarily and confirm the regression fails; restore it before final validation.
- Prefer test-only mutants or external source copies over modifying production files for mutation checks. Temporary working-tree defects can be captured by concurrent maintainer staging. If an in-place check is unavoidable, restore only your own exact changes, inspect both working-tree and staged diffs, and report any captured mutant without altering the maintainer's index. Rebuild the restored target before final validation; an old mutant executable is not evidence about restored code.

## Managed Lifetime Regressions

- Root only the intended owner across forced collection. Drop independent fixture roots that could keep the tested dependency alive and mask a missing visit edge; test probe callbacks must not accidentally trace borrowed captures.
- Observe dependencies with rooted WeakReference objects rather than keeping the dependencies strongly rooted for inspection. After collection, check the weak value's tag before accessing the target. Verify both retention while the owner is rooted and release after its root is removed.
- For constructor callback tests, observe the unfinished receiver as well as initialized managed fields: the currently installed superclass visitor may not trace derived state. Collect inside the actual callback, test success and failure, and verify caller-stack contents/depth, temporary-root release and any explicit dependency counts. Honor resource unload/unlink before expecting finalization.

## Discovery And CMake Metadata

- Set `${this}.WorkingDirectory`, `.TestArguments`, `.TestEnvironment` and `.TestProperties` before `EndProduct`; later assignments cannot update registered tests. Relative asset paths normally resolve from the suite binary directory, not its Debug/Release executable subdirectory.
- Set `.TestLabels` as a CMake list and `.TestTimeout` in seconds before EndProduct; TestPolicy.cmake supplies resource/coverage labels and a 60-second ordinary watchdog. Labels never grant parallel permission. Keep explicit 180/300-second nested-build/consistency limits unless measurements justify changes.
- Direct Testing consumers are discovered at CTest load time as `Executable.Case`, without an additional suite run. `.TestDiscovery FALSE` explicitly retains a suite fallback; the dedicated runner self-tests intentionally use this to remain one workload, separate from consuming-suite discovery/integration tests.
- `--list` must be callback-free and avoid fixture/backend/font setup before dispatch. Preserve optional leading arguments such as FontIO's font path. Graphics preflight/cleanup and existing skip codes remain execution-only behavior.
- Metadata is configuration-specific; pass `-C Debug` (or the built configuration) with multi-config generators. Discovery must not execute at configure time, silently omit unbuilt targets, or hide invalid output behind a stale cache.
- Keep `DiscoveryConsistency`, `DiscoveryInfrastructure`, `DiscoveryIntegration` and `FixtureIsolation` coverage when changing registration. Check actual `ctest -N`/JSON names, commands, prefixes, environments, working directories and properties. CTest JSON may omit false `RUN_SERIAL`; absence means false, not explicit serial protection.

## Fixtures And Parallel Safety

- Discovery is not permission for parallel execution. Serial is the default; opt into `.TestParallelSafe TRUE` only after a writer/global-state/backend/lifetime audit and repeated concurrency checks. Preserve the existing audited opt-ins; see `Runtime/Testing/Isolation.md` rather than inferring safety from named cases.
- Writer cases use `.TestIsolated TRUE` and, when needed, `.TestFixtureDirectory`/`.TestFixtureDestination`, configured before `EndProduct`. Copy pristine source fixtures, never a previously generated build-tree Assets directory. Outputs and temporary paths belong outside the checkout.
- Isolation allocates unique case/configuration/CTest-session directories, redirects TMP/TEMP/TMPDIR and retains workspaces for diagnosis. CTest listing also stages directories. `ctest --repeat` reuses a session workspace; separate invocations test fresh staging.
- Direct executable invocation uses caller cwd and is not automatically isolated. Use CTest or a fresh private fixture copy for writers; never launch concurrent writers in a shared cwd or run fixture generators against the checkout.
- Keep generated/staged compiler fixtures configuration-specific. For generated `.c.g` paths containing `$<CONFIG>`, use `PDLC/Tests/GeneratedSources.cmake`: LANGUAGE properties need concrete configuration paths. Inspect real compile entries, not just IDE visibility.
- Asset-copy targets must depend on declared file outputs, not destination directories created by those commands. Ninja rejects a missing directory with no output rule on a cold build; pre-existing VS/build-tree directories can mask this. DiscoveryIntegration's AssetFixture verifies cold copies, inherited/nested assets, directory recreation and incremental inputs with the real CopyProductAssets helper.

## Verification And Evidence

- Use the external build trees and CMake version documented in root `AGENTS.md`. Build affected targets and generator dependencies, run focused CTest selections, then broader gates appropriate to the changed shared infrastructure.
- For CLI changes, directly verify no arguments, `--case`, `--list`, unknown names and malformed arguments. For fixture changes, verify pristine repeatability and unchanged source inputs before parallel stress runs.
- Record platform, compiler, configuration and exact commands. Distinguish passed, failed, skipped and not run; a build is not an interactive smoke test, and source inspection is not runtime verification. Do not claim an unresolved intermittent failure is fixed because repetitions passed.
- Return `Arcadia_Testing_Skip` (77) with a reason for an unavailable optional prerequisite; CTest test products register SKIP_RETURN_CODE 77. Never turn assertions, corrupt inputs or post-preflight runtime failures into skips. All-skipped suites return 77, mixed pass/skip suites succeed, and any failure dominates. Preserve GLSL preflight/cleanup. FontIO CTest uses the existing OFL Noto fixture, missing files skip and corrupt fonts fail.
- CI uses explicit build directories and absolute JUnit paths, uploading the exact generated report even after failure. Linux has a focused ASan/UBSan runtime gate under Runtime/Testing/Tests/Sanitizers; an OFF harness-validation build on Windows is not a sanitizer/Linux pass. See Documentation/Testing.md for reproduction and scope.
- Detailed API/discovery contracts: `Runtime/Testing/README.md`; writer/state/backend inventory and retention: `Runtime/Testing/Isolation.md`. Keep those documents consistent with source; leave session logs, hashes and completion history in task records.
