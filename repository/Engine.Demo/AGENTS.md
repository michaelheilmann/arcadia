# AGENTS.md

## Start Here

- This module builds the demo/playground application of the Arcadia monorepo (`repository/Engine.Demo`). The CMake product name is `Arcadia.Engine.Demo`, and it is an interactive, windowed demo program (window title: "Michael Heilmann's Liminality").
- It is the reference consumer of the engine: it boots the engine, selects visuals + audials backends, loads ADL assets, and uses its scene framework to display scenes, including an interactive free-fly 3D viewer scene.
- General repository conventions (build, CMake macros, GC, licensing) are in `AGENTS.md` at the repository root; this file only documents what is specific to this module. Trust `CMakeLists.txt` in this directory over prose.

## Build And Test

- This module is an executable product, not a library. It is only enabled when at least one visuals backend is enabled:
  `Arcadia.Engine.Visuals.Implementation.OpenGL4.Enabled`, `Arcadia.Engine.Visuals.Implementation.Direct3D12.Enabled`, or `Arcadia.Engine.Visuals.Implementation.Vulkan.Enabled`. See the guard at the top of `CMakeLists.txt`.
- Build target name: `Arcadia.Engine.Demo` (e.g. `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.Engine.Demo`).
- There are no tests for this module (it is an interactive GUI app; nothing is registered with CTest here).
- Assets are staged at build time into the binary tree by `CopyProductAssets`: the demo's own `Assets/` tree plus the assets of every linked library (collected transitively by `Arcadia_collectTargetAssets` over the link closure via the `ARCADIA_ASSETS_*` target properties in `CMake/all.cmake`). The CSS colors under `Assets/Colors/CSS/` are not stored in this module; `${MyProjectName}.Engine` owns them (reachable transitively through `${MyProjectName}.Engine.UI`). The demo loads assets at runtime with paths relative to the process working directory (e.g. `Assets/Colors/CSS/...`), so run the executable from a directory where the `Assets` folder exists (typically the build tree).
- On first run, the demo creates its configuration file in the user configuration directory under `<config-dir>/Demo/Configuration.txt` and fills in defaults for missing/invalid settings.

## CMake Conventions

- Module target: `set(this ${MyProjectName}.Engine.Demo)`, declared as a `BeginProduct(... executable)` inside an `if (NOT TARGET ${this})` guard.
- `OnConfigurationFile` generates `Sources/Arcadia/Engine/Demo/Configure.h` into the binary `Sources` tree from the checked-in `Configure.h.i` template. The generated header is currently empty; it exists so downstream files include `Arcadia/Engine/Demo/Configuration.h` (note the letter-case distinction: `Configuration.h` is the checked-in config helper header, `Configure.h` is the generated stub). `Configure.h.i` still carries a legacy permissive notice header unlike the rest of the module.
- `Resources.rc` binds `Icon.ico` as the Windows application icon; it is wired only on Windows (`Arcadia.Engine.Demo.Windows.ResourceFile`).
- Module dependencies are all PRIVATE: `Arcadia.Engine.UI`, `Arcadia.Application`, `Arcadia.ADL`, `Arcadia.DDL`, `Arcadia.Engine.Audials.Implementation`, `Arcadia.Engine.Visuals.Implementation`.

## Repository Shape

- `Sources/Arcadia/Engine/Demo/Main.c` — program entry. Acquires a process/thread, creates the `Arcadia.Engine.Demo.Application`, starts it, installs an `onSceneChanged` handler, sets the initial scene (`Arcadia.Engine.Demo.ArcadiaLogoScene`), and enters the message loop. Each iteration calls `Arcadia_Process_stepARMS`, updates the audials/visuals backend contexts, updates the scene's logics/audials/visuals per window (`beginRender`/`endRender`), then dequeues engine events (window closed, mouse button/pointer, keyboard key) and dispatches them to the active scene. Quit is requested via the window close or Escape (event → `applicationQuitRequestSignal`).
- `Application` — subclasses `Arcadia.Engine.Application`. Owns the `SceneManager`, a `sceneOnQuitRequestedSlot`, the loaded configuration, and re-broadcasts the active scene's quit request. `startup` starts visuals then audials; `shutdown` saves the configuration (ignoring I/O errors), closes windows, purges the engine message queue, and unlocks the audials/visuals backend contexts.
- `Scene` — abstract base type (subclass of `Arcadia.Object`) for things the demo displays. It dispatches `updateAudials`/`updateLogics`/`updateVisuals` and `handleKeyboardKeyEvent`/`handleMouseButtonEvent`/`handleMousePointerEvent`; the base implementations are empty no-ops. Holds the engine, the scene manager, and an `applicationQuitRequestSignal`.
- `SceneManager` — owns the currently displayed scene and a `sceneChangedEvent` signal. `setScene` emits the signal with the old and new scene pushed onto the value stack; the `Main.c` handler re-wires the quit-request signal to the new scene.
- `Scenes/ArcadiaLogoScene` — loading/boot scene. Streams `Assets/Colors/CSS/*.adl` color definitions into an `Arcadia_ADL_Definitions` one file per frame; after ~3.25 s (and once loading finishes) it creates `MainMenuScene` and activates it. Uses an orthographic camera, a green clear-color viewport, a framebuffer pass, and the `LogoScene.MeshColorModel` asset.
- `Scenes/MainMenuScene` — interactive scene: free-fly `Arcadia.Starship.Viewer3D` (W/A/S/D movement, Q/E roll, mouse-look), a looping ambience sound source at low volume, a UI canvas with a "New Game" panel model, a perspective camera (60° FOV), and the `MainMenuScene.TextureColorModel` asset. Escape emits the quit signal; `R` logs (stub) backend re-initialization.
- `Scenes/MainScene` — split-screen scene: three viewports (red/green/blue clear colors) each rendering `Assets.DefaultWall.Model`, a shared camera and enter-pass node, and a looping ambience sound source. Note: current wiring in `ArcadiaLogoScene` uses `MainMenuScene`, not `MainScene` (the `MainScene_create` call is commented out).
- `Configuration` — `Cfg2_get/set` (boolean/int32/string/section) helpers over a DDL map node, `Cfg_loadConfiguration` (parses + semantically analyzes `<config-dir>/Demo/Configuration.txt`, creating an empty config if missing/corrupt), and `Cfg_saveConfiguration` (writes via the DDL unparser). Used by `Visuals`/`Audials` startup for settings such as `visuals.backend`, `visuals.monitor`, `visuals.windowMode`, `visuals.horizontalResolution`/`verticalResolution`/`colorDepth`/`verticalSynchronization`, `audials.backend`, and `audials.masterVolume`.
- `Visuals` — `Arcadia_Engine_Application_startupVisuals`: registers visuals backends and node factories, selects the backend named in the configuration (default `Arcadia.Visuals.Implementation.OpenGL4.Backend`), creates and locks the visuals backend context, picks display device 0, applies the window mode/resolution/fullscreen settings, creates/opens the window, sets icons/title, and creates the node factory.
- `Audials` — `Arcadia_Engine_Application_startupAudials`: mirror of the visuals startup for audials (default backend `Arcadia.Engine.Audials.Implementation.OpenAL.Backend`), plus master volume (default 10), then creates the node factory.
- `AssetUtilities` — header-only static-inline utilities: recursive file enumeration and `getColorDefinition`/`getMaterialDefinition`/`getMeshDefinition`/`getSampleBufferDefinition`/`getModelDefinition` that load an ADL definition from a file on demand (`AssetUtilities.c` only includes the header).
- `Sources/Arcadia/Starship/` — reusable, demo-local support types (not engine code): `Viewer3D` (free-fly camera with position/quaternion rotation, world-to-view and local-to-world matrices, right-handed coordinate system; `w/a/s/d/q/e` + mouse input semantics) and the `PositionChangeInputEvent`/`OrientationChangeInputEvent` (engine-event subclasses carrying a translation vector / rotation quaternion).

## Assets

- This module's `Assets/*.adl` are ADL (Arcadia Definition Language) assets: color, material, mesh, model, texture, pixel-buffer, and sample-buffer definitions, plus `Internal.Ambience.DSP.WhiteNoise.adl` (procedural white-noise ambience used by the sound sources). The local tree covers `MainScene/`, `MainMenuScene/` (with the `NewGameButton` panel), `DefaultWall/`, and `LogoScene/`.
- `Assets/Colors/CSS/` is not stored in this module. The shared CSS color definitions (one `.adl` per color keyword, defining `Colors.<CamelCaseName>`, e.g. `Red.adl` → `Colors.Red`) live in `Engine/Engine/Assets/Colors/CSS/`, owned by `${MyProjectName}.Engine`. `CopyProductAssets` collects them transitively through `${MyProjectName}.Engine.UI` (which links `${MyProjectName}.Engine`) and stages them into this executable's binary tree.
- Models reference meshes and materials by name (e.g. `DefaultWall/Model.adl` → `Assets.DefaultWall.Model` using `Mesh` + `Material`), and UI panels reuse the same mechanics (`MainMenuScene/NewGameButton/NewGameButtonModel.adl`).
- When adding/removing asset files, no CMake edit is needed for assets themselves (`OnAssetsDirectory` globs each `Assets` tree at configure time; `CopyProductAssets` copies the demo's own assets plus those of its linked libraries).

## Module-Specific Runtime Notes

- The visuals and audials backend contexts are created and locked by this module's `Visuals`/`Audials` startup and unlocked during application shutdown; the application object is manually locked in `Main.c`.
- Constructors, `setScene` signal emission, and asset loading pass/root temporaries on the value stack, which is the GC root for those calls.
- Scene updates are split into logics/audials/visuals and dispatched virtually; the main loop ticks with a millisecond delta computed from `Arcadia_getTickCount`.
