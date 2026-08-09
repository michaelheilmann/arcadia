# Arcadia Languages

`Languages` is the shared support library for Arcadia language frontends and compiler-like tools. It is not a complete language implementation; it provides reusable runtime objects for scanners, parsers, diagnostics, input files, line maps, scopes, string interning, and name mangling.

## Layout

- `Languages/CMakeLists.txt` only adds `Library`.
- `Languages/Library/CMakeLists.txt` defines the `${MyProjectName}.Languages` library target and explicitly registers every source and header.
- Sources live under `Languages/Library/Sources/Arcadia/Languages`.
- `Configure.h` is generated from `Configure.h.i` with `OnConfigurationFile`.
- New source or header files must be added to `Languages/Library/CMakeLists.txt`; the build does not glob sources.

## Main Components

- `Include.h` is the public aggregate include. Public consumers should include `Arcadia/Languages/Include.h` rather than individual module headers.
- `Scanner` is the abstract base object for lexical analyzers. Derived scanners provide virtual dispatch for current word text/type/start/length, stepping, input, diagnostics, and string table access.
- `Parser` is the abstract base object for parsers. Derived parsers provide virtual dispatch for input, diagnostics, string table access, and `run`.
- `Diagnostic`, `DiagnosticType`, and `Diagnostics` model diagnostic messages, severity, accumulation, emission, and error detection.
- `DiagnosticsOld` is a legacy diagnostic emission helper still used by older frontend code.
- `InputFile` owns a normalized absolute path, lazily loads file contents, lazily creates a `LineMap`, and exposes one-based line lookup through `Arcadia_Languages_InputFile_getLine`.
- `LineMap` computes ascending byte offsets at which lines start. It always contains offset `0`; end-of-input is not added as a sentinel. Invalid code points are treated like non-newline code points and are advanced by byte range.
- `InputFileManager` creates readers for input files and keeps shared diagnostics context.
- `StringTable` interns strings for keywords, identifiers, and symbols. `Arcadia_Languages_StringTable_getOrCreate` returns the process-wide instance.
- `Scope` is a simple hierarchical map used by semantic phases and symbol resolution.
- `mangleName` converts UTF-8 byte sequences into stable ASCII-compatible encoded names for generated code.

## Consumers

The public APIs are used by `DDL`, `DDLS`, `ADL`, `VPL`, and `repository/MILC`. Treat function names, object type names, dispatch layouts, and exported structs as cross-module API. Prefer small compatible changes unless all consumers are updated in the same change.

## Coding Notes

- Follow the Arcadia object/type pattern used here: `Arcadia_declareObjectType` in headers, `Arcadia_defineObjectType` in sources, explicit construct/destruct/visit/initializeDispatch callbacks, and `_Arcadia_BeginCreate`/`_Arcadia_EndCreate` factories.
- Keep GC visibility correct. Any object reference stored in a struct must be visited in the type's `visit` callback.
- Use `Arcadia_Thread_setStatus` and `Arcadia_Thread_jump` for runtime errors, matching the surrounding runtime style.
- Preserve the module include convention. Implementation files define `ARCADIA_LANGUAGES_MODULE` before including module headers directly; public callers include the aggregate `Include.h`.
- Keep diagnostics reusable. Language-specific diagnostic message objects should usually derive from `Arcadia.Languages.Diagnostic` in the consuming module instead of hard-coding frontend-specific behavior here.
- Keep `StringTable`, `Scope`, and `LineMap` generic. Do not add frontend-specific keywords, symbol classes, or semantic rules to this module.
- Be careful with unsigned indices in binary search code. Prefer half-open ranges (`low < high`) to avoid underflow and out-of-bounds access.

## Build And Test

- Configure and build through the repository CMake presets or external build directories described in the root `AGENTS.md`.
- There are no local `Languages` tests at the time of writing.
- For line-map or input-file changes, run the focused CTest target `Arcadia.MILC.Tests.LineMap` when available.
- For scanner/parser/diagnostics API changes, run affected downstream tests, especially DDL lexical/syntactical tests and MILC lexical/syntactical/diagnostics tests.
- A minimal source verification is building `${MyProjectName}.Languages`; broader API changes should also build affected consumers.
