# Engine UI Standard Controls Plan

## Purpose

Track the implementation plan for standard controls in `Arcadia.Engine.UI`, derived from Scenario I UI requirements and the
current state of the Engine UI module.

This document records the implementation state of the standard controls. The active work tracker is
[ENGINE-UI-M2 #100](https://github.com/michaelheilmann/arcadia-alm/issues/100); the checkboxes here describe only
features implemented on `develop`, not unmerged proposals or tickets that merely exist.

## Requirements Sources

- `Documentation/Game Design/Production I/UI Direction.md`
- `Documentation/Game Design/Production I/Technical Requirements.md`
- `Documentation/Game Design/Game Mechanics I/Interaction Model.md`

## Milestone 1: Interactive Button Foundation

### ADL Button Support

- [x] Make the `UI.Button` reader honor `horizontalAlignment` and `verticalAlignment`.
- [x] Add optional color-definition references for normal, hovered, pressed, disabled, and focused button states.
- [x] Preserve `backgroundRed`, `backgroundGreen`, and `backgroundBlue` as the normal-state fallback for existing assets.

### Input Preflight

- [x] Fix or verify mouse button event mapping in the WGL backend before relying on interactive UI tests.
- [x] Confirm mouse coordinates passed to UI input use a consistent origin and convert them at the UI root if needed.

### Canvas Root

- [x] Make `CanvasNode` support child add/get/remove through the `Container` interface.
- [x] Add reverse-order hit-test routing to `CanvasNode`.
- [x] Add pointer routing to `CanvasNode`, including hovered and pressed child tracking.
- [x] Add keyboard routing to `CanvasNode`, including focused child tracking.
- [x] Use `Tabulator` for focus traversal where possible.

### Widget Geometry

- [x] Standardize shared widget position, size, and bounds APIs on `WidgetNode`.
- [x] Provide widget position-changed and size-changed event types.
- [ ] Migrate controls from per-control geometry fields to the shared widget geometry where practical. `LabelNode`,
  `ButtonNode`, `ToggleNode`, `SliderNode`, `SegmentedChoiceNode`, and `ScrollPanelNode` now render and hit-test from
  shared widget bounds; audit and remove remaining redundant stored geometry without changing public behavior
  ([#102](https://github.com/michaelheilmann/arcadia-alm/issues/102)).
- [x] Replace the transitional `StackPanelNode` / `UI.StackPanel` with `LayoutPanelNode` / `UI.Panel`, where an
  independent `StackLayout` performs measure/arrange of horizontal or vertical child lists.
- [x] Keep shared `LayoutAlignment` and stack padding, gaps, main/cross-axis alignment, and cross-axis stretch.
- [x] Add child margins and per-child cross-axis overrides owned by `LayoutPanelNode`, including ADL `childLayouts`.
- [ ] Extend layout-slot metadata with visibility/collapse and main-axis sizing semantics ([#103](https://github.com/michaelheilmann/arcadia-alm/issues/103),
  [#104](https://github.com/michaelheilmann/arcadia-alm/issues/104)); see
  [StackLayout Slot Contract](../../../Engine/UI/Documentation/StackLayout-Slot-Contract.md).

### Interactive Button

- [x] Make `ButtonNode` implement `HitTestable`.
- [x] Make `ButtonNode` implement `Focusable`.
- [x] Make `ButtonNode` implement `Activatable`.
- [x] Make `ButtonNode` implement `PointerInput`.
- [x] Make `ButtonNode` implement `KeyboardInput`.
- [x] Add enabled, hovered, pressed, and focused state to `ButtonNode`.
- [x] Render `ButtonNode` background from the active visual state.
- [x] Activate `ButtonNode` on left mouse release inside the button.
- [x] Activate `ButtonNode` on `Return` or `Space` when focused.
- [x] Keep disabled buttons visible but non-interactive.

### Reusable Text

- [x] Add `LabelNode` as a reusable text node.
- [x] Support text, position, size, alignment, and color in `LabelNode`.
- [x] Add clipping support to `LabelNode`.
- [x] Extract or share the button-local text rendering path with `LabelNode` where practical.

### Example

- [x] Expand the former `Arcadia.Engine.UI.Examples.Button` target into the `Arcadia.Engine.UI.Examples.Controls` interactive gallery.
- [x] Demonstrate normal, disabled, focused, hovered, and pressed button states.
- [x] Route scene pointer and keyboard input through `CanvasNode`.
- [x] Show static gallery/help text through a `LabelNode`.
- [x] Show button activation feedback, preferably through a `LabelNode`.

## Milestone 2: Basic Options Controls

- [x] Add `ToggleNode`.
- [x] Add `SliderNode`.
- [x] Add `SegmentedChoiceNode`.
- [x] Add `ScrollPanelNode`.
- [x] Implement keyboard/wheel input for the scroll panel on existing WGL/GLX backends.
- [x] Add reusable selection styling to ordinary `ButtonNode` for tab headers; keep focus and activation independent
  ([#105](https://github.com/michaelheilmann/arcadia-alm/issues/105)).
- [ ] Compose a `TabBar` controller around ordinary buttons; add ADL composition and a gallery demonstration
  ([#106](https://github.com/michaelheilmann/arcadia-alm/issues/106),
  [#107](https://github.com/michaelheilmann/arcadia-alm/issues/107)).

## Milestone 3: Composite Options Screen Controls

- [ ] Add `DropdownNode`, its interaction contract, ADL schema, and gallery coverage
  ([#108–#110](https://github.com/michaelheilmann/arcadia-alm/issues/108)).
- [ ] Define portable binding capture and implement `KeyBindingRow` and its ADL support
  ([#111–#113](https://github.com/michaelheilmann/arcadia-alm/issues/111),
  [#115](https://github.com/michaelheilmann/arcadia-alm/issues/115)).
- [ ] Add reusable warning/status display for validation and binding conflicts
  ([#114](https://github.com/michaelheilmann/arcadia-alm/issues/114)).
- [x] Demonstrate a working prototype of the Anima options menu with segmented choices, toggles, sliders, and buttons.
- [ ] Complete category-based Anima options with tabs, bindings, dropdowns, warnings and reliable settings behavior
  ([#116](https://github.com/michaelheilmann/arcadia-alm/issues/116)).
- [ ] Complete cross-platform UI integration and regression verification
  ([#117](https://github.com/michaelheilmann/arcadia-alm/issues/117)).

## Notes

- Use HTML/CSS/JavaScript as architectural inspiration: small reusable nodes, layout and styles describe structure and
  presentation; composition/controllers provide behavior. Avoid a public `TabButtonNode` or `UI.TabButton` definition.
- Selected buttons reuse their existing `pressedBackgroundColor` until independent selection styling is justified.
  Selection does not imply focus, activate the button or enforce exclusivity; those belong to the composing controller.
- Prefer small, reusable control behavior over duplicating hit testing, focus, text rendering, and pointer handling in each
  control.
- Keep C construction APIs usable. Add ADL definitions after behavior is stable enough to declare as data.
- Preserve existing examples while expanding them into interactive demonstrations.
- `SegmentedChoiceNode` owns the segmented visual, option labels, and selection behavior.
- `ScrollPanelNode` implements `Container`, `Scrollable`, clipped child rendering through a child viewport/camera, and
  keyboard scroll steps. Wheel events are routed to the panel under the pointer on the existing WGL/GLX backends;
  availability on a future macOS window backend is not established by those implementations.
- The active layout composition is `UI.Panel` with an optional nested `UI.StackLayout`. `StackPanelNode` and
  `UI.StackPanel` were retired; do not use their names in new examples or ADL assets.
- The four Anima option categories are a **target state**, not a claim that dedicated tabs or all their settings exist today.
