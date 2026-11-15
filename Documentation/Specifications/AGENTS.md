# Specification Editorial And Visual Conventions

## Scope And Ownership

- These instructions apply to this entire tree: Common Lexical Translations, DDL,
  DDLS, MDNP, PDL, VPL and future specifications. They supplement the root and
  `Documentation/AGENTS.md` instructions; they do not replace them.
- Apply these conventions to the material being edited. Do not normalize every
  page or require identical file splits, section plans or language architecture.
- E01 (#22) owns the initial editorial guidance. The visual-consistency work
  (#21, initially V01) extends this same file; do not create competing policy.
  CSS declarations, design tokens and rendering implementation belong in stylesheets,
  not here. Editorial work alone does not authorize visual changes.
- A justified per-language exception is permitted: record its scope and rationale
  beside the affected material or in the task handoff. Obtain maintainer agreement
  for exceptions that affect a language contract or a shared convention.

## Technical English And Terminology

- Use concise, grammatical technical English. Define abbreviations and specialized
  terms before relying on them; use one term consistently for each concept.
- Distinguish source text, lexical words/tokens, syntax, values and validation.
  State the representation meant by an overloaded term such as "program" or "type".
- Preserve established language names, public symbols and production identifiers
  exactly, including capitalization. Do not rename `Syntactical` or `Semantical`
  symbols merely to prefer "syntactic" or "semantic" in prose.
- Separate normative requirements from explanations, notes, examples and proposed
  work. In new normative prose, use "must" for requirements, "should" for
  recommendations and "may" for permission. Do not mechanically reinterpret older
  wording or claim RFC requirement-keyword conformance without an explicit decision.

## Scope, Status And Compatibility

- State what a document defines and what it does not define. Shared lexical rules
  are building blocks; each consuming language owns its selections, refinements,
  start symbol and ambiguity handling. Do not imply that every shared alternative
  is accepted by every language.
- Keep specified behavior, draft proposals and verified implementation behavior
  distinct. Support implementation/conformance claims with current source and
  focused test evidence; record discrepancies instead of silently resolving them.
- Preserve DDLS's explicit **draft, v0.2** status unless a maintainer authorizes a
  status/version change. A clearer introduction is not evidence of implementation
  or release readiness.
- Editorial changes must not change accepted/rejected input, translation rules,
  mathematical meaning, public symbols or error contracts. If a correction could
  affect these, isolate it for maintainer decision and semantic verification.

## Structure And Formal Material

- Use one page-level `h1` and a logical heading hierarchy without skipped levels.
  Standardize headings below the unnumbered page title with decimal section numbers:
  `1 Introduction`, `2 Productions`, `2.1 Whitespace`, and so on. Keep numbering
  sequential and unique within each document, and contents labels aligned with it.
  Apply this in the owning page's editorial task, retaining existing anchor IDs;
  do not renumber unrelated pages as incidental cleanup.
- Retain Common Lexical Translations' repeated abstract: its separate abstract and
  the introduction's inclusion of that abstract are intentional maintainer choices.
- Start with a short statement of purpose, audience and prerequisites. A compact
  reader guide or complete example may precede formal rules, but must link to,
  not replace, the normative grammar or semantics. A shared-rule catalog need not
  pretend to be a complete executable language tutorial.
- Define grammar conventions, metavariables and mathematical notation before use
  or cite their owning definition. Distinguish literal input from notation; preserve
  namespaces, alternatives, ordering constraints, escapes and code-point spellings.
  Preserve formulas during editorial edits; presentation conversions need separate
  review of meaning and rendering.
- Label examples and invalid cases as such. Give the expected result or reason for
  rejection and distinguish lexical, syntactic, semantic and validation failures.
  Do not invent diagnostics or promise exact message text that is not contractual.
  Verify runnable examples against the relevant implementation when claiming support;
  label draft-only examples without implying current tool acceptance.

## Semantic Completeness And Conformance

- Specify observable language behavior independently of a particular parser,
  validator, runtime representation or implementation algorithm. Explain any
  deliberate dependency on another specification through a precise cross-reference.
- For each new construct, define its allowed contexts, required and optional parts,
  interpretation, name and type rules, and interactions with existing constructs.
  Where applicable, also define evaluation or translation order, reference resolution,
  cycle behavior and the stage at which invalid inputs are rejected. State intentional
  omissions instead of silently choosing an implementation-dependent interpretation.
- Distinguish a general language facility from its explicitly enabled applications.
  For example, permitting a representation form in principle does not enable it in
  every field or production; each consuming context specifies its accepted forms.
- Pair normative rules with positive, negative and boundary conformance cases.
  Make each case's expected acceptance, rejection or observable outcome clear.
  Conformance cases need not prescribe exact diagnostic text or internal error codes.
- Review semantic changes separately from editorial and presentation changes. Keep
  proposals, maintainer decisions, verified implementation support and test evidence
  distinct. An accepted specification is the contract for subsequent implementation;
  a documentation draft is not evidence that a feature already works.
- Keep one authoritative set of shared editorial conventions in this file. Language-
  specific exceptions and details belong with the owning specification, not in a
  competing general style guide.

## References And Stable Navigation

- Cite the owning specification/section for reused definitions. Give enough author,
  title, edition/version and section/page information to identify external sources;
  distinguish background citations from incorporated normative requirements.
- Preserve pre-existing IDs, compatibility aliases and canonical URLs. Prefer
  stable descriptive IDs for new destinations. If a heading or location changes,
  retain old destinations through appropriate aliases/redirects rather than silently
  breaking links. Do not repurpose an old anchor for unrelated content.
- Use `@{siteAddress}` for site cross-links and explicit fragments for section
  references. Verify local anchors and cross-page targets across included fragments,
  not just within the edited file. Repair the broken links identified in the E01
  audit in the owning page's editorial task, targeting the existing destination IDs.
  Other newly discovered historical defects are audit findings, not permission for
  unrelated navigation changes.
- For editorial work, hand off the inspected revision, affected sources, decisions,
  exact checks and unresolved choices. Keep audit evidence separate from normative
  guidance and out of the source tree; record it in the owning ticket. See the
  [E01 audit and maintainer decisions](https://github.com/michaelheilmann/arcadia-alm/issues/22#issuecomment-6554287955).

## Visual Conventions

- These expectations cover every specification in this tree. #21's initial visual
  implementation covers Common Lexical, DDL and DDLS; changes to shared styles must
  also be regression-checked against MDNP, PDL and VPL, not silently redesign them.
- Retain the Arcadia site header/footer, theme and navigation framework. Extend
  `specification.css.i` with specification-scoped components rather than a competing
  shell or unscoped element rules. `Documentation/index.css.te` owns the global shell;
  DDL's `ddl.css.i` is an existing reference, not an automatically approved standard.
- Define shared roles for readable content width, heading hierarchy, prose/list
  rhythm, surface/border treatment, code/formula panels, tables, notes and navigation.
  Keep their actual tokens/declarations in the owning stylesheet, not duplicated
  here. Document any essential local exception and its owner in the task ticket.
- Use a legible, consistent type hierarchy and the established site font families
  and theme colors. Keep links distinguishable without relying on color alone;
  verify text/UI contrast and visible keyboard focus. Dark example panels do not
  imply that a complete dark theme exists or has been tested.
- At narrow widths, reflow the content/sidebar without document-level horizontal
  overflow. Keep contents navigation discoverable near the reading entry point,
  with meaningful landmarks, a skip link and coherent keyboard/source order.
  Preserve the document's heading hierarchy, status labels and compatibility IDs.
- Distinguish prose, source examples, grammar, formulas and normative/error notes
  through reusable components. Preserve complete technical content; do not shorten
  productions, hide columns or alter notation to fit a viewport. Prefer semantic
  `pre/code` for source text, coordinating markup conversions with editorial owners.
- Preserve MathJax's mathematical meaning and legitimate internal layout. Do not
  apply generic paragraph/list descendant spacing or blanket font/margin overrides
  to its internals. Diagnose rendering defects separately from typography preference.
- When math text inherits prose fonts, wait for font readiness before typesetting.
  Test delayed font loads and painted-text separation; warm screenshots and stable
  container bounds alone do not rule out overlapping glyphs.
- Keep wide code/formulas/tables scrollable inside their own containers with visible
  affordances and keyboard access. Preserve semantic table structure, captions and
  appropriate row/column headers; do not detach a grid from its content to fake fit.
- Generate with the actual website target. Check representative prose, nested lists,
  TOCs, code, inline/display math, tables, notes, focus and links at desktop, tablet
  and mobile widths. Record actual browser/theme, console/MathJax, overflow, anchor,
  structural and relevant WCAG A/AA accessibility results and limitations separately
  from subjective visual approval. Automated checks are not accessibility certification.
- **Two maintainer screenshot checkpoints are mandatory:** first show a representative
  before/after prototype and obtain direction approval before propagating subjective
  styling; after implementation show comparable annotated before/after captures and
  request explicit visual acceptance. Use actual browser rendering at 1440, 768 and
  390px where relevant, with matched OS/browser, zoom and device scale. Explain the
  change, rationale, alternatives and tradeoffs; ask concrete questions about type,
  math, spacing, hierarchy, readability and navigation. Revise and repeat as requested.
- Record review decisions and evidence links in the owning ticket, not audit files
  in the source tree. **No response is not approval.** If approval is missing, report
  **"Awaiting maintainer visual feedback"**; do not close the visual work or claim
  final acceptance. Prior approval of a different task/prototype is not blanket
  approval for a new design. Keep objective correctness/accessibility gates regardless.
