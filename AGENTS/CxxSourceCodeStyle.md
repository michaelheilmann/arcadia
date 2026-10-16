# C/C++ Source Code Style

This document records the source code style conventions for Arcadia C and C++ files. Follow nearby code first when a local area intentionally predates or deviates from these rules, but use these conventions for new code and touched declarations unless there is a concrete local reason not to.

## File Encoding And Line Endings

- Use plain ASCII unless the file already requires non-ASCII text or a task explicitly needs it.
- Use LF line endings (`\n`) for C, C++, header, CMake, template, `.mil`, `.pdl`, and documentation source files covered by `sanitize.ps1`.
- Do not convert unrelated files from CRLF to LF as part of an unrelated change.
- Strip trailing spaces and tabs.
- Use `sanitize.ps1` from the repository root when broad whitespace cleanup is appropriate; do not use PowerShell `Get-Content | Set-Content` pipelines because they silently write CRLF.

## Header Guards

- Header guards are derived from the file path relative to the module `Sources` directory.
- Uppercase the relative path, replace each directory separator with `_`, and append `_INCLUDED`.
- Example: `Arcadia/Ring1/Objects/String.h` uses `ARCADIA_RING1_OBJECTS_STRING_H_INCLUDED`.
- For `*.h.i` templates, compute the guard from the generated `*.h` name, dropping the `.i` extension.

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

- Separate struct declarations and definitions from other declarations and preprocessor directives
  with one empty line before and after them.
- Apply this rule to forward declarations and `typedef struct` declarations as well.
- Separate adjacent struct declarations or definitions with one empty line.
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

- Function-like macro calls that span lines should follow the same indented-parenthesis style used for function declarations.
- Keep long object/type declaration macro calls readable, following nearby object-system files.
- Do not compress long generated-style declarations into single lines when adding new hand-written code.

## Object System Patterns

- Use the existing Arcadia object/type macros and nearby files as templates.
- Public object types are declared with `Arcadia_declareObjectType(...)` in headers and defined with `Arcadia_defineObjectType(...)` or `Arcadia_defineObjectTypeWithInterfaces(...)` in source files.
- Public interface types are declared with `Arcadia_declareInterfaceType(...)` and defined with `Arcadia_defineInterfaceType(...)`.
- Public Arcadia enumeration types use `Arcadia_declareEnumerationType(...)` in headers and `Arcadia_defineEnumerationType(...)` in source files.
- Visit callbacks must visit every owned Arcadia object reference stored in the object.
- Constructors should follow nearby value-stack argument ordering and superclass-construction patterns.
- Factory functions usually follow the `_Arcadia_BeginCreate` / `_Arcadia_EndCreate` pattern used by nearby object files.

## Runtime Calling Conventions

- Runtime-facing functions generally take `Arcadia_Thread* thread` as the first parameter.
- The Arcadia value stack is indexed from the top. If values are pushed in the order `x`, `y`, `z`, then `x` has stack index `2`, `y` has index `1`, and `z` has index `0`.
- Constructor forwarding relies on that value-stack convention; follow nearby constructor code rather than inventing a different argument layout.

## GC And Rooting Style

- Plain C locals holding `Arcadia_Object*` pointers are not GC roots.
- Store long-lived object references in owning objects and visit them from the owner callback.
- Use the Arcadia value stack or `Arcadia_Object_lock` for temporary roots only when an operation can trigger collection before another owner records the reference.
- Every manual lock must be exception-safe if code between lock and unlock can jump.
- Prefer ownership with correct `visit` callbacks over manual locks.

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

- Generated-looking `.c` and `.h` files often live beside `.mil` inputs.
- Do not hand-edit generated output unless the task specifically requires it and the owning workflow has been inspected.
- PDLC-generated files are committed; regenerate with PDLC when changing their `.mil` source or generator behavior.
- Preserve the relationship between `.mil`, generated `.c`, generated `.h`, and their `CMakeLists.txt` registration.
- When generated file headers or licensing text change, update the corresponding backend writer strings as well as checked-in outputs when regeneration is required.

## Licensing

- New C, C++, header, and CMake files use the repository's AGPL-3.0-or-later source header style.
- Preserve existing license headers when editing existing files.
