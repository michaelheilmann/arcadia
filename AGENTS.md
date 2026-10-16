# AGENTS.md

## Start Here

- Arcadia is a C/C++ CMake monorepo for a portable game creation and preservation environment.
- The repo includes runtime libraries, language/compiler tooling, media and IO modules, engine abstractions/implementations, generated documentation, tests, and command-line tools.
- Project goals from `README.md`: create games, provide games created with the environment, and preserve both long-term.
- Trust executable files over prose when they disagree: `CMakeLists.txt`, `CMakePresets.json`, `CMake/all.cmake`, and `appveyor-*.yml`.
- This file is the main agent-instruction file in the repo; also read topic-specific instructions under `AGENTS/` when they apply.
- For C/C++ source code style, read `AGENTS/CxxSourceCodeStyle.md`.
- For repository-wide file extension conventions, read `AGENTS/FileNamingConventions.md`.
- AI agents must not create commits or push to remotes; for the full rules, read `AGENTS/VersionControl.md`.

## Build And Test

- Requires CMake 3.29+. Never configure or build in the source tree; always use an out-of-source build directory that lives outside the checkout (nothing generated or downloaded goes under the repository). `CMake/all.cmake` rejects in-source builds.
- All non-validation build trees live in a dedicated build root outside the checkout. On this machine, that root is `C:/develop/Arcadia/Build` with one tree per architecture (`C:/develop/Arcadia/Build/x64`, ...). Because the trees are outside the repository, build artifacts never show up in Git.
- Windows presets exist only for Visual Studio 2022: `cmake --preset x64`, then `cmake --build --preset x64-debug` or another preset from `CMakePresets.json`.
- For local Windows preset builds, use the documented build tree commands: `cmake -S . -B C:/develop/Arcadia/Build/x64 --preset x64` followed by 
  `cmake --build C:/develop/Arcadia/Build/x64 --target "Arcadia.InstallDependencies"` followed by `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target <target>`, with the target optional.
- The expensive dependency download target `Arcadia.InstallDependencies` should be run only once per build tree, after the first configure/generate, and only when `<build>/.Dependencies` is missing.
  For example, check `C:/develop/Arcadia/Build/x64/.Dependencies`; if it exists, do not rerun `cmake --build C:/develop/Arcadia/Build/x64 --target "Arcadia.InstallDependencies"` for normal rebuilds or CMake regeneration.
- For disposable validation builds, use an out-of-source build directory under the Windows temporary directory, for example `C:/Users/Anwender/AppData/Local/Temp/opencode/arcadia-x64`; never place generated build files in the source tree.
- Linux CI does not use presets: configure from an external build directory with `cmake -D"Arcadia.Engine.Visuals.Implementation.OpenGL4.Enabled"=TRUE -D"Arcadia.Engine.Audials.Implementation.OpenAL.Enabled"=TRUE <source>`, then run `make all`.
- Run all tests from the build directory with `ctest`; for multi-config generators include `-C Debug` or the built configuration.
- Run one focused test by CTest name, for example `ctest -C Debug -R Arcadia.Ring2.Tests.StringTests`.
- Current Windows CI configure line appears malformed: it has `cmake - ${env:CMAKE_GENERATOR_NAME}` where `-G` is expected. Do not copy that command blindly.

## CMake Conventions

- Most targets are defined through custom macros in `CMake/all.cmake`, not raw `add_library` or `add_executable`.
- New libraries/tests should follow the local pattern: `set(this ${MyProjectName}.Area.Name)`, `BeginProduct(${this} library|test|executable)`, `OnSourceFile`, `OnHeaderFile`, `OnModuleDependency`, `EndProduct`.
- Source paths, generated configuration headers, header guards, and explicit source/header registration conventions are covered in `AGENTS/CxxSourceCodeStyle.md`.
- `BeginProduct(... test)` automatically registers a CTest test; one legacy `Runtime/Ring1/Tests/LiteralTests` file registers its test manually.

## Source Code Style

- For C/C++ source code style, including line endings, header guards, includes, function formatting, object-system patterns, GC/rooting, error handling, generated sources, and licensing, read `AGENTS/CxxSourceCodeStyle.md`.

## Repository Shape

- Root `CMakeLists.txt` wires the real module boundaries: `Runtime/*`, `Languages`, `DDL`, `DDLS`, `ADL`, `VPL`, `Media`, `PixelBufferIO`, `SampleBufferIO`, `FontIO`, `Engine/*`, `Documentation`, `Tools/*`, and `repository`.
- Runtime layers are ordered roughly as `Runtime/ARMS`, `Runtime/Ring1`, `Runtime/Ring2`, then `Runtime/Collections`, `FileSystem`, `Print`, and `Logging`.
- `Ring1` contains core primitives, values, object/type system, process/thread handling, diagnostics, numerics, bigint, strings, UTF-8, memory helpers, and test support.
- `Ring2` contains higher-level services built on Ring1: command line, exceptions, Unicode encoders, strings, time, and logging.
- `Runtime/Collections` contains collection abstractions and implementations, including hash and immutable collections.
- `Runtime/Ring1` has its own notes in `Runtime/Ring1/AGENTS.md`; read it before changing the object/type system, the value stack, the GC rooting rules, or Ring1 tests.
- `Languages` is shared scanner/parser/diagnostic infrastructure; `DDL`, `DDLS`, `ADL`, `VPL`, and `PDLC` build on it.
- `DDL` is the Data Definition Language reader/writer/AST/semantic-analysis module; `DDLS` is DDL schema validation.
- `PDLC` is the current compiler frontend/backend for Arcadia PDL, the Arcadia Program Definition Language: scanner, parser, AST, symbols, diagnostics, compiler phases, and code writers. `PDLC` is current implementation naming; prefer `PDLC` in prose.
- `Engine/Engine` contains common audials/visuals/input abstractions; concrete visuals implementation lives under `repository/Visuals.Implementation`, not beside `Engine/Engine`.
- `Engine/UI` contains UI nodes, widget nodes/events, and node factories; `Engine/Application` contains the application abstraction.
- `Engine/Audials.Implementation` is OpenAL-oriented; `repository/Visuals.Implementation` contains OpenGL4, GLX/WGL, Windows/Linux display/windowing, and Direct3D12/Vulkan-related code.
- The OpenGL4 WGL backend selects a double-buffered, fully accelerated pixel format (`WGL_DOUBLE_BUFFER_ARB` in `WGL/BackendContext.c`). Both `beginRenderImpl` paths (WGL and GLX) lazily resolve `wglSwapIntervalEXT`/`glXSwapIntervalEXT(MESA)` and apply the swap interval requested by a window backend, on change only, because the interval can only be adjusted while the GL context is current (that is, inside a rendering operation). The engine's `visuals.verticalSynchronization` configuration key is wired to `Window_setVerticalSynchronization` by `Arcadia_Engine_ApplicationHelper_startupVisuals` and is honored in every window mode. WGL's interval is a per-context setting, and the OpenGL4 WGL backend shares one context across its windows, so the interval is tracked per backend context; GLX's interval is per-drawable, so it is tracked per window backend. If the swap-control extension is unavailable the request is ignored and the driver default remains in effect.
- `PDLC.CLI` is the command-line MIL compiler tool; `Modules.mil` (at the repo root) currently lists `./Engine/Engine` and `./Media` as its module paths.

## Codegen And Documentation

- Generated-looking `.g` files and `.mil` inputs coexist in source directories; inspect the owning `CMakeLists.txt` and PDLC code before regenerating or editing generated outputs.
- Many `.mil` files define engine events, visual/input declarations, media declarations, and compiler test assets.
- `PDLC/Library/Sources/Arcadia/PDLC/Backend/*SymbolWriter.c` embeds source header strings for generated files; update those when license headers change.
- The tool that regenerates `.c`/`.h` from `.mil` sources is called PDLC (the modules `PDLC`/`PDLC.CLI` names are current implementation naming; prefer `PDLC` in prose).
  Its executable target is `Arcadia.PDLC.CLI`. There is no CMake wiring for `.mil` inputs; generated `.c`/`.h` files are committed next to their `.mil` source and are
  regenerated with PDLC, not by the build.
- Build PDLC before regenerating: `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.PDLC.CLI`.
- Regenerate by running the built executable with the `--mil2c` flag and a `--configuration=<path>` argument naming the config file, for example:
  `Arcadia.PDLC.CLI --mil2c --configuration=\"C:/develop/Arcadia/Repository/Modules.mil\"`
- Bare `--help` prints help.
- `modulePaths` in the configuration file are relative to the parent directory of the configuration file (the working directory); the root `Modules.mil` currently lists `./Engine/Engine` and `./Media`.
- On Windows, the `--configuration` value must be quoted and the quotes must survive `argv` parsing: escape them as `\"` in the raw command line (as `PDLC.CLI/Tool/CMakeLists.txt`
  does for `VS_DEBUGGER_COMMAND_ARGUMENTS`). Use forward slashes in the path; `parseString` treats `\` as an escape and rejects backslash-separated paths.
- A value that does not start with `"` is parsed as an integer and fails with `invalid command-line argument`. Windows PowerShell 5.1 strips quotes embedded in native-command arguments, so a direct `& exe ...` call does not work: invoke through a `.cmd` wrapper instead. See `PDLC.CLI/AGENTS.md`.
- Regeneration writes byte-identical files when neither the `.mil` inputs nor the generator changed, so `git status` should stay clean after a no-op run.
- Documentation is built through custom template engine macros in `CMake/tools-template-engine.cmake`; outputs are under `.Website` and should not be treated as primary source.
- For documentation structure, naming, or content changes, read `AGENTS/Documentation.md` first.
- Useful docs: `README.md`, `building-under-windows-11-visual-studio-community-2022.md`, `building-under-linux.md`, `Documentation/Arcadia/roadmap.html.te`, and `Documentation/Specifications/*`.
- Roadmap pages are useful but may be stale; active entries mention PDLC parser iteration, Ring2 immutable set/map work, and CI/CD tests.

## Runtime Coding Notes

- Runtime code commonly uses `Arcadia_Process`, `Arcadia_Thread`, `Arcadia_JumpTarget`, status values, and raised values instead of simple return-only error handling.
- Detailed runtime coding conventions for object/type definitions, value-stack ordering, precise GC rooting, visit callbacks, manual locks, and jump-target cleanup are in `AGENTS/CxxSourceCodeStyle.md`.

## Licensing

- The repo now has both `LICENSE` and `COPYING` with GNU AGPL-3.0 text.
- C/CMake source headers were converted to AGPL-3.0-or-later notices; preserve that form for new C, header, and CMake files.
