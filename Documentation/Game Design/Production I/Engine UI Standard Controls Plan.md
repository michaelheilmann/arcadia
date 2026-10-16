# Engine UI Standard Controls Plan

## Purpose

Track the implementation plan for standard controls in `Arcadia.Engine.UI`, derived from Scenario I UI requirements and the
current state of the Engine UI module.

This document acts as a lightweight ticket substitute. Keep it factual and update checklist items as implementation work lands.

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
  shared widget bounds.
- [x] Add an initial reusable Arcadia-native `StackPanelNode` layout container for horizontal/vertical rows, gaps, and
  centered cross-axis alignment.
- [x] Extend `StackPanelNode` with padding, explicit main/cross-axis alignment, and cross-axis stretch.
- [x] Add child margins and layout-slot metadata for per-child cross-axis alignment overrides.
- [ ] Add richer layout-slot metadata, such as per-child main-axis behavior and visibility/collapsed state.

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
- [ ] Add `TabButton` and `TabBar`.

## Milestone 3: Composite Options Screen Controls

- [ ] Add `DropdownNode`.
- [ ] Add `KeyBindingRow`.
- [ ] Add warning-label support for validation and conflict messages.
- [ ] Add an options screen example using the standard controls.

## Notes

- Prefer small, reusable control behavior over duplicating hit testing, focus, text rendering, and pointer handling in each
  control.
- Keep C construction APIs usable. Add ADL definitions after behavior is stable enough to declare as data.
- Preserve existing examples while expanding them into interactive demonstrations.
- `SegmentedChoiceNode` owns the segmented visual, option labels, and selection behavior.
- `ScrollPanelNode` implements `Container`, `Scrollable`, clipped child rendering through a child viewport/camera, and
  keyboard scroll steps. Mouse-wheel input is not available in the current input event set.
