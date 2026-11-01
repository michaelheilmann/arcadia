# AGENTS.md

## Start Here

- This module builds the first-person/free-fly camera controller example of the Arcadia monorepo
  (`Engine/Engine/Examples/FreeFlyCamera`). The CMake product name is `Arcadia.Engine.Examples.FreeFlyCamera`.
- It is an interactive, windowed engine example. It boots the engine, selects visuals + audials backends, loads ADL assets,
  and demonstrates the demo-local `Arcadia.Starship.Viewer3D` controller.
- Do not add Scenario I or `Project Arcadia's Anima` game logic here; use `Games/Anima` for the game executable.
- Shared build, assets and scene-loop rules are in the [example guide](../AGENTS.md); this file retains FreeFlyCamera-specific behavior.

## Build And Test

- Build target name: `Arcadia.Engine.Examples.FreeFlyCamera`.
- On first run, the example creates its configuration file under `<config-dir>/FreeFlyCameraExample/Configuration.txt` and
  fills in defaults for missing or invalid settings.

## Repository Shape

- `Sources/Arcadia/Engine/Demo/Main.c` is still the program entry. The implementation namespace remains
  `Arcadia.Engine.Demo` for now to keep this rename small and low risk.
- The local `Application` loads configuration in its constructor, unlike the other three scene demos' startup-time load. `Main.c` initially selects `ArcadiaLogoScene` before the menu/controller scenes.
- `Scenes/MainMenuScene` is the primary free-fly camera example scene: W/A/S/D movement, Q/E roll, mouse-look, perspective
  camera, ambience, and a small UI canvas.
- `Sources/Arcadia/Starship/` contains reusable example-local support types: `Viewer3D`,
  `PositionChangeInputEvent`, and `OrientationChangeInputEvent`.

## Assets

- This module's `Assets/*.adl` files define color, material, mesh, model, texture, pixel-buffer, and sample-buffer assets
  for the example scenes.
