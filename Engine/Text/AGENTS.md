# AGENTS.md

## Scope

- `Engine/Text` is the reusable text-markup library (`${MyProjectName}.Engine.Text`). It currently owns the markup scanner/parser, parsed text-run descriptors, text alignment enum, and markup word-type enum.
- Public consumers include `Arcadia/Engine/Text/Include.h`; individual headers intentionally reject direct inclusion unless `ARCADIA_ENGINE_TEXT_PRIVATE` is set by the aggregate include.
- Keep this library independent from rendering and application-specific dynamic values. The parser records dynamic tokens by name (for example `fps`); consumers decide what a token means.

## Build

- Product name: `Arcadia.Engine.Text`.
- Test target: `Arcadia.Engine.Text.Tests.MarkupTests`.
- Add new source/header files in `Engine/Text/Library/CMakeLists.txt`.
- The library depends on `Ring2`, `Collections`, `Logging`, and `Languages`.
