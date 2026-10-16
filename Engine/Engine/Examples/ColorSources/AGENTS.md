# AGENTS.md

## Start Here

- This module builds an example of the *Arcadia Engine*. The CMake product name is `Arcadia.Engine.Examples.ColorSources`,
  and it is an interactive, windowed demo program (window title: "Color Sources - Texture | Vertex | Mesh").
- It is a reference consumer of the engine: it boots the engine, registers + starts the visuals and audials backends, loads ADL assets, and displays three quads side by side: checkerboard texture colors on the left, interpolated vertex colors in the center, and a uniform mesh color on the right.
- Shared build, assets and scene-loop rules are in the [example guide](../AGENTS.md); this file retains ColorSources-specific behavior.
- The sibling `FontTexture` example uses the related application skeleton for a font-atlas texture demonstration; ColorSources compares the three material color sources together.

## Build And Test

- Build target name: `Arcadia.Engine.Examples.ColorSources` (e.g. `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.Engine.Examples.ColorSources`).
- On first run, the demo creates its configuration file in the user configuration directory under `<config-dir>/ColorSourcesExample/Configuration.txt` and fills in defaults for missing/invalid settings.

## CMake Conventions

- `OnConfigurationFile` generates `Sources/Arcadia/Engine/Examples/ColorSources/Configure.h` into the binary `Sources` tree from the checked-in `Configure.h.i` template. The example has no feature macro.
- Module dependencies are all PRIVATE: `Arcadia.Engine` (also provides the shared `Assets/Colors/CSS` via `CopyProductAssets`), `Arcadia.Engine.UI`, `Arcadia.ADL`, `Arcadia.DDL`, `Arcadia.Engine.Audials.Implementation`, `Arcadia.Engine.Visuals.Implementation`.

## Repository Shape

- `Sources/Arcadia/Engine/Examples/ColorSources/Main.c` starts the local `Arcadia.Engine.Demo.Application`, wires scene-change/quit signals and selects `Arcadia.Engine.Examples.ColorSources.ColorSourcesScene`. Quit is requested by window close or Escape. Application loads configuration during startup, not its constructor; the constructor creates the scene manager.
- `Scenes/ColorSourcesScene` — the demo's only scene (type `Arcadia.Engine.Examples.ColorSources.ColorSourcesScene`). Lazy `load()` reads `Assets/ColorSourcesScene/` and `Assets/Colors/` once each, then creates the enter-pass node, a full-canvas viewport cleared to `Colors.DimGray`, the camera, and `modelNodes[3]` in texture/vertex/mesh order. The three ADL model definitions select their geometry and material color source. `updateVisuals` maintains an aspect-correct orthographic projection with room for the entire row, skips zero-sized canvases, wires the viewport and camera into the enter-pass node, and calls `renderSceneWithModelNodes` once so clearing cannot erase earlier quads. `updateAudials` only triggers `load`; `updateLogics` is a no-op. Escape on key release requests quit; the mouse handlers are no-ops.
- `AssetUtilities` — header-only static-inline utilities: recursive file enumeration and `getColorDefinition`/`getMaterialDefinition`/`getMeshDefinition`/`getSampleBufferDefinition`/`getModelDefinition` that load an ADL definition from a file on demand (`AssetUtilities.c` only includes the header).

## Assets

- This module's `Assets/*.adl` are Arcadia Asset Description Language, or Arcadia ADL for short, assets: color, material, mesh, model, texture, pixel-buffer, and sample-buffer definitions, plus `Internal.Ambience.DSP.WhiteNoise.adl` (procedural white-noise audio). The local tree covers `Assets/ColorSourcesScene/` and `Assets/DefaultWall/`.
- The scene consumes `ColorSourcesScene.TextureColorModel`, `ColorSourcesScene.VertexColorModel`, and `ColorSourcesScene.MeshColorModel`, using their existing Texture/Vertex/Mesh materials. `TextureColorMesh.adl` positions the left unit quad at x=-1.2; `Mesh.adl` keeps the center quad at x=0; `MeshColorMesh.adl` positions the right unit quad at x=1.2. All three provide matching RGB vertex colors and full-range UVs; their materials choose which color source is used. Texture colors come from the white/black 8x8 checkerboard, and the mesh color is RGB (167,255,181). Geometry and color-source selection remain in ADL, not C mesh construction. The `DefaultWall/` tree, `NewGameButton` panel models and ambience definitions remain unused by this scene.

## Module-Specific Runtime Notes

- All node objects created by `load()` are stored in scene fields (`cameraNode`, `enterPassNode`, every `modelNodes` entry, `viewportNode`) and visited by the scene. Asset enumeration/loading roots temporary paths, iterators, definitions, file contents and model names on the value stack and restores the entry depth on errors.

## Visual Verification

- Run from the example's build directory containing `Assets`. Check that all three quads appear in one row: black/white checkerboard, smooth RGB vertex interpolation, and uniform mint mesh color, on a neutral gray background.
- Resize to landscape and portrait dimensions: quads must remain square, separated and fully visible. Minimize/restore and close normally to check zero-size handling and shutdown.
- Windows WGL/OpenGL live verification passed at client sizes 984x581 and 544x761, including pixel checks for all three color sources, minimize/restore and exit code zero. Audio used OpenAL's null driver for that graphics-focused smoke check.
