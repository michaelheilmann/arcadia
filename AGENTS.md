# AGENTS.md

## Start Here

- Arcadia is a C/C++ CMake monorepo for a portable game creation and preservation environment.
- The repo includes runtime libraries, language/compiler tooling, media and IO modules, engine abstractions/implementations, generated documentation, tests, and command-line tools.
- Project goals from `README.md`: create games, provide games created with the environment, and preserve both long-term.
- Trust executable files over prose when they disagree: `CMakeLists.txt`, `CMakePresets.json`, `CMake/all.cmake`, and `appveyor-*.yml`.
- This file is the main agent-instruction file in the repo; also read topic-specific instructions under `AGENTS/` when they apply.

## Build And Test

- Requires CMake 3.29+. Never configure or build in the source tree; always use an out-of-source build directory. `CMake/all.cmake` rejects in-source builds.
- Windows presets exist only for Visual Studio 2022: `cmake --preset x64`, then `cmake --build --preset x64-debug` or another preset from `CMakePresets.json`.
- For local Windows preset builds, prefer the documented build tree commands: `cmake -S . -B ./x64 --preset x64` followed by 
  `cmake --build ./x64 --target "Arcadia.InstallDependencies"` followed by `cmake --build ./x64 --config Debug --target <target>`.
- The expensive dependency download target `Arcadia.InstallDependencies` should be run only once per build tree, after the first configure/generate, and only when `<build>/.Dependencies` is missing.
  For example, check `x64/.Dependencies`; if it exists, do not rerun `cmake --build ./x64 --target "Arcadia.InstallDependencies"` for normal rebuilds or CMake regeneration.
- For disposable validation builds, use an out-of-source build directory under the Windows temporary directory, for example `C:/Users/Anwender/AppData/Local/Temp/opencode/arcadia-x64`; never place generated build files in the source tree.
- Linux CI does not use presets: configure from an external build directory with `cmake -D"Arcadia.Engine.Visuals.Implementation.OpenGL4.Enabled"=TRUE -D"Arcadia.Engine.Audials.Implementation.OpenAL.Enabled"=TRUE <source>`, then run `make all`.
- Run all tests from the build directory with `ctest`; for multi-config generators include `-C Debug` or the built configuration.
- Run one focused test by CTest name, for example `ctest -C Debug -R Arcadia.Ring2.Tests.StringTests`.
- Current Windows CI configure line appears malformed: it has `cmake - ${env:CMAKE_GENERATOR_NAME}` where `-G` is expected. Do not copy that command blindly.

## CMake Conventions

- Most targets are defined through custom macros in `CMake/all.cmake`, not raw `add_library` or `add_executable`.
- New libraries/tests should follow the local pattern: `set(this ${MyProjectName}.Area.Name)`, `BeginProduct(${this} library|test|executable)`, `OnSourceFile`, `OnHeaderFile`, `OnModuleDependency`, `EndProduct`.
- Source paths passed to `OnSourceFile` and `OnHeaderFile` are relative to the module's `Sources` directory.
- `OnConfigurationFile` generates `Configure.h` into the binary `Sources` tree from a checked-in `*.h.i` template (`configure_file` with `@ONLY`); many C files include that generated header, and the binary `Sources` dir is on every product's include path. `GENERATED`/`PRIVATE` keywords on `OnSourceFile`/`OnHeaderFile`/`OnInlayFile` switch to the binary `Sources` tree.
- When adding a new source/header, update the nearest `CMakeLists.txt`; files are registered explicitly.
- `BeginProduct(... test)` automatically registers a CTest test; one legacy `Runtime/Ring1/Tests/LiteralTests` file registers its test manually.

## Repository Shape

- Root `CMakeLists.txt` wires the real module boundaries: `Runtime/*`, `Languages`, `DDL`, `DDLS`, `ADL`, `VPL`, `Media`, `PixelBufferIO`, `SampleBufferIO`, `FontIO`, `Engine/*`, `Documentation`, `Tools/*`, and `repository`.
- Runtime layers are ordered roughly as `Runtime/ARMS`, `Runtime/Ring1`, `Runtime/Ring2`, then `Runtime/Collections`, `FileSystem`, `Print`, and `Logging`.
- `Ring1` contains core primitives, values, object/type system, process/thread handling, diagnostics, numerics, bigint, strings, UTF-8, memory helpers, and test support.
- `Ring2` contains higher-level services built on Ring1: command line, exceptions, Unicode encoders, strings, time, and logging.
- `Runtime/Collections` contains collection abstractions and implementations, including hash and immutable collections.
- `Languages` is shared scanner/parser/diagnostic infrastructure; `DDL`, `DDLS`, `ADL`, `VPL`, and `repository/MILC` build on it.
- `DDL` is the Data Definition Language reader/writer/AST/semantic-analysis module; `DDLS` is DDL schema validation.
- `repository/MILC` is the Machine Interface Language compiler frontend/backend: scanner, parser, AST, symbols, diagnostics, compiler phases, and code writers.
- `Engine/Engine` contains common audials/visuals/input abstractions; concrete visuals implementation lives under `repository/Visuals.Implementation`, not beside `Engine/Engine`.
- `Engine/UI` contains UI nodes, widget nodes/events, and node factories; `Engine/Application` contains the application abstraction.
- `Engine/Audials.Implementation` is OpenAL-oriented; `repository/Visuals.Implementation` contains OpenGL4, GLX/WGL, Windows/Linux display/windowing, and Direct3D12/Vulkan-related code.
- `repository/MILC.CIL` is the command-line MIL compiler tool; `Modules.mil` (at the repo root) currently lists `./Engine/Engine` as its only module path.

## Codegen And Documentation

- Generated-looking `.g` files and `.mil` inputs coexist in source directories; inspect the owning `CMakeLists.txt` and MILC code before regenerating or editing generated outputs.
- Many `.mil` files define engine events, visual/input declarations, media declarations, and compiler test assets.
- `repository/MILC/Library/Sources/Arcadia/MILC/Backend/*SymbolWriter.c` embeds source header strings for generated files; update those when license headers change.
- Documentation is built through custom template engine macros in `CMake/tools-template-engine.cmake`; outputs are under `.Website` and should not be treated as primary source.
- For documentation structure, naming, or content changes, read `AGENTS/Documentation.md` first.
- Useful docs: `README.md`, `building-under-windows-11-visual-studio-community-2022.md`, `building-under-linux.md`, `Documentation/Arcadia/roadmap.html.te`, and `Documentation/Specifications/*`.
- Roadmap pages are useful but may be stale; active entries mention MIL parser iteration, Ring2 immutable set/map work, and CI/CD tests.

## Runtime Coding Notes

- Runtime code commonly uses `Arcadia_Process`, `Arcadia_Thread`, `Arcadia_JumpTarget`, status values, and raised values instead of simple return-only error handling.
- Tests often use `Arcadia_Tests_safeExecute(...)`; compiler/runtime integration tests may manually acquire and relinquish `Arcadia_Process`.
- Object/type definitions use Arcadia macros such as `Arcadia_declareObjectType` and `Arcadia_defineObjectType`; follow nearby implementations rather than inventing a new pattern.
- Arcadia uses a precise garbage collector. Plain C locals holding `Arcadia_Object*` pointers are not GC roots.
- An object must be reachable through a precise root before any operation can trigger collection. Valid roots include the Arcadia value stack, object fields visited by a type's `visit` callback, and explicit `Arcadia_Object_lock` roots.
- `Arcadia_Process_stepARMS(process)` may collect any object that is not reachable from a precise root or explicitly locked. Do not call it while relying on unrooted C-local object pointers.
- Any object reference stored in a struct/object must be reported from that type's `visit` callback; otherwise the collector may reclaim it while the owner still stores the pointer.
- Prefer object ownership with correct `visit` callbacks over manual locks when an object has a natural owner.
- Use `Arcadia_Object_lock` only for temporary C-local objects that must survive possible GC points and are not otherwise reachable.
- Every `Arcadia_Object_lock` must be exception-safe: if code between lock and unlock can call `Arcadia_Thread_jump` directly or indirectly, wrap it with an `Arcadia_JumpTarget` cleanup path or restructure the code so the object is rooted without a manual lock.
- Re-throw after cleanup by popping the jump target, unlocking/ref-unrefing owned temporaries, and then calling `Arcadia_Thread_jump(thread)` again.

## Licensing

- The repo now has both `LICENSE` and `COPYING` with GNU AGPL-3.0 text.
- C/CMake source headers were converted to AGPL-3.0-or-later notices; preserve that form for new C, header, and CMake files.
