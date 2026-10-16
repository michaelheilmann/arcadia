# AGENTS.md

## Start Here

- This module builds an example of the *Arcadia Engine*. The CMake product name is `Arcadia.Engine.Examples.FontTexture`,
  and it is an interactive, windowed demo program (window title: "Michael Heilmann's Liminality").
- It is a reference consumer of the engine: it boots the engine, registers + starts the visuals and audials backends, loads ADL assets, and uses its scene framework to display a textured model whose ambient color texture is a font atlas rendered from a system TrueType font by `FontIO` (gated by `ARCADIA_ENGINE_EXAMPLES_FONT_TEXTURE`).
- Shared build, assets and scene-loop rules are in the [example guide](../AGENTS.md); this file retains FontTexture-specific behavior.

## Build And Test

- Build target name: `Arcadia.Engine.Examples.FontTexture` (e.g. `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.Engine.Examples.FontTexture`).
- On first run, the demo creates its configuration file in the user configuration directory under `<config-dir>/FontTextureExample/Configuration.txt` and fills in defaults for missing/invalid settings.

## CMake Conventions

- `OnConfigurationFile` generates `Sources/Arcadia/Engine/Examples/FontTexture/Configure.h` into the binary `Sources` tree from the checked-in `Configure.h.i` template.
- `target_compile_definitions(... ARCADIA_ENGINE_EXAMPLES_FONT_TEXTURE=1)` gates the FontIO font-atlas code in `FontTextureScene.c`: the `Arcadia/FontIO/Include.h` include and `setFontTexture`. Without it the scene renders without the font texture.
- Module dependencies are all PRIVATE: `Arcadia.Engine` (also provides the shared `Assets/Colors/CSS` via `CopyProductAssets`), `Arcadia.Engine.UI`, `Arcadia.ADL`, `Arcadia.DDL`, `Arcadia.FontIO`, `Arcadia.Engine.Audials.Implementation`, `Arcadia.Engine.Visuals.Implementation`.

## Repository Shape

- `Sources/Arcadia/Engine/Examples/FontTexture/Main.c` starts the local `Arcadia.Engine.Demo.Application`, wires scene-change/quit signals and selects `Arcadia.Engine.Demo.MainMenuScene`. Quit is requested by window close or Escape. Application loads configuration during startup, not its constructor; the constructor creates the engine-library scene manager.
- `Scenes/FontTextureScene` — the demo's only scene (type `Arcadia.Engine.Demo.MainMenuScene`; the type name is inherited from the checkerboard original). A lazy `load()` streams every file under `Assets/MainMenuScene/` and `Assets/Colors/` into ADL definitions, then creates the enter-pass node, a viewport (clear color `Colors.Red`, full canvas), the camera, and the model node for `MainMenuScene.TextureColorModel`. It then calls `setFontTexture`, which (gated by `ARCADIA_ENGINE_EXAMPLES_FONT_TEXTURE`) locates a system TrueType font (arial/segoeui/DejaVu), creates an `Arcadia_FontIO_AtlasBitmapFont`, warms the glyphs for code points 32..126, and installs the font atlas as the model material's ambient color texture. `updateVisuals` assigns the projection matrix and canvas size, wires the viewport and camera into the enter-pass node, and renders the model through it; `updateAudials` only triggers `load`; `updateLogics` is a no-op. Keyboard handling requests quit on Escape (on key release) and logs on `R` (a stub); the mouse handlers are no-ops. The sibling `Arcadia.Engine.Examples.ColorSources` now compares texture, vertex and mesh colors in one scene, while this example focuses on the font-atlas swap.
- `AssetUtilities` — header-only static-inline utilities: recursive file enumeration and `getColorDefinition`/`getMaterialDefinition`/`getMeshDefinition`/`getSampleBufferDefinition`/`getModelDefinition` that load an ADL definition from a file on demand (`AssetUtilities.c` only includes the header).

## Assets

- This module's `Assets/*.adl` are Arcadia Asset Description Language, or Arcadia ADL for short, assets: color, material, mesh, model, texture, pixel-buffer, and sample-buffer definitions, plus `Internal.Ambience.DSP.WhiteNoise.adl` (procedural white-noise audio). The local tree covers `Assets/MainMenuScene/` (with the `NewGameButton` panel) and `Assets/DefaultWall/`.
- Of those, the current scene actually consumes only `MainMenuScene.TextureColorModel` and the `Colors.Red` clear color; the `DefaultWall/` tree, the `NewGameButton` panel models, and the ambience definitions are staged by `CopyProductAssets` but not used by the scene at runtime.
- Models reference meshes and materials by name (e.g. `MainMenuScene.TextureColorModel` uses `MainMenuScene.Mesh` + `MainMenuScene.TextureColorMaterial`); the `NewGameButton` panel assets follow the same mechanics but are not used by the current scene.

## Module-Specific Runtime Notes

- `setFontTexture` creates an `Arcadia_FontIO_AtlasBitmapFont` and warms glyphs while the font is only reachable from a C local. It roots the font with `Arcadia_Object_lock` behind an `Arcadia_JumpTarget` (unlocking on both the success and error paths) until the atlas `Arcadia_Media_PixelBuffer` is assigned into the visuals node graph (`TextureNode`/`PixelBufferNode` `visit` callbacks), after which the atlas is reachable through scene references.
