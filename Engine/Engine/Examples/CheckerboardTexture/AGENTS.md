# AGENTS.md

## Start Here

- This module builds an example of the *Arcadia Engine*. The CMake product name is `Arcadia.Engine.Examples.CheckerboardTexture`,
  and it is an interactive, windowed demo program (window title: "Michael Heilmann's Liminality").
- It is a reference consumer of the engine: it boots the engine, registers + starts the visuals and audials backends, loads ADL assets, and uses its scene framework to display a textured model built from the module's `MainMenuScene` assets.
- General repository conventions (build, CMake macros, GC, licensing) are in `AGENTS.md` at the repository root; this file only documents what is specific to this module.
  Trust `CMakeLists.txt` in this directory over prose.
- The sibling example `Engine/Engine/Examples/FontTexture` has the same scene structure; it additionally swaps the model material's ambient color texture for a `FontIO` font atlas (gated by `ARCADIA_ENGINE_EXAMPLES_FONT_TEXTURE`).

## Build And Test

- This module is an executable product, not a library. It is only enabled when at least one visuals backend is enabled:
  `Arcadia.Engine.Visuals.Implementation.OpenGL4.Enabled`, `Arcadia.Engine.Visuals.Implementation.Direct3D12.Enabled`, or `Arcadia.Engine.Visuals.Implementation.Vulkan.Enabled`.
  See the guard at the top of `CMakeLists.txt`.
- Build target name: `Arcadia.Engine.Examples.CheckerboardTexture` (e.g. `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.Engine.Examples.CheckerboardTexture`).
- There are no tests for this module (it is an interactive GUI app; nothing is registered with CTest here).
- Assets are staged at build time into the binary tree by `CopyProductAssets`: the example's own `Assets/` tree plus the assets of every linked library (collected transitively by `Arcadia_collectTargetAssets` over the link closure via the `ARCADIA_ASSETS_*` target properties in `CMake/all.cmake`). The CSS colors under `Assets/Colors/CSS/` are not stored in this module; they are provided by the `${MyProjectName}.Engine` library the example links. The demo loads assets at runtime with paths relative to the process working directory (e.g. `Assets/Colors/CSS/...`), so run the executable from a directory where the `Assets` folder exists (typically the build tree).
- On first run, the demo creates its configuration file in the user configuration directory under `<config-dir>/Demo/Configuration.txt` and fills in defaults for missing/invalid settings.

## CMake Conventions

- Module target: `set(this ${MyProjectName}.Engine.Examples.CheckerboardTexture)`, declared as a `BeginProduct(... executable)` inside an `if (NOT TARGET ${this})` guard.
- `OnConfigurationFile` generates `Sources/Arcadia/Engine/Examples/CheckerboardTexture/Configure.h` into the binary `Sources` tree from the checked-in `Configure.h.i` template. The example currently has no feature macro; the FontTexture sibling uses a `target_compile_definitions(... ARCADIA_ENGINE_EXAMPLES_FONT_TEXTURE=1)` gate for its extra code.
- `Resources.rc` binds `Icon.ico` as the Windows application icon; it is wired only on Windows (`Arcadia.Engine.Examples.Windows.ResourceFile`).
- Module dependencies are all PRIVATE: `Arcadia.Engine` (also provides the shared `Assets/Colors/CSS` via `CopyProductAssets`), `Arcadia.Engine.UI`, `Arcadia.ADL`, `Arcadia.DDL`, `Arcadia.Engine.Audials.Implementation`, `Arcadia.Engine.Visuals.Implementation`.

## Repository Shape

- `Sources/Arcadia/Engine/Examples/CheckerboardTexture/Main.c` — program entry. Acquires a process/thread, creates the `Arcadia.Engine.Demo.Application`, starts it, installs an `onSceneChanged` handler, sets the initial scene (`Arcadia.Engine.Demo.MainMenuScene`), runs one GC step, and enters the message loop. Each iteration calls `Arcadia_Process_stepARMS`, updates the audials/visuals backend contexts, updates the scene's logics, then per window (`beginRender`/`endRender`) updates logics/audials/visuals with the canvas size, and finally dequeues engine events (window closed, mouse button/pointer, keyboard key) and dispatches them to the active scene. Quit is requested via window close or Escape (`applicationQuitRequestSignal`).
- `Application` — subclasses `Arcadia.Engine.Application`. Owns the `SceneManager` and a `sceneOnQuitRequestedSlot`, and re-broadcasts the active scene's quit request.
  - Its constructor loads the application configuration (`Arcadia_Engine_ApplicationConfiguration_loadConfiguration`) and creates the `SceneManager`.
  - `startup` registers audials backends and starts audials (`Arcadia_Engine_ApplicationHelper_startupAudials`), then registers visuals backends and starts visuals (`Arcadia_Engine_ApplicationHelper_startupVisuals`), opening the window.
  - `shutdown` saves the configuration (ignoring I/O errors via an `Arcadia_JumpTarget`), closes all windows, purges the engine message queue, and unlocks/releases the audials and visuals backend contexts.
  - `onWindowClosedEvent` removes the closed window from the windows list and requests quit.
- `Scene` — abstract base type (subclass of `Arcadia.Object`) for things the demo displays. Dispatches `updateAudials`/`updateLogics`/`updateVisuals` and `handleKeyboardKeyEvent`/`handleMouseButtonEvent`/`handleMousePointerEvent`; the base implementations are empty no-ops. Holds the engine, the scene manager, and an `applicationQuitRequestSignal`.
- `SceneManager` — owns the currently displayed scene and a `sceneChangedEvent` signal. `setScene` emits the signal with the old and new scene pushed onto the value stack; the `Main.c` handler re-wires the quit-request signal to the new scene.
- `Scenes/CheckerboardTextureScene` — the demo's only scene (type `Arcadia.Engine.Demo.MainMenuScene`; the type name predates the current slim structure). A lazy `load()` streams every file under `Assets/MainMenuScene/` and `Assets/Colors/` into ADL definitions, then creates the enter-pass node, a viewport (clear color `Colors.Red`, full canvas), the camera, and the model node for `MainMenuScene.TextureColorModel` (mesh `MainMenuScene.Mesh` + material `MainMenuScene.TextureColorMaterial`). `updateVisuals` assigns the projection matrix and canvas size, wires the viewport and camera into the enter-pass node, and renders the model through it; `updateAudials` only triggers `load`; `updateLogics` is a no-op. Keyboard handling requests quit on Escape (on key release) and logs on `R` (a stub); the mouse handlers are no-ops.
- `AssetUtilities` — header-only static-inline utilities: recursive file enumeration and `getColorDefinition`/`getMaterialDefinition`/`getMeshDefinition`/`getSampleBufferDefinition`/`getModelDefinition` that load an ADL definition from a file on demand (`AssetUtilities.c` only includes the header).

## Assets

- This module's `Assets/*.adl` are Arcadia Asset Description Language, or Arcadia ADL for short, assets: color, material, mesh, model, texture, pixel-buffer, and sample-buffer definitions, plus `Internal.Ambience.DSP.WhiteNoise.adl` (procedural white-noise audio). The local tree covers `Assets/MainMenuScene/` and `Assets/DefaultWall/`.
- Of those, the current scene actually consumes only `MainMenuScene.TextureColorModel` (mesh `MainMenuScene.Mesh` + material `MainMenuScene.TextureColorMaterial`, whose texture/pixel-buffer/color definitions are also under `Assets/MainMenuScene/`) and the `Colors.Red` clear color. The `DefaultWall/` tree, the `NewGameButton` panel models, and the ambience definitions are staged by `CopyProductAssets` but not used by the scene at runtime.
- `Assets/Colors/CSS/` is not stored in this module. The shared CSS color definitions (one `.adl` per color keyword, defining `Colors.<CamelCaseName>`, e.g. `Red.adl` → `Colors.Red`) live in `Engine/Engine/Assets/Colors/CSS/`, owned by `${MyProjectName}.Engine`. Because this example links the engine library, `CopyProductAssets` stages them into this executable's binary tree as well.
- When adding/removing asset files, no CMake edit is needed for assets themselves (`OnAssetsDirectory` globs each `Assets` tree at configure time; `CopyProductAssets` copies the example's own assets plus those of its linked libraries).

## Module-Specific Runtime Notes

- The visuals and audials backend contexts are created and locked by this module's startup and unlocked during application shutdown; the application object is manually locked in `Main.c`.
- Constructors, `setScene` signal emission, and asset loading pass/root temporaries on the value stack, which is the GC root for those calls.
- All node objects created by `load()` are stored in scene fields (`cameraNode`, `enterPassNode`, `modelNode`, `viewportNode`) that are reported from the scene's `visit` callback, so they do not need manual locks once assigned.
- Scene updates are split into logics/audials/visuals and dispatched virtually; the main loop ticks with a millisecond delta computed from `Arcadia_getTickCount`.