# Arcadia Engine: Resource Lifecycle and Ownership

**Status:** Implementation guidance based on the current source code (26 October 2026)  
**Scope:** `Arcadia_Engine_Visuals_Resource` and `Arcadia_Engine_Audials_Implementation_Resource`, their derived classes, and their backend contexts.  
**Recommended repository location:** `Engine/Resource-Lifecycle-and-Ownership.md`

## 1. Purpose

This document describes the resource lifetime and dependency-ownership protocol implemented by Arcadia's engine. It is intended for programmers and coding agents extending or repairing resource classes. It distinguishes existing, source-confirmed behavior from recommended invariants and subjects requiring further verification.

**Central rule:** An Arcadia resource object, its backend representation (such as an OpenGL or OpenAL handle), and its resource-to-resource ownership edges are distinct. Do not conflate their lifetimes.

## 2. Three independent concerns

### 2.1 Arcadia object lifetime

A resource is an Arcadia object (`Arcadia_Object` subclass). The backend context holds strong references to its resources and participates in the GC retention scheme. Object reachability and finalization are distinct from backend handle allocation/deallocation. Do not implement ordinary backend destruction in an Arcadia object destructor as a substitute for the resource lifecycle.

### 2.2 Backend representation lifetime

A live resource object can have its backend representation allocated or deallocated. `load` creates or updates the backend representation; `unload` disposes of it without invalidating the resource object. The public header explicitly calls `unload` reversible.

### 2.3 Logical resource dependencies

A resource can refer to other resources. Such dependencies normally increment a *resource-specific* `referenceCount` with `Resource_ref`, and release it with `Resource_unref`. This reference count is not the Arcadia garbage collector's reference count. It tells the backend context when a resource can be retired from its resource registry, subject to context policy.

## 3. Types and dispatch

The visuals base object is `Arcadia_Engine_Visuals_Resource`; the audials base object is `Arcadia_Engine_Audials_Implementation_Resource`. Both have:

- An `Arcadia_Object` base.
- A `referenceCount`, initialized to zero.
- An unmanaged pointer to their backend context.
- Virtual `load`, `unload`, `unlink`, and `render` operations.
- `ref` and `unref` helper operations.

The wrappers in `Resource.c` forward operations using `Arcadia_VirtualCall`. Derived classes install callbacks in their `initializeDispatchImpl` methods. Implement new resource classes through the existing dispatch system; do not bypass virtual dispatch by directly calling backend implementations from clients.

The render signatures differ: visuals `render` additionally takes an `EnterPassResource`; audials `render` does not. Never assume the two subsystems have identical implementations merely because their lifecycle interface is similar.

## 4. Lifecycle operations

| Operation | Meaning | Ownership/dependency implications |
| --- | --- | --- |
| `construct` | Establish a logical resource object and initial state | Acquire and count owned resource dependencies; handle partial failures |
| `load` | Ensure or update the backend representation | May load dependent resources; does not itself transfer ownership of dependencies |
| `render` | Visit/use the resource in a render operation | For visuals, ensure/update relevant backend state and rendering context; specific behavior varies by type |
| `unload` | Release this resource's backend representation | Reversible; do **not** release logical dependency counts merely because backend handles are unloaded |
| `unlink` | Irreversibly detach the resource from the context and its dependencies | Called after unload; unref held dependencies, clear pointers and context linkage |
| `destruct` | Finalize the managed Arcadia object | Must not silently stand in for `unload`/`unlink`; some classes assert unlink has already cleared pointers |

### 4.1 `load`

A leaf resource can allocate an API object (e.g., texture/buffer), then apply data and state. The OpenGL4 texture implementation uses an object ID and dirty bits; repeated calls can refresh changed properties. A composite resource may simply call `Resource_load` on referenced resources, even if it has no backend handle of its own.

Do not assume every implementation has identical failure recovery or full idempotency. Inspect each derived class and its backend before relying on those properties. In particular, a successful `load` must not increment logical dependency reference counts on every invocation.

### 4.2 `unload`

`unload` is **reversible**. It removes the *backend representation* and leaves the Arcadia resource object, configuration data, and ownership edges intact so `load` can run again. The OpenGL texture implementation deletes its GL texture ID and restores dirty bits so state can be uploaded after recreation. A composite resource that has no owned backend handle may legitimately have an empty `unload` implementation.

Do not automatically call `unload` on every dependency of a composite resource: dependencies may be shared and have their own backend lifetime.

### 4.3 `unlink`

`unlink` is **irreversible**. Its documented precondition is that the resource has first been unloaded. Derived implementations must undo dependency `Resource_ref` operations with matching `Resource_unref` operations, clear their dependency pointers, and clear their unmanaged backend-context pointer. Do not reload, render, or otherwise reuse an unlinked resource.

In contrast to `unload`, `unlink` changes the resource graph permanently.

### 4.4 `render`

Visuals `render` accepts an `EnterPassResource`. The base header describes its responsibilities as ensuring that a backend representation is created/updated and updating the rendering context resource. Rendering is type-specific: a concrete backend implementation may perform the GPU work, while an abstraction-layer implementation may be a no-op. `render` must not be interpreted as object creation/destruction or logical ownership acquisition.

## 5. Resource reference count and backend-context cleanup

The base resource constructor sets `referenceCount = 0`. `Resource_ref` checks overflow and increments the count; `Resource_unref` checks underflow and decrements it. Invalid counts result in an Arcadia thread status and jump.

For the native visuals Resource, the valid count domain is zero through
`Arcadia_Integer32Value_Maximum`. `Resource_ref` at the maximum and `Resource_unref`
at zero raise `Arcadia_Status_OperationInvalid` before changing the count. Ref from
maximum minus one and unref from one are valid. These operations do not change the
borrowed context pointer or synchronously dispose of the managed object. This
describes valid-state transitions, not validation of arbitrary corrupted negative
counts or invalid receiver pointers.

Backend contexts keep collections of their resources. The WGL visuals backend and OpenAL audials backend perform a resource cleanup pass broadly like:

```c
if (!resource->referenceCount || force) {
  Resource_unload(thread, resource);
  Resource_unlink(thread, resource);
  Arcadia_List_removeAt(thread, resources, i, 1);
}
```

`Resource_unload` and `Resource_unlink` above are schematic names; actual functions are subsystem-prefixed. **A zero reference count makes a resource eligible for context-driven retirement; it does not synchronously destroy it when `unref` is called.** Removing it from the context's list permits its ordinary managed-object lifetime to conclude according to GC behavior.

Because unlinking a composite resource decrements the counts of its dependencies, removal may require more than one cleanup pass depending on list ordering. Do not replace this with immediate recursive destruction. The semantics of `force` require special care: forced retirement can unlink resources that are still logically referenced, so usage must be limited to appropriate context shutdown/teardown pathways.

## 6. Ownership graph: `ModelResource`

The engine-level `ModelResource` is a useful canonical composite example. It acquires references to an owned constant buffer and referenced mesh constant buffer, mesh vertex buffer, and material resource. Its constructor increments each dependency's resource count. Its `loadImpl` loads the mesh constant buffer, mesh vertex buffer, and material. Its `unloadImpl` is intentionally empty. Its `unlinkImpl` decrements all four resource counts, clears pointers, and clears `context`. Its destructor asserts that those resource pointers have already been cleared.

Conceptually:

```text
BackendContext (holds strong references to resource objects)
  |
  +-- ModelResource  --ref--> own ConstantBufferResource
  |                  --ref--> mesh ConstantBufferResource
  |                  --ref--> mesh VertexBufferResource
  |                  --ref--> MaterialResource
  |
  +-- OtherResource  --ref--> same MaterialResource (possible sharing)
```

A model releasing its reference to a shared material does not itself mean that the material's GPU data must be immediately destroyed. The backend context is responsible for retiring resources when appropriate.

Count ownership edges, not distinct pointer identities. If a model's own constant
buffer and mesh constant buffer alias the same resource, successful construction
acquires two references to that resource; unlink releases both. On partial failure,
roll back only acquisitions that actually succeeded, even when the same pointer
occurs more than once. Another owner's counts must remain unchanged.

**Important distinction:** Even if a dependent resource is mentioned during `load`, that does not imply the same dependency must be recursively unloaded during `unload`.

## 7. Constructor failure and GC integration

A new resource must initialize dependency pointers to `NULL` before acquiring references, and must roll back every successfully acquired reference if a subsequent step fails. `ModelResource_constructImpl` demonstrates this with `Arcadia_JumpTarget` and explicit `Resource_unref` calls in the error branch.

Managed (GC) object fields requiring traversal must be visited in a derived `visitImpl` as appropriate. Resource dependency ownership bookkeeping and garbage-collector visitation are different responsibilities: do not replace `Resource_ref` with GC visitation, or vice versa. The `ModelResource` example visits its managed matrix but does not treat the referenced resources as ordinary GC edges in its `visitImpl`; the backend context retains them.

Do not rely on a destructor to recover partial resource-reference acquisitions or release live GPU/OpenAL handles after a failed constructor.

An initialized managed field is not necessarily reachable during construction.
Native allocation does not root the unfinished receiver, and until
`Arcadia_LeaveConstructor` installs the derived type, only the active superclass
visitor participates in GC. Before invoking a collecting callback, root both the
receiver and initialized managed state invisible to that visitor, and restore
the temporary roots on success and failure. ModelResource roots its receiver and
local-to-world matrix through the constant-buffer factory callback and subsequent
dependency acquisitions/rollback. Original argument-frame values retain its
context and input dependencies; temporary roots must not shift argument selection.
EnterPassResource does this for its constant-buffer factory callback, both matrices
and clear color. Its viewer-buffer
field is published only after `Resource_ref` succeeds; a failed acquisition must
not be treated as an owned dependency.

The headless VisualResources `ResourceLifecycle` regression checks boundary
errors without count mutation, ModelResource acquisition-stage rollback, shared
and aliased dependencies, unload preserving ownership and unlink balancing it.
The separate ModelConstruction case in that suite collects inside ModelResource's
constant-buffer callback, weakly observes the unfinished receiver/matrix and
context-retained dependencies, and checks success, callback failure, each refcount
overflow stage, aliased rollback, root release and caller-stack restoration. Its
receiver-only guarded pass isolates derived-field visitation; its unguarded pass
requires production receiver rooting too. These headless checks do not establish
concrete GPU retirement or generated borrowed-reference execution semantics. The Backends
`VisualsBackendContext` regression separately exercises EnterPassResource's
collecting constructor callback and failure/root cleanup.

## 8. Dirty state and reloads

Separate the **logical state** of a resource (texture pixels, sampler settings, vertex descriptions, program sources, transforms, etc.) from the **backend state** holding an API representation. A logical change should mark the affected backend state dirty and be applied when the resource is loaded or updated in accordance with its implementation.

In the OpenGL4 texture implementation, dirty bits track addressing modes, minification/magnification filters, and pixel data. `loadImpl` uploads changed data/state; `unloadImpl` deletes the backend texture and resets dirty bits to request a complete upload on reloading.

A new resource implementation should clearly specify which state survives `unload` and which state is regenerated on `load`. The code should support its advertised reload behavior instead of depending on stale GL/OpenAL handles.

## 9. Rules for new resource implementations

1. Derive from the correct subsystem base resource and install all required virtual callbacks in `initializeDispatchImpl`.
2. Retain resource dependencies exactly once when establishing each ownership edge; release them exactly once during rollback or unlink.
3. Keep ordinary Arcadia managed references and explicit resource reference counts conceptually separate.
4. Implement `load` to create/update the resource's own backend representation and load dependencies where appropriate, without changing logical ownership merely because it was called repeatedly.
5. Implement `unload` to free **only the backend representation owned by this resource**, preserving the resource object and reference graph.
6. Implement `unlink` to detach all logical resource dependencies and the unmanaged context pointer; call it only after unload.
7. Make reloading safe by restoring/marking necessary backend state as dirty after release.
8. Respect the context's resource registry and its cleanup protocol; do not manually free Arcadia resource objects or recursively destroy shared resources.
9. Handle constructor failures by undoing references acquired before the failure.
10. Follow the Arcadia thread-status / jump convention on invalid operations, and add tests for first load, repeated load, unload/reload, shared dependencies, zero-reference retirement, and context shutdown.

## 10. Common mistakes

| Incorrect approach | Correct approach |
| --- | --- |
| Treating `unload` as final object deletion | Keep the resource object and ownership graph available for reload |
| Releasing dependency counts from `unload` | Release those counts during irreversible `unlink` |
| Recursively unloading every child of a composite | Let each resource/context manage shared backend lifetimes |
| Incrementing child resource counts during every `load` | Count each established dependency edge once |
| Letting object destructors perform GL/OpenAL deletion | Perform backend teardown in `unload` with a valid context |
| Treating `Resource_unref` as immediate delete | Let backend-context cleanup retire zero-count resources |
| Assuming a no-op `unload` is missing code | First establish whether the class owns any backend representation |
| Assuming dirty bits and reload behavior are identical everywhere | Inspect the concrete resource implementation |

## 11. Caveats and checks before treating this as a fully normative specification

- **Per-implementation guarantees:** The code establishes a common *intent*, but exact load/unload idempotency, exception/jump cleanup, context-loss handling, and dirty-bit restoration should be audited for each concrete resource class.
- **Forced cleanup:** Verify the destruction order and the consequences of unlinking resources whose counts are still positive during forced context teardown.
- **Possible GLX cleanup defect:** The GLX backend resource-update implementation was seen checking `((Arcadia_Engine_Visuals_Resource*)self)->referenceCount` where WGL/OpenAL check `resource->referenceCount`. This appears inconsistent and deserves a targeted source review and regression test; do not silently codify it as intended behavior.
- **Different `render` implementations:** Some engine-layer render callbacks are empty; the concrete backend determines actual drawing/audio operations.
- **Terminology:** `Resource` here means an engine runtime resource. It must not be confused with the gameplay resource model under `Documentation/Game Design` or with Arcadia ARMS's memory-management terminology.

## 12. Source of truth

Read the following in order when making a resource-system change:

1. [`Engine/Engine/Library/Sources/Arcadia/Engine/Visuals/Resource.h`](https://github.com/michaelheilmann/arcadia/blob/main/Engine/Engine/Library/Sources/Arcadia/Engine/Visuals/Resource.h) and [`.c`](https://github.com/michaelheilmann/arcadia/blob/main/Engine/Engine/Library/Sources/Arcadia/Engine/Visuals/Resource.c): interface, wrappers, counts.
2. [`Engine/Engine/Library/Sources/Arcadia/Engine/Visuals/Resources/ModelResource.c`](https://github.com/michaelheilmann/arcadia/blob/main/Engine/Engine/Library/Sources/Arcadia/Engine/Visuals/Resources/ModelResource.c): composite dependency ownership and constructor rollback.
3. [`repository/Visuals.Implementation/Library/Sources/Arcadia/Visuals/Implementation/OpenGL4/Resources/TextureResource.c`](https://github.com/michaelheilmann/arcadia/blob/main/repository/Visuals.Implementation/Library/Sources/Arcadia/Visuals/Implementation/OpenGL4/Resources/TextureResource.c): leaf resource backend creation/deletion and dirty state.
4. [`repository/Visuals.Implementation/Library/Sources/Arcadia/Visuals/Implementation/OpenGL4/Resources/VertexBufferResource.c`](https://github.com/michaelheilmann/arcadia/blob/main/repository/Visuals.Implementation/Library/Sources/Arcadia/Visuals/Implementation/OpenGL4/Resources/VertexBufferResource.c): additional leaf resource behavior.
5. [`repository/Visuals.Implementation/Library/Sources/Arcadia/Visuals/Implementation/OpenGL4/WGL/BackendContext.c`](https://github.com/michaelheilmann/arcadia/blob/main/repository/Visuals.Implementation/Library/Sources/Arcadia/Visuals/Implementation/OpenGL4/WGL/BackendContext.c): context-driven cleanup.
6. [`Engine/Engine/Library/Sources/Arcadia/Engine/Audials/Resource.h`](https://github.com/michaelheilmann/arcadia/blob/main/Engine/Engine/Library/Sources/Arcadia/Engine/Audials/Resource.h): parallel audio resource protocol.
7. [`Engine/Audials.Implementation/Library/Sources/Arcadia/Audials/Implementation/OpenAL/BackendContext.c`](https://github.com/michaelheilmann/arcadia/blob/main/Engine/Audials.Implementation/Library/Sources/Arcadia/Audials/Implementation/OpenAL/BackendContext.c): audio context cleanup.

**Repository instruction for coding agents:** Before modifying any engine `*Resource` type, resource factory, or backend-context resource cleanup, read this document, then verify the relevant concrete implementation. Preserve `load`/`unload` reversibility, `unlink` irreversibility, explicit dependency reference accounting, and context-managed retirement.
