# Arcadia.Testing

## Introduction

An Engine-independent, test-only named-case runner. Test executables link
`${MyProjectName}.Testing` privately; production libraries must not depend on it.
Testing itself depends privately on Ring1. No external testing framework is used.

For build/debugging commands, suite authoring, CI and sanitizer coverage, see
[the testing guide](../../Documentation/Testing.md). For fixture/state/backend
audits and retained workspaces, see [Isolation.md](Isolation.md). Agent rules are
in [AGENTS/Testing.md](../../AGENTS/Testing.md).

## Module Interface

Include `Arcadia/Testing/Include.h`. `Arcadia_Testing_Case` contains a borrowed
`const char* name` and `int (*run)(void)` callback. Pass its table/count to
`Arcadia_Testing_runSuite(argc, argv, cases, count)`:

```c
static const Arcadia_Testing_Case cases[] = {
  { "Example", &runExample },
};
return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
```

- No arguments runs every case in declaration order, continuing after failures
  and skips. `--case NAME` selects exactly one case, case-sensitively.
- `--list` writes precisely one name per line in declaration order, with no
  banners, callbacks, progress logs or stderr on success. Output uses native text
  line endings. This is the authoritative case inventory; no manual inventory is
  maintained here. Quote selectors containing spaces when invoking a shell.
- All modes validate the entire table first: it must be non-null/nonempty, names
  must be non-null/nonempty, unique and contain no CR/LF, and callbacks must be
  non-null. Distinct names may share a callback, but redundant workloads are not
  justified merely by distinct names. Table/name storage must outlive the call.
- Invalid suites/arguments, unknown/empty selectors and discovery output errors
  return `EXIT_FAILURE`. Invalid argv entries are rejected without dereferencing
  NULL. Invalid tables produce no partial callback execution or name output.

### Results And Skips

Callbacks return `EXIT_SUCCESS`, `Arcadia_Testing_Skip` (77), or another value
for failure. Each execution prints RUN then OK, SKIPPED or FAILED. Callbacks
should print a reason before returning a skip; 77 means an unavailable optional
prerequisite, never a failed assertion, corrupt input or unexpected runtime error.

The suite returns failure if any callback failed, 77 if every selected callback
skipped, otherwise success. Thus a mixed pass/skip whole-suite run returns success
but still logs each skip; CTest's per-case runs preserve individual skip results.
Successful `--list` returns success even if all callbacks would skip at runtime.
Other nonzero/negative callback codes are aggregated to failure, not propagated.

The callback owns its `safeExecute`, process, jump target, fixture setup and
cleanup. Catch runtime jumps inside callbacks: the runner neither catches
exceptions nor manages runtime lifetimes. It scopes/restores Ring1's borrowed
OS-thread-local case-name pointer around execution, including nested suites and
ordinary failures. The name survives process teardown, is unchanged by listing
and does not propagate to worker threads.

## CMake Integration

Use `BeginProduct(... test)`, source/header registration and
`OnModuleDependency(${this} ${MyProjectName}.Testing PRIVATE)`. Set metadata
**before** `EndProduct`:

```cmake
set(${this}.WorkingDirectory "${CMAKE_CURRENT_BINARY_DIR}")
set(${this}.TestArguments "optional leading argument")
set(${this}.TestEnvironment "NAME=value")
set(${this}.TestLabels integration requires-font)
set(${this}.TestTimeout 60)
set(${this}.TestProperties SKIP_RETURN_CODE 77)
```

Direct Testing consumers use test-time `--list` discovery, yielding
`Executable.Case` invocations with `--case Case` and no duplicate suite workload.
`.TestDiscovery FALSE` retains an explicit suite fallback; TRUE opts in without a
direct Testing dependency. AUTO is the default. The runner's self-tests retain
one suite fallback, independently of consuming-suite integration checks.

CMake-script regressions use the same `BeginProduct(... test)` / `EndProduct`
pair: set `.TestScript` to an existing absolute script path, `.TestArguments` to
the script's `-D` arguments and `.TestDependencies` to required build targets.
The product is a buildable custom target (not a compiled launcher), and CTest
runs configured CMake with the arguments followed by `-P <script>`. Labels,
timeout, environment, working directory and skip properties use the common policy.
Scripts manage their own private fixture setup, remain serial, cannot be installed
or linked, and do not use case discovery or TestIsolated. Do not call add_test
directly from a module's CMakeLists.txt; low-level registration belongs to the
shared CMake implementation.

Metadata is configuration-specific; configure/generate never executes tests.
CTest loads fresh names rather than a mutable case cache. Unbuilt executables
produce a failing NOT_BUILT placeholder; missing/unknown multi-config selection
produces CONFIGURATION_REQUIRED. Discovery failure, malformed/empty/duplicate
output or collisions are errors, not silent omissions. Discovery times out after
30 seconds. The configured CMake executable is serialized into metadata because
CTest's include interpreter need not define CMAKE_COMMAND.

Arguments/environment are CMake lists; `.TestProperties` is a property/value
list. Use `.TestLabels` for multiple labels and `.TestTimeout` for seconds.
`CMake/TestPolicy.cmake` supplies classification, a 60-second ordinary watchdog
and code-77 skip handling to discovered and fallback test products. Infrastructure
has separate limits. Labels do not authorize parallel execution. Serial remains
the default; only audited `.TestParallelSafe TRUE` suites may run concurrently.

For writer cases set `.TestIsolated TRUE`, optionally `.TestFixtureDirectory` and
`.TestFixtureDestination` before EndProduct. CTest stages pristine source copies
in unique case/configuration/session workspaces and redirects TMP/TEMP/TMPDIR.
Listing also stages workspaces. `ctest --repeat` reuses a session's directory;
separate CTest invocations allocate fresh copies. Direct executable invocation is
not automatically isolated. See Isolation.md before running writers directly.

## Ring1 Assertion Support

Include `Arcadia/Ring1/Include.h` and link Ring1 for opt-in
`Arcadia_Tests_assertEqualInteger64`, `Natural64`, `Real64`, `Size`, `Status`,
`CxxString` and `Bytes`. Expected operands precede actual operands. Each argument
is evaluated once in unspecified C argument order. Failures show expected/actual
values, expressions, case and file/line, then raise `Arcadia_Status_TestFailed`.
Simple assertTrue/assertFalse remain available unchanged.

Choose the correct signed/unsigned domain; Size remains distinct. Real64 uses
exact equality (signed zeros equal, NaNs unequal), not a tolerance. CxxString
requires NUL termination; NULL differs from empty. Bytes compares explicit lengths
including embedded NULs; NULL is allowed only for empty spans. Diagnostics escape
controls, preview at most 32 bytes per value, show lengths and locate the first
difference even beyond the preview. Inputs must remain readable for their lengths.

Outside the runner, the name is `<unselected>` unless callers scope
`Arcadia_Tests_setCaseName` and restore the returned previous pointer. Its borrowed
string must remain alive for that scope. Exact compiler diagnostic assertions live
in [PDLC test support](../../PDLC/Tests/Support/README.md), not this generic runner.

## Regression Tests

`Arcadia.Testing.Tests.Runner` is one CTest workload with selectable Execution,
Selection, Arguments, Descriptors, Discovery, Failures, CaseContext,
ValueAssertions and Skips regressions. They invoke the real runner with controlled
callbacks, capture both streams in unique temporary files, and test status/order,
discovery, nested context, assertions and skip/failure precedence. Capture files
are closed/deleted after invocation; Windows uses the user's temporary directory.

DiscoveryConsistency checks fresh names, commands, metadata, isolation and
scheduling against CTest JSON. DiscoveryInfrastructure, DiscoveryIntegration and
FixtureIsolation exercise malformed output, unbuilt/configuration placeholders,
arbitrary punctuation, generated-source compilation, native skips and pristine
workspace behavior with standalone external projects. These are separate from
the dedicated runner workload. FontIO's Prerequisites regression additionally
checks real missing/corrupt/valid font inputs and CTest/JUnit pass/skip/fail states.
DiscoveryIntegration also builds the real asset-copy helper in a pristine tree,
then checks nested/inherited copies, removed-directory recreation and changed
input propagation. This catches rule-less destination-directory dependencies
that warmed build trees can conceal from Ninja validation.
TemplateEngine's separate Depfile infrastructure workload builds nested includes
through the real CLI and CMake DEPFILE integration, checking a single rendered
output rule, escaped paths, depfile reload and deepest-input regeneration.
