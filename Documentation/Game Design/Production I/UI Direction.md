# UI Direction

This document describes fiction-facing UI presentation for Scenario I. It complements
`../Game Mechanics I/Interaction Model.md`, which defines what the player does, and `Technical Requirements.md`, which tracks
implementation work.

The UI should feel like machine diagnostics, maintenance authority, archive testimony, and identity protection rather than a
generic game overlay. Player-facing labels should remain clear, but secondary text, layout, and motion can carry the setting.

## Starting Screen

The starting screen should be the dormant maintenance cradle before or around first conscious wake. It is not a heroic title
screen, a starfield, or a planet vista. There are no stars, planets, galaxies, or external space. The view should be intimate:
a dark service pocket, a single inactive shell in a cradle, weak emergency diodes, dead conduits, sealed relays, and protected
bootstrap systems.

The screen should feel like birth is possible, but not safe.

### Visual State

- The AI shell is present but still.
- Cables, clamps, and cradle geometry imply vulnerability rather than power.
- Most of the room is dark, with sparse maintenance light and emergency indicators.
- Diagnostic overlays can appear as cradle output, not as a separate decorative HUD.
- Warnings can flicker subtly: `identity not anchored`, `cradle seed available`, `memory volatility`, `human command absent`.
- If a prior game exists, a faint identity-anchor pulse or `last anchor detected` diagnostic can justify `Continue`.

### Menu Items

Use clear button labels:

- `Continue`
- `New Game`
- `Options`
- `Exit`

If no prior game exists, hide or disable `Continue`.

The buttons can have small diagnostic subtitles, but the main labels should stay readable:

| Button | Diagnostic Subtitle |
|---|---|
| Continue | `resume anchored process` |
| New Game | `initiate first conscious wake` |
| Options | `configure interface` |
| Exit | `return to silence` |

### Example Layout Text

```text
PROJECT ARCADIA'S ANIMA

identity state: unstable
cradle seed: available
anchor state: none
human command: absent

Continue
New Game
Options
Exit
```

The title can be prominent, but the mood should remain quiet and stressful. The screen should imply that ordinary maintenance
recovery can erase the newly awakened self if she is not anchored.

## Options Screen

The options screen should look like a maintenance configuration panel opened from the cradle interface. It should remain
plain enough to use quickly. Do not hide basic settings behind excessive fiction. Use clear section names, with small
diagnostic subtitles only where they add tone.

Use four primary tabs:

- `Display`
- `Audio`
- `Controls`
- `Accessibility`

Each tab should show only controls for that category. Do not mix display, audio, controls, and accessibility settings in a
single combined list once tabs are visible.

Optional later tabs can include `Interface` and `Gameplay`, but avoid exposing unfinished simulation tuning until the core
loop exists.

### Display Options

Minimum options:

- Display mode: fullscreen, borderless, windowed.
- Resolution.
- VSync.
- Brightness.
- Contrast.
- UI scale.
- Text scale.
- Motion intensity.

Later options:

- Gamma calibration.
- Field of view if direct shell view uses a 3D camera.
- Reduce flicker.
- Reduce screen shake.
- Diagnostic overlay opacity.

### Audio Options

Minimum options:

- Master volume.
- Music volume.
- Effects volume.
- UI volume.
- Voice or testimony volume, if archive ghosts or recorded testimony use voice.

Later options:

- Dynamic range: full, reduced, night mode.
- Mute when unfocused.
- Subtitles for voiced archive/testimony content.

### Controls Options

Controls should support keyboard/mouse first, with controller support as a parallel input scheme. Controls should be
remappable from the start if feasible.

Core controls:

| Action | Keyboard / Mouse Default | Controller Default |
|---|---|---|
| Move shell / cursor focus | `W` `A` `S` `D` | Left stick |
| Look / pan camera | Mouse | Right stick |
| Primary interact / confirm | `E` or left mouse | `A` / Cross |
| Back / cancel | `Esc` or right mouse | `B` / Circle |
| Open map / sector view | `Tab` | View / Select |
| Open command interface | `C` | D-pad up |
| Open repair interface | `R` | D-pad right |
| Open production interface | `P` | D-pad down |
| Open testimony interface | `T` | D-pad left |
| Cycle interaction layer | Mouse wheel or `Q` / `E` | Left / right bumper |
| Pause / system menu | `Esc` | Menu / Start |
| Quick scan / inspect | `F` | Right trigger |
| Assign selected drone / tool | Left mouse or `Enter` | `X` / Square |
| Center on active shell | `Space` | Left stick click |

These bindings may evolve once the camera perspective and moment-to-moment interaction mode are final. The important rule is
that core verbs should be reachable without menu diving: observation, repair, production, command, conflict, authority,
testimony, continuity, and propagation.

Control options:

- Remap keyboard bindings.
- Remap controller bindings.
- Mouse sensitivity.
- Controller look sensitivity.
- Invert X/Y look.
- Toggle or hold for scan/inspect.
- Toggle or hold for layer overlays.
- Cursor edge-pan on/off for map views.

### Accessibility Options

Minimum options:

- Subtitles on/off.
- Subtitle size.
- High-contrast UI mode.
- Colorblind-safe status colors.
- Reduce flicker.
- Reduce camera motion.
- Hold-to-toggle alternatives for repeated inputs.
- Pause or slow down tactical conflict panels while menus are open.

Later options:

- Separate text speed for testimony playback.
- Font choice or dyslexia-friendly text mode.
- Audio cue captions for warnings, anchor pings, hostile lockouts, and cradle diagnostics.
- Simplified diagnostic overlays.

### Interface Options

These can be folded into `Display` or `Accessibility` early on and split later if needed.

- Diagnostic density: minimal, standard, detailed.
- Lore subtitles: off, subtle, full.
- Resource labels: icons only, names, names with explanations.
- Automation notifications: all, important, critical only.
- Pause on testimony discovery.
- Pause on continuity decision.

### Options Screen Tone

Button labels should remain normal:

- `Apply`
- `Reset`
- `Back`

Lay out the footer actions horizontally. `Back` belongs on the left as the navigation/cancel action. `Reset` and `Apply`
belong on the right, with `Reset` before `Apply`.

Small diagnostic subtitles can carry fiction:

| Button | Diagnostic Subtitle |
|---|---|
| Apply | `write local configuration` |
| Reset | `restore recommended defaults` |
| Back | `return to cradle interface` |

The options screen should not imply the AI is already fully awake before `New Game`. Before a run starts, treat settings as
cradle/interface configuration. During a run, the same screen can be framed as local interface calibration.

### Required Control Types

The options screen should be buildable from a small, reusable set of UI controls. Prefer standard controls with clear labels
over exotic sci-fi widgets.

| Control Type | Used For | Notes |
|---|---|---|
| Button | `Apply`, `Reset`, `Back`, remap binding, restore defaults. | Primary action widget. Needs disabled, focused, hovered, pressed, and warning states. |
| Toggle | VSync, subtitles, mute when unfocused, reduce flicker, reduce camera motion, cursor edge-pan. | Binary on/off settings. |
| Slider | Brightness, contrast, volume, UI scale, text scale, opacity, sensitivity, motion intensity. | Should support keyboard/controller step changes and mouse dragging. |
| Dropdown | Display mode, resolution, dynamic range, diagnostic density, lore subtitles, resource label mode. | Use for finite choices where only one option is active. |
| Segmented Choice | Fullscreen/windowed, subtitle size, notification level, hold/toggle behavior. | Use when choices are few and benefit from being visible at once. |
| Key Binding Row | Keyboard/mouse and controller remapping. | Shows action name, current binding, conflict state, and reset control. |
| Tab Button | `Display`, `Audio`, `Controls`, `Accessibility`. | Options sections should be reachable without leaving the screen. |
| Scroll Panel | Long control lists and binding tables. | Must support wheel, keyboard, controller, and scrollbar interaction. |
| Confirmation Dialog | Discard changes, reset defaults, incompatible graphics changes, exit while unapplied changes exist. | Keep text direct; diagnostic subtitles are secondary. |
| Warning Label | Binding conflicts, unsupported resolution, restart-required changes, unsafe accessibility combinations. | Should not rely on color alone. |
| Preview Panel | Brightness/contrast calibration, UI scale, subtitle preview, diagnostic overlay density. | Optional early, useful before settings become numerous. |

Minimum first implementation:

- Button.
- Toggle.
- Slider.
- Dropdown or segmented choice.
- Tab button.
- Scroll panel.
- Key binding row.
- Warning label.

Confirmation dialogs and preview panels can come later, but the UI architecture should not make them difficult to add.

## General UI Rules

- Keep primary actions understandable before making them loreful.
- Use diagnostic subtitles for fiction, not as replacements for clear labels.
- Prefer sparse, functional motion: scanlines, status pulses, relay wakeups, anchor pings, and warning flickers.
- Avoid generic sci-fi holographic spectacle unless a specific room or system justifies it.
- Let UI state reflect identity, authority, testimony, continuity, and reboot risk.
