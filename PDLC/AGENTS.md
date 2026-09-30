# MILC Folder Guide

## Scope

- `repository/MILC` contains the current compiler library target `${MyProjectName}.MILC` for Arcadia PDL, the Arcadia Program Definition Language.
- The library owns Arcadia PDL scanning, parsing, AST nodes, symbols, compiler phases, diagnostics, compilation task orchestration, and C backend writers.
- `MILC` is legacy/current implementation naming; do not rename code, targets, paths, or file extensions here unless the user explicitly asks for that rename.
- Command-line behavior lives in `repository/MILC.CIL`; keep reusable compiler behavior in this library rather than in the CLI.

## Language Specification

- The Arcadia PDL specification is `Documentation/Specifications/Program Definition Language/index.html.te`.
- Arcadia PDL intentionally does not imply a particular abstraction level, storage duration, or implementation strategy; it simply defines a program.
- Treat `Scanner.c`, `Parser.c`, AST node types, semantic phases, backend behavior, tests, and checked-in `.mil` inputs as the primary executable evidence for current behavior.
- When changing accepted Arcadia PDL syntax, lexical words, instruction forms, primitive semantics, or observable diagnostics, update the PDL specification in the same change when practical.
- If implementation and specification disagree, prefer the implementation for fixing immediate behavior, but either update the specification or leave an explicit follow-up note in the change summary.

## Build Wiring

- The root module file adds `Library` and `Tests`; the library target is declared in `Library/CMakeLists.txt` with the repository `BeginProduct` macros.
- Source and header files are registered explicitly with `OnSourceFile` and `OnHeaderFile`; update `Library/CMakeLists.txt` when adding, moving, or deleting compiler source files.
- Generated configuration headers come from `Configure.h.i` via `OnConfigurationFile`; include the generated `Configure.h` through normal target include paths.

## Compiler Structure

- `Scanner.c`, `Keywords.c`, and `WordType.h` define the lexical word stream and reserved words.
- `Parser.c` defines the accepted syntax and builds `AST/*` nodes.
- `EnterPhase`, `MemberEnterPhase`, and `TypeResolutionPhase` build and complete symbols; keep symbol invariants local to the relevant phase where possible.
- `Backend*` code writes generated C and headers. Preserve generated-file header strings when changing licensing or public generated output.
- Diagnostics are ordinary compiler output. Prefer adding precise diagnostics over raising generic status values when the caller can recover and report useful source information.

## Runtime And GC

- MILC code follows the Arcadia runtime conventions from the root `AGENTS.md`: `Arcadia_Thread*` first, status/raise/jump error handling, and precise GC rooting.
- Do not keep `Arcadia_Object*` values only in plain C locals across calls that may collect. Root them through object fields with visit callbacks, the value stack, or exception-safe locks.
- If a new AST, symbol, diagnostic, or compiler object owns references, update its `visit` callback.

## Tests

- MILC tests live under `Tests/AST`, `Tests/LexicalAnalysis`, `Tests/SyntacticalAnalysis`, `Tests/SemanticalAnalysis`, `Tests/LineMap`, and `Tests/Diagnostics`.
- `Tests/SemanticalAnalysis` drives a `CompilationTask` over an `Assets/<CaseName>/` module tree. Its `compile` helper succeeds only when the task ran to completion without raising and produced no error diagnostic, so a case is negative when the compilation task raises or reports an error diagnostic. The helper always emits all collected diagnostics, so a negative case prints its expected diagnostics to stdout.
- Add or update `.mil` assets for parser or scanner behavior changes.
- Use focused CTest names when available; on Windows run tests from an out-of-source configured build directory with `ctest -C Debug -R <test-name>`.

## Licensing

- New C, header, and CMake files should use the repository AGPL-3.0-or-later header style.
