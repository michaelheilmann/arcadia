# Engine Folder Guide

## Scope

- `Engine/Engine` is the core engine abstraction library (`${MyProjectName}.Engine`). It owns generic engine state, event queues, backend/backend-context abstractions, node factories, audials abstractions, visuals abstractions, input events, visuals resources, and visuals node interfaces.
- `Engine/UI` is the UI layer (`${MyProjectName}.Engine.UI`). It depends on `Engine` and `Math`, and provides UI events, UI nodes, widget nodes, and concrete UI node types such as canvas and panel.
- `Engine/Application` is the application abstraction (`${MyProjectName}.Application`). It wraps engine startup/shutdown and input/window/application event hooks.
- `Engine/Audials.Implementation` is the OpenAL-oriented audials backend implementation (`${MyProjectName}.Engine.Audials.Implementation`). It is implementation code, not the core audials API.
- Visuals implementations are not in this folder. The concrete OpenGL4/GLX/WGL/Windows/Linux visuals code lives under `repository/Visuals.Implementation`.

## Build Wiring

- The repository root adds these modules individually with `add_subdirectory(Engine/Engine)`, `add_subdirectory(Engine/UI)`, `add_subdirectory(Engine/Application)`, and `add_subdirectory(Engine/Audials.Implementation)`. There is no `Engine/CMakeLists.txt` aggregator.
- Library products use the repo macros from `CMake/all.cmake`: `BeginProduct`, `OnSourceFile`, `OnHeaderFile`, `OnConfigurationFile`, `OnModuleDependency`, and `EndProduct`.
- When adding a source, header, inlay, or generated configuration file, update the nearest `Library/CMakeLists.txt`; files are registered explicitly.
- `Configure.h` files are generated from checked-in `Configure.h.i` templates via `OnConfigurationFile`. Include the generated `Configure.h` through the module's normal public/private include path, not by hard-coding the binary tree.
- `Engine/Audials.Implementation` is option-gated by `${MyProjectName}.Engine.Audials.Implementation.OpenAL.Enabled`. Do not assume OpenAL headers or libraries are available unless that option is enabled.

## Include Boundaries

- Public consumers of the core engine should include `Arcadia/Engine/Include.h`, not individual core headers. Many core headers intentionally reject direct inclusion unless `ARCADIA_ENGINE_PRIVATE` is set by the aggregate include.
- Public consumers of UI should include `Arcadia/Engine/UI/Include.h`.
- Public consumers of Application should include `Arcadia/Application/Include.h`.
- Public consumers of Audials implementation registration should include `Arcadia/Audials/Implementation/Include.h`.
- Keep aggregate include files in sync when adding public types.

## Runtime Patterns

- Engine types follow the Arcadia object system. Use nearby files as templates for `Arcadia_declareObjectType`, `Arcadia_defineObjectType`, dispatch structs, constructor functions, visit functions, and virtual-call wrappers.
- Functions generally take `Arcadia_Thread*` first and use Arcadia status/raise/jump-target conventions from the runtime instead of plain C error returns.
- Follow the root `AGENTS.md` runtime GC/rooting rules for owned object references, `visit` callbacks, and temporary locks.
- Backend objects represent available systems; backend contexts are heavyweight active resources such as audio or video contexts. Avoid creating contexts implicitly unless nearby code already does so.
- Engine registration APIs store backend and node-factory type objects in `Arcadia_Set` instances. Register derived type objects, not instances.

## MIL Files

- Several event and enum-like declarations have `.mil` inputs beside C and header files. Generated-looking `.c`/`.h` files coexist with these inputs.
- Before editing generated-looking output, inspect the owning `CMakeLists.txt` and MILC workflow. Do not regenerate or rewrite generated files unless the task specifically requires it.
- Preserve the existing relationship between `.mil`, `.c`, and `.h` files when adding related declarations.

## Documentation

- `Engine/Engine/Documentation` and `Engine/UI/Documentation` are wired through CMake. Keep documentation changes near the module they describe.
- Roadmap/specification prose can be stale; prefer executable build files and source code when resolving contradictions.

## Verification

- There are currently no tests under `Engine`. For code changes, build the affected target from an out-of-source configured build tree.
- On Windows, use the existing Visual Studio preset flow when available: `cmake --preset x64`, then a relevant build preset such as `cmake --build --preset x64-debug`.
- If OpenAL implementation code changes, verify with `${MyProjectName}.Engine.Audials.Implementation.OpenAL.Enabled` enabled in a build that has OpenAL dependencies available.
- Run `ctest` from the build directory after broader behavior changes, using `-C Debug` or the selected configuration for multi-config generators.

## Licensing

- New C, header, and CMake files in this folder should use the repository's AGPL-3.0-or-later header style.
- New `.mil` files should use the same AGPL-3.0-or-later header style as existing `.mil` files in this folder.
