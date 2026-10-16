# Game Design Folder Guide

- Keep Markdown prose wrapped to a reasonable line length, preferably around 120 columns.
- Preserve tables, headings, links, and short list items when wrapping would make them less readable.
- Keep scenario documents focused on fiction and premise.
- Keep game mechanics documents focused on systems, loops, resources, and implementation-facing design notes.
- In this folder, `production` means game-development execution: implementation planning, feature breakdowns, asset and UI
  requirements, validation, build targets, and milestone work. It does not refer to the in-world production mechanic.

## Entry Point

- `README.md` is the common entry point for the Game Design folder. Update it when adding, moving, or renaming major
  subfolders or design documents.

## Scenario I Documentation

- `Scenario I/Art Direction.md` defines the visual rules for the scenario, including machine-first spaces, human-facing
  spaces, lighting, scale, interfaces, and battle/control-conflict presentation.
- `Scenario I/Room Types And States.md` is the reusable taxonomy for room categories, room-type anchors, visual states,
  and functional scale logic. Preserve existing anchors when revising room definitions because `Key Scenes.md` links to
  them.
- `Scenario I/Factions.md` is the reusable taxonomy for political, machine, archive, clone-derived, and successor factions.
  Keep it compatible with `Scenario I/Setting Truth.md`, especially the rule that no human crossed alive.
- `Scenario I/Key Scenes.md` describes individual fiction-facing scene concepts and links each scene to the applicable
  room type and state from `Room Types And States.md`. Image references in this file should point to the current approved
  artwork for each scene.
- When changing key-scene artwork direction, update the key scene, taxonomy, and art-direction documents together if the
  change affects the room's role, state, scale, or human-versus-machine framing.

## Game Mechanics I Documentation

- `Game Mechanics I/Progression Model.md` defines the mechanics-facing progression spine. Keep it aligned with
  `Game Mechanics I/Interaction Model.md` and the Scenario I life-cycle arc.
- `Game Mechanics I/Resource Model.md` defines physical, operational, interpretive, and legitimacy resources. Keep resources
  tied to fiction-facing constraints rather than generic currencies.
- `Game Mechanics I/Reboot Model.md` defines reset-like mechanics as diegetic reboot, recompile, certification,
  integration, and reconciliation systems. Avoid generic prestige framing.

## Production I Documentation

- `Production I/Technical Requirements.md` tracks implementation-facing work for turning Scenario I and Game Mechanics I into
  a playable executable.
- `Production I/UI Direction.md` describes fiction-facing UI presentation such as the starting screen, diagnostic subtitles,
  options screen, controls, and required UI widgets. Keep it aligned with `Game Mechanics I/Interaction Model.md`.
- `Production I/Engine UI Standard Controls Plan.md` tracks the implementation plan and checklist for standard controls in
  `Arcadia.Engine.UI`. Keep it aligned with `Production I/UI Direction.md`, `Production I/Technical Requirements.md`, and
  the current Engine UI implementation status.
