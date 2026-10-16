# PDLC Folder Guide

## Scope

- `PDLC` contains the current compiler library target `${MyProjectName}.PDLC` for Arcadia PDL, the Arcadia Program Definition Language.
- The library owns Arcadia PDL scanning, parsing, AST nodes, symbols, compiler phases, diagnostics, compilation task orchestration, and C backend writers.
- Command-line behavior lives in `PDLC.CLI`; keep reusable compiler behavior in this library rather than in the CLI.

## Language Specification

- The Arcadia PDL specification is `Documentation/Specifications/Program Definition Language/index.html.te`.
- Arcadia PDL intentionally does not imply a particular abstraction level, storage duration, or implementation strategy; it simply defines a program.
- Treat `Scanner.c`, `Parser.c`, AST node types, semantic phases, backend behavior, tests, and checked-in `.mil` inputs as the primary executable evidence for current behavior.
- When changing accepted Arcadia PDL syntax, lexical words, instruction forms, primitive semantics, or observable diagnostics, update the PDL specification in the same change when practical.
- If implementation and specification disagree, prefer the implementation for fixing immediate behavior, but either update the specification or leave an explicit follow-up note in the change summary.

## Build Wiring

- The root module file adds `Library` and `Tests`; the library target is declared in `Library/CMakeLists.txt` with the repository `BeginProduct` macros.
- Source and header files are registered explicitly with `OnSourceFile` and `OnHeaderFile`; update `Library/CMakeLists.txt` when adding, moving, or deleting compiler source files.
- Generated configuration headers come from `Configure.h.i` via `OnConfigurationFile`; include the generated `Configure.h` through normal target include paths.

## Compiler Structure

- `Scanner.c`, `Keywords.c`, and `WordType.h` define the lexical word stream and reserved words.
- `Parser.c` defines the accepted syntax and builds `AST/*` nodes.
- `EnterPhase`, `MemberEnterPhase`, and `TypeResolutionPhase` build and complete symbols; keep symbol invariants local to the relevant phase where possible.
- `Backend*` code writes generated C and headers. Preserve generated-file header strings when changing licensing or public generated output.
- Diagnostics are ordinary compiler output. Prefer adding precise diagnostics over raising generic status values when the caller can recover and report useful source information.

## Native Definitions And Native Names

- `native` is an optional clause of a procedure, constructor, or method definition: `procedure native "print" print (...)`, `procedure native "nativePrint" MyNativeProcedure (...)`. It takes a string literal naming the native target.
- `nativeName` on `ProcedureDefinitionNode`, `MethodDefinitionNode`, and `ConstructorDefinitionNode` is **optional and source-derived**: it is populated only when the compilation unit carries the clause, and is `NULL` otherwise. `NULL` means "no clause was written", **not** "this member is not native".
- No symbol has a `nativeName` field of its own. Native-ness is reachable only through `symbol->ast->nativeName`, and a member supplied by a built-in type has no AST node at all. A check written as `ast && ast->nativeName` therefore classifies built-in native members as ordinary members, and they receive the generated stub body instead of a call.
- Treat "is this member native?" and "what is its native target?" as two separate facts. A native member has two provenances: annotation-derived from PDL source, and registry-derived from a built-in type. Resolve both at one point and expose the result through a single accessor; do not let a backend writer branch on AST presence or on provenance.
- A native target name is a **verbatim C identifier**. Never pass it through `Arcadia_PDLC_Backend_Implementation_computeCxxName`, which replaces `.` with `_` to derive an implementation name from a fully qualified PDL name.
- `TypeResolutionPhase` completes a symbol only when `symbol->completer` is non-null, so a native member whose types are supplied from C rather than from AST type names, and which has no completer, is skipped safely and needs no phase changes.
- `Backend/ProcedureSymbolWriter` inserts only the copyright and returns for a native procedure: it emits no declaration, no definition, and no call site. Non-native procedures receive a body that sets `Arcadia_Status_NotImplemented` and jumps. Native emission is therefore unimplemented.
- Member lookup is by **declared, unqualified name** in the scope of the class type that declares the member. Only type names are fully qualified.

## Type Names

- Type names are always fully qualified and there is no relative resolution, so a `.mil` source must spell `Arcadia.Natural64` and not `Natural64`. A member or procedure *name* is never qualified.
- Type resolution is performed by the type resolution phase and reports `SymbolIsNotDefinedDiagnostic` for an unresolved type name; it is not deferred to the backend.
- Because resolution is enforced, a latent unqualified type name in a `.mil` input becomes a hard error. When a new error appears while regenerating `.mil` inputs, check for a mistyped or unqualified type name before assuming a compiler regression.

## Instruction Operands

- An instruction does not use a comma. The operands of an instruction are adjacent, as the grammar in `Documentation/Specifications/Program Definition Language/4. Syntactical Translation.i` shows; `binaryInstruction : binaryOperator operand operand operand` is the model.
- `invokeInstruction : 'invoke' operand name operand operand*` and `constructInstruction : 'construct' operand name operand operand*` do not parenthesize the argument list. `InvokeInstructionNode` stores the 1st operand as its `target`, exactly like `ConstructInstructionNode`; a new consumer must expect that field.
- Commas are still used by declarations: parameter lists, `variable` declarations, and interface name lists.
- `invoke` and `construct` require at least one argument. The parenthesized form they replaced had a spelling for the empty argument list; the unparenthesized form does not, so `onInvokeInstruction` and `onConstructInstruction` parse a mandatory first operand and then call `onOperands`.
- `next` skips a line continuation word, so an instruction may span several lines when every line but the last ends with a reverse solidus. Only the line terminator of the last line terminates the statement. A reverse solidus that is not followed by a line terminator is a lexical error.
- Do not reintroduce a comma or parentheses into instruction operands in the specification, the parser, or the checked-in `.mil` inputs; the three forms are kept in sync deliberately.

## Runtime And GC

- PDLC code follows the shared runtime conventions in `AGENTS/CxxSourceCodeStyle.md`: `Arcadia_Thread*` first, status/raise/jump error handling, and precise GC rooting.
- If a new AST, symbol, diagnostic, or compiler object owns references, update its `visit` callback.

## Tests

- PDLC tests live under `Tests/AST`, `Tests/LexicalAnalysis`, `Tests/SyntacticalAnalysis`, `Tests/SemanticalAnalysis`, `Tests/LineMap`, and `Tests/Diagnostics`.
- `Tests/SemanticalAnalysis` drives a `CompilationTask` over an `Assets/<CaseName>/` module tree. Its `compile` helper succeeds only when the task ran to completion without raising and produced no error diagnostic, so a case is negative when the compilation task raises or reports an error diagnostic. The helper always emits all collected diagnostics, so a negative case prints its expected diagnostics to stdout.
- Add or update `.mil` assets for parser or scanner behavior changes.
- Use focused CTest names when available; on Windows run tests from an out-of-source configured build directory with `ctest -C Debug -R <test-name>`.

## Licensing

- New C, header, and CMake files should use the repository AGPL-3.0-or-later header style described in `AGENTS/CxxSourceCodeStyle.md`.
