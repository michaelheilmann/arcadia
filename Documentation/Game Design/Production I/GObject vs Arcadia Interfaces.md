# GObject Interfaces vs Arcadia Interfaces

This note compares a GObject-style interface model with the current Arcadia interface model, using a small UI-oriented example.

## Example Types

Assume two interfaces:

```text
Renderable
  render()

Scrollable extends Renderable
  scrollBy(amount)
```

And one object type:

```text
ScrollPanel
```

## GObject-Style Interface Model

In GObject, interfaces are integrated into the `GType` runtime system. A type can implement an interface, and interface
membership, lookup, prerequisites, default methods, properties, and signals are handled through the type system conventions.

Conceptually:

```c
interface Renderable {
  void render(Renderable* self);
}

interface Scrollable extends Renderable {
  void scroll_by(Scrollable* self, int amount);
}

class ScrollPanel implements Scrollable {
}
```

The intended high-level interpretation is:

```text
ScrollPanel implements Scrollable
Scrollable is related to Renderable
The type system can query and route interface use through runtime metadata
```

A caller can conceptually ask:

```c
if (implements(panel, Renderable)) {
  render(panel);
}

if (implements(panel, Scrollable)) {
  scroll_by(panel, 10);
}
```

The interface relationship is part of a broader runtime type system.

## Current Arcadia Interface Model

Arcadia interfaces currently use exact, independent dispatch tables.

A dispatch table for `Renderable` might look like:

```c
struct RenderableDispatch {
  Arcadia_InterfaceDispatch parent;
  void (*render)(Arcadia_Thread* thread, Arcadia_Object* self);
};
```

A dispatch table for `Scrollable` might look like:

```c
struct ScrollableDispatch {
  Arcadia_InterfaceDispatch parent;
  void (*scrollBy)
    (
      Arcadia_Thread* thread,
      Arcadia_Object* self,
      Arcadia_Integer32Value amount
    );
};
```

Important detail:

```c
struct ScrollableDispatch {
  Arcadia_InterfaceDispatch parent;
  void (*scrollBy)(...);
};
```

`ScrollableDispatch` does not embed or inherit `RenderableDispatch`.

So this is not the current Arcadia layout:

```c
struct ScrollableDispatch {
  RenderableDispatch parent;
  void (*scrollBy)(...);
};
```

## Practical Consequence

In Arcadia, if `Scrollable extends Renderable`, an object explicitly implementing `Scrollable` automatically receives a
separate ancestor implementation record for `Renderable`. The object becomes a descendant of both interface types, and exact
dispatch lookup for `Renderable` can find a `Renderable` dispatch record.

Conceptually, this source-level registration is enough to establish interface membership:

```text
ScrollPanel implements Scrollable
```

The runtime expands that into effective implementation records similar to:

```text
ScrollPanel implements Scrollable
ScrollPanel implements Renderable
```

However, the dispatch blocks remain independent. A synthesized ancestor dispatch has no initializer of its own. If it is not
inherited from an ancestor object type, its operation slots are zero until the ancestor interface is explicitly specified with
an initializer.

So this registration establishes membership but does not implement `Renderable.render`:

```c
Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.ScrollPanel",
    Arcadia_Engine_UI_ScrollPanel,
    u8"Arcadia.Object",
    Arcadia_Object,
    &ScrollPanel_typeOperations,

    {
      .interfaceType = _Arcadia_Engine_UI_Scrollable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_ScrollableDispatch),
      .initializeDispatch = &ScrollPanel_initializeScrollableDispatch
    }
  )
```

To implement both operation sets, the object still needs explicit initializers for both dispatch blocks:

```c
Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.Engine.UI.ScrollPanel",
    Arcadia_Engine_UI_ScrollPanel,
    u8"Arcadia.Object",
    Arcadia_Object,
    &ScrollPanel_typeOperations,

    {
      .interfaceType = _Arcadia_Engine_UI_Renderable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_RenderableDispatch),
      .initializeDispatch = &ScrollPanel_initializeRenderableDispatch
    },

    {
      .interfaceType = _Arcadia_Engine_UI_Scrollable_getType(thread),
      .dispatchSize = sizeof(Arcadia_Engine_UI_ScrollableDispatch),
      .initializeDispatch = &ScrollPanel_initializeScrollableDispatch
    }
  )
```

Interface calls are exact:

```c
Arcadia_Engine_UI_Renderable_render(thread, (Arcadia_Object*)panel);
Arcadia_Engine_UI_Scrollable_scrollBy(thread, (Arcadia_Object*)panel, 10);
```

The first call looks for the exact `Renderable` dispatch.

The second call looks for the exact `Scrollable` dispatch.

If `ScrollPanel` only specified `Scrollable`, then `Scrollable_scrollBy` could work and `Renderable_render` could find the
synthesized `Renderable` dispatch, but `Renderable_render` would raise `Arcadia.Status.NotImplemented` if the synthesized
dispatch's `render` operation slot is still zero.

## Difference In One Sentence

GObject treats interfaces as part of a rich runtime type/interface system, while current Arcadia interfaces use exact
independent dispatch tables and synthesize ancestor implementation records for interface membership.

## Why This Matters

A natural UI interface hierarchy might be:

```text
Interactive extends Renderable
ButtonLike extends Interactive
Button implements ButtonLike
```

In current Arcadia, that establishes ancestor membership automatically, but it can still create explicit dispatch-initializer
obligations for every operation-bearing ancestor interface.

If:

```text
Interactive extends Renderable, HitTestable, Focusable
ButtonLike extends Interactive, Activatable
```

then `Button` will become an instance of these interfaces:

```text
Renderable
HitTestable
Focusable
Interactive
Activatable
ButtonLike
```

But each operation-bearing ancestor still needs a valid dispatch implementation if callers invoke it. A synthesized dispatch
record with zero operation slots is not enough.

## Design Implication For Current UI Work

For the first UI foundation pass, operation-bearing UI interfaces should stay mostly flat and capability-oriented:

```text
Button implements Renderable
Button implements HitTestable
Button implements Focusable
Button implements PointerInput
Button implements KeyboardInput
Button implements Activatable
```

Instead of:

```text
Button implements ButtonLike
ButtonLike extends Interactive
Interactive extends Renderable, HitTestable, Focusable
```

## Possible Arcadia Interface Improvement

Arcadia now synthesizes ancestor implementation records. Remaining ergonomic improvements are mostly about making operation
implementation harder to forget:

1. Generate ancestor dispatch initializers through PDLC when a class implements a derived interface.
2. Add helper macros for explicit multi-interface dispatch specifications.
3. Add richer query/cast helpers for optional interfaces.
4. Add interface prerequisites for constraints like "pointer input requires hit testing".
5. Add default interface methods if repeated UI behavior proves that worthwhile.
