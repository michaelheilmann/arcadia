# Documentation Folder Guide

This folder contains the source for Arcadia's generated static website and project specifications. Most files are template-engine inputs rather than final output. Generated pages are written under `.Website` by CMake template-engine targets.

## Build Model

- `Documentation/CMakeLists.txt` owns all documentation subdirectory wiring. There is currently no `Documentation/Specifications/CMakeLists.txt`.
- The top-level documentation target is `${MyProjectName}.Website.Generate`, created with `BeginTemplateEngine(${MyProjectName}.Website.Generate "Website")`.
- Pages and assets are generated with `OnTemplateEngine(source.te, output, website.env)`.
- Template output paths are rooted at `${REPOSITORY_SOURCE_DIR}/.Website`, for example `.Website/index.html`, `.Website/assets/index.css`, and `.Website/Arcadia/roadmap.html`.
- Shared template variables come from `website.env`: `siteAddress`, `siteAuthor`, `arcadiaGitHubAddress`, and `arcadiaDependenciesGitHubAddress`.
- Subdirectories with generated pages must be added explicitly from `Documentation/CMakeLists.txt`; do not assume directories under `Documentation/Specifications` are discovered automatically.

## Current CMake Wiring

- Root website outputs: `index.html`, `error.html`, `imprint.html`, `data-protection-statement.html`, `sitemap.xml`, `.htaccess`, `assets/index.css`, `assets/reset.css`, and `assets/cxx-doc.css`.
- `add_subdirectory("Arcadia")` builds `.Website/Arcadia/index.html`, `.Website/Arcadia/roadmap.html`, `.Website/Arcadia/roadmap-archive.html`, and `.Website/Arcadia/news.html`.
- `add_subdirectory("Specifications/Common Lexical Translations")` builds `.Website/specifications/common-lexical-translations/index.html`.
- `add_subdirectory("Specifications/MDNP")` builds `.Website/specifications/mathematical-definitions-notations-and-proofs/index.html`.
- `add_subdirectory("Specifications/Data Definition Language")` builds `.Website/specifications/data-definition-language/index.html`.
- `add_subdirectory("Specifications/Data Definition Language Schema")` builds `.Website/specifications/data-definition-language-schema/index.html`.
- `add_subdirectory("Specifications/Program Definition Language")` builds `.Website/specifications/program-definition-language/index.html`.

## Template Conventions

- `.html.te`, `.css.te`, `.xml.te`, and `.htaccess.te` files are template-engine sources.
- `.i` files are reusable include fragments consumed with `@{include("...")}`.
- Template variables use `@{name}` syntax, for example `@{siteAddress}` and `@{siteAuthor}`.
- Date helper calls such as `@{dayOfMonth()}`, `@{monthName()}`, and `@{year()}` appear in footers.
- Common page headers are usually included from `Commons/header-common.i`.
- Specification pages commonly include `Specifications/specification.css.i` inside an inline `<style>` block.
- Preserve canonical URLs in generated pages when moving or renaming documentation paths.

## Main Areas

- Root website pages: homepage, error page, imprint, data-protection statement, sitemap, `.htaccess`, and shared CSS.
- `Arcadia/`: project documentation landing page, roadmap, roadmap archive, news, sitemap fragment, and engine-concepts page sources. Only the pages listed in `Arcadia/CMakeLists.txt` are currently generated.
- `Arcadia/Roadmap/`: active milestone fragments. Completed milestones are under `Arcadia/Roadmap/Archive/`.
- `Arcadia/News/`: dated news fragments included from `Arcadia/news.html.te` and other pages.
- `Commons/`: shared HTML head and MathJax fragments.
- `DataProtectionStatement/`: legal/privacy content fragments included by `data-protection-statement.html.te`.
- `Specifications/`: generated specification source directories for Common Lexical Translations, MDNP, DDL, DDLS, and Arcadia PDL, plus shared `specification.css.i`.
- `Trigonometric Functions/`: present as a directory, but not wired from the current documentation CMake files.

## Specifications

- `Specifications/Common Lexical Translations`
   defines a reuable lexical grammar.
- `Specifications/MDNP` documents mathematical definitions, notations, context-free grammars, lists, and computer arithmetic.
- `Specifications/Data Definition Language` documents lexical, syntactical, and semantical properties of the
  *Arcadia Data Definition Language*, or *Arcadia DDL* for short.
- `Specifications/Data Definition Language Schema` documents lexical, syntactical, and semantical properties of the 
  *Arcadia Data Definition Schema Language*, or *Arcadia DDLS* for short.
- `Specifications/Program Definition Language` documents the *Arcadia Program Definition Language*, or *Arcadia PDL*,
   directly in `Specifications/Program Definition Language/index.html.te`, including the grammar accepted by the current
   parser and the compatible abstract execution-model material. The name intentionally does not imply a particular abstraction
   level, storage duration, or implementation strategy; it simply says the language defines a program.

## Editing Guidance

- When adding a generated page, add the `.te` file and register it with `OnTemplateEngine` in the nearest active `CMakeLists.txt`.
- When adding a new generated documentation subtree, add `add_subdirectory(...)` to `Documentation/CMakeLists.txt` unless a nearer parent `CMakeLists.txt` is introduced deliberately.
- When adding reusable content, prefer `.i` include fragments and include them from a `.te` page.
- When adding a static binary asset that must appear under `.Website`, add explicit CMake copy/generation wiring; do not assume entire source directories are copied.
- Keep generated website output under `.Website` only when repository convention expects the published site to stay in sync; keep unrelated transient build artifacts out of source control.
- Several static pages have hard-coded sitemap `lastmod` dates and canonical URLs. Update these manually when changing published content.
- The template sources contain raw HTML and inline styles in places; preserve the existing local style unless doing a broader cleanup.
- Preserve the AGPL-3.0-or-later header style in new CMake files.
