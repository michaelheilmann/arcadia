# AGENTS.md

## Start Here

- Arcadia is a C/C++ CMake monorepo for a portable game creation and preservation environment.
- The repo includes runtime libraries, language/compiler tooling, media and IO modules, engine abstractions/implementations, generated documentation, tests, and command-line tools.
- Project goals from `README.md`: create games, provide games created with the environment, and preserve both long-term.
- Trust executable files over prose when they disagree: `CMakeLists.txt`, `CMakePresets.json`, `CMake/all.cmake`, and `appveyor-*.yml`.
- This file is the main agent-instruction file in the repo; also read topic-specific instructions under `AGENTS/` when they apply.
- Invariant: `PDLC`, `PDLC.CLI`, `Runtime/ARMS`, `Runtime/Ring1`, `Runtime/Collections`, and `Runtime/FileSystem` production implementations remain hand-written C for now. Do not port these components to PDL or generate their production headers/bodies with PDLC. Do not include the compiler/CLI themselves as production regeneration modules. Opaque PDL imports of existing native APIs (such as Collections' native type declarations), PDL test fixtures, and generated-code tests remain allowed; these must not transfer implementation ownership to PDLC. Broad requests to "keep porting" do not override this restriction; changing it requires explicit maintainer approval. Existing self-generation artifacts are not an exception or permission to continue that work.
- For C/C++ source code style, read `AGENTS/CxxSourceCodeStyle.md`.
- For repository-wide file extension conventions, read `AGENTS/FileNamingConventions.md`.
- For test creation, assertions, discovery, fixture isolation and verification, read `AGENTS/Testing.md`; ADL and Media ownership/compatibility notes live in their respective `AGENTS.md` files.
- AI agents must not create commits or push to remotes; for the full rules, read `AGENTS/VersionControl.md`.

## Build And Test

- Requires CMake 3.29+. Never configure a CMake binary/build tree in the source tree; build trees and downloaded dependencies must live outside the checkout. `CMake/all.cmake` rejects in-source builds. The intentional generated website output at source-root `.Website` is the sole exception, not permission for other build artifacts; see [AGENTS/Documentation.md](AGENTS/Documentation.md) and [Documentation/AGENTS.md](Documentation/AGENTS.md).
- All non-validation build trees live in a dedicated build root outside the checkout. On this machine, that root is `C:/develop/Arcadia/Build` with one tree per architecture (`C:/develop/Arcadia/Build/x64`, ...). Because the trees are outside the repository, build artifacts never show up in Git.
- Windows presets exist only for Visual Studio 2022: `cmake --preset x64`, then `cmake --build --preset x64-debug` or another preset from `CMakePresets.json`.
- For local Windows preset builds, use the documented build tree commands: `cmake -S . -B C:/develop/Arcadia/Build/x64 --preset x64` followed by 
  `cmake --build C:/develop/Arcadia/Build/x64 --target "Arcadia.InstallDependencies"` followed by `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target <target>`, with the target optional.
- The expensive dependency download target `Arcadia.InstallDependencies` should be run only once per build tree, after the first configure/generate, and only when `<build>/.Dependencies` is missing.
  For example, check `C:/develop/Arcadia/Build/x64/.Dependencies`; if it exists, do not rerun `cmake --build C:/develop/Arcadia/Build/x64 --target "Arcadia.InstallDependencies"` for normal rebuilds or CMake regeneration.
- For disposable validation builds, use an out-of-source build directory under the Windows temporary directory, for example `C:/Users/Anwender/AppData/Local/Temp/opencode/arcadia-x64`; never place generated build files in the source tree.
- Linux CI does not use presets: it explicitly configures an external Debug Ninja tree with OpenGL4/OpenAL enabled, builds with `cmake --build <build>`, and runs CTest with `--test-dir <build>`. See `appveyor-linux.yml` for the pinned minimum CMake requirement and separate runtime sanitizer tree.
- Run all tests from the build directory with `ctest`; for multi-config generators include `-C Debug` or the built configuration.
- Run one focused test by CTest name, for example `ctest -C Debug -R Arcadia.Ring2.Tests.StringTests`.
- `Documentation/Testing.md` covers discovery, labels/timeouts, code-77 skips, private fixture debugging, CI JUnit paths and the focused Linux sanitizer gate. Use executable `--list` or CTest JSON for the current inventory, not historical test counts.
- Windows CI currently selects the generator with `cmake -G ${env:CMAKE_GENERATOR_NAME}` in `appveyor-windows.yml`; use the documented local preset commands above rather than assuming the CI invocation is a local build recipe.

## CMake Conventions

- Most targets are defined through custom macros in `CMake/all.cmake`, not raw `add_library` or `add_executable`.
- New libraries/tests should follow the local pattern: `set(this ${MyProjectName}.Area.Name)`, `BeginProduct(${this} library|test|executable)`, `OnSourceFile`, `OnHeaderFile`, `OnModuleDependency`, `EndProduct`.
- `OnSourceFile` recognizes committed `.c.g` files as C. Register them normally; the `GENERATED` argument still means build-tree output location, not merely that a committed file was produced by PDLC. Temporary native migration references belong to their tests rather than HEADER_FILE_ONLY production entries. After a completed migration's gates pass, retire Native/Generated providers and keep one ordinary linked-library regression test; see the Engine migration workflow.
- Source paths, generated configuration headers, header guards, and explicit source/header registration conventions are covered in `AGENTS/CxxSourceCodeStyle.md`.
- `BeginProduct(... test)` automatically registers CTest cases for direct Testing consumers using test-time `--list` discovery; other tests and explicit `${this}.TestDiscovery FALSE` fallbacks retain one suite registration. Configure WorkingDirectory/TestArguments/TestEnvironment/TestProperties before EndProduct. Serial is the default; only audited TestParallelSafe suites opt in. TestIsolated writer cases get pristine per-case/configuration/session build-tree workspaces, including redirected temporary paths; CTest listing also stages these directories. See Runtime/Testing/Isolation.md. Ring1 LiteralTests uses the standard macros and is enabled by its parent like other maintained suites.

## Source Code Style

- For C/C++ source code style, including line endings, header guards, includes, function formatting, object-system patterns, GC/rooting, error handling, generated sources, and licensing, read `AGENTS/CxxSourceCodeStyle.md`.

## Repository Shape

- Root `CMakeLists.txt` wires the real module boundaries: `Runtime/*`, `Languages`, `DDL`, `DDLS`, `ADL`, `VPL`, `Media`, `PixelBufferIO`, `SampleBufferIO`, `FontIO`, `Engine/*`, `Documentation`, `Tools/*`, and `repository`.
- Runtime layers are ordered roughly as `Runtime/ARMS`, `Runtime/Ring1`, `Runtime/Ring2`, then `Runtime/Collections`, `FileSystem`, `Print`, and `Logging`.
- `Runtime/Testing` owns the Engine-independent named-case runner used by test executables across the repository. It depends privately on Ring1; production libraries must not depend on Testing. Suites expose component cases or retain their internal test sequences as callbacks; CTest discovers those names through `--list` without adding duplicate whole-suite runs. Direct no-argument invocation still runs the whole suite. See `Runtime/Testing/README.md` for API/discovery/fallback contracts and `Documentation/Testing.md` for font-path arguments, graphics preflight and skip policy. Obtain current selectors from `--list` rather than a manual inventory.
- `Ring1` contains core primitives, values, object/type system, process/thread handling, diagnostics, numerics, bigint, strings, UTF-8, memory helpers, and test support.
- `Ring2` contains higher-level services built on Ring1: command line, exceptions, Unicode encoders, strings, time, and logging.
- `Runtime/Collections` contains collection abstractions and implementations, including hash and immutable collections. Its Arcadia.Collections PDL module owns the shared opaque native Collection/List/ArrayList/Set/HashSet/Deque/ArrayDeque declarations; their executable implementations remain native. Do not redeclare those types in Engine or make Media depend on Engine to use them.
- `Runtime/Ring1` has its own notes in `Runtime/Ring1/AGENTS.md`; read it before changing the object/type system, the value stack, the GC rooting rules, or Ring1 tests.
- `Languages` is shared scanner/parser/diagnostic infrastructure; `DDL`, `DDLS`, `ADL`, `VPL`, and `PDLC` build on it.
- `DDL` is the Data Definition Language reader/writer/AST/semantic-analysis module; `DDLS` is DDL schema validation.
- ADL's Arcadia.ADL PDL module owns eleven opaque native imports, five generated enums (TextureFilter, TextureAddressMode, MaterialType, AmbientColorSource and BlendFunction), and generated ColorDefinition/WhiteNoiseDefinition/ConstantDefinition/SineWaveDefinition/SawtoothWaveDefinition classes at their existing Definitions/Visuals or Definitions/DSP paths. Definition/Definitions, readers, reference resolution and reference-bearing definition implementations remain native. The native Definition import describes only its constructor/link boundary, not regenerated storage; native visitors retain its definitions/name fields. Use Arcadia/ADL/Include.h across module boundaries. ADL's four-way AmbientColorSource (including TextureVertex) remains distinct from Engine's three-way ColorSource. ADL's C/header include switch is ARCADIA_ADL_PRIVATE, scoped by its aggregate. Media DSP implementations remain native and excluded from the current migration scope; ADL DSP definition/configuration objects are separate from those sample algorithms.
- `PDLC` is the current compiler frontend/backend for Arcadia PDL, the Arcadia Program Definition Language: scanner, parser, AST, symbols, diagnostics, compiler phases, and code writers. `PDLC` is current implementation naming; prefer `PDLC` in prose.
- `Engine/Engine` contains common audials/visuals/input abstractions; concrete visuals implementation lives under `repository/Visuals.Implementation`, not beside `Engine/Engine`.
- `Engine/UI` contains UI nodes, widget nodes/events, and node factories. The `Arcadia.Engine.Application` class and its configuration/startup helpers belong to core `Engine/Engine`, under `Engine/Engine/Library/Sources/Arcadia/Engine/`.
- `Engine/Audials.Implementation` is OpenAL-oriented; `repository/Visuals.Implementation` contains OpenGL4, GLX/WGL, Windows/Linux display/windowing, and Direct3D12/Vulkan-related code.
- The OpenGL4 WGL backend selects a double-buffered, fully accelerated pixel format (`WGL_DOUBLE_BUFFER_ARB` in `WGL/BackendContext.c`). Both `beginRenderImpl` paths (WGL and GLX) lazily resolve `wglSwapIntervalEXT`/`glXSwapIntervalEXT(MESA)` and apply the swap interval requested by a window backend, on change only, because the interval can only be adjusted while the GL context is current (that is, inside a rendering operation). The engine's `visuals.verticalSynchronization` configuration key is wired to `Window_setVerticalSynchronization` by `Arcadia_Engine_ApplicationHelper_startupVisuals` and is honored in every window mode. WGL's interval is a per-context setting, and the OpenGL4 WGL backend shares one context across its windows, so the interval is tracked per backend context; GLX's interval is per-drawable, so it is tracked per window backend. If the swap-control extension is unavailable the request is ignored and the driver default remains in effect.
- `PDLC.CLI` is the command-line MIL compiler tool; `Modules.pdl` (at the repo root) currently lists `./Runtime/Collections`, `./ADL`, `./Engine/Engine`, `./Engine/Text`, `./Engine/UI`, `./Media`, and `./repository/Math` as its module paths. Collections contributes only opaque native declarations, not generated implementation code; PDLC itself is excluded.

## Codegen And Documentation

- Generated-looking `.g` files and `.pdl` inputs coexist in source directories; inspect the owning `CMakeLists.txt` and PDLC code before regenerating or editing generated outputs.
- Many `.pdl` files define engine events, visual/input declarations, media declarations, and compiler test assets.
- `PDLC/Library/Sources/Arcadia/PDLC/Backend/*SymbolWriter.c` embeds source header strings for generated files; update those when license headers change.
- The tool that regenerates `.c`/`.h` from `.pdl` sources is called PDLC (the modules `PDLC`/`PDLC.CLI` names are current implementation naming; prefer `PDLC` in prose).
  Its executable target is `Arcadia.PDLC.CLI`. Production `.pdl` inputs are registered in CMake for project/IDE visibility, but a normal production build does not regenerate their committed outputs; run PDLC explicitly. Backend/ResourceModels tests have separate custom generation steps in the external build tree. Do not confuse those test steps or IDE registrations with production regeneration.
- Build PDLC before regenerating: `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.PDLC.CLI`.
- Regenerate by running the built executable with the `--mil2c` flag and a `--configuration=<path>` argument naming the config file, for example:
  `Arcadia.PDLC.CLI --mil2c --configuration=\"C:/develop/Arcadia/Repository/Modules.pdl\"`
- Bare `--help` prints help.
- `modulePaths` in the configuration file are relative to the parent directory of the configuration file (the compilation task's working directory, not the shell process CWD); the root `Modules.pdl` currently lists `./Runtime/Collections`, `./ADL`, `./Engine/Engine`, `./Engine/Text`, `./Engine/UI`, `./Media`, and `./repository/Math`. Do not add PDLC/PDLC.CLI or implementation-generation models for the protected native-C components.
- On Windows, the `--configuration` value must be quoted and the quotes must survive `argv` parsing: escape them as `\"` in the raw command line (as `PDLC.CLI/Tool/CMakeLists.txt`
  does for `VS_DEBUGGER_COMMAND_ARGUMENTS`). Use forward slashes in the path; `parseString` treats `\` as an escape and rejects backslash-separated paths.
- A value that does not start with `"` is parsed as an integer and fails with `invalid command-line argument`. Windows PowerShell 5.1 strips quotes embedded in native-command arguments, so a direct `& exe ...` call does not work: invoke through a `.cmd` wrapper instead. See `PDLC.CLI/AGENTS.md`.
- Regeneration writes byte-identical files when neither the `.pdl` inputs nor the generator changed, so `git status` should stay clean after a no-op run.
- PDLC generates class/procedure headers directly as `.h`, alongside interface/enumeration headers. Their sources use `.c.g`; selected verified implementations are compiled in production. Plain `native class` imports externally owned declarations (for example Window) without overwriting them. `native implementation "Path/To/Header.h" class ...` instead regenerates the mapped declaration header while retaining native executable bodies. There is no header-copying/publishing step. See `PDLC/AGENTS.md` for output ownership and `Engine/AGENTS.md` for the implementation-migration workflow.
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
