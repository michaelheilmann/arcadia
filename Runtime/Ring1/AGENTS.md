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
- Scalar Type_Operations must leave objectTypeOperations null, even when an object-operations table would be empty. Natural16 previously violated this registrar invariant; NaturalToStringTests now exercises its real descriptor registration and equality/inequality at zero/one/maximum values. UI ValueControl's general-value round trips also cover Natural16.

## Interface Dispatch Semantics

- Interface dispatch structs are independent. A derived interface does **not** embed its ancestors' dispatch structs; each interface has its own struct starting with `Arcadia_InterfaceDispatch parent`.
- Consequently, an object implementing a derived interface also implements every ancestor interface. Registration automatically adds missing ancestor interface implementation records, each with its own dispatch. A synthesized ancestor dispatch has no initializer of its own; if it is not inherited from an ancestor object type, its operation slots are zero until an explicit specification for that interface is provided.
- `Arcadia_ObjectType_getInterfaceDispatch` matches the interface type exactly, then walks the ancestor *object* types. It deliberately does **not** fall back to a descendant's dispatch. Implementing an ancestor does not implement a derived interface, and implementing a derived interface uses distinct dispatch records for its ancestors.
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

- Follow the shared error-handling conventions in `AGENTS/CxxSourceCodeStyle.md`.
- `Arcadia_JumpTarget` (`Thread.h`) holds a `jmp_buf` plus a `previous` pointer; pair `Arcadia_Thread_pushJumpTarget` with `Arcadia_Thread_popJumpTarget`, and guard the body with `Arcadia_JumpTarget_save`.
- Common statuses: `Arcadia_Status_AllocationFailed`, `Arcadia_Status_OperationFailed`, `Arcadia_Status_StackCorruption`, `Arcadia_Status_ArgumentValueInvalid`, `Arcadia_Status_NotImplemented`, `Arcadia_Status_TestFailed` (used by the test assertions).

## Garbage Collection

- Follow the shared rooting rules in `AGENTS/CxxSourceCodeStyle.md`.
- `Arcadia_Object_lock`/`Arcadia_Object_unlock` (`Object.h`) are the Ring1 manual root APIs for temporary C-local objects.
- Object and interface type nodes are visited and finalized through `TypeSystem/ObjectTypeNode.module.c` and `TypeSystem/InterfaceTypeNode.module.c`; interface teardown is ordered so that an implemented interface is destructed after the objects that depend on it.
- Barriers are gated on `Arcadia_Configuration_withBarriers`, which is defined in `Object.h` as `Arcadia_ARMS_Configuration_WithBarriers`.

## Strings And Objects

- `Arcadia_RuntimeUTF8String` is a value type declared in `ImmutableByteArray.h`, created with `Arcadia_RuntimeUTF8String_create(thread, u8"...", sizeof(u8"...")-1)` and boxed with `Arcadia_Value_makeRuntimeUTF8StringValue`. It is a common interface operation return type.
- `Arcadia_String` is a separate *object* type in `Objects/String.h`, declared with `Arcadia_declareObjectType` and with constructors from the boolean, integer, and natural scalar types.
- `Arcadia_ImmutableUTF8String` (with `ImmutableUTF8StringExtensions.{h,c}`) and `Objects/ByteArray.h` are the immutable buffer types. `Objects/ByteReader.h` and the `*.ByteReader` types are separate object types wrapping a buffer plus a read cursor.
- Note the naming trap: `Arcadia_ImmutableByteArray` is declared in the plain file `ImmutableByteArray.h`, and the `RuntimeUTF8String` family lives alongside it in that same header (the misleading empty `ImmutableByteArray` folder is gone after the `Implementation/` flattening).

## Tests

- BigInteger zero has sign 0 and zero limbs; a non-NULL malloc(0) result does not make limb 0 readable. Scalar conversions must use that canonical zero representation and handle Integer64.Minimum without negating a signed minimum. Two's-complement arrays are little endian; use unsigned carry/sign-mask arithmetic, advance every loop, preserve sign-extension words and release a destination's old owned buffer after input reads (including aliases).
- Decimal Natural64 parsing batches 19 digits, so its power table must include exponent 19 (20 entries). Preserve exact maximum/overflow rejection and parser stack cleanup; Natural64ParsingBoundaries covers 18/19/20-digit and overflow cases.
- Process teardown must release unmanaged type-table buckets/container and ARMS-owned intern/notification allocations. Callback owners must unregister their callbacks: modules during shutdown and the process's ThreadVisitCallback after module shutdown. Assert the callback list is empty on normal and startup-failure teardown; do not blanket-clear registrations and hide missing unregister calls. Type-removed callbacks still run before ARMS frees interned type names; freeing table storage must not directly free managed type entries or external callback state. ProcessTests exercises repeated reference-counted startup/shutdown, callback-failure recovery and allocation balance (native MSVC debug heap; Linux sanitizer process-exit checks).

- Each test is its own subdirectory under `Runtime/Ring1/Tests/<Name>Tests`, with sources under `Sources/Arcadia.Ring1.Tests.<Name>Tests` and a `CMakeLists.txt` following this pattern:
  - `set(this ${MyProjectName}.Ring1.Tests.<Name>Tests)`, `BeginProduct(${this} test)`, `SetFolder(${this} "Runtime/Ring1")`, `OnSourceFile`/`OnHeaderFile` entries, `OnModuleDependency(${this} ${MyProjectName}.Ring1 PRIVATE)`, `EndProduct(${this})`.
  - `BeginProduct(... test)` registers the CTest automatically.
- `Runtime/Ring1/Tests/CMakeLists.txt` must list every test subdirectory. A new test directory is not built until it is added there.
- `Runtime/Ring1/Tests/LiteralTests` uses the standard test-product macros and is included by its parent. Its single LiteralTests callback checks number-literal parsing and invalid-input rejection; CTest discovers it as Arcadia.Ring1.Tests.LiteralTests.LiteralTests. The obsolete manual registration and Arms spelling are retired.
- Some test source directories do not match their product name (for example `StringToIntegerTests` uses the source dir `Arcadia.Ring1.Tests.StringToInteger`). Check the include paths in a given test's `Main.c` rather than assuming.
- Test support is in `Tests.h`: `Arcadia_Tests_assertTrue`, `Arcadia_Tests_assertFalse`, and `static inline Arcadia_Tests_safeExecute`, which acquires a process, pushes a jump target, runs the test function, checks the thread status, and relinquishes the process. On failure it sets `Arcadia_Status_TestFailed` and jumps.
- Suite entry points include `Arcadia/Testing/Include.h` and dispatch through `Arcadia_Testing_runSuite`, linking `${MyProjectName}.Testing` privately. Multi-case Ring1 suites expose one named callback per existing `Arcadia_Tests_safeExecute` entry point, retaining the original order and process/jump-target boundary. Fixture loops and grouped operation regressions remain inside those callbacks. Use `--case NAME` to select a component, or omit arguments to run all components, continuing after component failure. Former whole-suite selectors for split suites are retired; single-case suites retain their suite-directory selectors. CTest discovers `Executable.Case` registrations through `--list`, without duplicate whole-suite runs; they remain RUN_SERIAL. Use executable `--list` for current names, `Runtime/Testing/README.md` for runner contracts and `Documentation/Testing.md` for commands/debugging.
- Expected-failure tests need their own jump target rather than `safeExecute`, because `safeExecute` would report the raise as a test failure. Clear the status with `Arcadia_Thread_setStatus(thread, 0)` after asserting that a raise happened, or the test itself is reported as failed.
- Read `AGENTS/Testing.md` for shared test rules. Opt-in typed equality/status/string/byte assertions in Tests.h/Tests.c report compared values and selected case/source location before raising TestFailed; simple assertTrue/assertFalse remain unchanged. Keep Size distinct from Natural64 and choose signed/unsigned helpers deliberately. Real64 uses exact equality, not a tolerance.
- The runner scopes a borrowed OS-thread-local case name using `Arcadia_Tests_getCaseName`/`Arcadia_Tests_setCaseName`. It must survive process teardown without adding fields/roots to runtime objects; restore the previous pointer for nested scopes and keep its string alive. It is not inherited by worker threads. Catch runtime jumps inside callbacks, not across the runner boundary.
- To check that a negative test is still meaningful, temporarily disable the validation it exercises and confirm the test then fails. A negative test that passes with the guard removed is not testing anything.

## Build And Verify

- Build trees live outside the checkout, one per architecture; this machine uses `C:/develop/Arcadia/Build/x64`.
- A focused target build: `cmake --build C:/develop/Arcadia/Build/x64 --config Debug --target Arcadia.Ring1.Tests.InterfaceTypeTests`.
- One focused test: `ctest -C Debug -R Arcadia.Ring1.Tests.InterfaceTypeTests`, or run the built `.exe` under `Runtime/Ring1/Tests/<Name>Tests/Debug/` and check the exit code.
- Full suite: `ctest --test-dir C:/develop/Arcadia/Build/x64 -C Debug`. The inventory is discovered from the built configuration; do not use a historical fixed test count as a completeness check.
- Prefer running the built test executable directly for iteration; it is much faster than a full `ctest` run and gives the same exit code.
- Editing `Library/CMakeLists.txt` or any `CMakeLists.txt` triggers a CMake regenerate on the next build, which takes noticeably longer than an incremental compile.
- When scanning build output for problems, `error.html.te` and prebuilt third-party `libpng18_staticd.lib` `LNK4099` PDB warnings are expected noise. Judge success on the C compiler diagnostics, not on a raw `error|warning` grep.

## Documentation

- `Runtime/Ring1/Documentation` is built by the template engine as product `Arcadia.Ring1.BuildDocs`, from `index.html.te` into `.Website/Arcadia/Ring1/index.html` using a `website.env` file. Output under `.Website` is generated and not primary source; see `AGENTS/Documentation.md`.
- `.Website/*.html` files change whenever the compiler or template engine runs, because the build injects a timestamp. Treat that churn as expected and do not revert it as if it were an unrelated edit.
- `Runtime/Ring1/Documentation/3.documentation.5.jumps-and-jump-targets.i` documents the precise `Arcadia_JumpTarget` semantics.

## Conventions

- Follow the shared C/C++ source code style guide in `AGENTS/CxxSourceCodeStyle.md`.
- Prefer adding a test rather than relying on review to catch a regression in the type system.

## Licensing

- Preserve the repository AGPL-3.0-or-later header form for new C, header, and CMake files in this module.
