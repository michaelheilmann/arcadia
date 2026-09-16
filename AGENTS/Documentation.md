# Documentation Guidelines

## Module Documentation Structure

- Organize module documentation into the following four document types. Include only the document types that are useful for the module.
- Provide an introduction when readers need a quick start. Make it tutorial-oriented and example-driven. Show how to use the module before explaining every detail.
- Provide concepts and terms when readers need background knowledge to use, maintain, or extend the module effectively. Define the module's vocabulary and explain the underlying model. For example, a mathematics module may need the relevant mathematical background.
- Provide a specification when the module defines a formal system, language, data format, protocol, or behavior that must be described normatively. For example, a programming language module may need a language specification.
- Provide a module interface document for the public surface of the module. Document public types, functions, constants, declarations, events, configuration points, and externally visible behavior.

## Naming

- Prefer `Introduction`, `Concepts and Terms`, `Specification`, and `Module Interface` as document titles unless local documentation already uses clearer established names.
- Avoid the term `API documentation` when a more precise term such as `Module Interface` or `Module Reference` fits.

## Authoring Rules

- Keep these document types distinct. Do not bury normative specification text inside tutorial prose, and do not turn the module interface into a quick-start guide.
- Treat introductions, concepts and terms, and specifications as optional. Treat the module interface as required when the module has a public interface.
- Keep documentation near the module it describes unless the repository already provides a shared documentation location for that document type.

## Template Engine Build

- Use the full name `Arcadia Template Engine` when introducing the tool. After that, treat `template engine` as a synonym for `Arcadia Template Engine`.
- Build documentation with the repository's template engine unless local documentation already uses another checked-in mechanism.
- Treat `Tools/TemplateEngine` as the source of the Arcadia Template Engine. It provides the `${MyProjectName}.TemplateEngine` library and the `${MyProjectName}.TemplateEngine.CLI` executable.
- Wire generated documentation through `CMake/tools-template-engine.cmake` with `BeginTemplateEngine`, `OnTemplateEngine`, and `EndTemplateEngine`.
- Pass a `.te` template file, an output path under `.Website`, and an environment file such as `website.env` to `OnTemplateEngine`.
- Use template includes such as `@{include("header.i")}` to compose pages from checked-in fragments. Template include paths must be absolute or relative to the Arcadia Template Engine process working directory; they are not relative to the file containing the include.
- Keep included fragments beside the document they support unless they are intentionally shared, but make their include paths valid for the template engine working directory used by CMake.
- Put generated documentation outputs under `.Website`; do not edit generated output as primary source.
- Rely on the template engine dependency file support for included fragments. If an included file is not tracked after a build, inspect the template include path rather than adding generated files manually.

## Generated Website Policy

- Treat `.Website` as the generated website output directory.
- Keep `.Website` tracked when it represents the project's published documentation or preservation surface.
- Do not treat `.Website` like an ephemeral compiler build directory.
- Do not commit platform-specific binaries, object files, CMake build trees, logs, dependency caches, or other transient build artifacts.
- When changing documentation sources, update generated website output only when the task requires published-site changes or repository convention expects generated website output to stay in sync.
