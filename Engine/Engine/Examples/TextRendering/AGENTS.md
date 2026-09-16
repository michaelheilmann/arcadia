# AGENTS.md

## Start Here

- This module builds an example of the *Arcadia Engine*. The CMake product name is `Arcadia.Engine.Examples.TextRendering`,
  and it is an interactive, windowed demo program (window title: "Michael Heilmann's Liminality").
- It is a reference consumer of the engine: it boots the engine, registers + starts the visuals and audials backends, loads ADL assets, and uses its scene framework to display a textured model whose ambient color texture is a font atlas rendered from a system TrueType font by `FontIO`. The scene renders the text `"Hello, World!"`, centered on the canvas: it parses the text into Unicode code points with the code point reader, warms those code points into an `Arcadia_FontIO_AtlasBitmapFont`, and builds its glyph mesh (two triangles per code point) programmatically into ADL at runtime.
- General repository conventions (build, CMake macros, GC, licensing) are in `AGENTS.md` at the repository root; this file only documents what is specific to this module.
  Trust `CMakeLists.txt` in this directory over prose.

## Build And Test

- This module is an executable product, not a library. It is only enabled when at least one visuals backend is enabled:
  `Arcadia.Engine.Visuals.Implementation.OpenGL4.Enabled`, `Arcadia.Engine.Visuals.Implementation.Direct3D12.Enabled`, or `Arcadia.Engine.Visuals.Implementation.Vulkan.Enabled`.
  See the guard at the top of `CMakeLists.txt`.
- Build target name: `Arcadia.Engine.Examples.TextRendering` (e.g. `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.Engine.Examples.TextRendering`).
- There are no tests for this module (it is an interactive GUI app; nothing is registered with CTest here).
- Assets are staged at build time into the binary tree by `CopyProductAssets`: the example's own `Assets/` tree plus the assets of every linked library (collected transitively by `Arcadia_collectTargetAssets` over the link closure via the `ARCADIA_ASSETS_*` target properties in `CMake/all.cmake`). The CSS colors under `Assets/Colors/CSS/` are not stored in this module; they are provided by the `${MyProjectName}.Engine` library the example links. The demo loads assets at runtime with paths relative to the process working directory (e.g. `Assets/TextRenderingScene/...`, `Assets/Colors/CSS/...`), so run the executable from a directory where the `Assets` folder exists (typically the build tree).
- On first run, the demo creates its configuration file in the user configuration directory under `<config-dir>/Demo/Configuration.txt` and fills in defaults for missing/invalid settings.

## CMake Conventions

- Module target: `set(this ${MyProjectName}.Engine.Examples.TextRendering)`, declared as a `BeginProduct(... executable)` inside an `if (NOT TARGET ${this})` guard.
- `OnConfigurationFile` generates `Sources/Arcadia/Engine/Examples/TextRendering/Configure.h` into the binary `Sources` tree from the checked-in `Configure.h.i` template.
- `target_compile_definitions(... ARCADIA_ENGINE_EXAMPLES_TEXT_RENDERING=1)` marks this build of the example.
- `Resources.rc` binds `Icon.ico` as the Windows application icon; it is wired only on Windows (`Arcadia.Engine.Examples.Windows.ResourceFile`).
- Module dependencies are all PRIVATE: `Arcadia.Engine` (also provides the shared `Assets/Colors/CSS` via `CopyProductAssets`), `Arcadia.Engine.UI`, `Arcadia.ADL`, `Arcadia.DDL`, `Arcadia.FontIO`, `Arcadia.Engine.Audials.Implementation`, `Arcadia.Engine.Visuals.Implementation`.
- The example is registered from `Engine/Engine/CMakeLists.txt` with `add_subdirectory(Examples/TextRendering)`.

## Repository Shape

- `Sources/Arcadia/Engine/Examples/TextRendering/Main.c` — program entry, identical to the sibling `FontTexture` example (message loop, scene manager wiring, event dispatch).
- `Application`, `Scene`, `SceneManager`, `AssetUtilities` — the shared `Arcadia.Engine.Demo.*` skeleton inherited from the `CheckerboardTexture`/`FontTexture` examples.
- `Scenes/TextRenderingScene` — the demo's only scene (type `Arcadia.Engine.Demo.TextRenderingScene`).
  - `load()` reads every file under `Assets/TextRenderingScene/` and `Assets/Colors/` into ADL definitions, then creates the enter-pass node, a viewport (clear color `Colors.Red`, full canvas), a camera, and the model node for `TextRenderingScene.TextureColorModel`.
  - The mesh is NOT an `.adl` asset. The scene constructs the mesh definition `TextRenderingScene.Mesh` programmatically (`Arcadia_ADL_MeshDefinition_create` with `6 * numberOfCodePoints` vertices, white per-vertex ambient colors, zeroed positions and texture coordinates) and registers it into the definitions map with `Arcadia_Map_set` before the model definition is linked, exactly like `Arcadia_ADL_Context` registers definitions it reads. The model's mesh reference `TextRenderingScene.Mesh` resolves to this programmatic mesh.
  - `setFontTexture` (inside `load()`, guarded by `self->modelNode` being unset) locates a system TrueType font (arial/segoeui/DejaVu), creates an `Arcadia_FontIO_AtlasBitmapFont` with pixel size 32, parses the text with `Arcadia_UnicodeCodePointReader` into an unmanaged array of code points, warms the glyphs for exactly those code points, installs the font atlas (`Arcadia_FontIO_AtlasBitmapFont_getAtlas`) as the model material's ambient color texture, and stores the `Arcadia_FontIO_AtlasGlyphInformation` of each code point in an unmanaged array owned by the scene (freed in the scene's destructor).
  - `updateVisuals` assigns the projection matrix and canvas size, wires the viewport and camera into the enter-pass node, and renders the model through it. `setGlyphQuads` re-computes the glyph quads whenever the canvas size changes: it renders a quad per code point (two triangles, 9 floats per vertex; only positions and uvs are written), centering the text horizontally by `width/2 - (firstBearingX + sum(advanceX))/2` and vertically by `baseline = height/2 - (top + bottom)/2` where `top = max(bearingY)` and `bottom = min(bearingY - height)`.
  - Keyboard handling requests quit on Escape; the mouse handlers are no-ops.

## Assets

- This module's `Assets/*.adl` are Arcadia Asset Description Language, or Arcadia ADL for short, assets: color, material, texture, pixel-buffer, and model definitions.
- Models reference meshes and materials by name. `TextRenderingScene.TextureColorModel` references the programmatic mesh `TextRenderingScene.Mesh` and the material `TextRenderingScene.TextureColorMaterial`. The material is Blinn-Phong with ambient color source `Texture`, uses `TextRenderingScene.Texture` as ambient color texture, and blends `SourceAlpha`/`OneMinusSourceAlpha`.
- `Assets/TextRenderingScene/PixelBuffer.adl` is a 256x256 pixel buffer with a `CheckerboardFillOperation` (32x32 checkers, colors `TextRenderingScene.FirstColor` white and `TextRenderingScene.SecondColor` black). The atlas produced by the font replaces the pixel buffer content at runtime; the checkerboard is only the initial texture.
- `Assets/Colors/CSS/` is not stored in this module. The shared CSS color definitions (one `.adl` per color keyword, defining `Colors.<CamelCaseName>`, e.g. `Red.adl` → `Colors.Red`) live in `Engine/Engine/Assets/Colors/CSS/`, owned by `${MyProjectName}.Engine`. Because this example links the engine library, `CopyProductAssets` stages them into this executable's binary tree as well.
- When adding/removing asset files, no CMake edit is needed for assets themselves (`OnAssetsDirectory` globs each `Assets` tree at configure time; `CopyProductAssets` copies the example's own assets plus those of its linked libraries).

## Module-Specific Runtime Notes

- The visuals and audials backend contexts are created and locked by this module's startup and unlocked during application shutdown; the application object is manually locked in `Main.c`.
- Constructors, `setScene` signal emission, and asset loading pass/root temporaries on the value stack, which is the GC root for those calls.
- The unmanaged arrays (code points parsed from the text, the glyph information records, and the temporary vertex arrays passed to `Arcadia_ADL_MeshDefinition_create`) are allocated with `Arcadia_Memory_allocateUnmanaged` and explicitly deallocated with `Arcadia_Memory_deallocateUnmanaged`. The glyph information array is stored in the scene struct and freed in the scene's `destruct` callback; the mesh is registered into the definitions map before the model is linked, so no unmanaged state escapes the load path.
- `setFontTexture` creates an `Arcadia_FontIO_AtlasBitmapFont` and warms glyphs while the font is only reachable from a C local. It roots the font with `Arcadia_Object_lock` behind an `Arcadia_JumpTarget` (unlocking on both the success and error paths) until the atlas `Arcadia_Media_PixelBuffer` is assigned into the visuals node graph (`TextureNode`/`PixelBufferNode` `visit` callbacks), after which the atlas is reachable through scene references.
- Scene updates are split into logics/audials/visuals and dispatched virtually; the main loop ticks with a millisecond delta computed from `Arcadia_getTickCount`.