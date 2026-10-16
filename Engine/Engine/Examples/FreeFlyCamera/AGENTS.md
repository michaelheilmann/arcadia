# AGENTS.md

## Start Here

- This module builds the first-person/free-fly camera controller example of the Arcadia monorepo
  (`Engine/Engine/Examples/FreeFlyCamera`). The CMake product name is `Arcadia.Engine.Examples.FreeFlyCamera`.
- It is an interactive, windowed engine example. It boots the engine, selects visuals + audials backends, loads ADL assets,
  and demonstrates the demo-local `Arcadia.Starship.Viewer3D` controller.
- Do not add Scenario I or `Project Arcadia's Anima` game logic here; use `Games/Anima` for the game executable.
- General repository conventions are in `AGENTS.md` at the repository root. Trust `CMakeLists.txt` in this directory over
  prose.

## Build And Test

- This module is an executable product, not a library. It is only enabled when at least one visuals backend is enabled:
  `Arcadia.Engine.Visuals.Implementation.OpenGL4.Enabled`, `Arcadia.Engine.Visuals.Implementation.Direct3D12.Enabled`, or
  `Arcadia.Engine.Visuals.Implementation.Vulkan.Enabled`. See the guard at the top of `CMakeLists.txt`.
- Build target name: `Arcadia.Engine.Examples.FreeFlyCamera`.
- There are no tests for this module because it is an interactive GUI app.
- Assets are staged at build time into the binary tree by `CopyProductAssets`: the example's own `Assets/` tree plus the
  assets of every linked library collected transitively by `Arcadia_collectTargetAssets`.
- On first run, the example creates its configuration file under `<config-dir>/FreeFlyCameraExample/Configuration.txt` and
  fills in defaults for missing or invalid settings.

## Repository Shape

- `Sources/Arcadia/Engine/Demo/Main.c` is still the program entry. The implementation namespace remains
  `Arcadia.Engine.Demo` for now to keep this rename small and low risk.
- `Application` subclasses `Arcadia.Engine.Application`, owns the scene manager and configuration, starts visuals and
  audials, and shuts them down.
- `Scenes/MainMenuScene` is the primary free-fly camera example scene: W/A/S/D movement, Q/E roll, mouse-look, perspective
  camera, ambience, and a small UI canvas.
- `Sources/Arcadia/Starship/` contains reusable example-local support types: `Viewer3D`,
  `PositionChangeInputEvent`, and `OrientationChangeInputEvent`.

## Assets

- This module's `Assets/*.adl` files define color, material, mesh, model, texture, pixel-buffer, and sample-buffer assets
  for the example scenes.
- Shared CSS color assets are owned by `${MyProjectName}.Engine` and staged transitively through `${MyProjectName}.Engine.UI`.
- When adding or removing asset files, no CMake edit is needed for assets themselves; `OnAssetsDirectory` globs the local
  `Assets` tree at configure time.
