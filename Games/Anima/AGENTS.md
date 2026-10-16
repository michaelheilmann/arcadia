# AGENTS.md

## Start Here

- This module builds the Scenario I game executable, project title `Project Arcadia's Anima`.
- CMake product name: `Arcadia.Games.Anima`.
- Keep game-specific logic, scene descriptors, assets, configuration, and save data here or in supporting game modules, not
  in engine examples.
- General repository conventions are in `AGENTS.md` at the repository root. Scenario I design constraints are in
  `Documentation/Game Design/Scenario I/`.

## Build And Test

- Build target name: `Arcadia.Games.Anima`.
- The application stores settings in the user configuration directory under `<config-dir>/Anima/Configuration.txt`.
- This executable is currently a minimal bootstrapping target. It should grow only after the required engine facilities are
  implemented in the order documented by `Documentation/Game Design/Production I/Technical Requirements.md`.
