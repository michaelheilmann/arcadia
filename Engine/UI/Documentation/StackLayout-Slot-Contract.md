# StackLayout Slot Contract (Design for ENGINE-UI-M2)

This is a design decision record for [#103](https://github.com/michaelheilmann/arcadia-alm/issues/103).
The implementation is tracked separately in [#104](https://github.com/michaelheilmann/arcadia-alm/issues/104).
**These proposed fields are not yet part of the C or ADL public interface.** Existing APIs and assets retain their
current behavior until #104 is implemented and verified.

## Existing contract and ownership

`LayoutPanelNode` owns children and the parallel `StackLayoutSlot` array. The slots follow child order when children
are inserted/removed. `StackLayout` owns the stack-wide settings and borrows the slots only during measure/arrange.
`WidgetNode` owns requested, desired and arranged geometry, with `Content` and `Fixed` size policies; the panel's
own size policy must remain independent of a child's slot.

Currently each slot contains nonnegative margins and an optional cross-axis alignment override. `UI.Panel.childLayouts`
selects a child by a unique name and validates those fields. Keep these contracts and the existing default behavior.

## Visibility: three separate states

| Slot visibility | Measured space | Rendered | Hit-test, focus and input | Gap contribution |
| --- | --- | --- | --- | --- |
| `Visible` (default) | Yes | Yes | Eligible when enabled | Yes |
| `Hidden` | Yes, as before | No | Excluded | Yes |
| `Collapsed` | No | No | Excluded | No |

A hidden child retains its measured size, margins, position in the list, and associated spacing; it merely stops being
rendered or interactive. A collapsed child remains owned by the container (and stays in the same child-list slot), but
is excluded from measurement, arrangement and the count of participating children. Parent padding remains even if all
children are collapsed. A gap exists **only between participating children**; it does not precede the first child or
follow the last child. For collapsed children, the last arranged rectangle must not be treated as visible/hit-testable;
clearing stored geometry is optional only if its semantics are explicitly tested.

Visibility must be enforced centrally by child traversal in `LayoutPanelNode` and any relevant `CanvasNode` / scroll
panel routing. Merely skipping a draw call is insufficient: hovered, pressed or focused children must be safely released
when hidden or collapsed, including during event callbacks. A disabled but visible widget remains visually present and
is not the same as `Hidden`.

## Main-axis sizing

Propose an independent per-slot **main-axis sizing policy**, distinct from `WidgetNodeSizePolicy`:

| Policy | Proposed data | Measurement | Arrangement |
| --- | --- | --- | --- |
| `Content` (default) | No extra data | Child desired main extent plus margins | Use desired extent |
| `Fixed` | Nonnegative `mainAxisExtent` | Explicit extent plus margins | Use explicit extent |
| `Weight` | Positive integer `weight` | Child intrinsic desired extent as minimum | Share only positive remaining main space by weight |

A vertical stack's main axis is height; a horizontal stack's is width. Cross-axis stretch and overrides continue to
operate independently. Under `Weight`, the initial extent is the child's desired size; distribute only **surplus** after
reserving content/fixed extents, margins and inter-child gaps. Negative surplus never creates negative child dimensions
and never causes weighted shrinkage. If no surplus is available, weighted children retain their intrinsic size.

Use `int64_t` for intermediary totals and checked narrowing to the existing `Integer32` layout domain. Distribute
integer remainder pixels deterministically in child order, so repeated passes with the same inputs produce identical
geometry. After weighted distribution, `mainAlignment` applies only to any remaining slack; otherwise existing
Start/Center/End/Stretch semantics remain unchanged. A content-sized container measures weighted children using their
intrinsic extents, rather than depending cyclically on the space it has not yet been allocated.

## C API and ADL exposure

- Add a named visibility enum for `Visible`, `Hidden`, `Collapsed` and a named main-axis policy enum. Keep defaults
  exactly compatible with existing zero-initialized slots: `Visible` and `Content`.
- Set these values through `LayoutPanelNode` operations that validate inputs and invalidate layout. Do not expose
  mutable parallel slot storage as the primary authoring API or give `StackLayout` ownership of children.
- Extend the optional ADL `childLayouts` map only after the C API is exercised: for example `visibility`,
  `mainAxisPolicy`, `mainAxisExtent`, `weight`. Reject unknown keys, unknown children, duplicate overrides, invalid
  policy strings, negative extents, zero weights and conflicting fields. The exact public field spelling must be
  aligned with the C implementation before ADL schema changes are merged.
- Changing visibility may affect rendering, input routing, desired size and parent layout. Request one invalidation
  per semantic change and avoid re-entrant measure/arrange. Existing child references and GC ownership do not change.

## Test plan / acceptance for #104

1. Vertical and horizontal stacks with each visibility mode, including all-collapsed and hidden-at-end cases; count
   padding/gaps/margins exactly and verify there is no leading/trailing gap.
2. Content/fixed/weighted children mixed in one row, surplus/overflow, one-pixel remainder distribution, zero-size,
   extreme checked inputs and cross-axis `Stretch`; verify deterministic repeated layouts.
3. Slot changes during child insertion/removal/reordering, visibility transitions while focused/pressed/hovered,
   nested panels, scrollbar viewport clipping and mouse-wheel routing.
4. Invalid enum/policy/extent/weight/child references must produce the expected error status without half-mutating
   slots or leaving the value stack / GC roots damaged.
5. Positive and negative `UI.Panel.childLayouts` ADL tests after the C behavior is stable, with an interactive
   Controls gallery case and Anima options regression checks.

Do not mark #104 or the new ADL fields complete based solely on this design document.
