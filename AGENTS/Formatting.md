# Cross-Language Source Formatting

This document is normative for maintained, hand-edited Arcadia source files. The
C/C++-specific requirements of [CxxSourceCodeStyle.md](CxxSourceCodeStyle.md)
and the string/diagnostic requirements of
[CodingStandards.md](CodingStandards.md) continue to apply. A language
specification determines what is syntactically valid; this guide determines
which of those valid forms is preferred when adding or touching source.

## Scope and exceptions

- Follow this guide for maintained C/C++, CMake, PDL, ADL, DDL, DDLS, VPL,
  Markdown, templates and related text sources.
- Use LF line endings and strip trailing ASCII spaces/tabs. Preserve an existing
  non-ASCII encoding or BOM; use ASCII for new code except when real Unicode
  text is required. End ordinary text files with one LF.
- A touched legacy file can be normalized within the same focused change. Do
  not reformat unrelated files or published/generated `.Website` artifacts just
  to satisfy a style preference.
- Generated `.c.g`, `.h`, `.h.i` and other generated files belong to their
  generator. Modify generator output policy and regenerate deliberately; do not
  make hand edits that will disappear. Keep test fixture byte-for-byte
  expectations intact unless the fixture update is intentional.
- If a local area intentionally uses a different convention, document the
  exception in its scoped `AGENTS.md`. A neighboring file is a useful example,
  not an implicit waiver of explicit shared rules.

## C and C++

- Follow [CxxSourceCodeStyle.md](CxxSourceCodeStyle.md) for declaration layout,
  guards, includes, runtime ownership, comments and object-system conventions.
- A function with zero parameters uses `void` in the parameter list in C17.
  `()` is not a zero-argument function prototype in that language version.
- Function-pointer typedefs and dispatch-table members may use compact
  signatures; ordinary function declarations and definitions use the project's
  one-parameter-per-line convention.
- Module-private include guards must check both definition and value `1`.
- Do not run an off-the-shelf C formatter over the tree unless its output has
  been proven compatible with the project-specific conventions.

## PDL

- Keep declarations, constructors, methods and procedures with one logical
  statement/instruction per line. Indent brace-delimited bodies by two spaces.
- Prefer the established multiline parameter format for long parameter lists:
  `(` on the next line at two spaces, each parameter at four spaces, and `)`
  on its own line at two spaces. Short declarations may remain on one line when
  easy to read; avoiding a broad canonical reformat takes precedence.
- Use a space before and after `:` in type annotations and around `=`. Preserve
  qualified identifiers and exact case. Do not rewrite literal tokens,
  registers, labels, or generated declarations for aesthetic reasons.
- The PDL grammar and compiler tests decide whether a construct is valid;
  this guide does not authorize syntax changes.

## ADL, DDL and DDLS assets

- Use two-space indentation for nested maps and lists, with a space after a
  comma in ordinary inline lists. Use `key : value` when matching existing ADL
  examples. Keep named keys in stable semantic order when the reader or human
  review relies on it.
- Break long maps or arrays into multiline layouts when that helps reading.
  Large geometry/sample arrays may group numeric tuples per line to make their
  dimensionality visible. Avoid padding or reordering data just to align it.
- Retain commas and trailing commas exactly where the respective parser allows
  them; never assume JSON syntax applies. Never alter field names, token case,
  numeric precision, references or asset names solely for formatting.
- `.adl` denotes Arcadia asset definitions even if their concrete syntax is
  parsed by DDL; `.ddl` denotes DDL-only inputs.

## VPL

- Canonical VPL source uses `.vpl` and keeps one instruction per line, stable
  argument order, and case-sensitive identifiers. Syntax is governed by the
  VPL specification; formatting must not transform instructions.

## CMake

- Use two spaces for nested command bodies and consistent indentation within a
  block. Prefer `if(...)`, `foreach(...)`, `function(...)` and related command
  syntax (no space between command name and opening parenthesis) for new code.
- When splitting commands, put each significant argument on a separate
  indented line where practical. Quote paths containing spaces and do not
  change evaluation, variable scoping, generator expressions or list semantics
  for formatting alone.
- Keep `CMakeLists.txt` and `.cmake` files under the same whitespace policy.

## Markdown and documentation

- Use one `#` heading per standalone document and a consistent heading
  hierarchy. Use fenced code blocks with a language identifier for code and
  shell examples. Separate headings, lists and paragraphs with blank lines.
- Prefer meaningful link text over raw URLs. Proofread headings, examples,
  command names, and technical terminology. Keep normative specifications,
  tutorials and module interfaces distinct per [Documentation.md](Documentation.md).
- Do not change specification anchors, table semantics or generated website
  output as part of unrelated formatting cleanup.

## Validation

- Read-only: `python3 Tools/CodeStyle/check.py --check` is equivalent to running
  the checker without `--fix` (the checker defaults to inspection). Use
  `python3 Tools/CodeStyle/check.py --strict path/to/file` for additional
  header-guard and C17 checks.
- Focused whitespace repair: `python3 Tools/CodeStyle/check.py --fix path/to/file`.
  This only normalizes CR/CRLF and trailing spaces/tabs; inspect all diffs.
- Run `python3 -m unittest discover -s Tools/CodeStyle/tests -p 'test_*.py'`
  before integrating new checker rules. Integrate repository-wide blocking CI
  only after existing debt is either repaired or explicitly baselined.
