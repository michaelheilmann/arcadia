# Runtime/Ring1 Notes

## Module Shape

- `Runtime/Ring1` builds the `Arcadia.Ring1` library. `Runtime/Ring1/CMakeLists.txt` only does `add_subdirectory(Library)`, `add_subdirectory(Tests)`, `add_subdirectory(Documentation)`.
- `Library/CMakeLists.txt` is the authority for which files are compiled: every `.c`/`.h`/`.i` file must be registered with `OnSourceFile`, `OnHeaderFile`, or `OnInlayFile`, and every registered path must exist. A file that exists but is unregistered is silently not built; a registered path that does not exist breaks configure.
- `SetFolder(${this} "Runtime/Ring1")` places the product in the `Runtime/Ring1` group.
- `OnConfigurationFile` generates `Configure.h` into the binary `Sources` tree from the checked-in `Library/Sources/Arcadia/Ring1/Configure.h.i`. The generated `Configure.h` is on every product's include path, so C files include it as `"Arcadia/Ring1/Configure.h"`. It defines the instruction-set-architecture and operating-system `Arcadia_Configuration_*` families. Do not add `Configure.h.i` to the source list; it is a template input, not a compiled source.
- Source and header files live under `Library/Sources/Arcadia/Ring1`, with subdirectories for the type system (`TypeSystem`), `Objects` (concrete object types), `Concurrency`, `BigInteger`, `Signals`, `Numerics`, `Network`, the `*ToString`/`*ToInteger` families, `Unicode`, and `ImmutableUTF8String`.
- `Object/TypeFunctions.i` is an X-macro `Operation(...)` list of the 18 `Arcadia_ObjectDispatch` operations. It is registered with `OnInlayFile`, not `OnHeaderFile`.

## Include Boundaries

- Public consumers include `Arcadia/Ring1/Include.h`, not individual headers. It pushes `ARCADIA_RING1_MODULE` and `ARCADIA_RING1_EXPORT` around the include list and pops them afterwards.
- The dominant guard idiom is `#if !defined(ARCADIA_RING1_MODULE)` → `#error("do not include directly, include \`Arcadia/Ring1/Include.h\` instead")`. There is no `ARCADIA_RING1_PRIVATE`; do not invent one.
- Private `*.module.h` headers add a second guard: `#if defined(ARCADIA_RING1_EXPORT)` → `#error("this file is not supposed to be exported")`.
- The guard is not universal: `Object.h`, `Value.h`, `Arrays.h`, `Enumeration.h`, the `ImmutableUTF8String/*` files, and several others have no `ARCADIA_RING1_MODULE` guard. Follow the nearest sibling file rather than assuming uniformity.
- `Include.h` itself is unguarded by design.

## Value Stack

- The value stack is indexed from the top. If values are pushed in the order `x`, `y`, `z`, then `x` has stack index `2`, `y` has index `1`, `z` has index `0`. `Thread.c` computes `stack.size - 1 - index`.
- Calling convention: the topmost value is a `Natural8` argument count; the callee pops that count value plus the arguments and leaves a single return value.
- Core stack functions are in `Thread.h` (`getSize`, `getValue`, `pushValue`, `popValues`, `reverse`). Typed wrappers (`getNatural8Value`, `pushBooleanValue`, `pushSizeValue`, `isObjectReferenceValue`, `getObjectReferenceValueChecked`, ...) are generated in `ThreadExtensions.h`.
- `Arcadia_EnterConstructor`/`Arcadia_LeaveConstructor` depend on this convention: the argument count lives at stack index `0`.
- `Arcadia_InterfaceCall`/`Arcadia_InterfaceCallWithReturn` deliberately do *not* touch the value stack. The receiver and the result are passed in C.

## Type System

- Runtime registrars live in `TypeSystem/Types.h` and are implemented in `TypeSystem/Types.module.c`: `Arcadia_registerInterfaceType`, `Arcadia_registerObjectTypeWithInterfaces`, `Arcadia_registerScalarType`, `Arcadia_registerEnumerationType`.
- Declaration/definition macros:
  - `Arcadia_declareObjectType` / `Arcadia_defineObjectType` in `Object.h`.
  - `Arcadia_declareInterfaceType` / `Arcadia_defineInterfaceType` / `Arcadia_defineObjectTypeWithInterfaces` in `TypeSystem/Types.h`.
  - `Arcadia_declareScalarType` / `Arcadia_defineScalarType` in `_declareScalarType.h` / `_defineScalarType.h`.
  - `Arcadia_declareEnumerationType` / `Arcadia_defineEnumerationType` in `Enumeration.h`.
- `Arcadia_declareInterfaceType(_cilName, _cName)` and `Arcadia_defineInterfaceType(_cilName, _cName, ...)` are variadic; `Arcadia_defineInterfaceType` requires a `NULL` `extends` terminator and a parent interface type is mandatory for a derived interface.
- `Arcadia_defineObjectTypeWithInterfaces` is variadic and requires a parent object type. The interface specifications are passed as bare braced specifiers, one per implemented interface, without an extra outer list.
- `Arcadia_TypeKind` (`TypeSystem/TypeKind.h`) has exactly the kinds `Enumeration`, `Interface`, `Internal`, `Object`, `Scalar`. The kind accessor is spelled `Arcadia_Type_isInterfacelKind` (with the trailing `l`) - that typo is in the real API.
- Per-type storage: `ObjectTypeNode.module.{h,c}` holds an object's per-interface implementation records; `InterfaceTypeNode.module.{h,c}` holds interface dispatch sizes and ancestor interfaces; `TypeNode.module.{h,c}` is the shared base.

## Interface Dispatch Semantics

- Interface dispatch structs are independent. A derived interface does **not** embed its ancestors' dispatch structs; each interface has its own struct starting with `Arcadia_InterfaceDispatch parent`.
- Consequently, an object implementing a derived interface must register a separate specification and a separate dispatch for **every** ancestor interface. Registration validates that the specification set is closed under the ancestors of the specified interface types and raises `Arcadia_Status_ArgumentValueInvalid` when it is not.
- `Arcadia_ObjectType_getInterfaceDispatch` matches the interface type exactly, then walks the ancestor *object* types. It deliberately does **not** fall back to a descendant's dispatch. Implementing an ancestor does not implement a derived interface, and implementing a derived interface does not stand in for an ancestor.
- When an object type registers an implementation of an interface type its parent (or another ancestor) also implements, the new dispatch is first zeroed, then initialized with a copy of that interface type's dispatch on the nearest ancestor object type, and only then is the dispatch initializer invoked. This mirrors the object dispatch, which is copied from the parent before its initializer runs. So an object type that overrides only some operations of an inherited interface implementation only has to set *those* operations in its initializer; the rest are inherited for free. A dispatch that is not inherited is all zero until the initializer sets every operation.
- Because every implemented interface has its own allocation, there is no cross-interface dispatch-size constraint, and the dispatches of two interfaces implemented by one object are distinct pointers. Do not reintroduce a size equality check.
- An interface value is erased: in C it is just the `Arcadia_Object*`, and on the value stack it is an ordinary object reference value. There is no interface-specific `Arcadia.Value` tag and no interface reference value type. Do not add one.
- An object reference value whose object implements an interface type is an instance of that interface type, and `Arcadia_ValueStack_getObjectReferenceValueChecked` accepts that interface type as a target.

## Object Dispatch And Constructor Helpers

- `Arcadia_ObjectDispatch` is generated from the `Operation(...)` list in `Object/TypeFunctions.i`.
- Call macros in `Object.h`, all requiring `thread` and `self` in scope:
  - `Arcadia_VirtualCallWithReturn(Type, Function, ...)` and `Arcadia_VirtualCall(Type, Function, ...)` resolve via `Arcadia_ObjectType_getDispatch` and do **not** null-check the dispatch.
  - `Arcadia_InterfaceCallWithReturn(InterfaceType, Function, ...)` and `Arcadia_InterfaceCall(InterfaceType, Function, ...)` resolve via `Arcadia_ObjectType_getInterfaceDispatch` and raise `Arcadia_Status_NotImplemented` when the interface dispatch is absent. They take the receiver as the first variadic argument.
- `Arcadia_InterfaceDispatch_InitializeCallbackFunction` is a dedicated callback type taking `Arcadia_InterfaceDispatch* self`. Interface initializers still need a cast when they write a derived dispatch struct, because the callback type is not generic.
- `Arcadia_EnterConstructor`/`Arcadia_LeaveConstructor`, `_Arcadia_BeginCreate`, `_Arcadia_EndCreate`, `_Arcadia_EndCreate0`, and `Arcadia_superTypeConstructor` all require a `_##Type##_getType(thread)` function. A test type that does not have one must spell out the constructor body - consume the argument count, call `Arcadia_Object_setType`, and pop `numberOfArguments + 1`.
- `_Arcadia_EndCreate` is declared twice in `Object.h`; the later definition is the live one. There is also a `@deprecated` `_Arcadia_EndCreate` variant.

## Error Handling

- Report errors with `Arcadia_Thread_setStatus(thread, ...)` followed by `Arcadia_Thread_jump(thread)`. Do not return error codes.
- `Arcadia_JumpTarget` (`Thread.h`) holds a `jmp_buf` plus a `previous` pointer; pair `Arcadia_Thread_pushJumpTarget` with `Arcadia_Thread_popJumpTarget`, and guard the body with `Arcadia_JumpTarget_save`.
- Re-throw after cleanup by popping the jump target, releasing owned temporaries, and calling `Arcadia_Thread_jump(thread)` again.
- Common statuses: `Arcadia_Status_AllocationFailed`, `Arcadia_Status_OperationFailed`, `Arcadia_Status_StackCorruption`, `Arcadia_Status_ArgumentValueInvalid`, `Arcadia_Status_NotImplemented`, `Arcadia_Status_TestFailed` (used by the test assertions).

## Garbage Collection

- Follow the rooting rules in the root `AGENTS.md`: plain C locals holding `Arcadia_Object*` are not GC roots, and `Arcadia_Process_stepARMS` may collect anything not reachable from a precise root.
- Any object reference stored in a struct or object must be reported from that type's `visit` callback. A missing report lets the collector reclaim a live pointer.
- `Arcadia_Object_lock`/`Arcadia_Object_unlock` (`Object.h`) are for temporary C-local objects only, and every lock must be exception-safe: if code between lock and unlock can jump, wrap it in a jump-target cleanup path.
- Prefer a natural owner with a correct `visit` callback over a manual lock.
- Object and interface type nodes are visited and finalized through `TypeSystem/ObjectTypeNode.module.c` and `TypeSystem/InterfaceTypeNode.module.c`; interface teardown is ordered so that an implemented interface is destructed after the objects that depend on it.
- Barriers are gated on `Arcadia_Configuration_withBarriers`, which is defined in `Object.h` as `Arcadia_ARMS_Configuration_WithBarriers`.

## Strings And Objects

- `Arcadia_RuntimeUTF8String` is a value type declared in `ImmutableByteArray.h`, created with `Arcadia_RuntimeUTF8String_create(thread, u8"...", sizeof(u8"...")-1)` and boxed with `Arcadia_Value_makeRuntimeUTF8StringValue`. It is a common interface operation return type.
- `Arcadia_String` is a separate *object* type in `Objects/String.h`, declared with `Arcadia_declareObjectType` and with constructors from the boolean, integer, and natural scalar types.
- `Arcadia_ImmutableUTF8String` (with `ImmutableUTF8StringExtensions.{h,c}`) and `Objects/ByteArray.h` are the immutable buffer types. `Objects/ByteReader.h` and the `*.ByteReader` types are separate object types wrapping a buffer plus a read cursor.
- Note the naming trap: `Arcadia_ImmutableByteArray` is declared in the plain file `ImmutableByteArray.h`, and the `RuntimeUTF8String` family lives alongside it in that same header (the misleading empty `ImmutableByteArray` folder is gone after the `Implementation/` flattening).

## Tests

- Each test is its own subdirectory under `Runtime/Ring1/Tests/<Name>Tests`, with sources under `Sources/Arcadia.Ring1.Tests.<Name>Tests` and a `CMakeLists.txt` following this pattern:
  - `set(this ${MyProjectName}.Ring1.Tests.<Name>Tests)`, `BeginProduct(${this} test)`, `SetFolder(${this} "Runtime/Ring1")`, `OnSourceFile`/`OnHeaderFile` entries, `OnModuleDependency(${this} ${MyProjectName}.Ring1 PRIVATE)`, `EndProduct(${this})`.
  - `BeginProduct(... test)` registers the CTest automatically.
- `Runtime/Ring1/Tests/CMakeLists.txt` must list every test subdirectory. A new test directory is not built until it is added there.
- `Runtime/Ring1/Tests/LiteralTests` is the one legacy test: it uses `set(MyTestName ...)`, a hand-built `SourceFiles` list, and no `BeginProduct`/`EndProduct`, so it registers its CTest manually. Do not copy that pattern for new tests.
- Some test source directories do not match their product name (for example `StringToIntegerTests` uses the source dir `Arcadia.Ring1.Tests.StringToInteger`). Check the include paths in a given test's `Main.c` rather than assuming.
- Test support is in `Tests.h`: `Arcadia_Tests_assertTrue`, `Arcadia_Tests_assertFalse`, and `static inline Arcadia_Tests_safeExecute`, which acquires a process, pushes a jump target, runs the test function, checks the thread status, and relinquishes the process. On failure it sets `Arcadia_Status_TestFailed` and jumps.
- Standard `main`: `if (!Arcadia_Tests_safeExecute(&test)) { return EXIT_FAILURE; } return EXIT_SUCCESS;`. When a test directory holds several test functions, `main` calls `Arcadia_Tests_safeExecute` once per function, and each function is exposed from its own header.
- Expected-failure tests need their own jump target rather than `safeExecute`, because `safeExecute` would report the raise as a test failure. Clear the status with `Arcadia_Thread_setStatus(thread, 0)` after asserting that a raise happened, or the test itself is reported as failed.
- To check that a negative test is still meaningful, temporarily disable the validation it exercises and confirm the test then fails. A negative test that passes with the guard removed is not testing anything.

## Build And Verify

- Build trees live outside the checkout, one per architecture; this machine uses `C:/develop/Arcadia/Build/x64`.
- A focused target build: `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.Ring1.Tests.InterfaceTypeTests`.
- One focused test: `ctest -C Debug -R Arcadia.Ring1.Tests.InterfaceTypeTests`, or run the built `.exe` under `Runtime/Ring1/Tests/<Name>Tests/Debug/` and check the exit code.
- Full suite: `ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug`. A clean run is 68/68.
- Prefer running the built test executable directly for iteration; it is much faster than a full `ctest` run and gives the same exit code.
- Editing `Library/CMakeLists.txt` or any `CMakeLists.txt` triggers a CMake regenerate on the next build, which takes noticeably longer than an incremental compile.
- When scanning build output for problems, `error.html.te` and prebuilt third-party `libpng18_staticd.lib` `LNK4099` PDB warnings are expected noise. Judge success on the C compiler diagnostics, not on a raw `error|warning` grep.

## Documentation

- `Runtime/Ring1/Documentation` is built by the template engine as product `Arcadia.Ring1.BuildDocs`, from `index.html.te` into `.Website/Arcadia/Ring1/index.html` using a `website.env` file. Output under `.Website` is generated and not primary source; see `AGENTS/Documentation.md`.
- `.Website/*.html` files change whenever the compiler or template engine runs, because the build injects a timestamp. Treat that churn as expected and do not revert it as if it were an unrelated edit.
- `Runtime/Ring1/Documentation/3.documentation.5.jumps-and-jump-targets.i` documents the precise `Arcadia_JumpTarget` semantics.

## Conventions

- New C, header, and CMake files carry the AGPL-3.0-or-later notice used by the surrounding files, with the current copyright year range.
- Source files are CRLF. Verify there are no bare LF line endings after editing; text tools can silently introduce them and the diff then shows whole files as changed.
- Format function definitions and macro parameter lists in the repo's two-line style: the return type on its own line, then the function name, then the parenthesized parameter list indented by two, one parameter per line, closing paren on its own line.
- Prefer adding a test rather than relying on review to catch a regression in the type system.

## Licensing

- Preserve the AGPL-3.0-or-later header form for new C, header, and CMake files in this module.
