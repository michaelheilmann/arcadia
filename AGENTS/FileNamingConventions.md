# File Naming Conventions

## Arcadia Definition Language Assets

- Files that define Arcadia assets or objects using the Arcadia Definition Language (ADL) use the `.adl` extension.
- The extension is determined by the file's semantic role, even when the ADL representation is parsed using DDL syntax.
- Do not use `.ddl` for ADL assets. `.ddl` identifies Data Definition Language source and input files.
- Keep references to an ADL asset consistent with its `.adl` filename when renaming or moving it.

## Visuals Programming Language

- Canonical typed three-address VPL source files use the `.vpl` extension.
- These files describe canonical modules and instructions, rather than the legacy VPL expression AST.
