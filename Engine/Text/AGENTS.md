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

## Tests

- MarkupTests exposes `ParseRuns`, `RejectUnknownAttribute`, and `RunModels` through Arcadia.Testing, in that order. Use `--case NAME` for one component or omit arguments for the full suite, continuing after component failure. The old `MarkupTests` whole-suite selector is retired; CTest still registers one test executable.
- `RejectUnknownAttribute` deliberately succeeds when its original `Arcadia_Tests_safeExecute` call reports failure, and fails on unexpected normal completion. Keep this inverted expectation and the original process/jump-target boundary. `RunModels` retains its grouped regression helper and internal execution boundaries.

## PDL Ownership And Verification

- Engine/Text is a normal PDL module named Arcadia.Engine.Text, included in the root Modules.pdl. It remains independent of the Engine runtime/rendering libraries. Production builds compile committed outputs; normal CLI regeneration owns the files rather than a build-time publishing step.
- MarkupTextRun.pdl owns MarkupTextRun.h/.c.g, its thirteen fields and seven-argument factory. Preserve constructor order text/glyphSource/dynamicToken/pixelSize/colorName/alignment/fixedPlacement. text, glyphSource and colorName are required but may be empty or aliased; dynamicToken is nullable. A small native requireRunStrings binding enforces the required-string checks without adding content/size policy. All four references are traced by the generated visitor. Rectangle coordinates/sizes retain zero defaults, hasRectangle/lineBreakBefore start false, and fixedPlacement retains the supplied Boolean.
- TextAlignment.pdl owns ordinary TextAlignment.h/.c outputs: Left=0, Center=1, Right=2. Preserve runtime/C names and enum ABI. Runtime constructor callbacks accept legacy Integer32 alignment arguments and exact TextAlignment-tagged values; a numerically equal value of another enum is not the same enum. Existing out-of-enumerator Integer32 payloads remain transportable; do not invent enumerator-membership validation.
- MarkupWordType, scanner, parser and TextLayout implementations remain native. Do not claim the header cleanup of MarkupWordType as a PDL implementation migration.
- RunModels.c extends existing MarkupTests with separate safeExecute boundaries: thirteen-field/slot/factory ABI models, alignment metadata/boxing, nullable token and required-string/empty/alias behavior, scalar boundaries/defaults, all constructor parameters' tag/type/count errors above 255 stack slots, native subclass construction, independent four-string/payload GC/finalization and typed/legacy enum transport. Existing parser cases additionally force collection while only the parsed-run list is rooted. Temporary Native/Generated providers are retired; no permanent comparison target is added.
