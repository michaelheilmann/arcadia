# Test fixture isolation and scheduling

## Runtime inventory

| Suites | Mutable state / external inputs | Policy |
| --- | --- | --- |
| PDLC SemanticalAnalysis | Compilation writes headers/bodies into module trees; `FileSystem` exercises FreshDirectories and blocked/missing paths. Diagnostic flags are process-local. | Each CTest case gets its own pristine copy of source Assets. No callback/error boundary or fixture loop is split. |
| TemplateEngine AtAt, BuiltInProcedures, HelloWorld, RecursiveIncludes | Relative templates/includes, output text and dependency files. RecursiveIncludes retains grouped expected-error callbacks. | Copy source Assets into each private working directory. Never seed from previously generated build-tree outputs. |
| PixelBufferIO WriteBMPTests / WritePNGTests | Six fixed image filenames per callback; image-manager state is process-local. | Private empty working directory; original grouped callback retained. |
| FileSystem FileContentsTests | Native temporary-file API, overwrite/read/delete sequence. | Private working directory and TMP/TEMP/TMPDIR; unsuccessful runs can leave only uniquely scoped build-tree files. |
| Runtime Testing runner self-tests | Process-wide stdout/stderr redirection and callback counters; unique temporary captures deleted on close. | Retain one serial suite-level workload. Do not split the capture orchestration. |
| Discovery infrastructure | CMake registration state, fixture generation, nested configure/build/test operations. | Serial orchestration; uniquely scoped disposable integration/unit-test roots. The isolated writer fixture itself supports parallel cases. |
| VPL GLSL Driver | Real WGL/OpenGL context/window, driver resources; specification examples read-only. | RUN_SERIAL; hardware availability is external, not solved by output isolation. `--list` still bypasses setup. |
| FontIO suites | Read-only OFL Noto CTest fixture (overridable), or optional/system fonts for direct invocation; font-library resources. | RUN_SERIAL pending a separate backend/concurrency audit. Missing/unreadable font skips with 77; existing corrupt input fails. |
| Core Engine, Engine.Text/UI, Media, ADL | Headless/native probes, mutable dispatch tables, weak singletons, GC/finalization counters, process-local graphs. Engine AudialsStreaming exercises DSP buffers/providers, not an OpenAL device. ADL/source assets are read-only. | Existing callback/process boundaries retained. RUN_SERIAL remains the conservative policy; output isolation is not evidence for enabling these suites concurrently. |
| Other Runtime, Collections, DDL/DDLS, PDLC, VPL, Math | In-memory operations or read-only fixtures. LineMap and SyntacticalAnalysis deliberately share process/jump-target state internally. DDL Writer and VPL Text serialize to memory, not files. | No new file outputs found. Retain RUN_SERIAL until separately assessed; do not split grouped lifetimes merely to raise case counts. |
| SampleBufferIO WriteFLACTests / PixelBufferIO WriteICOTests | The current FLAC callback creates/finishes an encoder without writing a file; ICO's callback is empty. | No output directory required today. These remain limited smoke-test coverage, not codec/file-writing equivalence; the scoped PixelBuffer/ADL cleanup did not expand them. No codec/Media DSP implementation change is implied. |

This inventory distinguishes runtime test callbacks from build tools. PDLC Backend,
CrossModules and ResourceModels generators write into external, configuration-specific
staging trees. Their generated C/header roots contain `$<CONFIG>`; source properties
are set on concrete paths so `.c.g` files really compile in VS and single-config
generators. Backend/CrossModules recreate their own configuration's staging tree.
ResourceModels keeps copied inputs separate from a freshly recreated writable stage.
Do not run simultaneous builds of the *same* configuration/tree or direct generators
against source modules; normal build dependencies still own those outputs.

## Per-case CTest workspaces

Before `EndProduct`, a writing test opts in with:

```cmake
set(${this}.TestIsolated TRUE)
set(${this}.TestFixtureDirectory "${CMAKE_CURRENT_SOURCE_DIR}/Assets")
set(${this}.TestFixtureDestination Assets) # default "."; omit fixtures for output-only tests
```

`CMake/TestIsolation.cmake` allocates a fresh directory under:

```text
<suite-binary-dir>/Testing/Isolated/<configuration>/<SHA256(suite.case)>/run-<unique-id>
```

Allocation is protected by a short-lived file lock. Concurrent CTest processes,
different cases and Debug/Release cannot reuse the same workspace. Only allocation
is locked, not execution. Fixtures are copied from the explicitly configured pristine
source directory, preserving nested directories and intentional file blockers. The
executable runs directly in that directory; there is no subprocess wrapper to alter
arguments, process exit codes, signals, or GLSL's skip code. TMP, TEMP and TMPDIR are
also redirected there for isolated cases.

**Listing stages fixtures.** CTest loads all case registrations before applying
filters, so `ctest -N` and JSON listing also allocate these directories. Executable
`--list` remains side-effect-free. This deliberate tradeoff preserves direct native
execution and skip semantics without setup/cleanup test registrations or shared
mutable fixture caches. Workspaces are retained for failure inspection and are
uniquely scoped even when a test exits early or is killed. The external build tree
owns them; they may be pruned when no CTest process is using it. No checked-in input
or arbitrary user path is deleted.

One workspace is allocated per case per CTest **listing/session**. `ctest --repeat`
reuses that case's workspace within the session; use separate CTest invocations to
recheck fresh-directory creation. Each separate invocation stages pristine inputs
again. Inspect the executed test's directory in `Testing/Temporary/LastTest.log`;
a later `ctest -N` creates a new workspace rather than showing the previous run's.

Direct executable invocation remains available, including no-argument whole-suite
execution. It uses the caller's working directory: run serially from the normal
binary directory or supply a freshly copied private fixture directory yourself.
Do not launch direct writer cases concurrently in a shared working directory or
run compilation tests/generators from checked-in fixture directories. CTest is the
supported automatically isolated path.

## Scheduling

Serial remains the default and is enforced for fallback/infrastructure/backend
workloads. A suite may set `TestParallelSafe TRUE` before `EndProduct` only after
its filesystem outputs and external resources have been audited and tested. This
is an explicit scheduling decision, not a side effect of discovering case names.
The dedicated discovery consistency test validates the configured policy and
isolated working-directory/environment metadata.

T04 validates writing cases first with fresh copies and repetitions, then randomized
and concurrent runs. No process/ARMS/GC/expected-error group is refactored, and no
repository-wide parallel CI policy is enabled by this work.

The eight isolated writer suites in the first four inventory rows explicitly opt
in to parallel scheduling after their serial/randomized gates: SemanticalAnalysis,
the four TemplateEngine suites, BMP/PNG and FileContentsTests. There were 25 cases
at T04 close; T05's independent semantic-fixture callbacks bring this to 80 without
changing the scheduling policy or enabling another suite.
All remaining production test suites and infrastructure orchestration stay serial.
Single-config builds without a build type use `Default` as the workspace partition.
