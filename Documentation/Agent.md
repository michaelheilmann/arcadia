# Documentation Folder Findings

This folder contains the source for Arcadia's generated static website and project specifications. Most files are template-engine inputs rather than final output. Generated pages are written under the repository `.Build` directory by CMake template-engine targets.

## Build Model

- `Documentation/CMakeLists.txt` defines the top-level website generation target with `BeginTemplateEngine(${MyProjectName}.Website.Generate "Website")`.
- Pages and assets are generated with `OnTemplateEngine(source.te, output, website.env)`.
- Template output paths are rooted at `${REPOSITORY_SOURCE_DIR}/.Build`, for example `.Build/index.html`, `.Build/assets/index.css`, and `.Build/Arcadia/roadmap.html`.
- Shared template variables come from `website.env`: `siteAddress`, `siteAuthor`, `arcadiaGitHubAddress`, and `arcadiaDependenciesGitHubAddress`.
- Subdirectories with their own `CMakeLists.txt` must be added explicitly from the nearest parent CMake file.
- `MyCopyFile` is used for non-template assets that must be copied into `.Build`, currently the Data Definition Language Schema diagram PNG.

## Template Conventions

- `.html.te`, `.css.te`, `.xml.te`, and `.htaccess.te` files are template-engine sources.
- `.i` files are reusable include fragments consumed with `@{include("...")}`.
- Template variables use `@{name}` syntax, for example `@{siteAddress}` and `@{siteAuthor}`.
- Date helper calls such as `@{dayOfMonth()}`, `@{monthName()}`, and `@{year()}` appear in footers.
- Common page headers are usually included from `Commons/header-common.i`.
- Specification pages commonly include `Specifications/specification.css.i` inside an inline `<style>` block.

## Main Areas

- Root website pages: homepage, error page, about page, imprint, data-protection statement, sitemap, `.htaccess`, and shared CSS.
- `Arcadia/`: project documentation landing page, roadmap, roadmap archive, news, sitemap fragment, and engine-concepts page.
- `Arcadia/Roadmap/`: active milestone fragments. Completed milestones are under `Arcadia/Roadmap/Archive/`.
- `Arcadia/News/`: dated news fragments included from the homepage and news page.
- `Commons/`: shared HTML head and MathJax fragments.
- `DataProtectionStatement/`: legal/privacy content fragments included by `data-protection-statement.html.te`.
- `Specifications/`: generated specification pages for Common Lexical Translations, MDNP, DDL, DDLS, and MIL.
- `Specifications/Hll/`: LaTeX/PDF HLL specification material. This subtree is present but is not currently wired by a `CMakeLists.txt` found under `Documentation`.
- `Trigonometric Functions/`: present as a directory, but not wired from the inspected documentation CMake files.

## Specifications

- Common Lexical Translations is built from numbered include fragments and documents reusable lexical concepts.
- MDNP documents mathematical definitions, notations, context-free grammars, lists, and computer arithmetic.
- Data Definition Language documents lexical, syntactical, and semantical translation from Unicode code points to typed values.
- Data Definition Language Schema includes schema type fragments and copies `diagram-1.png` to the generated website.
- MIL documents Machine Interface Language grammar directly in `index.html.te`; `instructions.md` describes a tiny interpreter.

## Notable Findings

- `Documentation/CMakeLists.txt` currently contains `add_subdirectory("InformationSecurity")`, but `Documentation/InformationSecurity` was intentionally removed. The stale CMake reference should be removed if documentation configure/build reaches this folder.
- `Arcadia/index.html.te` links to `specifications/data-definitions-language-schema`, while the CMake output path is `specifications/data-definition-language-schema/index.html`. Check this before relying on that link.
- Several static pages have hard-coded sitemap `lastmod` dates and canonical URLs. Update these manually when changing published content.
- The template sources contain raw HTML and inline styles in places; preserve the existing local style unless doing a broader cleanup.

## Editing Guidance

- When adding a generated page, add the `.te` file and register it with `OnTemplateEngine` in the nearest `CMakeLists.txt`.
- When adding reusable content, prefer `.i` include fragments and include them from a `.te` page.
- When adding a static binary asset, copy it with the local CMake helper pattern rather than assuming the whole directory is copied.
- Keep generated output out of source control unless the repository already tracks that exact generated artifact type in this folder.
- Preserve the AGPL-3.0-or-later header style in new CMake files.
