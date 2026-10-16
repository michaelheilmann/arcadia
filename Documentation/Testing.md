# Testing Arcadia

## Architecture

Arcadia tests are ordinary C executables using the Engine-independent
[Arcadia.Testing](../Runtime/Testing/README.md) runner and Ring1 assertions.
Callbacks retain their own runtime/process/jump-target and fixture boundaries.
CTest discovers `Executable.Case` names from callback-free executable `--list`
output at test time; no extra whole-suite run duplicates those cases. Grouped
fixture loops and shared-lifetime cases are intentionally not recursively split.
The runner's self-tests remain one suite-level CTest workload.

Production libraries must not depend on Testing. PDLC test generators are build
tools, not runner suites. Root `AGENTS.md` documents protected hand-written-C
components; testing work does not authorize production PDL migrations.

## Build And Run

Use CMake 3.29 or newer and an external build directory, never the checkout.
Windows / Visual Studio 2022, from the repository root:

```powershell
cmake -S . -B C:/develop/Arcadia/Build/x64 --preset x64
# Only when the build tree's .Dependencies directory does not yet exist:
cmake --build C:/develop/Arcadia/Build/x64 --target Arcadia.InstallDependencies
cmake --build C:/develop/Arcadia/Build/x64 --config Debug
ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug --output-on-failure
```

On Linux, configure outside the checkout (substitute absolute paths):

```sh
cmake -S /path/to/Repository -B /path/to/Build -G Ninja -DCMAKE_BUILD_TYPE=Debug \
  -DArcadia.Engine.Visuals.Implementation.OpenGL4.Enabled=TRUE \
  -DArcadia.Engine.Audials.Implementation.OpenAL.Enabled=TRUE
cmake --build /path/to/Build
ctest --test-dir /path/to/Build --output-on-failure
```

Install the platform dependencies from the build guides/CI configuration first.
Arcadia.InstallDependencies is Windows-only; Linux uses system development packages.
Linux single-config builds use CMAKE_BUILD_TYPE; multi-config generators require
CTest `-C` matching the built configuration. CTest does not build executables or
their fixture generators for you. An unbuilt/missing configuration is a visible
failure placeholder, not an empty passing inventory.

### List And Select

```powershell
ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug -N
ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug -N -V
ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug --show-only=json-v1
ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug -R '^Arcadia.Engine.Tests.Backends.AudialsBackendContext$' --output-on-failure
ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug -L requires-font --output-on-failure
```

Direct executable options (add `.exe` on Windows) are `--list`, `--case NAME`, or
no arguments for the full suite. Use actual `--list` output, not a static inventory:

```text
Arcadia.Ring1.Tests.SafeAdd --list
Arcadia.Ring1.Tests.SafeAdd --case Natural64
Arcadia.Media.Tests.PixelBuffer --case FormatMetadata
Arcadia.PDLC.Tests.Diagnostics --case ExpectedDiagnostics
```

Run fixture-dependent executables from the configured suite binary directory,
not its Debug/Release executable subdirectory. CTest `-N -V`/JSON reveals the exact
command, leading arguments, environment and working directory. Unknown selectors
and malformed arguments fail. No arguments remains the way to run a whole suite;
retired selector aliases are not reintroduced as duplicate workloads.

## Fixtures, Isolation And Debugging

Writing cases use pristine source fixture copies in unique external build-tree
workspaces partitioned by case/configuration/CTest session. TMP/TEMP/TMPDIR point
there too. They are retained for inspection, including failed/interrupted runs.
`ctest -N`/JSON listing also stages private directories. `ctest --repeat` reuses the
current session workspace; separate invocations verify pristine staging anew.

Direct executable invocation uses caller cwd and does **not** allocate a private
workspace. For a writer, use CTest or a fresh private fixture copy. Do not run
concurrent writers in a shared cwd, generate against the checkout, or copy already
generated build Assets as a supposedly clean baseline. See
[Isolation.md](../Runtime/Testing/Isolation.md) for the writer/state/backend audit
and retention/cleanup policy. Build-time PDLC staging is configuration-specific.

All cases remain serial unless their suite explicitly passed a resource audit and
opts in with TestParallelSafe. `ctest -j 8 --schedule-random` still respects these
serial flags; labels and discovery alone never grant parallel safety.

For a failure, use `--output-on-failure` or `-V`, then inspect
`<build>/Testing/Temporary/LastTest.log` and retained private working directories.
Rerun the selected case with the recorded cwd, arguments and environment under a
debugger. Do not erase failure workspaces before collecting evidence. Ring1 typed
assertions print expected/actual values and selected case/file/line. PDLC negatives
use [exact diagnostic contracts](../PDLC/Tests/Support/README.md); arbitrary failure
does not prove the intended negative behavior.

## Skip Policy

- `EXIT_SUCCESS` means assertions/behavior actually ran successfully.
- `Arcadia_Testing_Skip` / process code **77** means an unavailable optional
  prerequisite. Print a reason; CTest test products set SKIP_RETURN_CODE 77.
- Any other nonzero callback result is failure. Assertion failures, corrupt
  inputs and errors after a successful preflight must never be converted to skips.
- The runner logs each SKIPPED case and continues. A failed case dominates suite
  status; all-skipped returns 77; mixed pass/skip returns success. CTest runs cases
  individually, preserving per-case skip results in output and JUnit.
- Discovery always remains setup-free and succeeds even when execution would skip.
  Existing Windows GLSL driver preflight/cleanup and code-77 semantics are retained.

FontIO CTest cases use the existing OFL-licensed Noto Sans asset under
`.Website/assets/noto-new/Noto_Sans/`, without copying/downloading a new font.
Override it at configure time with `-DARCADIA_FONT_TEST_FILE=/absolute/font.ttf`.
Direct execution still accepts an optional leading path and, without one, checks
known system fonts. An explicit path suppresses fallback; a missing/unreadable file
skips, while an existing but corrupt font fails. For example:

```text
Arcadia.FontIO.Tests.CreateBitmapFontTests /absolute/font.ttf --case CreateBitmapFontTests
Arcadia.FontIO.Tests.CreateAtlasBitmapFontTests /missing/font.ttf --list
```

## Labels And Timeouts

`CMake/TestPolicy.cmake` provides defaults to discovered and fallback products:
`unit` for focused core behavior, `integration` for audited multi-component/file
consumer suites, plus `requires-font` and `requires-graphics` for external resources.
`infrastructure` identifies testing harnesses; `slow` identifies consistency/nested
build checks, not all tests in a module. Labels are filters, not scheduling policy.

Ordinary observed case durations were below one second; a generous **60-second**
watchdog permits slower machines while bounding hangs. Discovery has a separate
30-second limit. Nested configure/build integration gets **180 seconds** and full
inventory consistency **300 seconds** (the latter has taken about 40 seconds here).
Review measurements before tightening limits. Set TestLabels/TestTimeout explicitly
before EndProduct for new workloads; do not put multi-label lists in the flat
TestProperties list.

## Adding A Suite

1. Add its directory from the owning CMake parent. Use BeginProduct(test), explicit
   OnSourceFile/OnHeaderFile registration and PRIVATE Testing/Ring1 dependencies.
2. Implement named `int runCase(void)` callbacks with meaningful assertions and
   existing safeExecute/error boundaries; register them in one table. Keep shared
   lifetime/fixture sequences together rather than splitting mechanically.
3. Set WorkingDirectory, TestArguments, TestEnvironment, TestLabels, TestTimeout
   and any TestProperties **before** EndProduct. Writers also declare TestIsolated
   and pristine TestFixtureDirectory/TestFixtureDestination. Keep serial by default.
4. Make `--list` bypass all backend/font/fixture initialization. Missing optional
   prerequisites return 77 only during execution, with a reason.
5. Build the real target, verify direct listing/selection/whole-suite/error options,
   inspect CTest registrations and run focused tests plus DiscoveryConsistency.
   Check source fixtures remain unchanged. See [agent rules](../AGENTS/Testing.md).

For a script-based infrastructure regression, use the same product macros rather
than a module-level add_test or a dummy C executable:

```cmake
set(this ${MyProjectName}.Area.Tests.ScriptRegression)
BeginProduct(${this} test)
SetFolder(${this} "Area")
set(${this}.TestScript "${CMAKE_CURRENT_SOURCE_DIR}/Regression.cmake")
set(${this}.TestArguments "-DTOOL=$<TARGET_FILE:RequiredTool>")
set(${this}.TestDependencies RequiredTool)
set(${this}.TestLabels infrastructure integration)
set(${this}.TestTimeout 120)
EndProduct(${this})
```

This creates a buildable script product whose dependencies prepare the tool, then
registers CMake -P through the shared implementation. Script products preserve the
usual working-directory/environment/properties and manage their own private
fixtures; they remain serial and do not opt into executable case discovery.

## CI, JUnit And Sanitizers

Both AppVeyor configurations pass an explicit build directory to CTest and write
an absolute `test-results-<job-id>.xml`. Windows passes its matrix configuration;
Linux is a Debug Ninja single-config build. `on_finish` uploads that exact file,
including after test failure; it warns rather than guessing a relative report if
no file was produced. Uploads require AppVeyor/network access and are not a local
test result. Source/build paths do not depend on shell working-directory persistence.

Linux CI additionally configures the dependency-free runtime slice at
`Runtime/Testing/Tests/Sanitizers`: real ARMS/Ring1/Testing libraries, Ring1 suites
and testing infrastructure under GCC/Clang ASan+UBSan, with leak detection and
non-recovering errors. Its separate sanitizer-results report is uploaded too.
This is focused runtime coverage, not sanitizer validation of every media/backend
dependency. Reproduce on Linux:

```sh
cmake -S /path/to/Repository/Runtime/Testing/Tests/Sanitizers \
  -B /path/to/SanitizerBuild -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build /path/to/SanitizerBuild
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
  ctest --test-dir /path/to/SanitizerBuild --output-on-failure --output-junit /path/to/SanitizerBuild/results.xml
```

The gate rejects unsupported platforms/compilers. ARCADIA_ENABLE_SANITIZERS=OFF
permits harness-wiring validation elsewhere but is **not** a sanitizer run. Report
platform/compiler/configuration and actual commands; never claim Linux, sanitizer,
remote CI or interactive coverage from a Windows build or source inspection.
