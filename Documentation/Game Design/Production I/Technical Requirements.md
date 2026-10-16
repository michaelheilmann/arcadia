# Technical Requirements

This document lists implementation activities for realizing Scenario I as an Arcadia game executable. It translates the
visual targets from `../Scenario I/Art Direction.md`, `../Scenario I/Room Types And States.md`,
`../Scenario I/Factions.md`, `../Scenario I/Key Scenes.md`, and `UI Direction.md` into engine, asset, scene, interaction,
and validation work.

The project title is `Project Arcadia's Anima`. The game should live as its own executable product, separate from engine
examples.
The former engine demo is being repurposed as `Engine/Engine/Examples/FreeFlyCamera`, a first-person/free-fly camera
controller example that should not carry Scenario I game logic.

## Guiding Constraints

- [ ] Keep machine rooms compact, dense, utilitarian, and scaled by function rather than spectacle.
- [ ] Reserve larger volumes for machinery, material flow, heat, transport clearance, storage, agriculture, or human
  assembly.
- [ ] Keep human-facing spaces more legible, comfortable, and public in scale, but prepared rather than inhabited; no scene
  should imply that living humans crossed alive.
- [ ] Treat human-derived entities as archive ghosts, failed biological continuities, simulations, or derivative life rather
  than surviving humans.
- [ ] Present hostile encounters as access denial, system authority, quarantine, lockout, or repair conflict rather than
  arena combat.
- [ ] Choose lighting color from function and state. Do not allow all scenes to drift toward warm yellow or amber.
- [ ] Avoid decorative neon, ornamental bling, cathedral-like machine rooms, and accidental human workbench layouts in
  machine-first spaces.

## Implementation Order

- [ ] Implement advanced text layouting and clipping in `Engine/Text`: line breaking, alignment, measurement, overflow,
  clipping rectangles, and layout data suitable for rendering and hit testing.
- [ ] Integrate text rendering into `Engine/UI` so UI nodes can display clipped, styled text without relying on
  example-local rendering code.
- [ ] Implement standard `Engine/UI` elements such as labels, buttons, panels, lists, sliders, text fields, and focus or
  navigation behavior.
- [ ] Implement options-screen control types required by `UI Direction.md`: buttons, toggles, sliders, dropdowns or segmented
  choices, tab buttons, scroll panels, key binding rows, warning labels, and later confirmation dialogs or preview panels.
- [ ] Implement the Scenario I starting screen as a cradle boot interface with `Continue`, `New Game`, `Options`, and `Exit`,
  following `UI Direction.md`.
  Initial Engine/UI PoC exists in `Games/Anima` with the required top-level menu actions and cradle diagnostics mood.
- [ ] Implement the Scenario I options screen with display, audio, controls, and accessibility sections, following
  `UI Direction.md`.
  Initial Engine/UI PoC exists in `Games/Anima` using segmented tabs, toggles, sliders, and buttons.
- [ ] Implement rendering and lighting facilities needed by Scenario I: practical light groups, state-driven palettes,
  material states, dense geometry readability, and debug views.
- [ ] Implement static scene management: scene descriptors, asset references, room taxonomy links, state variants,
  loading/unloading, interaction anchors, and validation.
- [ ] Implement Scenario I scenes and gameplay in the `Project Arcadia's Anima` executable after the engine facilities above
  are usable.

## Product Split

- [x] Create `Project Arcadia's Anima` as a dedicated Scenario I executable target, initially with a minimal boot path and no
  dependency on the camera example module.
- [ ] Keep game-specific names, configuration directories, saves, scene descriptors, and assets under the
  `Project Arcadia's Anima` product rather than under engine examples.
- [ ] Convert the former engine demo into a focused first-person/free-fly camera controller example.
- [ ] Keep the camera example useful for engine validation: window creation, backend selection, asset staging, event routing,
  W/A/S/D movement, Q/E roll, and mouse-look.
- [ ] Avoid putting Scenario I art direction, gameplay, scene progression, or story state into the camera example.

## Core Scene Architecture

- [ ] Define a data model for room type, visual state, faction presence, scale reason, human-versus-machine framing, lighting
  palette, and active scene event.
- [ ] Implement scene descriptors that can reference reusable room taxonomy entries from `Room Types And States.md`.
- [ ] Implement scene descriptors that can reference reusable faction entries and faction states from `Factions.md`.
- [ ] Implement state-driven scene variants for unrestored, restored, locked-down, hostile, rebooting, human-preserved,
  and failed spaces.
- [ ] Add support for per-scene milestone flags so restoration, access denial, archive discovery, faction contact, threshold
  evidence, and signal reception can persist in save data.
- [ ] Build a scene loading path that can assemble static geometry, animated machinery, lighting, particles, audio, and
  interaction points from one descriptor.
- [ ] Add debug visualization for scene anchors, drone routes, interactable ports, light groups, access gates, and trigger
  volumes.

## Modular Environment Kits

- [ ] Build a compact machine-room kit with low ceilings, wall cradles, cable trays, pipework, filters, hatches, sensor
  clusters, diagnostic ports, service panels, and drone sockets.
- [ ] Build a printer-bay kit with feedstock cartridges, part queues, compact fabrication heads, automated tool ports,
  calibration jigs, manipulator tracks, conveyor pockets, and service cassettes.
- [ ] Build a relay-cabinet kit with command racks, breaker columns, cable trunks, armored panels, isolation shutters,
  lockout clamps, route indicators, and authority cores.
- [ ] Build a service-junction kit with drone rails, pipe manifolds, conduit bundles, access shafts, heavy shutters,
  layered paths, and reboot-capable relay groups.
- [ ] Build a large factory-hall kit with assembly lanes, suspended manipulators, cargo rails, feedstock trunks, part
  buffers, sorting gates, recycling mouths, gantries, and module clamps.
- [ ] Build a habitat kit with apartments, public walkways, railings, planters, trees, civic surfaces, medical points,
  school or plaza fragments, orientation landmarks, and preserved comfort materials.
- [ ] Build an archive/relay kit with storage racks, data monoliths, memory cores, retrieval arms, record cassettes,
  signal-routing columns, machine rails, and sparse human-facing interpretation surfaces.
- [ ] Build a threshold-archive kit with signal buffers, transmission strata, farewell packet storage, failed passage
  diagrams, archive flood routing, and evidence surfaces for the last data crossing.
- [ ] Build a biogenesis/quarantine kit with sealed gestation racks, inert culture vessels, medical manipulators, containment
  doors, decontamination frames, failed ecology modules, and warning surfaces that avoid body-horror spectacle by default.
- [ ] Build a simulation/witness kit with inactive witness terminals, personality-model racks, legal testimony chambers,
  education surfaces, degraded avatar projection points, and archive-ghost containment cues.
- [ ] Build a deep-access-seal kit with seal rings, armored shutters, authority relays, segmented security frames,
  command racks, restricted service paths, and lockout mechanisms.
- [ ] Build a command-deck kit with operator stations, handrails, observation panes, command displays, sealed welcome kits,
  formal circulation paths, and aged human decision-space materials.
- [ ] Build an observation-spine kit with reinforced panorama windows, lounge seating alcoves, handrails, planters,
  personal-scale walkways, dust layers, derelict public-space surfaces, and exterior engine vistas.

## Materials And Surface States

- [ ] Create shared material variants for dormant metal, active metal, aged polymer, sealed glass, damaged glass, dust,
  frost, grime, soot, residue, worn paint, hazard markings, preserved human interiors, sterile biogenesis surfaces, and
  degraded archive media.
- [ ] Implement state overlays for dead indicators, lit indicators, locked slots, open slots, sealed cassettes, active
  ports, clogged filters, heated circuits, and restored task surfaces.
- [ ] Add decal support for stains, scratches, maintenance markings, route arrows, hazard bands, old public signage shapes,
  and unreadable data fragments.
- [ ] Create material rules that prevent machine rooms from looking decorative unless the room is explicitly human-facing
  or mixed.

## Lighting And Color Systems

- [ ] Implement named lighting palettes for unrestored machine, restored machine, locked-down, hostile, rebooting,
  human-preserved, command, archive, habitat, and cosmic-window scenes.
- [ ] Support light groups that can be enabled, disabled, sequenced, dimmed, flickered, or color-shifted by scene state.
- [ ] Add cool white inspection strips, pale blue-white diagnostics, green-white status indicators, red warning pinlights,
  amber hazard markers, neutral daylight, and exterior cosmic reflection lights as reusable fixtures.
- [ ] Add validation or editor warnings for scenes whose palette is dominated by warm yellow or amber without a declared
  reason.
- [ ] Implement reboot lighting sequences where groups turn off and return in controlled waves.
- [ ] Implement hostile lighting states that read as denial, lockout, or quarantine rather than decorative combat color.

## Machinery And Animation

- [ ] Implement dormant machinery poses for stalled rails, locked part buffers, sealed cassettes, hanging fabrication arms,
  closed shutters, clamped feedstock trunks, and inactive diagnostic ports.
- [ ] Implement restored machinery loops for moving trays, calibrated tool heads, active ports, manipulator tracks,
  conveyor pockets, relay cycling, and purposeful drone motion.
- [ ] Implement lockout animation for shutters, clamps, denial frames, authority relays, and deep-access seal rings.
- [ ] Implement reboot animation for relays cycling, shutters unlatching, diagnostic surfaces waking, and lights returning
  in sequence.
- [ ] Implement archive discovery animation for retrieval arms, record cassettes, data layers, routing diagrams, and
  contradictory command fragments without readable text.
- [ ] Implement threshold-evidence animation for transmission strata, failed passage diagrams, archive flood sequences,
  farewell packet indexing, and delayed signal echoes.
- [ ] Implement faction reveal animation for archive ghosts, sealed biogenesis systems, derivative-life containment, and
  authority classifications without implying ordinary surviving humans.
- [ ] Implement cosmic-window exterior motion for distant engine petals, construction fields, artificial light fields, and
  slow megastructure-scale mechanisms.

## Drones, Hostile Systems, And Access Conflict

- [ ] Implement repair drones as fragile utility tools with inspection, unlock, carry, weld, cut, calibrate, and restore
  behaviors.
- [ ] Implement lock crawlers, quarantine motes, security frames, denial devices, and authority relays as system opponents
  rather than humanoid enemies.
- [ ] Implement access-control interactions where the player contests ownership of relays, shutters, printers, seals,
  archives, and route gates.
- [ ] Implement localized combat rules for compact machine rooms where conflict happens through ports, cables, clamps,
  shutters, drones, and command racks.
- [ ] Implement habitat quarantine behavior that cuts power, isolates routes, and denies movement without turning the
  scene into a conventional combat arena.
- [ ] Implement Root Sentinel behavior as a procedural authority challenge with phased access checks, seal negotiation,
  lockout responses, and legitimacy escalation.
- [ ] Implement faction encounter rules for hostile, negotiable, reclaimed, schismatic, and propagating faction states.
- [ ] Implement archive-ghost interactions as testimony, accusation, instruction, or deception rather than standard NPC
  conversation by default.

## Interaction And Gameplay Hooks

- [ ] Add inspectable objects for diagnostic ports, drone cradles, feedstock lockers, command racks, archive records,
  sealed welcome kits, command stations, and observation panes.
- [ ] Add restoration interactions for power routing, printer activation, relay repair, shutter release, route reopening,
  archive retrieval, and signal capture.
- [ ] Add authority progression that unlocks higher-level systems after key relays, control nodes, and seals are reclaimed.
- [ ] Add faction progression that records first contact, faction state changes, contested allegiance, and whether a faction
  has been preserved, sealed, reclaimed, or allowed to propagate.
- [ ] Add failure and retry rules for hostile access denial that avoid simple deathmatch outcomes.
- [ ] Add scene-specific objectives that express care, repair, and understanding rather than conquest.
- [ ] Add narrative reveal triggers for contradictory orders, failed threshold evidence, cosmic-engine revelation,
  command-deck absence, forbidden continuity paths, faction origins, and the answering signal.

## Camera And Presentation

- [ ] Define camera framing presets for compact machine pockets, printer bays, relay rooms, service junctions, factory
  halls, habitats, archives, deep seals, command decks, and observation windows.
- [ ] Support tight oppressive framing in machine rooms without hiding required interaction points.
- [ ] Support wide functional scale framing only where the room type justifies it, such as factory halls, deep seals, and
  cosmic observation scenes.
- [ ] Add reveal cameras for first wake, first restoration, archive contradiction, root seal denial, sector reboot, cosmic
  window, empty command deck, threshold evidence, first faction reveal, and answering signal.
- [ ] Ensure all key scenes have readable silhouettes for machinery, routes, interactables, and active denial systems.

## Audio Requirements

- [ ] Create ambience layers for dormant machine spaces, restored machinery, relay rooms, service junctions, factory halls,
  sealed habitats, archives, deep seals, command decks, and observation windows.
- [ ] Add state transitions for power loss, cold start, restoration, reboot, hostile lockout, quarantine activity, archive
  retrieval, authority denial, faction contact, threshold evidence, and signal acquisition.
- [ ] Use sparse mechanical audio in machine spaces: relays, fans, clamps, rails, shutters, servos, diagnostics, and distant
  structure noise.
- [ ] Use absence-focused audio in human spaces: room tone, air systems, faint public infrastructure, glass stress,
  preserved fixtures, and distant machinery.
- [ ] Add nonverbal signal audio for `A Signal That Answers` that suggests a human-origin threshold echo without resolving
  whether it is message, remnant, or response.

## Per-Scene Implementation Tasks

- [ ] `First Wake`: implement maintenance access pocket layout, shell cradle, protected bootstrap systems, dormant conduits,
  first conscious-wake presentation, identity-volatility diagnostics, first camera reveal, and minimal unrestored lighting.
- [ ] `The Unrestored Room`: implement dormant printer bay with sealed feedstock, stalled rails, empty drone cradles,
  locked fabrication heads, and restoration target markers.
- [ ] `The Unrestored Factory Hall`: implement large factory hall streaming, distant machinery culling, stalled assembly
  lanes, sealed transfer locks, suspended manipulators, and functional-scale composition.
- [ ] `The First Restored Room`: implement printer restoration sequence, drone emergence, active tool ports, calibrated
  fabrication heads, and first purposeful drone behaviors.
- [ ] `The Hostile Control Node`: implement relay-cabinet room, authority core, denial frames, lock crawlers or quarantine
  motes, access legitimacy objective, and hostile lockout lighting.
- [ ] `Quarantine Swarm In The Habitat Ring`: implement empty habitat plaza, prepared human details, quarantine mote swarm,
  route isolation, power-cut behavior, and tragic absence presentation.
- [ ] `Archive Of Contradictory Orders`: implement archive restoration, data retrieval surfaces, abstract contradiction
  visualization, threshold transmission evidence, record sequencing, and narrative evidence triggers.
- [ ] `Root Sentinel At The Deep Seal`: implement deep seal arena-as-threshold, Root Sentinel authority logic, lockout
  phases, seal-ring animation, and legitimacy escalation.
- [ ] `Sector Reboot`: implement service-junction reboot sequence, ordered light return, relay cycling, route reset,
  hostile routine changes, and post-reboot state persistence.
- [ ] `The Cosmic Engine Window`: implement derelict observation lounge, panorama window, exterior cosmic engine vista,
  slow exterior animation, and revelation camera.
- [ ] `The Empty Command Deck`: implement preserved command deck, unused operator stations, sealed welcome kits, restrained
  command lighting, and absence-focused interaction points.
- [ ] `A Signal That Answers`: implement restored archive or relay chamber, incoming signal visualization, signal audio,
  diagnostic routing, and obligation-oriented narrative trigger.

## Tooling And Pipeline

- [ ] Define authoring conventions for scene descriptors, asset names, room type IDs, visual state IDs, faction IDs, faction
  state IDs, light palette IDs, and interaction IDs.
- [ ] Add editor or debug tooling to preview room state changes without restarting the game.
- [ ] Add asset validation that detects missing meshes, missing materials, missing light groups, missing collision, missing
  interaction points, and missing accessibility routes.
- [ ] Add screenshot capture presets for each key scene so art direction can be reviewed consistently over time.
- [ ] Add performance captures for compact rooms, factory halls, habitat plazas, and cosmic-window vistas.
- [ ] Add documentation links from each scene descriptor back to the corresponding key scene, room taxonomy entry, and faction
  taxonomy entry when applicable.

## Rendering And Performance

- [ ] Support many small practical lights through batching, clustering, baking, or explicit per-scene light budgets.
- [ ] Support dense machine geometry with instancing, modular kits, occlusion, level-of-detail models, and streaming.
- [ ] Support large vistas for factory halls and cosmic-window scenes without requiring every exterior mechanism to be fully
  simulated.
- [ ] Support particles for dust, frost, vapor, quarantine motes, sparks, and signal traces with per-scene budgets.
- [ ] Support restrained effects for degraded simulations, archive ghost projections, sterile containment vapor, and signal
  echoes without making them look like fantasy magic.
- [ ] Support transparent or reflective panorama glass without breaking readability or performance.
- [ ] Add fallback visual settings for lower-end hardware that preserve function, silhouettes, and state readability.

## Testing And Acceptance

- [ ] Add automated checks that every key scene references a valid room type, visual state, image, scene descriptor, and
  lighting palette.
- [ ] Add automated checks that faction-bearing scenes reference valid faction IDs and faction states from `Factions.md`.
- [ ] Add playtest checklist items for spatial readability, interactable readability, state readability, performance,
  absence of unintended living humans, clarity of human-derived factions, and absence of accidental decorative spectacle.
- [ ] Add visual review checklist items for color policy compliance, machine-versus-human framing, functional scale,
  interface plausibility, and scene-specific emotional target.
- [ ] Add regression screenshots for each key scene after major rendering, lighting, material, or asset pipeline changes.
- [ ] Add save/load tests for scene state transitions: unrestored to restored, locked down to accessible, hostile to
  reclaimed, rebooting to stable, and undiscovered to revealed.
- [ ] Add save/load tests for faction state transitions: dormant to hostile, dormant to negotiable, hostile to reclaimed,
  negotiable to schismatic, and dormant to propagating.
- [ ] Add performance budgets and acceptance thresholds for compact rooms, medium junctions, large halls, habitat scenes,
  and cosmic vistas.
