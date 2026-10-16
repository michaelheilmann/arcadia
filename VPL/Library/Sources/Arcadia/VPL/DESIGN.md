# VPL Three-Address Language, Compiler Design, and Migration Plan

## Purpose

This document records the canonical language model for the *Visuals Programming Language* (VPL)
and the migration to a layered compiler that translates it to GLSL and other shader targets.

**VPL is a public, typed, three-address, virtual-register language.** Authors and tools may write
that language directly. Its in-memory representation is `Arcadia.VPL.IR.*`; it is not merely a
private intermediate layer behind a required expression-language frontend. The textual language
and the programmatic construction interface describe the same canonical module model.

The former expression AST, builders, symbol model, and AST semantic phases have been removed.
A future higher-level language will live in a **separate module** and produce
canonical VPL. VPL will not provide a permanent expression-language frontend or structured-statement
convenience layer.

It is an engineering design and migration plan. It is not a language specification and does not
replace the module interface documentation.

### Scope and semantic baseline

VPL v1 is a rendering shader/material-editor compiler target, bounded by the needs of tools such
as Unreal's material editor, not a promise of that product's complete feature catalog. Do not expand
specification closure into general-purpose GPU compute, scheduling, pointer/memory, exception, or
precision-control functionality. New shader operations should be justified by actual producer needs.

Numerics and sampling deliberately follow GLSL 4.20 core where practical. Native operators,
constructors, and texture calls should implement the contract without software emulation. Another
shader target must declare restrictions/differences instead of assuming identical capabilities.

## Status and Recorded Decisions

- Language direction: chosen. Typed three-address VPL is the **canonical public language**, suitable
  for direct authorship, generated programs, visual editors, and other compiler frontends.
- Representation: chosen. Explicit types, virtual registers, storage references, instructions,
  basic blocks, and branch/return terminators (see "Canonical Language and IR Model").
- Register policy: chosen. Registers are mutable and retain one declared type. Declarations form
  the entry-block prefix, and the verifier checks definite assignment on every reachable incoming
  path using a fixed-point must-analysis. Single-assignment/phi machinery is not part of this model.
- Input surfaces: chosen. Textual `.vpl` input and programmatic canonical-module construction are
  both supported directions. A scanner/parser is a **required milestone**, not optional Phase 3 work.
- Lexical basis: chosen. VPL's specification selects productions and translation rules from
  **Common Lexical Translations**. VPL-specific words and any refinements must be
  specified explicitly rather than duplicating the common rules. VPL defines its own lexical start
  symbol and word alternatives; the common specification supplies no universal word production.
- Grammar independence: chosen. VPL and PDL remain independently designed languages for now.
  Sharing Common Lexical Translations does not adopt PDL's grammar, register spelling, reserved
  words, lexical refinements, or statement-boundary conventions. Possible convergence is future work.
- Validation boundary: chosen. Every backend receives a verified canonical module. Directly built
  modules and parsed modules must pass the same verifier; IR constructors alone are not verification.
- Module boundary: chosen. VPL owns canonical three-address syntax, module construction,
  verification, and backend translation. A future higher-level language owns its own AST, symbols,
  expression typing, and lowering in a separate module.
- Legacy frontend: removed. `Tree/*`, `Symbols/*`, AST builders, `EnterPhase`, `ResolvePhase`,
  AST `TypePhase`, and their context accessors are no longer module code. No AST `LowerPhase` is
  planned within VPL. Generic scanner forms are private parsing data, not a higher-level frontend.
- Early scope: chosen. Control flow (basic blocks, branches, loops) and uniform arrays are **not
  deferred**. They belong to the initial canonical-language scope because a small game needs
  `discard` (alpha test), conditional branches, loops, and indexed light arrays almost immediately.
- Implementation: underway. Canonical objects and a baseline module verifier exist, including
  mutable-register data flow, root storage namespaces, copy/comparison operations, void calls,
  a discard terminator, and nested member/index storage references with inherited permissions.
  Explicit entries/linkage, output/helper effects, version-1 text reading/writing, located diagnostics,
  finite binary32 constants, and immutable VerifiedModule publication are implemented. The IR-based
  GLSL 420 core backend and engine artifact consumption are implemented. The old AST transpiler
  and map/out-parameter API are removed, as are the legacy AST/symbol code and AST-only tests.
- Plan adjustment: canonical specification, verification, and parsing were brought ahead of output
  migration and are implemented. Four checked-in complete `.vpl` descriptions are embedded into
  the library at build time; engine consumers use artifacts. Phases 0 through 5 are complete.
  Completed AST work is recorded as
  implementation history; it does not become a permanent convenience feature.

## Motivation

The original implementation mixed three concerns in one place (all three legacy components below
are now retired):

1. A hand-written abstract syntax tree in `VPL/Tree/*`.
2. A semantic model in `VPL/Symbols/*`.
3. GLSL text emission in `VPL/GLSL/Transpiler.c`.

The original variants built trees in C, and the original GLSL backend read both trees and symbols.
Canonical VPL now provides a stable, typed interchange
language that is independent of those C builders and of GLSL.

For tool producers, explicit operations and types reduce the amount of implicit source-language
behavior they must reproduce. A textual representation also makes generated programs inspectable,
editable, and preservable. More verbose handwritten code is an accepted tradeoff. A separate
higher-level language module may eventually provide concise authoring and target VPL through its
public canonical interface.

## Rationale

### Explicit registers and arbitrary control flow

VPL uses explicit virtual registers rather than an implicit operand stack. This avoids having to
reconcile operand-stack heights and stack-slot types at labels reached by different branches or
loop back-edges. In a stack-based language, one path could arrive with `[value]` and another with
`[value, anotherValue]`; a jump into their shared successor leaves the expected operand positions
ambiguous unless the language constrains or validates those stack states.

A VPL operation names its inputs directly, for example `r2 : float = add r0, r1`. Branching to its
block does not change which operands the operation refers to. This makes explicit block graphs
natural for tool producers and removes operand-stack-height reconciliation from VPL verification.
It does not eliminate function call stacks or implementation-specific runtime stacks; the decision
concerns the language's operand model.

Register-based control flow still requires data-flow validation. If only one of two incoming paths
assigns `r0`, an instruction reading `r0` in their shared successor is not valid merely because its
register name and type are explicit. The verifier must establish availability on every path that
can reach the use, including paths through loops.

The chosen policy is **mutable, statically typed registers**. The verifier checks definite assignment
before reads on all reachable incoming paths. It intersects predecessor assignment sets and solves
loops to a fixed point, then checks reads in instruction order. A first-iteration read cannot be
justified by a write that exists only on a loop back-edge.

Single-assignment registers were considered as an alternative; that model would require dominance
checks and explicit merge mechanisms such as block parameters or phi operations. They are not
required by the chosen mutable-register language.

Thus the register model replaces implicit operand-stack-state reconciliation with explicit,
type-aware register data-flow checks. Stack-based languages can also enforce valid control-flow
joins; the rationale is the simpler, more direct fit of explicit registers to VPL's chosen block
and instruction model, not that stack-based control flow is inherently unverifiable.

## Current State

### Pipeline as implemented

```
Programs_create(flags)
  -> select embedded bytes of a checked-in canonical .vpl program
  -> Text_read -> canonical binding -> IR.Verifier
  -> VerifiedModule immutable publication

GLSL_compile(VerifiedModule, diagnostics, 420)
  -> materialize and verify an independent graph
  -> NameResolver -> Printer -> ProgramArtifact(code + typed reflection)

OpenGL ProgramResource
  -> compile artifact once -> upload both shaders with explicit source lengths
  -> bind resolved fragment output before link -> build native bindings from reflection
```

The module no longer contains the legacy expression frontend or its tests. Context owns shared
diagnostics only; parser binding, verification, publication, and backends operate on canonical data.

### Built-in source storage and loading

MaterialNode selects MeshAmbientColor, VertexAmbientColor, TextureAmbientColor, or
TextureVertexAmbientColor through Programs_create. Their complete, editable descriptions live in
`VPL/Library/Assets/Programs/*.vpl`; no shader description is assembled from C string fragments.
The four programs preserve matrix association, attribute locations, uniform layouts, UV/color
transfers, and fragment behavior. Shared declarations are duplicated explicitly, rather than adding
an include/template language to canonical VPL.

`EmbedPrograms.cmake` reads the exact source bytes and configures a private `BuiltinPrograms.h`
under the external build tree. The generated header is a build dependency of VPL; all four sources,
the generator, and its header template are explicit prerequisites. Programs_create selects a source
through a fixed descriptor table, reads/binds/verifies it with its filename as a diagnostic label,
and publishes VerifiedModule. Runtime loading requires no current-directory, source-tree, or copied
asset dependency. The checked-in `.vpl` descriptions are the source of truth; generated C byte arrays
are not primary source. Arbitrary caller-provided text still uses Text_read directly.

### Completed fixes and remaining boundaries

- Resolution moved out of emission, definition conflicts use structured diagnostics, and the
  fragment-output name is derived from the actual emitted declaration.
- Sampling uses `Texture(sampler2D, vec2) -> vec4`. Stage information and semantic checks belong to
  canonical verification, not retired procedure symbols or AST TypePhase. Canonical goldens and
  native driver tests are the output contract; obsolete AST-era golden files are removed.
- The backend consumes VerifiedModule and owns deterministic numeric target names, location spans,
  std140 block bindings, and gl_Position/gl_FragCoord mapping. Engine consumers use artifact
  reflection; logical block binding points stay stable even when the driver optimizes a block away.
- IR constructor tests establish representation integrity, not executable-module validity.
- AST-only tests are retired. Diagnostic coverage uses VerificationDiagnostic and canonical
  duplicate-uniform failure through the shared context; applicable semantics remain covered by
  parser, verifier, text, GLSL, and driver tests.

## Target Architecture

```
Direct .vpl text          Tools / canonical builders       Future higher-level language
      |                              |                       (separate module)
Scanner + Parser                     |                               |
(declarations, register/            |                     Its own frontend + lowering
 block/function references)          |                               |
      +------------------------------+-------------------------------+
                                     |
                          Canonical Arcadia.VPL.IR.Module
                                     |
                                IR Verifier
                                     |
                           Verified canonical module
                                     |
                       +-------------+-------------+
                       |                           |
                 GLSL backend               Future backends
                  NameResolver              (HLSL, SPIR-V, ...)
                  Printer
                  Reflection
```

All producers converge on the same canonical module and validation rules. The textual parser binds
explicit identifiers and checks syntax; it does not route through the legacy expression AST or rely on
AST `TypePhase` to infer canonical operand types. The verifier checks the resulting graph.

Frontends and backends communicate **only** through verified canonical VPL and a small reflection
result. Backends no longer receive the expression tree or symbol graph, and no longer mutate
caller-provided maps. Direct tool producers can construct a module and invoke verification without
the expression frontend. The external higher-level language is a future producer, not a VPL
submodule or a prerequisite for this migration.

## Canonical Language and IR Model

Canonical VPL is a **typed, three-address, virtual-register language**. This is a decided public
language model, not an implementation detail or an open question. Every value and register carries
an explicit `Type`, so a backend does not re-infer types. Every result-producing operation writes
an explicit destination register; stores, discard, and terminators do not produce result registers.

Three-address describes the simple register-operation form, not a requirement that every instruction
have exactly three operands. Arithmetic normally has one destination and two sources; calls and
constructors have argument lists, and control-flow operations have their own shapes. Nested
arithmetic expressions and structured source-language loops belong to a future higher-level
language, which must decompose them before handing a program to VPL.

The following pseudo-code illustrates the operation model; canonical text syntax is specified in
section 10 of the language specification:

```text
r0 : vec4 = multiply transform, position
r1 : vec4 = add r0, offset
store outputPosition, r1
return
```

This illustrates direct authorship with explicit types and operations. It does not specify canonical
punctuation, identifier syntax, or implicit conversions. The public grammar belongs in
`Documentation/Specifications/Visuals Programming Language/`, not in this engineering plan.

A `Module` contains uniform declarations and one stage per shader stage; a stage contains its
interface and functions; a function contains basic blocks; a block contains a list of typed
instructions and a terminator.

Control flow is part of canonical VPL from the start, not deferred. A small game reaches for
`discard` (alpha test), conditional fog/material branches, loops (multi-light, kernel sampling), and
indexed uniform arrays early, so basic blocks, branches, and arrays are first-class. Three-address
instructions and control flow are orthogonal: blocks layer on top of the typed register core without
changing it. Direct VPL uses explicit blocks and branch/return terminators. The retired AST's
structured nodes are historical work, not a canonical VPL input surface.

All IR objects follow the repository object-type conventions (`Arcadia_declareObjectType`,
`Arcadia_defineObjectType`, construct/visit, GC rooting) and live under `Arcadia.VPL.IR.*`.

### Types

```
Type
  Void
  Scalar { element: Boolean|Integer|Floating }
  Vector { element: Boolean|Integer|Floating, size: 2|3|4 }
  Matrix { rows, columns, element }
  Sampler { kind: Sampler2D }
  Record { name, fields: List<Field{ name, Type }> }
  Array  { element: Type, count }              // fixed-size; uniform arrays and locals
```

Current built-in value-type names include `bool`, `int`, `float`, `mat4`, `sampler2D`, `vec2`, `vec3`,
`vec4`, and `void`. The type is target-neutral; the GLSL spelling is a backend table keyed by value
type, not by legacy symbol identity. Additional numeric kinds are a separate extension.

### Values

```
Value
  Register  { index, Type }        // a virtual register (local or temporary)
  Constant  { Type, literal }      // number/boolean literal, already typed
  Reference { storageKind, slot, Type } // Local register, Parameter, Uniform, or StageIO
  MemberReference : Reference { base: Reference, fieldName } // inherited Type, slot = 0
  IndexReference  : Reference { base: Reference, index: Value } // inherited Type, slot = 0
```

`Reference` distinguishes storage locations from computations, so loads/stores are explicit.
Local slots alias declared register indices; the other namespaces use positions in the function's
parameter list, module's uniform list, or stage's complete interface list. Computation operands are
registers or scalar constants, not implicitly loaded references.

### Instructions

```
Instruction (abstract; has an optional destination Register)
  DeclareLocal { register, type }
  Copy         { result, source }
  Load         { result, source: Reference }
  Store        { target: Reference, value }
  Binary       { result, op: arithmetic or scalar comparison, lhs, rhs }
  Call         { result?, callee, arguments }
  Construct    { result, type, arguments }     // e.g. vec4(x, y, z, w)
  Swizzle      { result, source, pattern }     // e.g. .xyz
  Member       { result, source, field }
  Index        { result, source, index }        // array element, e.g. lights[i]

Terminator (exactly one per basic block; no destination)
  Branch            { target }
  ConditionalBranch { condition, trueTarget, falseTarget }
  Return            { value? }
  Discard           { }                       // fragment stage only; no successor
```

The current call representation uses `calleeFunction` and a `builtin` selector. A module-function
call has a function reference and `BuiltinFunction_None`; a built-in call has a NULL function
reference and a supported selector (`Texture`, ...). Built-in signatures belong to canonical
language validation, and the backend maps the selector to a target spelling (`texture`).

The baseline verifier requires a result for non-void calls and no result for void calls. Discard
uses `DiscardTerminator` and has no successor; the older `DiscardInstruction` is rejected by
canonical verification. Scalar widening is explicit through `Construct`; copies, stores, calls,
and returns require matching types. Arithmetic operators include Add/Subtract/Multiply/Divide;
Equal/NotEqual and ordered scalar comparisons produce Boolean results.

`DeclareLocal` declares a register without initialization; Local references alias its value.
`Member`/`Index` extract values into registers and do not create sub-storage references.
`MemberReferenceValue` and `IndexReferenceValue` instead describe acyclic storage access paths,
which may be nested. Their types match the selected fields/elements, their slots are zero, and
their read/write permissions come from the root reference. Selecting a uniform or stage-input
field/element does not make it writable. The verifier rejects inconsistent class/kind tags, cyclic
paths, invalid selectors, and mismatching types.

Index registers are evaluated when a memory operation uses the path, before that operation writes
its result/value. They must be definitely assigned at each use. Constant indices are checked
against each containing array. Local aggregates must be wholly initialized before a derived load
or partial store; partial stores never establish whole-register assignment or initialize siblings.
Parameters have complete invocation-local values. These semantics apply directly to canonical
producers, not implicit conventions inside a higher-level frontend.

### Module, stage, uniforms, interface

```
Module
  uniforms: List<Uniform>
  vertex: Stage
  fragment: Stage

Uniform (abstract)
  ScalarUniform { name, type }
  RecordUniform { name, recordType: Record }
  ArrayUniform  { name, elementType, count }   // e.g. an array of light scalars or records

Stage
  kind: Vertex | Fragment
  interface: List<StageIO>
  functions: List<Function>
  entryFunction: Function             // explicit same-stage parameterless void function

StageIO
  direction: In | Out
  name, type
  location: optional integer
  semantics: None | Position | Color | TexCoord | FragmentColor

Function
  name, returnType, parameters: List<Reference>, blocks: List<Block>

Block
  instructions: List<Instruction>
  terminator: Terminator
```

`StageIO.semantics` replaces the current inlined `gl_Position`/`gl_FragCoord`/fragment-output
special-casing. The backend decides the concrete target name for each semantics value:
`Position` in a vertex stage -> `gl_Position`, in a fragment stage -> `gl_FragCoord`; `FragmentColor`
-> a declared `out` variable.

## Canonical Input and Verification

The canonical parser reads declarations, explicit value types, simple operations, storage operands,
functions, and labeled basic blocks. It resolves type, register, slot, function, and block identifiers
into the canonical object graph while retaining source locations for diagnostics. Its binding work
is not the legacy `Symbols/*` model or expression-oriented `EnterPhase`/`ResolvePhase` pipeline.
The implemented forward-reference rules and version-1 grammar are specified in section 10.

The lexical specification selects reusable Common Lexical Translations productions from
`Documentation/Specifications/Common Lexical Translations/`. Reuse its Unicode input alphabet,
names, literals, whitespace, comments, line endings/continuations, and applicable punctuation.
Specify VPL's keywords, register/label forms, token disambiguation, and the syntactic significance
of line endings independently. Do not inherit PDL-specific lexical overrides or syntax as a shortcut.

Programmatic producers construct the same module graph directly. They are not trusted to have
established its invariants merely because its object constructors succeeded. The shared verifier
must check at least:

- Type validity, type identity, literal/type agreement, positive array sizes, and legal record fields.
- Register and slot identity, declarations/definitions, availability at uses, and result types.
- Instruction operand kinds and types, call signatures, constructor rules, and explicit conversions.
- Writable storage and read-only uniforms/stage inputs, including indexed/member storage.
- Function parameter and return types, entry blocks, branch-target ownership, Boolean branch
  conditions, block termination, and stage restrictions such as fragment-only discard.
- Stage interfaces, locations, semantics, and uniform declarations, including arrays of records.

The verifier reports structured diagnostics and is shared by parsed text, tools constructing modules,
and future external language frontends. The successful result establishes the backend's input
contract. A backend does not repair invalid modules or consult a higher-level symbol graph.

The first function block is entry, and register declarations are its non-initializing prefix.
`Arcadia_VPL_IR_Verifier_run` implements the baseline typed instruction, graph-ownership, root
storage, and mutable-register checks with `VerificationDiagnostic` messages and offending-object
references. Its scratch state and unmanaged data-flow buffer are released on success and failure.
Verification applies to the current graph; mutation requires a new run, not reliance on a cached flag.

Text_read binds directly into canonical objects and invokes this verifier. Verification diagnostics
from parsed sources carry a source label, byte offset, and one-based Unicode line/column. Stage
entries are explicit same-stage parameterless void functions. Ordinary fragment inputs link by
equal explicit location, or equal names when both locations are unspecified, with equal types and
compatible semantics. Built-in Position is separate. Required outputs and helper effects use the
leaf-level rules in specification section 6.3 and IR/OutputVerification.c; recursion is rejected.

VerifiedModule publishes an opaque immutable canonical-text snapshot; materialize returns a fresh
parsed and verified graph. This is the implemented GLSL backend boundary, independent of mutable builders.
The initial GLSL 420 backend, typed reflection, engine integration, target diagnostics, and native
driver compile/link/pixel tests implement Phase 4. Remaining targets and capabilities are explicit
extensions, not assumed from target-neutral validity.

## Legacy Frontend Retirement (Completed)

There is no AST-to-VPL adapter. The old LowerPhase milestone remains cancelled. Tree/Symbols,
builders/visitors, AST phases/modifier helpers, legacy diagnostics, context phase accessors, and
all related CMake/header registrations are removed. The old Tree test target and its files are
removed; generic diagnostics are tested against canonical objects. The rejected DiscardInstruction
scaffold is removed too; its old numeric kind 9 remains reserved/invalid and DiscardTerminator is
the only discard representation. The canonical verifier and typed metadata replace the retired
semantic responsibilities. VPL no longer depends directly on Math or DDL.

The specification and module interface describe only canonical VPL. A future higher-level language
may deliberately adopt past ideas in its own module, but that is not retained VPL functionality.

## Backend Contract

The GLSL backend consumes only the IR and returns a reflection result instead of mutating caller
maps:

```
Arcadia.VPL.Backends.GLSL.ProgramArtifact
  vertexCode, fragmentCode: String
  uniforms: List<binding>                          // name, GL name, type, kind, slot, block point
  vertexInterface, fragmentInterface: List<binding>  // canonical interface -> GLSL name/location
  fragmentColorOutput: String                        // the actual declared out name
  fragmentColorLocation: Integer32
  version: Natural32
```

Internal pipeline:

1. `NameResolver` assigns target names and locations (mangling, uniform block instance names, and
   explicit `layout(location=...)` for stage interfaces). This replaces `symbolNameMapping` and
   `numberOfConstantBlocks`.
2. `Printer` emits GLSL text from the IR plus the name table. Type spelling, built-in procedure
   spelling, and `gl_*` mapping are backend tables.
3. `Reflection` records the mapping outputs above. `fragmentColorOutput` is taken from the resolved
   interface name, fixing the `_2_fragmentColor` defect by construction.

The explicit version parameter currently selects 420 core; unsupported versions are rejected.
Shader strings have no extra terminator and GL receives a byte length. Native binding names have
one trailing zero byte. General CFGs lower to a local block-selector loop/switch; single-block
return/discard functions emit directly. Calls use forward declarations and main invokes the selected
entry. Samplers use single-definition static alias chains to uniforms/parameters, not illegal GLSL
local sampler assignments. Arrays of arrays, empty records, opaque aggregates, unsupported I/O types,
and overlapping location spans are target diagnostics. See specification section 12 for the exact
supported target contract.

## Consumer Impact

- Replace `Arcadia_VPL_Symbols_Program_createProgram` with canonical module loading/construction
  and verification. Existing material flags select canonical program descriptions; they do not
  require AST construction or the AST semantic phases.
- Expose text-reading, programmatic canonical construction, and verification through VPL's public
  module interface. A future higher-level language targets that interface from its own module.
- `ProgramResource.c` (OpenGL4) calls the backend once and reads the `ProgramArtifact`, removing the
  caller-allocated maps and the `Arcadia_String**` out-parameter.
- `ModelResource.c` uses `artifact->fragmentColorOutput` (the resolved name).
- `VPL/Tests/GLSL` becomes a golden-file test of emitted GLSL per variant, plus structural IR tests.

## Migration Plan

### Phase 0 - Stop the bleeding (low risk, independent of the IR)

- [x] Move `ResolvePhase_run` out of the GLSL backend; run it as part of program compilation.
      `Arcadia_VPL_Symbols_Program_createProgram` now runs `ResolvePhase` after `EnterPhase`, and
      the backend's two call sites were removed.
- [x] Derive the fragment output name from the actual emitted declaration; stop returning
      `_2_fragmentColor`. `writeDefaultFragmentShader` now returns the emitted mangled name of the
      fragment-stage `PixelProcessing` `out` scalar via `symbolNameMapping`.
- [x] Use `texture()` instead of `texture2D`; centralize the `#version` directive.
      The built-in procedure is now named `texture`, and both entry points share the `HEADER`
      constant for the `#version` directive.
- [x] Report resolution/definition conflicts through `Arcadia_Languages_Diagnostics` instead of
      `logf` + jump. `Arcadia_VPL_Context` now owns the shared `Arcadia_Languages_Diagnostics`
      sink and the `EnterPhase`/`ResolvePhase` instances; the existing
      `SymbolAlreadyDefinedDiagnostic` is emitted on definition conflicts (fail-fast).

### Phase 1 - Implemented foundations and legacy frontend work

This phase records historical work under the earlier AST-first plan. Canonical IR objects and their
tests remain foundations of the new strategy. AST work was retired in Phase 5; checked items below
record what was implemented, not a second current language surface.

- [x] Add `VPL/IR/*` (`Type`, `Value`, `Instruction`, `Terminator`, `Block`, `Function`, `Stage`,
      `Uniform`, `Module`). All `Arcadia.VPL.IR.*` types live under `Arcadia/VPL/IR/`, follow the
      repository object-type conventions (construct/visit, GC rooting), and are registered in
      `VPL/Library/CMakeLists.txt`.
- [x] Add the `ArrayType` and `ArrayUniform` representations. End-to-end array support remains a
      required canonical verifier/parser/backend milestone; constructor availability is not enough.
- [x] Add basic blocks and terminators (`Branch`, `ConditionalBranch`, `Return`) and `Discard`.
      The IR terminator hierarchy and the `Discard` instruction are in place.
- [x] Add the matching AST constructs (`if`, `for`/`while`, `return`, `discard`, array indexing) and
      their builders (historical; removed in Phase 5). Tree tests covered constructors, optional
      clauses, nesting, argument errors, name binding, shadowing, and visibility boundaries.
- [x] Register irreducibles in `VPL/Library/CMakeLists.txt` (no source globbing). Every `VPL/IR`
      source and header is registered with `OnSourceFile`/`OnHeaderFile`; `VPL/Include.h` includes
      `Arcadia/VPL/IR/Include.h`.
- [x] Add the legacy AST `TypePhase` that assigns types to expressions and validates operators,
      swizzles, member access, and array indexing.
      It used IR types and validated conditions, returns, calls, constructors, storage, and discard.
      Historical tree tests covered valid programs, 30 failures, diagnostics, cleanup, and reruns.
      Applicable rules now belong to canonical verification and text/driver tests; the AST phase
      and its tests were removed in Phase 5. Scalar IR types remain canonical implementation.

**Cancelled:** implementing an AST `LowerPhase` inside VPL and retaining an expression convenience
frontend. New higher-level language work belongs to a separate module. Replace the built-in program
descriptions directly rather than extending the legacy frontend to preserve it indefinitely.

**Current milestone:** Phases 4 and 5 are complete: published canonical modules reach
the engine's GLSL artifact path. Native driver tests compile/link all material variants, validate
std140 layouts and binding names, render expected pixels, and exercise loop dispatch and aggregate
fixtures. Built-ins are checked-in `.vpl` files embedded at build time, and the legacy frontend is
removed. Next is Phase 6 hardening and additional-target work. Further AST
frontend work is outside the revised VPL strategy.

### Phase 2 - Canonical language semantics, specification, and verification

- [x] Rework `Documentation/Specifications/Visuals Programming Language/` around the public typed
      three-address language: declarations, values, instructions, blocks, functions, stages, and
      interfaces. Remove AST expression/structured-statement syntax from its public language surface.
- [x] Choose mutable fixed-type registers, entry-block declaration prefixes, function-local numeric
      register IDs, first-block entry, and fixed-point definite-assignment checks at joins and loops.
      `VPL/Tests/Verifier` covers repeated assignments and branch/back-edge availability failures.
- [x] Complete local storage and indexed/member-reference representations, the initial explicit
      scalar widening, void calls, and discard/block-termination rules.
      Root namespace bindings, Local register aliases, explicit scalar widening via Construct,
      nullable void-call results, and DiscardTerminator are implemented. Member/Index reference
      subclasses inherit permissions, evaluate index registers at use, and require whole-local
      initialization for partial writes. Tests cover nested arrays/records, malformed/cyclic paths,
      read-only writes, selector availability, constructor arguments, and failure recovery.
- [x] Implement a shared canonical module verifier with structured diagnostics and source-location
      support. Cover directly constructed modules as well as future parser output.
      Both builders and text reading use IR.Verifier. Text binding records object/word offsets;
      lexical/syntactic/binding and verification diagnostics expose source label, byte offset, and
      one-based Unicode-code-point line/column. Parsed text invokes the full verifier.
- [x] Validate types, operands/results, call signatures, writable storage, uniform arrays of scalars
      and records, control flow, stage restrictions, and interface consistency.
      Explicit same-stage parameterless void entries and strict location/name linkage are checked.
      Vertex Position and primary fragment FragmentColor declarations are required. Output leaves
      are tracked through branches/loops and acyclic helper summaries (read prerequisites, guaranteed
      writes, normal continuation). Discard/non-returning calls cut execution; recursion is rejected.
- [x] Define the initial numeric contract: bool, signed integer32, binary32; finite constants,
      explicit int-to-float construction, nearest-even decimal parsing in a private C locale,
      preserved signed zero, and rejection of explicit constant integer zero/overflow divisions.
      GLSL-aligned add/subtract/multiply wrap to low 32 bits; invalid integer division and dynamic
      out-of-bounds accesses have undefined execution results. Exact CPU literal serialization is
      separate from runtime GLSL zero signs, subnormal flushing, rounding, NaN, and contraction
      allowances. No strict IEEE emulation or cross-target bit reproducibility is required.
- [x] Define a verified-module/backend boundary so no consumer can mistake a merely constructed
      object graph for a valid program.
      VerifiedModule is an opaque immutable canonical-text snapshot. Publication verifies and
      serializes the graph; materialization returns a fresh parsed/verified graph. Original builder
      or materialization mutations cannot alter saved input. There is no mutable graph accessor or
      cached-validity flag. GLSL_compile accepts this boundary.

### Phase 3 - Required textual canonical VPL

- [x] Specify the textual `.vpl` grammar for the canonical three-address model, including explicit
      types, instructions, labels, and forward-reference rules. Incorporate Common Lexical
      Translations by reference and explicitly define VPL-specific lexical additions/refinements.
      Keep VPL's grammar independent of PDL's; cross-language syntax convergence is deferred.
      Version 1 uses parenthesized module/type/stage/function/block/instruction forms. Each operation
      has simple operands; derived storage paths are descriptors, not nested arithmetic expressions.
- [x] Implement a scanner/parser and canonical identifier binding without the legacy AST/symbol
      pipeline. Retain diagnostic locations and run the shared verifier on the result.
- [x] Provide a deterministic text writer for tools, inspection, and round-trip tests.
      Ordered graph discovery assigns canonical t/u/s/f/p/b IDs, preserves numeric register IDs and
      display names/nominal records, and prints round-trippable binary32 values including signed zero.
      The writer reads/verifies its own result before publishing it. Comments and unused textual
      type declarations are intentionally not preserved.
- [x] Expose text reading/writing and direct canonical construction/verification through the module
      interface. Text input is a first-class required surface, not an optional higher-level parser.
- [x] Add valid text fixtures, malformed/invalid programs, and text-versus-builder equivalence tests.
      VPL.Tests.Text covers all instructions, branches/loops, calls/forward references, records,
      arrays, storage paths, CLT trivia/escapes, malformed UTF-8/syntax/binding, located diagnostics,
      numeric ranges, publication isolation, round trips, and the exact published example.vpl.

### Phase 4 - GLSL backend over verified canonical VPL

- [x] Rewrite `Transpiler.c` as `Printer.c` + `NameResolver.c` consuming only verified canonical modules.
- [x] Return `Arcadia.VPL.Backends.GLSL.ProgramArtifact`; delete the map/out-parameter API.
- [x] Update `ProgramResource.c`, `ModelResource.c`, and the GLSL test to the new API.
- [x] Support the initial canonical instruction/control-flow set and uniform arrays, including
      read/write references for writable aggregate storage and fragment discard.
      Target capability restrictions are documented, not silently lowered. New canonical goldens
      cover material variants; Windows driver tests compile/link/render the four variants and loop
      variants, check std140 sizes and native reflection, and compile/link aggregate/helper/discard
      fixtures. Lack of a modern driver is an explicit test skip.

### Phase 5 - Canonical program descriptions and legacy removal

- [x] Replace the four `create*MainProcedureTree` variants with canonical VPL descriptions and
      direct module construction/loading. Preserve existing material selection and shader behavior.
- [x] Move program descriptions to complete `.vpl` files with a fixed selection table. Embed exact
      bytes into a private generated build-tree header; track source/generator/template dependencies
      and avoid runtime file distribution dependencies. GLSL tests independently read each source
      and compare canonical publication with Programs_create, in addition to unchanged shader goldens.
- [x] Migrate all engine consumers away from `Symbols.Program` and legacy AST compilation.
- [x] Remove `Tree/*`, `Symbols/*`, `TreeBuilder`, `NodeFactory`, `NodeVisitor`, AST `EnterPhase`,
      `ResolvePhase`, and `TypePhase`, their context accessors, and their CMake/include registrations.
- [x] Replace or retire AST-only tests; retain canonical construction, parser, verifier, backend,
      and engine regression coverage. Eliminate backend and diagnostic dependencies on legacy types.
- [x] Ensure the VPL specification and module interface expose only the canonical language and its
      compiler services. A future higher-level language is a separate module, not retained VPL code.

### Phase 6 - Future hardening and additional targets

- [ ] Extend numeric kinds and explicit operation/conversion rules as needed beyond the version-1
      swizzle legality and numeric range/overflow contract.
- [ ] Extend low-level control-flow/indexing operations when useful beyond the version-1 contract.
      Structured `switch`/`break`/`continue` syntax belongs to the future higher-level language.
- [ ] Strengthen control-flow/data-flow validation, output-assignment rules, and diagnostic quality.
- [ ] Add a second backend to validate that the IR is genuinely target-neutral.

The future higher-level language module has its own design, syntax, frontend, and lowering plan.
It is not a prerequisite for completing canonical VPL and is not an optional VPL subcomponent.

## Version-1 Specification Closure

Completed implementation/migration phases do not by themselves establish specification closure.
Closure preserves the current instruction/grammar scope and records independent language rules,
implementation mapping, and positive/negative conformance evidence.

| Item | Normative location | Current evidence / status |
| --- | --- | --- |
| C1 invocation state, calls, value copies, control transfers | Specification 3.2 | Language-level model written; existing frame/flow, helpers, and discard tests exercise its implementation |
| C2 nominal record identity | Specification 4 | Declaration identity within a module instance, not C pointers; verifier linkage nominal/structural cases and snapshot tests |
| C3 native GLSL-friendly numerics | Specification 4.1, 7.1, backend 12.3 | Driver pass-color test checks signed wrapping, negative division, exact conversion, zero comparisons, finite arithmetic; finite CPU literal bit round trips remain separate |
| C4 texture/sampler execution contract | Specification 7.3.1, backend 12.3 | Normalized coordinates, external sampler state, stage LOD and non-uniform derivative preconditions; driver pixels distinguish repeat/clamp and fragment mip versus vertex base level |
| C5 implementation size limits | Text format 10.1 | Existing Arcadia.Size bound explicitly disclosed (32/64-bit), independent of shader int32 and GL/hardware limits; existing inputs are not silently narrowed |
| C6 binary operation domain | Specification 7.1 complete operand table | Verifier table tests cover 18 operand/result categories across all 10 operators, plus GPU execution checks |
| C7 grammar, binding, and diagnostic boundaries | Text format 10.1–10.2, publication 11 | Text tests cover contextual/reserved words, case sensitivity, token adjacency, comment/continuation boundaries, declaration scopes and forward references, malformed forms, semantic error categories, and located diagnostics including exact trailing-token offsets |
| C8 constructor alternatives and type depth | Specification 7.2, text format 10.2 | 33 constructor cases cover scalar widening, vector composition/splats, rectangular matrices, exact record/array fields, empty records, and sampler/void rejection; depth 128/129 is checked in both declaration orders |
| C9 hand-authored executable examples | Text format 10.3 | Exact control-flow.vpl and aggregate-storage.vpl files pass text publication/materialization/round trips and native driver compile/link/opaque-green pixel checks |
| C10 final consistency review | Specification 3, 7.2, 13 | Nonempty/exact name rules and constructor component boundaries are explicit; reader/verifier/backend/publication rules and target restrictions reviewed against implementation |

GLSL reference: the official GLSLangSpec.4.20.pdf sections 4.1.3, 4.7.1, 5.4, 5.9, and 8.9.
In particular, the earlier draft's signed-overflow-undefined wording was too weak/different from
the chosen native GLSL arithmetic contract and is replaced by wrapping add/subtract/multiply.
Likewise, serialized signed-zero/subnormal preservation does not promise shader preservation, and
GLSL's allowed special-value behavior is not upgraded to strict CPU IEEE NaN propagation.

Texture remains the existing implicit-LOD operation, not a new sampling API. The verifier does not
prove texture completeness, dynamic uniformity, derivative availability, or external compatibility.
The dispatch backend provides no additional quad reconvergence guarantee. Explicit LOD/gradient,
integer/shadow sampling, or additional node functions are future capability work, not spec closure.

The grammar/constructor audit and hand-authored example pass are implemented. The audit corrected
quote token boundaries, semantic-versus-syntactic diagnostic categories, trailing/fallback offsets,
and dependency-depth enforcement for previously resolved type graphs. Cached subtree heights keep
the depth check independent of declaration order without repeatedly expanding shared dependencies.
Full original-source provenance through publication is a separate compiler-interface task for a
future producer, not a new v1 language feature. The final consistency review completes the version-1
closure checklist, and the published specification is labeled version 1. Additional targets and
operations remain extensions. Validation builds all six VPL test targets, the visuals implementation,
and the specification page; the full Debug CTest suite passes 80/80, including native driver tests.

## Testing Strategy

- Golden-file tests: emitted vertex and fragment GLSL for each of the four variants, checked in and
  compared byte-for-byte.
- Canonical construction tests: structural assertions on directly built modules (uniforms,
  interfaces, instruction kinds, and block/terminator graphs).
- Verifier tests: malformed graphs, incorrect types/signatures, register availability, invalid
  branch targets, missing terminators, read-only writes, array access, and stage restrictions.
- Parser tests: valid canonical programs, malformed syntax, identifier binding, locations, and
  invalid-program diagnostics. Equivalent textual and directly built modules must validate alike.
- Text round trips: read/write/read preserves canonical module meaning and produces deterministic
  output. Published `.vpl` examples must be accepted by the actual reader and verifier.
- Control-flow and array tests: explicit branch/back-edge graphs, uniform arrays of scalars and
  records, aggregate reads/writes, and fragment discard through verified-module backend generation.
- Legacy tests: the AST target and obsolete golden snapshots are retired. Canonical parser/verifier
  diagnostics, IR construction, source/embedding equivalence, GLSL goldens, and native driver tests
  cover the supported language and output paths.
- End-to-end: the OpenGL4 `ProgramResource` path already compiles the emitted GLSL; keep it in CI.

Build and run with the repository presets; focused test by CTest name where one exists.

## Risks and Open Questions

- **Version-1 language contract.** Mutable registers, storage paths, void calls/discard, explicit
  entries/linkage, output/helper effects, initial numeric semantics, and source locations are
  specified and implemented. The backend migration must preserve these contracts. Changes to text
  compatibility must be explicit versioned language decisions.
- **Verifier scope.** Constructor tests and the old AST `TypePhase` are not substitutes for
  validating arbitrary canonical modules. Direct tools must receive the same guarantees as text input.
- **Migration dependency (resolved).** Canonical sources, artifact consumers, shader goldens, and
  native pixel tests replaced the old path before AST removal. There is no remaining temporary
  expression-frontend dependency to preserve as permanent API.
- **Format stability.** Textual VPL is decided and required. Specify compatibility/versioning and
  deterministic writing so tool-generated programs remain inspectable and preservable over time.
- **External higher-level language.** Its design is future work in a separate module. Do not
  reintroduce implicit expression typing or structured-source convenience into canonical VPL while
  waiting for that module.
- **GLSL dialect.** `texture()` and the centralized existing header are already in use. Future
  profile/dialect changes and explicit location changes belong to backend configuration; regenerate
  and review golden files intentionally when those choices change emitted text.
- **Naming.** Replacing the interim `mangleName` scheme with deterministic backend names is a visible
  change and must be coordinated with the OpenGL consumer.
