# Arcadia Languages

`Languages` is Arcadia's shared library for building language frontends. It provides common services used by scanners, parsers, semantic phases, diagnostics, and code-generation support in modules such as `DDL`, `DDLS`, `ADL`, `VPL`, and `repository/MILC`.

## What It Provides

- Abstract scanner and parser base objects with virtual dispatch hooks.
- Diagnostic objects, severity types, diagnostic collections, and diagnostic emission.
- Input file loading, path normalization, and byte-offset to line-number mapping.
- A line map implementation for one-based source line lookup.
- A process-wide string table for interning keywords, identifiers, and symbols.
- Hierarchical scopes for semantic analysis and symbol resolution.
- A UTF-8 byte-sequence name mangler for generated code names.

## Public Include

Consumers should include the aggregate header:

```c
#include "Arcadia/Languages/Include.h"
```

Most individual headers are guarded by `ARCADIA_LANGUAGES_MODULE` and are intended to be included directly only by the module implementation.

## Source Locations

- Library target: `Languages/Library/CMakeLists.txt`
- Public sources: `Languages/Library/Sources/Arcadia/Languages`
- Generated configuration header template: `Languages/Library/Sources/Arcadia/Languages/Configure.h.i`

## Line Mapping

`Arcadia_Languages_LineMap` stores the byte offsets at which source lines start. Offset `0` is always present and represents line `1`, including empty files. End-of-input is not stored as a sentinel. Invalid Unicode code points are advanced by byte range and treated as ordinary non-newline input for line counting.

Use `Arcadia_Languages_InputFile_getLine(thread, inputFile, offset)` to map a byte offset to a one-based line number.

## Building And Testing

Build this module through the repository CMake configuration. The target is `${MyProjectName}.Languages`.

There are no tests directly under `Languages`. Focused coverage exists downstream; for line-map behavior, use the `Arcadia.MILC.Tests.LineMap` CTest test when that target is configured.
