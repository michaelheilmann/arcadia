# VPL Three-Address Language, Compiler Design, and Migration Plan

## Purpose

This document records the canonical language model for the *Visuals Programming Language* (VPL)
and the migration to a layered compiler that translates it to GLSL and other shader targets.

**VPL is a public, typed, three-address, virtual-register language.** Authors and tools may write
that language directly. Its in-memory representation is `Arcadia.VPL.IR.*`; it is not merely a
private intermediate layer behind a required expression-language frontend. The textual language
and the programmatic construction interface describe the same canonical module model.

The expression-based AST is legacy implementation, not part of the target VPL module. Its builders,
symbol model, and AST semantic phases will be removed as the canonical implementation replaces the
existing program path. A future higher-level language will live in a **separate module** and produce
canonical VPL. VPL will not provide a permanent expression-language frontend or structured-statement
convenience layer.

It is an engineering design and migration plan. It is not a language specification and does not
replace the module interface documentation.

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
- Legacy frontend: scheduled for removal. `Tree/*`, `Symbols/*`, AST builders, `EnterPhase`,
  `ResolvePhase`, and AST `TypePhase` are migration-era code. Do not extend them as a supported VPL
  frontend, and do not implement the previously planned AST `LowerPhase` within VPL.
- Early scope: chosen. Control flow (basic blocks, branches, loops) and uniform arrays are **not
  deferred**. They belong to the initial canonical-language scope because a small game needs
  `discard` (alpha test), conditional branches, loops, and indexed light arrays almost immediately.
- Implementation: underway. Canonical objects and a baseline module verifier exist, including
  mutable-register data flow, root storage namespaces, copy/comparison operations, void calls,
  a discard terminator, and nested member/index storage references with inherited permissions.
  Explicit entries/linkage, output/helper effects, version-1 text reading/writing, located diagnostics,
  finite binary32 constants, and immutable VerifiedModule publication are implemented. The IR-based
  GLSL 420 core backend and engine artifact consumption are implemented. The old AST transpiler
  and map/out-parameter API are removed; remaining AST/symbol code is legacy cleanup work.
- Plan adjustment: canonical specification, verification, and parsing were brought ahead of output
  migration and are implemented. The four built-in programs now use a shared canonical-text
  template and engine consumers use artifacts. Finish externalizing template data and migrate consumers to
  the verified-module backend, and retire the AST pipeline. Completed AST work is recorded as
  implementation history; it does not become a permanent convenience feature.

## Motivation

VPL currently mixes three concerns in one place:

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
  -> select substitutions in one canonical three-address text template
  -> Text_read -> canonical binding -> IR.Verifier
  -> VerifiedModule immutable publication

GLSL_compile(VerifiedModule, diagnostics, 420)
  -> materialize and verify an independent graph
  -> NameResolver -> Printer -> ProgramArtifact(code + typed reflection)

OpenGL ProgramResource
  -> compile artifact once -> upload both shaders with explicit source lengths
  -> bind resolved fragment output before link -> build native bindings from reflection
```

The active shader path no longer uses the legacy expression frontend. Its classes and tests remain
for removal in Phase 5; no permanent AST-to-VPL adapter is planned.

### The four hard-coded variants

The same four variants
(`MeshAmbientColor`, `VertexAmbientColor`, `TextureAmbientColor`, `TextureVertexAmbientColor`) are
selected in MaterialNode.c now call Programs_create. One parameterized canonical-text template
preserves the original matrix association, vertex attribute locations, uniform record layouts,
UV/color transfers, and fragment color/texture behavior. Moving that text into external `.vpl`
assets remains separate data packaging work.

### Completed fixes and remaining boundaries

- Resolution moved out of emission, definition conflicts use structured diagnostics, and the
  fragment-output name is derived from the actual emitted declaration.
- The built-in sampling signature is now `texture(sampler2D, vec2) -> vec4`; procedure symbols
  retain their stage flags. Canonical output has reviewed new golden files; older snapshots remain
  reference material rather than the active emitter contract.
- `TypePhase` records explicit types for the legacy expression frontend and checks conditions, returns,
  calls, indexing, assignment compatibility, writable storage, and fragment-only discard.
- The backend consumes VerifiedModule and owns deterministic numeric target names, location spans,
  std140 block bindings, and gl_Position/gl_FragCoord mapping. Engine consumers use artifact
  reflection; logical block binding points stay stable even when the driver optimizes a block away.
- IR constructor tests establish representation integrity, not executable-module validity.
- `Tree/NodeVisitor` remains an unused abstraction; removing or adopting it is not a prerequisite
  for the canonical language.

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

For illustration only, before the textual grammar is fixed:

```text
r0 : vec4 = multiply transform, position
r1 : vec4 = add r0, offset
store outputPosition, r1
return
```

This illustrates direct authorship with explicit types and operations. It does not specify final
punctuation, identifier syntax, or implicit conversions. The public grammar belongs in
`Documentation/Specifications/Visuals Programming Language/`, not in this engineering plan.

A `Module` contains uniform declarations and one stage per shader stage; a stage contains its
interface and functions; a function contains basic blocks; a block contains a list of typed
instructions and a terminator.

Control flow is part of canonical VPL from the start, not deferred. A small game reaches for
`discard` (alpha test), conditional fog/material branches, loops (multi-light, kernel sampling), and
indexed uniform arrays early, so basic blocks, branches, and arrays are first-class. Three-address
instructions and control flow are orthogonal: blocks layer on top of the typed register core without
changing it. Direct VPL uses explicit blocks and branch/return terminators. The legacy AST has
structured `if`/`for`/`while` and indexing nodes, but those are not the canonical VPL input surface
and will be retired with the legacy pipeline.

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
Forward-reference rules and final textual grammar must be specified before parser implementation.

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

## Retiring the Legacy Frontend

There is no permanent AST-to-VPL adapter in the target module. The old `LowerPhase` milestone is
cancelled. Instead, translate the four existing ambient-color program descriptions directly into
canonical VPL data or canonical builder calls, then replace them with parsed `.vpl` descriptions
where appropriate. Preserve their stage interfaces, uniforms, output names, and behavior during
the consumer migration.

The existing AST path may remain temporarily to keep the current engine working while the canonical
backend and replacement programs are built. The reader/verifier/publication boundary already exists.
That is a migration dependency, not
a supported convenience frontend. Once the canonical equivalents and consumers pass their checks,
remove `Tree/*`, `Symbols/*`, `TreeBuilder`, `NodeFactory`, `NodeVisitor`, the AST phase objects,
their context accessors, and their source/header registrations. Move reusable semantic rules and
diagnostics into canonical validation rather than leaving dependencies on the retired classes.

AST-only tests and source-language specification sections must also be retired or moved to the
future higher-level language project if that project deliberately adopts them. The VPL specification
must lead with the canonical three-address language; expression syntax must not be presented as a
second VPL language surface.

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

This phase records completed work under the earlier AST-first plan. Canonical IR objects and their
tests remain foundations of the new strategy. AST work is migration-era implementation scheduled
for retirement, not a second supported language surface.

- [x] Add `VPL/IR/*` (`Type`, `Value`, `Instruction`, `Terminator`, `Block`, `Function`, `Stage`,
      `Uniform`, `Module`). All `Arcadia.VPL.IR.*` types live under `Arcadia/VPL/IR/`, follow the
      repository object-type conventions (construct/visit, GC rooting), and are registered in
      `VPL/Library/CMakeLists.txt`.
- [x] Add the `ArrayType` and `ArrayUniform` representations. End-to-end array support remains a
      required canonical verifier/parser/backend milestone; constructor availability is not enough.
- [x] Add basic blocks and terminators (`Branch`, `ConditionalBranch`, `Return`) and `Discard`.
      The IR terminator hierarchy and the `Discard` instruction are in place.
- [x] Add the matching AST constructs (`if`, `for`/`while`, `return`, `discard`, array indexing) and
      their builders. `Tree/*StmtNode`, `IndexExprNode`, and `NodeFactory` builders are registered;
      `VPL/Tests/Tree` covers constructors, optional clauses, nesting, and invalid arguments.
      `ResolvePhase` traverses these forms with body-local and for-initializer scopes; tree tests
      also cover name binding, shadowing, visibility boundaries, and duplicate definitions.
- [x] Register irreducibles in `VPL/Library/CMakeLists.txt` (no source globbing). Every `VPL/IR`
      source and header is registered with `OnSourceFile`/`OnHeaderFile`; `VPL/Include.h` includes
      `Arcadia/VPL/IR/Include.h`.
- [x] Add the legacy AST `TypePhase` that assigns types to expressions and validates operators,
      swizzles, member access, and array indexing.
      The context owns the phase and standard program creation runs it after resolution. Node
      metadata uses IR types, including new scalar bool/int/float types. Conditions, returns,
      calls, constructors, writable storage, and fragment-only discard are validated. Tree tests
      cover valid programs, 30 failure cases, diagnostics, stack cleanup, and reruns. The current
      xy/xyz access surface is checked. Applicable rules will be implemented in canonical verification
      before this frontend is removed.

**Cancelled:** implementing an AST `LowerPhase` inside VPL and retaining an expression convenience
frontend. New higher-level language work belongs to a separate module. Replace the built-in program
descriptions directly rather than extending the legacy frontend to preserve it indefinitely.

**Current milestone:** Phase 4 output migration is implemented: published canonical modules reach
the engine's GLSL artifact path. Native driver tests compile/link all material variants, validate
std140 layouts and binding names, render expected pixels, and exercise loop dispatch and aggregate
fixtures. Next is Phase 5 data packaging and remaining legacy frontend removal. Further AST
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
      Dynamic signed overflow/invalid integer division and out-of-bounds indexing are undefined;
      backend floating operation profiles must be declared rather than assuming cross-target bits.
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
- [ ] Move program descriptions to `.vpl` data and use a parameterized canonical template or an
      equivalent declarative selection table where it avoids duplication.
- [x] Migrate all engine consumers away from `Symbols.Program` and legacy AST compilation.
- [ ] Remove `Tree/*`, `Symbols/*`, `TreeBuilder`, `NodeFactory`, `NodeVisitor`, AST `EnterPhase`,
      `ResolvePhase`, and `TypePhase`, their context accessors, and their CMake/include registrations.
- [ ] Replace or retire AST-only tests; retain canonical construction, parser, verifier, backend,
      and engine regression coverage. Eliminate backend and diagnostic dependencies on legacy types.
- [ ] Ensure the VPL specification and module interface expose only the canonical language and its
      compiler services. A future higher-level language is a separate module, not retained VPL code.

### Phase 6 - Canonical language hardening and additional targets

- [ ] Extend numeric kinds and explicit operation/conversion rules as needed; complete swizzle
      legality and numeric range/overflow behavior.
- [ ] Specify dynamic indexing behavior and extend low-level control-flow operations when useful.
      Structured `switch`/`break`/`continue` syntax belongs to the future higher-level language.
- [ ] Strengthen control-flow/data-flow validation, output-assignment rules, and diagnostic quality.
- [ ] Add a second backend to validate that the IR is genuinely target-neutral.

The future higher-level language module has its own design, syntax, frontend, and lowering plan.
It is not a prerequisite for completing canonical VPL and is not an optional VPL subcomponent.

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
- Legacy tests: keep AST regression tests only during migration; replace their applicable semantic
  checks with canonical verifier tests before removing the frontend.
- End-to-end: the OpenGL4 `ProgramResource` path already compiles the emitted GLSL; keep it in CI.

Build and run with the repository presets; focused test by CTest name where one exists.

## Risks and Open Questions

- **Version-1 language contract.** Mutable registers, storage paths, void calls/discard, explicit
  entries/linkage, output/helper effects, initial numeric semantics, and source locations are
  specified and implemented. The backend migration must preserve these contracts. Changes to text
  compatibility must be explicit versioned language decisions.
- **Verifier scope.** Constructor tests and the old AST `TypePhase` are not substitutes for
  validating arbitrary canonical modules. Direct tools must receive the same guarantees as text input.
- **Migration dependency.** Removing the AST pipeline before canonical program descriptions and
  their backend consumers work would break existing engine materials. Keep that temporary dependency
  explicit, complete the replacements, then remove it rather than turning it into permanent API.
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
