# C/C++ Source Code Style

This document records the source code style conventions for Arcadia C and C++ files. Follow nearby code first when a local area intentionally predates or deviates from these rules, but use these conventions for new code and touched declarations unless there is a concrete local reason not to.

## File Encoding And Line Endings

- Use plain ASCII unless the file already requires non-ASCII text or a task explicitly needs it.
- Use LF line endings (`\n`) for C, C++, header, CMake, template, `.pdl`, `.pdl`, and documentation source files covered by `sanitize.ps1`.
- Do not convert unrelated files from CRLF to LF as part of an unrelated change.
- Strip trailing spaces and tabs.
- Use `sanitize.ps1` from the repository root when broad whitespace cleanup is appropriate; do not use PowerShell `Get-Content | Set-Content` pipelines because they silently write CRLF.

## Header Guards

- Header guards are derived from the file path relative to the module `Sources` directory.
- Uppercase the relative path, replace each directory separator with `_`, and append `_INCLUDED`.
- Example: `Arcadia/Ring1/Objects/String.h` uses `ARCADIA_RING1_OBJECTS_STRING_H_INCLUDED`.
- For `*.h.i` templates, compute the guard from the generated `*.h` name, dropping the `.i` extension.
- Generated and hand-written headers use `#if !defined(GUARD)` inclusion guards: a blank line after `#define GUARD`, a blank line before `#endif`, and a trailing comment `// GUARD` on the `#endif`.

## Module-Private Include Guards

- A header that must not be included directly also carries a module-private guard after the inclusion guard's `#define` and before the includes. The module aggregate header sets `<MODULE>_PRIVATE`; the guard rejects direct inclusion with a compile error.
- Shape (use the header's own include path and the module aggregate header's include path):

  ```c
  #if !defined(<MODULE>_PRIVATE) || 1 != <MODULE>_PRIVATE
    #error("do not include `<this header>` directly, include `<module aggregate header>` instead")
  #endif
  ```

- `<MODULE>_PRIVATE` is the module symbol's C++ name uppercased with a `_PRIVATE` suffix (for example `ARCADIA_ENGINE_PRIVATE`); the example message uses the include paths from the module's aggregate `Include.h`.
- The nested `#endif` is bare (no trailing comment), and the block is followed by a blank line before the includes.
- Generated class/procedure headers, interface headers, and enumeration headers all carry this guard.
- Generated implementation files define `<MODULE>_PRIVATE (1)` before their includes so that their own module headers pass the guard. Test code acting as a module implementation follows the same rule.

## Public Includes

- Respect module aggregate include boundaries.
- Public consumers should usually include a module's aggregate `Include.h` rather than individual public headers.
- Keep aggregate include files in sync when adding public types.
- Public consumers of Engine use `Arcadia/Engine/Include.h`.
- Public consumers of Engine UI use `Arcadia/Engine/UI/Include.h`.
- Public consumers of Languages use `Arcadia/Languages/Include.h`.
- Public consumers of Ring1 use `Arcadia/Ring1/Include.h`.
- Public consumers of ADL use `Arcadia/ADL/Include.h`.
- Files inside a module may define the module-private macro and include internal headers following nearby examples.

## CMake Source Registration

- Most targets are defined through repository macros from `CMake/all.cmake`, not raw `add_library` or `add_executable`.
- Add, move, or delete C/C++ source, header, inlay, and generated configuration files in the nearest `CMakeLists.txt`; source discovery is explicit.
- Source paths passed to `OnSourceFile`, `OnHeaderFile`, and `OnInlayFile` are relative to the module's `Sources` directory unless marked `GENERATED` or otherwise handled locally.
- `OnConfigurationFile` generates configuration headers such as `Configure.h` from checked-in `*.h.i` templates into the binary `Sources` tree. Include generated headers through the module's normal include path rather than hard-coding the binary directory.

## C Struct Declaration Formatting

- Separate complete struct definitions from other declarations and preprocessor directives
  with one empty line before and after them. Separate adjacent struct definitions with one empty line.
- Group related forward declarations, including `typedef struct` declarations, compactly with no blank line between adjacent entries. Put one empty line before and after the entire group, and place the include group before the forward-declaration group.
- Keep documentation comments attached to the declaration they describe.

## C Function Formatting

- Put the return type on its own line.
- Put the function name on the next line at column 0.
- Put the opening parenthesis on its own line indented by two spaces.
- Put one parameter per line, indented by four spaces.
- Put the closing parenthesis on its own line indented by two spaces.
- For definitions, put the opening body brace on the next line at column 0.
- Apply the same layout to declarations and definitions.
- An empty parameter list still uses the same parenthesis layout.
- A single parameter still gets its own line.
- Separate adjacent function declarations and function definitions with an empty line.
- Separate function declarations and function definitions from inclusion guards and include directives with one empty line.
- Apply this layout to generated `pdl*` helper functions and interface operation declarations/definitions too. Body-local native-target `extern` declarations may remain single-line. Dispatch-struct function-pointer members may also remain single-line.

Example:

```c
Arcadia_Math_Matrix4Real32*
Arcadia_Starship_Viewer3D_getLocalToWorldTransform
  (
    Arcadia_Thread* thread,
    Arcadia_Starship_Viewer3D* self
  )
{
  ...
}
```

Forward declarations use the same shape:

```c
static void
Arcadia_ADL_UI_LabelDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_UI_LabelDefinition* self
  );
```

## Macro Call Formatting

- Function-like macro calls that span lines should follow the same indented-parenthesis style used for function declarations, except for the type-macro layout below.
- Keep long object/type declaration macro calls readable, following nearby object-system files.
- In object/class type declaration macros, keep the type-name string and C type on the first line, and put the parent-type string on the next line. Align its opening quote with the opening quote of the first type-name string. For example:

  ```c
  Arcadia_declareObjectType("Arcadia.Engine.Visuals.Icon", Arcadia_Engine_Visuals_Icon,
                            "Arcadia.Object");
  ```

- Do not compress long generated-style declarations into single lines when adding new hand-written code.

## Object System Patterns

- Use the existing Arcadia object/type macros and nearby files as templates.
- Name the embedded base member `_parent` in both object and dispatch structs.
- Access an inherited field by casting the object pointer to the base type that declares the field, then using `->field`. For example, if `Base` declares `f` and `x` points to a derived object, write `((Base*)x)->f`, not `x->_parent.f`. Cast directly to the declaring ancestor for deeper inheritance instead of chaining `_parent` accesses. Apply the same rule to inherited dispatch slots. The `_parent` member expresses the embedded layout; it is not the access path for inherited members. Fields declared by the current type continue to use `x->field`.
- Generated PDL fields and dispatch slots preserve their original names. Append `_pdl` once when the original name is a C/C++ keyword, `_parent`, or already ends in `_pdl`; for example, `_parent_pdl` becomes `_parent_pdl_pdl`. Use the same mapping in declarations, accesses, initialization, visits, dispatch, and native adapters. The C backend diagnoses names containing `__` or starting with an underscore followed by an uppercase ASCII letter.
- Public object types are declared with `Arcadia_declareObjectType(...)` in headers and defined with `Arcadia_defineObjectType(...)` or `Arcadia_defineObjectTypeWithInterfaces(...)` in source files.
- Public interface types are declared with `Arcadia_declareInterfaceType(...)` and defined with `Arcadia_defineInterfaceType(...)`.
- Public Arcadia enumeration types use `Arcadia_declareEnumerationType(...)` in headers and `Arcadia_defineEnumerationType(...)` in source files.
- Visit callbacks must visit every owned Arcadia object reference stored in the object.
- Constructors should follow nearby value-stack argument ordering and superclass-construction patterns.
- Factory functions usually follow the `_Arcadia_BeginCreate` / `_Arcadia_EndCreate` pattern used by nearby object files.

## Runtime Calling Conventions

- Runtime-facing functions generally take `Arcadia_Thread* thread` as the first parameter.
- Generated PDL procedure/method headers use typed C parameters and results, matching hand-written APIs. Methods take a declaring-class `self` pointer after `thread`; procedures have no receiver. Dispatch slots use the same typed signature, with exact-signature thunks for overrides. Only `Arcadia.Value` results use `Arcadia_Value`; void results use C `void`.
- Constructors are private `void` initialization operations using runtime value-stack callbacks. Public concrete factories take typed constructor parameters and return `Class*`; abstract classes expose no factory. Class/procedure headers are generated directly as `.h`, with canonical `.h` dependencies; their sources remain `.c.g`.
- For generated-header work, always compare any existing `xxx.h.g` directly with `xxx.h` before replacing it to identify layout/API gaps. Once output is directly `.h`, review its diff against the previous hand-written/Git version and compile hand-written sources against the generated header where practical. No header-copying or installation step is needed.
- The Arcadia value stack is indexed from the top. If values are pushed in the order `x`, `y`, `z`, then `x` has stack index `2`, `y` has index `1`, and `z` has index `0`.
- A runtime constructor caller pushes arguments in declaration order, then their `Arcadia.Natural8` count. On entry, offset `0` is the count, the last argument is at `1`, and the first of `n` arguments is at `n`. For `(timestamp, horizontal, vertical)`, read offsets `3`, `2`, `1`, not `2`, `1`, `0`.
- To initialize a superclass, push copies of its arguments and its own count. Successful superclass construction consumes only that forwarded segment; the derived constructor's original arguments and count remain at their original relative offsets until its leave-constructor cleanup.
- Use `Arcadia_SizeValue` for stack-depth snapshots and slot offsets. The Natural8 constructor count is not a stack-depth type; narrowing a depth to Natural8 corrupts checks once the stack exceeds its range.

## GC And Rooting Style

- Plain C locals holding `Arcadia_Object*` pointers are not GC roots.
- Store long-lived object references in owning objects and visit them from the owner callback.
- Use the Arcadia value stack or `Arcadia_Object_lock` for temporary roots only when an operation can trigger collection before another owner records the reference.
- Every manual lock must be exception-safe if code between lock and unlock can jump.
- Prefer ownership with correct `visit` callbacks over manual locks.
- Native object allocation does not itself root the receiver. Before `Arcadia_LeaveConstructor` publishes a derived runtime type, GC uses the currently installed superclass type and cannot discover fields through the derived visitor. If a constructor calls code that can collect, temporarily root the receiver and any initialized managed fields not reachable through that active visitor; rooting the receiver alone is insufficient. Keep constructor argument offsets intact, and restore temporary roots on both success and a runtime jump before leaving or rethrowing. Do not publish an unfinished derived type merely to enable its visitor/destructor.

## Error Handling

- Use `Arcadia_Thread_setStatus(thread, ...)` followed by `Arcadia_Thread_jump(thread)` rather than ad hoc process termination or unchecked error returns.
- Preserve and clean up through `Arcadia_JumpTarget` when local cleanup is required across jumps.
- Re-throw after cleanup by popping the jump target, releasing owned temporaries, and calling `Arcadia_Thread_jump(thread)` again.
- Follow nearby tests and runtime code for `Arcadia_Tests_safeExecute(...)` and process/thread lifetime patterns.

## Comments

- Prefer self-explanatory code and small functions over comments that restate the code.
- Add short comments only where they explain non-obvious invariants, ownership, GC rooting, or backend constraints.
- Do not add decorative section comments unless the surrounding file already uses them.

## Generated Sources

- Generated-looking `.c` and `.h` files often live beside `.pdl` inputs.
- Do not hand-edit generated output unless the task specifically requires it and the owning workflow has been inspected.
- PDLC-generated files are committed; regenerate with PDLC when changing their `.pdl` source or generator behavior.
- Preserve the relationship between `.pdl`, generated `.c`, generated `.h`, and their `CMakeLists.txt` registration.
- When generated file headers or licensing text change, update the corresponding backend writer strings as well as checked-in outputs when regeneration is required.

## Licensing

- New C, C++, header, and CMake files use the repository's AGPL-3.0-or-later source header style.
- Preserve existing license headers when editing existing files.
