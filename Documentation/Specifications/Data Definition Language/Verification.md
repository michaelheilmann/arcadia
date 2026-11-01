# DDL contract and specification handoff (#15, #16)

The original #15/#16 record and [strict hexadecimal-escape follow-up](#approved-follow-up-strict-hexadecimal-escape-digits) below are historical evidence.
The current contract includes [braced-only escapes](#approved-follow-up-braced-only-unicode-scalar-escapes), which supersede the unbraced syntax,
and [rejection of top-level trailing commas](#approved-follow-up-no-top-level-trailing-comma),
with [double-quoted strings only](#approved-follow-up-double-quoted-strings-only)
and the [ASCII name profile](#approved-follow-up-ascii-name-profile),
plus [lexical rejection of malformed numeric words](#approved-follow-up-malformed-number-lexical-errors).
The latest editorial pass is [shared escape cleanup and specification review](#shared-escape-cleanup-and-specification-review).

## Baseline and ownership

- Inspected `develop` revision: `7d0918ac41f1f8b693c4d4190e7bcbb53a2469fd`.
- Initial working tree was clean. The maintainer staged snapshots during this session; the agent did not stage, commit, push or alter the index.
  Some final source, generated-page and handoff updates remain unstaged. Review both staged and unstaged diffs before committing.
- Production DDL/DDLS behavior and shared `Specifications/specification.css.i` are unchanged.
- Reader path: `DDL/Library/Sources/Arcadia/DDL/Reader/{DefaultReader,Scanner,Parser}.c`.
  Value representation: `Nodes/{NumberNode,StringNode,BooleanNode,VoidNode}.c`.
  Duplicate-name checking: `SemanticalAnalysis.c`. `DefaultReader_run` does not invoke that checker.
- Source specification: `index.html.te`, the three translation fragments, `reader-guide.i`, `conformance.i` and DDL-local `ddl.css.i`.
- Canonical output remains `.Website/specifications/data-definition-language/index.html`, generated, not hand-edited.
- Common lexical rules were inspected in `Common Lexical Translations/2. Productions.i`; their independent inconsistencies are not silently repaired here.
  In particular, its HexadecimalDigit range comment is broader than conventional hexadecimal notation, and its escape labels contain mismatches.
  The conformance notes do not authorize those non-hexadecimal forms as portable DDL.

## Observed acceptance/rejection matrix

Evidence: `DDL/Tests/DDL/SyntacticalAnalysis/.../Main.c`, named cases `DocumentationContract`,
`FirstDocument` and the existing `SyntacticalAnalysis`; semantic cases in
`DDL/Tests/DDL/SemanticalAnalysis/.../Main.c`. Accepted scalar cases assert stored bytes, not just successful return.
Here **L** = `LexicalError`, **S** = `SyntacticalError`, **M** = `SemanticalError`, **A** = `ArgumentValueInvalid`.

| Area | Input | Observed result / representation |
| --- | --- | --- |
| Boolean | `true`, `false` | Accept Boolean, preserve keyword text |
| Void | `void` | Accept Void, preserve keyword text |
| Wrong case / unknown keyword | `True`, `null` | S (names are not values) |
| Document | empty input; `true false`; `true, false` | S |
| Root comma | `true,`; `true,,` | Accept Boolean; S, respectively |
| Empty collections | `[]`, `{}` | Accept List / Map |
| Trailing commas | `[1,]`, `{x:1,}` | Accept List / Map |
| Separators | `[1 2]`, `[,]`, `{,}`, `{x 1}` | S |
| Map names | `{"x":1}`, `{true:1}` | S |
| Map names | `{_x2:1, X:2}`, `{_:1}` | Accept; `_` alone differs from shared Name production |
| Trivia | block comment, space, tab, CRLF, LF, line comments around/in `[true, void]` | Accept List |
| Trivia errors | `/* unfinished`; vertical tab before `true` | L |
| Numbers | `01`, `+1`, `-1.0e-10`, `1.`, `.1`, `1E+2`, `123456789012345678901234567890` | Accept Number; preserve exact text |
| Numeric errors | `1e`, `.`; `0x10` | L; S, respectively |
| Numeric validation boundary | `+`, `-`, `+e2` | Scanner admits numeric word; Number constructor rejects with A |
| Strings | `""`, `"Stone Lantern"` | Accept decoded String |
| Escapes | `"\u0041\n"`; backspace/form feed/tab/vertical tab/backslash/quote | Accept; assert decoded bytes |
| Existing escape regression | newline/CR/tab/vertical tab/NUL/backslash; `\u1d28` | Accept, including embedded NUL (explicit byte lengths) |
| Direct Unicode | UTF-8 U+00E9 | Accept; exact UTF-8 bytes |
| Supplementary Unicode | UTF-8 U+1F3EE in quotes | L on current reader path; not claimed fully supported |
| String errors | `'text'`, `"\q"`, `"\u12"`, raw LF in quotes, unterminated string | L |
| Hexadecimal divergence | `"\u000G"` | Accept decoded U+0017, despite invalid hexadecimal digit |
| Complete nested example | Stone Lantern example in `reader-guide.i` | Accept; assert five keys in source order, types, two list elements, nested Void; semantic analysis succeeds |
| Duplicate map names | `{x:void, x:1}`; `[{x:1, x:2}]`; `{outer:{x:1, x:2}}` | Parsing succeeds, explicit semantic analysis reports M |
| Name comparison/scope | `{x:1, X:2, inner:{x:3}}` | Parsing and semantic analysis succeed |

Number is retained text, not arbitrary-precision arithmetic. String text is decoded during scanning and stored as UTF-8;
`codePoints` in semantic actions refers to that associated text, not always to the source spelling.
Surrogate-pair handling and normalization are not promised. No runtime fix was made for any observed divergence.

## Changes and compatibility

- Reader-facing order: overview, complete example, six-type table, pipeline, all three formal phases, errors, conformance, references.
- Existing heading IDs retained. Previously linked but absent semantic subsection IDs now exist; new syntax/type headings have stable IDs.
- Public grammar symbols and all syntax-directed production/action tables retained; Colon added to the lexical alternatives.
- Removed inconsistent manual numbering, malformed Void TeX, pseudo-semantic markup and malformed table row; made attribute assignment consistent.
- Semantic tables have captions and scoped column headers. Examples use `pre/code`; DDL-local CSS supplies responsive layout, horizontal formula/table scrolling and visible focus.

## Verification and remaining work

Platform: Windows x64, Visual Studio 2022 / MSBuild 17.14, Debug; existing external build tree
`C:/develop/Arcadia/Build/x64`, with dependencies already present (no dependency installation rerun).

Baseline: existing DDL/DDLS CTest selection passed. An initial build command guessed nonexistent aggregate
`Arcadia.DDLS.Tests`; corrected to the three real DDLS test targets below. Initial contract runs corrected preliminary
expectations for bare signs, supplementary Unicode and malformed escape decoding; the matrix records executed outcomes, not those guesses.

```powershell
cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.DDL.Tests.LexicalAnalysis Arcadia.DDL.Tests.SyntacticalAnalysis Arcadia.DDL.Tests.SemanticalAnalysis Arcadia.DDL.Tests.Writer Arcadia.DDLS.Tests.Reader Arcadia.DDLS.Tests.Validation Arcadia.DDLS.Tests.SchemaReferenceValidation
ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug -R '^Arcadia\.(DDL|DDLS)\.' --output-on-failure
cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.Website.Generate
git diff --check
```

DDL/DDLS verification passed: all 37 discovered cases, no failures or skips. The website generation target passed.
Both `git diff --check` and `git diff --cached --check` passed. All five pre-existing heading anchors and every
pre-existing public lexical/syntactical grammar symbol were checked against HEAD and retained.
The generated depfile includes all new source fragments. Website target spelling was verified from
`Documentation/CMakeLists.txt`, not assumed. Browser QA uses external temporary Node/Playwright/html-validate tools;
no tool dependencies or build products were added to the checkout.

Design archive: the GitHub attachment URL returned HTTP 404 on this workstation. The user supplied extracted assets in
`C:/develop/Arcadia/Handover`. Inspected `ddl-overview.i`, `ddl-spec-preview.html` and both preview images.
The implementation follows their example-first order, teal accents, dark code panel, type reference and single-column mobile layout,
within the existing specification framework. The abbreviated mockup grammar was not copied; the full formal rules remain.

### Generated-page QA

Passed with Node 22.22.0, Playwright 1.64.1 / Chromium 156, html-validate 11.17.0 and axe-core/playwright 4.14.0:

- Selected HTML structural checks: close order, permitted content/parents, unique/valid IDs and table-header scope.
  This is not a claim of a full standards-validator audit of shared site assets.
- All TOC/internal targets resolve, no duplicate IDs, every section heading is represented in the TOC, four cross-page references/targets resolve locally.
- All 127 MathJax containers render without MathJax error nodes or browser exceptions.
- The published first-document text is byte-for-byte equal to the C fixture's assembled string.
- Viewports 1440, 768 and 390 pixels: document width equals viewport width, with intentional local horizontal scrolling for wide tables/formulas.
- Axe WCAG 2/2.1 A/AA selection: zero violations at all three widths. Skip-link keyboard focus is visible;
  overflowing code examples are explicitly keyboard-focusable. Screenshot review caught and corrected inherited code-text contrast.
- Desktop/tablet/mobile screenshots were inspected. Firefox, Safari, physical mobile devices and manual screen-reader checks were not run;
  automated axe results are not a blanket accessibility certification.

External evidence directory: `C:/Users/Anwender/AppData/Local/Temp/opencode/ddl-qa-tools`.
It contains `check.mjs`, `package-lock.json`, `ctest.log` and `browser.log`.
Screenshots: `C:/Users/Anwender/AppData/Local/Temp/opencode/ddl-{1440,768,390}.png`
and corresponding `ddl-{width}-viewport.png` files. Re-run the retained QA script with:

```powershell
& 'C:/Users/Anwender/AppData/Local/Temp/opencode/ddl-qa-tools/node-v22.22.0-win-x64/node.exe' 'C:/Users/Anwender/AppData/Local/Temp/opencode/ddl-qa-tools/check.mjs'
```

### Editorial review

- Complete fixture-backed example precedes the six-type reference; each type has accepted/rejected inputs, reasons and exact rule links.
- Formal lexical, syntactical and semantical material is retained, not replaced with the mockup's abbreviated grammar.
- Number preservation is distinguished from mathematical arithmetic; String decoding and `codePoints` are explicit.
- All error examples have verified statuses; diagnostic messages are not invented.
- Canonical URL, existing anchors and cross-links are retained. No claim of DDLS v0.3 implementation is made.
- Desktop/tablet/mobile rendering was inspected against the supplied conceptual previews; shared page framework is intentionally retained, not replaced by the static mockup.

Next: maintainer review of the changes and the separate behavior decisions listed in `conformance.i`.
Shared SPEC styles remain outside this task's ownership. No production parsing changes, commits, pushes or issue closures were performed.

## Approved follow-up: strict hexadecimal escape digits

- Inspected revision: `b7724af7142cd415ce1c5017e56faae438887534` (`develop`).
- Maintainer approved exactly four digits from `0-9`, `a-f` or `A-F` for `\uXXXX` escapes in chat.
  This supersedes the original matrix's acceptance of `"\u000G"`; surrogate-pair handling and other divergences remain undecided and unchanged.
- Initial working-tree edit was only the generated DDL page's footer date (31 October to 1 November); regeneration preserves that date update.
- Changed `DDL/Library/Sources/Arcadia/DDL/Reader/Scanner.c` to stop the uppercase hexadecimal range at `F`.
  Corrected the shared `Lexical.HexadecimalDigit` range in `Common Lexical Translations/2. Productions.i`.
  Updated DDL semantics/conformance text and the existing `DocumentationContract` fixture, without changing shared CSS.
- Regression cases cover uppercase/lowercase valid boundaries, non-hex letters and punctuation, invalid `G` in each of the four positions,
  truncated escapes, and the four-digit boundary (`"\u00410"` decodes to `"A0"`, not a five-digit escape).
- Red gate before changing production code: built `Arcadia.DDL.Tests.SyntacticalAnalysis` and ran its `--case DocumentationContract`;
  the unchanged scanner failed the new `"\uG000"` expectation (`EncodingInvalid` instead of `LexicalError`).
- Rebuilt the seven DDL/DDLS test targets with the original command above; generated the site with
  `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.Website.Generate`.
  Rebuilding the template-engine consumer also refreshed footer dates on other generated pages; those additional page diffs contain only `Last modified on` changes and are retained per repository policy.
- Generated DDL and Common Lexical pages contain the corrected rules. The retained `check.mjs` gate passed again:
  HTML structure, four cross-page targets, all anchors/TOC, 127 MathJax containers, first-document fixture equality,
  no page overflow and zero selected axe violations at widths 1440/768/390, visible skip-link focus and no browser exceptions.
  Browser/tool/platform limitations remain those recorded above.
- Original QA logs/screenshots were preserved under the external evidence directory's `original-specification-qa` subdirectory before refreshing screenshots.
  Follow-up logs are `hex-escape-browser.log` and `hex-escape-ctest.log` in that evidence directory.
- Final gates passed: direct `Arcadia.DDL.Tests.SyntacticalAnalysis.exe --case DocumentationContract`,
  then `ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug -R '^Arcadia\.(DDL|DDLS)\.' --output-on-failure`
  (37 passed, zero failures/skips). Both working-tree and staged whitespace checks passed.
- Next decision: whether surrogate-pair escapes should be supported. No other reader behavior was intentionally changed; no commits or pushes were made.

## Approved follow-up: braced-only Unicode scalar escapes

- Baseline HEAD: `b7724af7142cd415ce1c5017e56faae438887534`. The preceding strict-digit changes and generated date refreshes were still uncommitted at the start; they are preserved.
- Maintainer decision: support only `\u{...}`, containing 1-6 hexadecimal digits and denoting a Unicode scalar value (U+0000 through U+10FFFF, excluding U+D800 through U+DFFF).
  Unbraced escapes are rejected with `LexicalError`, not accepted as a compatibility alias. Surrogate-pair escapes are not supported; use the complete scalar value instead.
  Signs, spaces, underscores, empty braces, missing braces and more than six digits are invalid. Leading zeros count toward the limit.
- Inventory: repository search found no `\u` occurrences in `.ddl`, `.adl`, `.ddls` or `.pdl` inputs.
  DDL C-string fixtures and live documentation examples were migrated. VPL's independent four-digit syntax, C universal-character escapes used for expected bytes,
  JavaScript in the common site header, and historical evidence were deliberately not rewritten.
- DDL changes: `Reader/Scanner.c` reads/counts braced digits and validates scalar bounds before encoding;
  `Writer/Unparser.c` emits braced escapes for control characters (for example `\u{0000}`), and keeps supplementary text as UTF-8.
  Scanner and writer fixtures assert decoded bytes, embedded NUL, old-form rejection, malformed inputs, scalar boundaries and serialized/reparsed string equality.
- Required native Ring1 fixes: the byte-reader and UTF8ArrayIterator both had reversed four-byte upper-bound checks; both now accept valid supplementary values and reject surrogate encodings/out-of-range values.
  `Unicode.c` had an incorrect four-byte output prefix (`0x70` instead of `0xF0`); corrected it for supplementary encoding.
  These remain hand-written C changes. No type-system, GC or generation ownership change was made.
- New named Ring1 case: `ScalarBoundaries`, covering U+10000, U+1F3EE and U+10FFFF decoding/byte ranges/exact encoded bytes,
  shared UTF-8 validation, and rejection of raw U+D800, U+DFFF and U+110000 encodings. Existing overlong-encoding tests are retained.
- Source documentation: Common Lexical string production/transformation, DDL quick reference, semantic text and migration/conformance notes.
  Existing public grammar symbols/heading IDs and shared specification CSS are retained.
- Intermediate regression evidence: after fixing the streaming reader and encoder, the new supplementary DDL fixture still failed with `EncodingInvalid`;
  inspection found the same inverted bound in UTF8ArrayIterator, used by string validation. The new Ring1 validation checks cover that path as well.
- Final checks passed on Windows x64 / Visual Studio 2022 Debug: rebuilt all 24 Ring1 test targets from their owning CMake definitions,
  all seven DDL/DDLS targets and `Arcadia.Website.Generate`. The following CTest selection passed all 123 discovered cases, with no skips or failures:
  `ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug -R '^Arcadia\.(Ring1\.Tests|DDL|DDLS)\.' --output-on-failure`.
  Direct DDL reader/writer suite checks also passed. The new writer fixtures account for the unparser's existing final newline.
- Generated-page checks passed using the retained `check.mjs`: DDL structure/TOC/anchors, four cross-page references, 127 MathJax containers,
  exact first-document fixture text, no document overflow and no selected axe violations at widths 1440/768/390.
  Common Lexical's six MathJax containers also render without errors; its braced rule is present and the erroneous standalone `#5c #75` alternative
  was corrected to the documented tab spelling `#5c #74`, so it no longer admits bare `\u`.
- Both `git diff --check` and `git diff --cached --check` passed. Generated footer-date churn and prior staged/unstaged work were preserved;
  the agent did not stage changes, commit or push. HEAD remained the inspected revision.
- Evidence in the external directory recorded above: `braced-escape-build.log`, `braced-escape-ctest.log`, `braced-escape-browser.log`.
  Previous strict-digit logs/screenshots are preserved under `strict-hexadecimal-qa`; original specification evidence remains under `original-specification-qa`.
  Linux/sanitizers, other browsers and manual screen-reader testing were not run.
- Remaining scope: no Unicode normalization; no migration of independently specified VPL/PDL escape syntax; external users must migrate their own old-form DDL documents.

Exact target-discovery/build command used for the broad gate (PowerShell, repository root):

```powershell
$targets = @(Get-ChildItem Runtime/Ring1/Tests -Recurse -Filter CMakeLists.txt | ForEach-Object {
  $text = [System.IO.File]::ReadAllText($_.FullName)
  $match = [regex]::Match($text, 'set\(this \$\{MyProjectName\}([^\)]+)\)')
  if ($match.Success) { 'Arcadia' + $match.Groups[1].Value.Trim() }
})
$targets += @('Arcadia.DDL.Tests.LexicalAnalysis','Arcadia.DDL.Tests.SyntacticalAnalysis','Arcadia.DDL.Tests.SemanticalAnalysis','Arcadia.DDL.Tests.Writer','Arcadia.DDLS.Tests.Reader','Arcadia.DDLS.Tests.Validation','Arcadia.DDLS.Tests.SchemaReferenceValidation','Arcadia.Website.Generate')
cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target $targets
```

After the last fixture edits, rebuilt `Arcadia.DDL.Tests.SyntacticalAnalysis` and `Arcadia.DDL.Tests.Writer`;
after the final Common Lexical correction, rebuilt `Arcadia.Website.Generate` and reran the browser gate.
Next: maintainer review of this intentionally breaking input-format change and migration notes; remaining non-Unicode conformance differences are unchanged.

## Approved follow-up: no top-level trailing comma

- Inspected HEAD: `664855565b913fd687fe1bb050a16243d413bdfc` (`develop`); initial working tree clean.
- Maintainer approved rejecting a comma after the complete document value. Collection trailing commas remain accepted.
  Whitespace/newlines/comments may still follow the root value before end-of-input.
- Changed only the root entry point in `DDL/Library/Sources/Arcadia/DDL/Reader/Parser.c`: removed the optional comma consumption;
  its existing end-of-input check now reports `SyntacticalError`. List/map parsers and the formal Sentence production are unchanged.
- Expanded the existing `DocumentationContract` fixture: rejection for all six root value types, collection roots with both inner and outer trailing commas,
  comments/whitespace around root commas, and accepted trailing trivia after scalar/list/map roots. Existing inner collection-comma positives remain.
- Red gate: rebuilt `Arcadia.DDL.Tests.SyntacticalAnalysis` and ran `--case DocumentationContract` before changing the parser.
  It failed on unexpected acceptance of `true,` (expected `SyntacticalError`, actual `Success`).
- Inventory: the external `check-root-commas.mjs` scanned 588 tracked `.ddl`, `.adl`, `.ddls`, `.pdl` and `.env` inputs;
  none has a final comma outside strings/comments. This is a lexical inventory, not a claim that every file is valid DDL, and does not include C-string fixtures.
  No standalone input files needed migration.
- The first full selection passed the DDL cases but failed 30 DDLS cases because their embedded C-string schemas/documents still had root commas.
  Migrated 37 root delimiters in the five DDLS fixture files (`Reader/{Main,Schema1}.c`, `Validation/{Main,Default}.c`, and `SchemaReferenceValidation/Main.c`).
  Inner collection commas, fixture content and semantic-error expectations are unchanged, so those tests continue checking their original semantics rather than failing during DDL parsing.
- Updated `syntactical-translation.i` and `conformance.i` to state the approved rule, give accepted/rejected examples and explain migration.
  Historical matrices and prior verification records above are preserved, not current conformance evidence.
- The seven DDL/DDLS test targets and real `Arcadia.Website.Generate` target built successfully in the existing external Debug tree.
  Direct `--case DocumentationContract` passed after the fix. After rebuilding the three DDLS targets for fixture migration,
  `ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug -R '^Arcadia\.(DDL|DDLS)\.' --output-on-failure` passed all 37 discovered cases, with no failures/skips.
- Generated-page `check.mjs` passed: HTML structure, TOC/anchors, cross-links, MathJax, fixture text equality, no page overflow and no selected axe violations at widths 1440/768/390.
  Both working-tree/staged whitespace checks passed. Previous braced-escape screenshots/logs were preserved under `braced-unicode-qa` before browser verification.
- Evidence in the external directory recorded above: `root-comma-build.log`, `root-comma-fixture-build.log`, `root-comma-initial-ctest.log`,
  `root-comma-ctest.log`, `root-comma-browser.log`, `root-comma-inventory.log` and `check-root-commas.mjs`.
  Checks ran on Windows x64 / Visual Studio 2022 Debug; Linux/sanitizers, other browsers and manual screen-reader checks were not run.
- Next: maintainer review; other conformance differences and independent languages remain unchanged. No staging, commits or pushes by the agent.

## Approved follow-up: double-quoted strings only

- Inspected HEAD: `664855565b913fd687fe1bb050a16243d413bdfc`; the preceding root-comma changes were already staged by the maintainer and were preserved.
- Maintainer confirmed double quotes as the only DDL string delimiter. This retains current reader/writer behavior: no production implementation change in this batch.
  Single-quoted strings and character literals are not supported. Apostrophes inside double-quoted strings are ordinary text;
  embedded double quotes use `\"`. Braced Unicode escapes remain unchanged.
- `lexical-translation.i` explicitly restricts the DDL use of `Lexical.String` to the shared `Lexical.DoubleQuotedString` production,
  preserving public symbols and leaving the broader Common Lexical categories available to independently specified languages.
  `semantical-translation.i` and `conformance.i` state the intended restriction; the single-quote entry was removed from the unresolved-mismatch table.
- Existing `DocumentationContract` fixture now covers accepted apostrophes and escaped double quotes, rejected empty/one-character/text/Unicode single-quoted inputs,
  a mismatched closing quote, and unsupported `\'`. Writer fixtures assert exact double-quoted output and decoded-text round trips.
- Passed `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.DDL.Tests.SyntacticalAnalysis Arcadia.DDL.Tests.Writer Arcadia.Website.Generate`.
  Passed `ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug -R '^Arcadia\.(DDL|DDLS)\.' --output-on-failure`: all 37 discovered cases, no failures/skips.
- The retained `check.mjs` passed HTML structure, TOC/anchors, four cross-page targets, 129 DDL MathJax containers (including the explicit string selection),
  six Common Lexical MathJax containers, fixture-text equality, no page overflow and no selected axe violations at widths 1440/768/390, visible focus and no browser exceptions.
  Both staged and unstaged whitespace checks passed. Browser/platform limitations remain as recorded above.
- Evidence: `double-quotes-build.log`, `double-quotes-ctest.log` and `double-quotes-browser.log` in the external evidence directory;
  previous root-comma screenshots/logs were preserved under `root-comma-qa` before this check.
- Next: maintainer review. No changes to other languages, shared CSS, staging, commits or pushes by the agent.

## Approved follow-up: ASCII name profile

- Inspected HEAD: `664855565b913fd687fe1bb050a16243d413bdfc`. Earlier root-comma and double-quote work was already staged by the maintainer and was preserved.
- Maintainer approved `[A-Za-z_][A-Za-z0-9_]*` for DDL map-entry names, including `_`, `__` and `_1`.
  Names remain nonempty, ASCII and case-sensitive; exact lowercase `true`, `false` and `void` remain reserved.
  This retains current scanner behavior; no production implementation change in this batch.
- `lexical-translation.i` explicitly defines the DDL `Lexical.Name` profile without changing the shared Name production or independently specified languages.
  `conformance.i` records it as intentional and removes the underscore entry from the unresolved-mismatch table. Public grammar symbols and anchors are preserved.
- New named case `MapNames` checks ten actual stored key spellings and their associated numeric values, including keyword-prefix names and a case variant.
  `DocumentationContract` also checks reserved-word, digit-leading, empty, quoted and non-ASCII keys with exact syntactical/lexical statuses.
  Executable `--list` exposes `MapNames`; direct `--case MapNames` passed.
- Passed `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.DDL.Tests.SyntacticalAnalysis Arcadia.Website.Generate`.
  Rebuilt the syntactical suite after the final fixture edit, then passed
  `ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug -R '^Arcadia\.(DDL|DDLS)\.' --output-on-failure`: all 38 discovered cases, no failures/skips.
  The external browser gate's first-document extraction was scoped to `testFirstDocument`, so the new preceding name fixture cannot confuse example equality checking.
- The retained `check.mjs` passed structure/anchors/TOC, four cross-page targets, all 129 DDL and six Common Lexical MathJax containers,
  first-document fixture equality, no document overflow and zero selected axe violations at widths 1440/768/390; focus is visible and no browser exceptions occurred.
  Both staged and unstaged whitespace checks passed. Checks used Windows x64 / Visual Studio 2022 Debug and the browser tools recorded above;
  no Linux/sanitizer, other-browser or manual screen-reader claim is made.
- Evidence: `name-profile-build.log`, `name-profile-ctest.log` and `name-profile-browser.log` in the external evidence directory;
  previous double-quote screenshots/logs were preserved under `double-quotes-qa` before browser checks.
- Next: maintainer review. Other languages, shared CSS and remaining number-diagnostic behavior are unchanged. No staging, commits or pushes by the agent.

## Approved follow-up: malformed-number lexical errors

- Inspected HEAD: `5307ae563d3e3ee36702c83e96ef82bc5eea7f37` (`develop`); initial working tree clean.
- Maintainer approved rejecting missing mantissa digits in the scanner with `LexicalError`, rather than letting invalid numeric words reach Number-node construction.
  Accepted numeric forms are unchanged. Direct Number-node API validation remains untouched.
- `Reader/Scanner.c` now raises a lexical error when the optional sign is followed by neither a decimal digit nor a decimal point with digits.
  Existing fractional/exponent validation remains in place. No token-boundary or numeric-representation change was made.
- New lexical named case `NumberLiterals` checks 12 valid spellings/token kinds, 17 malformed inputs with exact `LexicalError` status,
  caller-stack restoration and scanner reset between inputs. It also confirms that `1x` remains two lexical words, not a newly merged invalid token.
  Parser-contract fixtures cover `+`, `-`, `+e2`, `-E2`, nested malformed numbers, valid `1e2`, and syntactical rejection of the two-word document `1x`.
- Red gate before the production fix: built `Arcadia.DDL.Tests.LexicalAnalysis` and ran `--case NumberLiterals`;
  it failed because the unchanged scanner emitted a numeric token for `+`. After the fix and final fixture rebuild, direct `--case NumberLiterals` passed; `--list` exposes the new case.
- `conformance.i` now describes the approved numeric error category and valid forms; removed the last obsolete mismatch-table row/table.
  Historical matrices above remain evidence for their revisions, not current conformance claims. Shared lexical escape-label inconsistencies are still noted separately.
- Rebuilt all seven DDL/DDLS targets with the command recorded above, plus `Arcadia.Website.Generate`, then rebuilt the lexical test target after its last assertion edit.
  Passed `ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug -R '^Arcadia\.(DDL|DDLS)\.' --output-on-failure`: all 39 discovered cases, no failures/skips.
- The retained browser gate passed structure, TOC/anchors, four cross-page references, 129 DDL and six Common Lexical MathJax containers,
  first-document fixture equality, no page overflow and zero selected axe violations at widths 1440/768/390, visible focus and no browser exceptions.
  Both staged and unstaged whitespace checks passed. Platform/browser limitations remain those recorded above (Windows Debug/Chromium, no Linux/sanitizer or manual screen-reader claim).
- Evidence in the external directory: `number-errors-build.log`, `number-errors-ctest.log`, `number-errors-browser.log`.
  Previous name-profile screenshots/logs were preserved under `name-profile-qa` before browser checks.
- Next: maintainer review of this diagnostic-classification change. No changes to independently specified languages, shared CSS, staging, commits or pushes by the agent.

## Shared escape cleanup and specification review

- Inspected HEAD: `5307ae563d3e3ee36702c83e96ef82bc5eea7f37`. Prior numeric-diagnostic changes were already staged by the maintainer and were preserved.
  No production implementation file was changed in this editorial batch; runtime behavior and independently specified language profiles remain unchanged.
- Common Lexical string productions now spell `\b` with `#62` and decode it to U+0008, not bell; tab uses `#74`/U+0009.
  Added the existing `\v` and `\0` forms with U+000B/U+0000, matching quote/backslash decoding, and explicit unknown-escape rejection.
  Raw backslashes are excluded from ordinary string characters so they cannot bypass escape validation.
  Replaced the malformed list opener and converted the string/comment production blocks to `pre/code`; all existing production labels and heading IDs are retained.
- The final source review corrected unambiguous referenced-symbol/transcription errors: the space alternative for `Lexical.Whitespace` (retaining the legacy `Lexial.Whitespace` label),
  ASCII Alphabetic/Underscore definitions, the Digit/DecimalDigit alias, Lexical.True/False aliases (retaining `true`/`false` labels), the stray Void `#`, and the Period code-point marker.
  Corrected the Common introduction's dead context-free-grammar link to the existing MDNP section. DDL now explicitly defines its retained Newline label's grouping.
  No changes to shared Name requirements or independently specified newline/escape profiles were inferred from these editorial fixes.
- Updated DDL overview/conformance notes to remove obsolete mismatch warnings and link the corrected escape reference.
  VPL prose now accurately describes its independent supported set and retained four-digit Unicode syntax, rather than referring to repaired common-listing errors.
- Existing reader fixtures already assert backspace/form-feed/tab/vertical-tab/backslash/quote and embedded-NUL behavior.
  Added exact decoded-byte fixtures for the newly published `"A\tB\n"` and `"\0"` examples, plus lexical rejection of the former documentation typo `\d`.
  The test changes do not modify production behavior or add duplicate whole-suite registration.
- Static review confirmed all 38 pre-existing Common Lexical production labels remain. Both staged and unstaged whitespace checks passed.
- Passed `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.DDL.Tests.SyntacticalAnalysis Arcadia.Website.Generate` after the final source edits.
  Passed `ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug -R '^Arcadia\.(DDL|DDLS)\.' --output-on-failure`: all 39 discovered cases, no failures/skips.
- The retained `check.mjs` passed selected HTML structural checks for both DDL and Common Lexical, all DDL anchors/TOC,
  five DDL cross-page references, 129 DDL and six Common Lexical MathJax containers, and the corrected Common-to-MDNP grammar-reference target.
  First-document fixture equality, no DDL page overflow and zero selected axe violations at widths 1440/768/390, visible focus and no browser exceptions also passed.
- Evidence in the external directory: `escape-docs-build.log`, `escape-docs-ctest.log`, `escape-docs-browser.log`.
  Previous numeric-diagnostic logs/screenshots were preserved under `number-errors-qa`. Checks ran on Windows Debug/Chromium;
  no full-site standards/accessibility certification, Linux/sanitizer, physical-device or manual screen-reader claim is made.
- Next: maintainer review of the source/generated-page changes and recorded decisions. Shared CSS, staging, commits and pushes remain untouched by the agent.
