# Engine Example Guidelines

## Scope And Source Of Truth

- Follow the [root conventions](../../../AGENTS.md) and [Engine guide](../../AGENTS.md). This guide covers all example descendants; child guides retain scene-specific rules. Trust each example's `CMakeLists.txt` and sources over prose.
- These are separate executable consumers of the engine, not one shared concrete Application/Scene implementation. Similar namespaces or copied startup loops do not transfer ownership between examples. Keep game-specific logic in `Games/Anima`.

## Build And Verification

- Targets use `${MyProjectName}.Engine.Examples.<Name>` with `BeginProduct(... executable)` and explicit sources/dependencies. `Engine/Engine/CMakeLists.txt` registers the six example directories; there is no Examples CMake aggregator.
- Backend gates differ; do not apply GUI requirements to the finite audio example:

  | Example | CMake enable condition | Runtime shape |
  |---|---|---|
  | ColorSources, FontTexture, FreeFlyCamera, TextRendering | At least one of `Arcadia.Engine.Visuals.Implementation.OpenGL4.Enabled`, `.Direct3D12.Enabled`, `.Vulkan.Enabled` | Windowed, scene-based; startup also selects audials |
  | SpaceshipAmbience | `Arcadia.Engine.Audials.Implementation.OpenAL.Enabled` | Finite audio/sample-buffer example; no window or scene manager |
  | SpaceshipAmbienceStreaming | OpenAL **and** at least one of those visuals options | Windowed infinite audio stream; no scene manager |

- Use the external-build instructions in the root guide. Generic command: `cmake --build <external-build-dir> --config Debug --target Arcadia.Engine.Examples.<Name>`. For example, on the documented Windows workstation: `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.Engine.Examples.ColorSources`.
- None of these six example directories registers a CTest test. A successful build is not visual/audio verification: report the actual executable, backend/device availability, interaction and result when running a smoke check. Preserve child-specific visual checks; do not reuse an old smoke result as proof of a new change.
- Where an example declares `Resources.rc` through its `.Windows.ResourceFile` target property, the product macros include it only on Windows. Keep target-specific configuration headers, feature macros and dependencies in the child guide/CMake file.

## Assets And Configuration

- For **ColorSources, FontTexture, FreeFlyCamera and TextRendering**, `OnAssetsDirectory` registers the local `Assets` tree and `CopyProductAssets` stages it with assets from linked libraries. `Arcadia_collectTargetAssets` follows the transitive link closure via `ARCADIA_ASSETS_*` properties in `CMake/all.cmake`.
- Their shared CSS definitions belong to the Engine library at `Engine/Engine/Library/Assets/Colors/CSS/` (one `.adl` per keyword, e.g. `Red.adl` defines `Colors.Red`), not to an example's source assets. The staged runtime path is `Assets/Colors/CSS/`. Run an asset-reading example from its binary directory containing `Assets`; loading is relative to the process working directory.
- Adding/removing assets requires no per-file CMake registration, but rerun configuration to refresh `OnAssetsDirectory`'s configure-time glob before rebuilding. Do not assume an ordinary source rebuild discovers new ADL files.
- The two **ambience** examples build their DSP graphs directly in C and do not register a local `Assets` tree or read the four scene demos' ADL assets. Do not impose their asset-directory working-directory requirement on these programs.
- The four scene demos keep distinct configuration-directory names in their child guides. Streaming uses `SpaceshipAmbienceStreaming` and the engine's configuration load/save helpers; the finite audio example has no Application configuration-file path. Do not unify names or assume the same load phase across applications.

## Scene-Based Demo Patterns And Lifetime

- This section applies to **ColorSources, FontTexture, FreeFlyCamera and TextRendering**, not automatically to the ambience programs. Each has its own Application subclass of `Arcadia.Engine.Application`, owns a scene manager and a scene quit-request connection, and uses the engine-library `Arcadia.Engine.Scene`/`SceneManager` services. Consult the local `Main.c` for the initial scene and signal wiring.
- Their startup registers/starts audials, then visuals using `ApplicationHelper` services. Shutdown attempts configuration save, closes windows, drains engine events, and unlocks/releases backend contexts. Main manually locks the application. Preserve balanced cleanup/error paths rather than copying one demo's concrete implementation into another.
- Their loops step ARMS, update backend contexts and scene logics, bracket per-window audials/visuals scene updates with `beginRender`/`endRender`, and dispatch input/window events. Scene deltas use millisecond ticks from `Arcadia_getTickCount`; keep special rendering or event handling in the child source/guide.
- Preserve value-stack roots and exception cleanup during constructors, scene-change signals and asset loading. Scene-held nodes must be visited; C locals are not roots. Font/atlas locks, unmanaged glyph storage and cache ownership are **child-specific** contracts, not interchangeable demo scaffolding.
- Streaming has its own Application fields for graph/provider/stream node and visits them; its shutdown stops the stream before releasing the audials context. The finite example instead balances explicit graph/sample-buffer/backend locks and resource ref/unref around playback. Neither uses the four demos' scene loop.
