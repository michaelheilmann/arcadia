# MILC.CIL Folder Guide

## Scope

- `repository/MILC.CIL` contains the command-line interface target `${MyProjectName}.MILC.CIL` for the current Arcadia PDL compiler implementation.
- The CLI parses command-line arguments, loads and validates the DDL configuration file, creates compilation tasks, and invokes the compiler library from `repository/MILC`.
- `MILC.CIL`, `--mil2c`, and `--mil2mil` are legacy/current implementation names; do not rename code, targets, paths, or command-line options here unless the user explicitly asks for that rename.
- Keep compiler language behavior in `repository/MILC`; this folder should contain only command-line, configuration, help, and tool-invocation behavior.

## Language Specification

- The Arcadia PDL specification is `Documentation/Specifications/Program Definition Language/index.html.te`.
- Arcadia PDL intentionally does not imply a particular abstraction level, storage duration, or implementation strategy; it simply defines a program.
- CLI changes usually should not alter the language specification. Update the specification only when a CLI change exposes or changes accepted Arcadia PDL source language behavior, module discovery behavior that affects source interpretation, or user-facing compiler modes described by the specification.
- If command help, examples, or defaults mention Arcadia PDL syntax or configuration semantics, keep them consistent with the specification and with `repository/MILC` behavior.

## Build Wiring

- `CMakeLists.txt` adds the `Tool` subdirectory; `Tool/CMakeLists.txt` declares the executable target with the repository `BeginProduct` macros.
- Add new CLI source or header files to `Tool/CMakeLists.txt` with `OnSourceFile` or `OnHeaderFile`.
- The executable depends on `${MyProjectName}.MILC`, `${MyProjectName}.DDL`, and `${MyProjectName}.DDLS`; avoid duplicating functionality from those modules in the CLI.

## Command-Line Behavior

- `Tool/Sources/Arcadia/MIL/Compiler/Main.c` owns argument parsing, configuration loading, DDL schema validation, and dispatch to compilation tasks.
- `HelpSystem.c` owns user-facing help text. Keep help output synchronized with accepted command-line options and configuration schema.
- The configuration file is DDL and currently provides `modulePaths`, a list of module directories. Relative paths are interpreted relative to the current working directory.
- Preserve the current non-interactive command style: commands are selected by flags such as `--help`, `--mil2c`, and `--mil2mil` with named arguments such as `--configuration=<path>`.

## Runtime And GC

- Follow Arcadia runtime conventions from the root `AGENTS.md`: `Arcadia_Thread*` first, status/raise/jump error handling, and precise GC rooting.
- Keep cleanup exception-safe around file paths, diagnostics, DDL/DDLS nodes, command-line arguments, and compilation tasks.

## Verification

- For command-line parsing or help changes, build `${MyProjectName}.MILC.CIL` and run focused manual invocations when a configured build tree is available.
- For source-language behavior changes, verify the relevant `repository/MILC` tests in addition to the CLI target.

## Licensing

- New C, header, and CMake files should use the repository AGPL-3.0-or-later header style.
