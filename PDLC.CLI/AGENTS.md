# PDLC.CLI Folder Guide

## Scope

- `PDLC.CLI` contains the command-line interface target `${MyProjectName}.PDLC.CLI` for the current Arcadia PDL compiler implementation.
- The CLI parses command-line arguments, loads and validates the DDL configuration file, creates compilation tasks, and invokes the compiler library from `PDLC`.
- The library API is `Arcadia_PDLC_*`; the public include is `Arcadia/PDLC/Include.h`. Do not reintroduce the former `MILC` spelling.
- `--mil2c` and `--mil2mil` are still current command-line option names; do not rename command-line options here unless the user explicitly asks for that rename.
- Keep compiler language behavior in `PDLC`; this folder should contain only command-line, configuration, help, and tool-invocation behavior.
- `PDLC.CLI` must remain hand-written C: no production PDL port, generated headers/bodies, self-hosting or compiler/CLI production regeneration entries. Follow the [root native-C invariant](../AGENTS.md#start-here) for the full protected set, test/import exceptions and explicit-approval requirement.

## Language Specification

- The Arcadia PDL specification is `Documentation/Specifications/Program Definition Language/index.html.te`.
- Arcadia PDL intentionally does not imply a particular abstraction level, storage duration, or implementation strategy; it simply defines a program.
- CLI changes usually should not alter the language specification. Update the specification only when a CLI change exposes or changes accepted Arcadia PDL source language behavior, module discovery behavior that affects source interpretation, or user-facing compiler modes described by the specification.
- If command help, examples, or defaults mention Arcadia PDL syntax or configuration semantics, keep them consistent with the specification and with `PDLC` behavior.

## Build Wiring

- `CMakeLists.txt` adds the `Tool` subdirectory; `Tool/CMakeLists.txt` declares the executable target with the repository `BeginProduct` macros.
- Add new CLI source or header files to `Tool/CMakeLists.txt` with `OnSourceFile` or `OnHeaderFile`.
- The executable depends on `${MyProjectName}.PDLC`, `${MyProjectName}.DDL`, and `${MyProjectName}.DDLS`; avoid duplicating functionality from those modules in the CLI.

## Command-Line Behavior

- `Tool/Sources/Arcadia/PDLC/CLI/Main.c` owns argument parsing, configuration loading, DDL schema validation, and dispatch to compilation tasks.
- `HelpSystem.c` owns user-facing help text. Keep help output synchronized with accepted command-line options and configuration schema.
- The configuration file is DDL and currently provides `modulePaths`, a list of module directories. Relative entries resolve against the parent directory of the configuration file, not the shell process CWD. Resolving a relative `--configuration` argument itself uses the process CWD; that is a separate step.
- Example: from `C:/develop/Arcadia/Build/x64`, launch the built CLI with `--mil2c --configuration=\"C:/develop/Arcadia/Repository/Modules.pdl\"` (through a `.cmd` wrapper on PowerShell 5.1, as below). A `modulePaths` entry `./ADL` in that file resolves to `C:/develop/Arcadia/Repository/ADL`, not to a directory beneath the build tree.
- Preserve the current non-interactive command style: commands are selected by flags such as `--help`, `--mil2c`, and `--mil2mil` with named arguments such as `--configuration=<path>`.
- An argument value must be quoted: a value that does not start with `"` is parsed as an integer and fails with `invalid command-line argument`. Pass `--configuration=\"<path>\"` so the program actually receives the quote characters; note that Windows PowerShell 5.1 strips embedded quotes from native-command arguments, so verify invocations through a `.cmd` wrapper rather than assuming a direct `& exe` call works.
- The CLI reports compiler diagnostics but does not print them on the `--mil2c` path, and it exits non-zero without output when compilation raises. Drive `Arcadia_PDLC_CompilationTask` from a test when the diagnostics themselves matter.

## Runtime And GC

- Follow Arcadia runtime conventions from `AGENTS/CxxSourceCodeStyle.md`: `Arcadia_Thread*` first, status/raise/jump error handling, and precise GC rooting.
- Keep cleanup exception-safe around file paths, diagnostics, DDL/DDLS nodes, command-line arguments, and compilation tasks.

## Verification

- For command-line parsing or help changes, build `${MyProjectName}.PDLC.CLI` and run focused manual invocations when a configured build tree is available.
- For source-language behavior changes, verify the relevant `PDLC` tests in addition to the CLI target.

## Licensing

- New C, header, and CMake files should use the repository AGPL-3.0-or-later header style described in `AGENTS/CxxSourceCodeStyle.md`.
