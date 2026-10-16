# Room Types And States

This document lists reusable room types and visual states for Scenario I. The goal is to describe spaces before creating
artwork, so illustrations follow the logic of a machine-run megastructure rather than defaulting to spectacle.

See also `Art Direction.md` for the broader rules on light, scale, and machine-facing interfaces.

## Taxonomy

| Category | Type | Description | Common States | Visual Cues | Examples |
|---|---|---|---|---|---|
| Machine Room | <a id="maintenance-access-pocket"></a>Maintenance Access Pocket | Very small service space for inspection, local repair, and drone access; barely human-passable, because routine work is performed by drones and manipulators. | Unrestored, Restored, Locked Down | Low ceiling, tight standing room, cable trays, pipework, filters, lubrication lines, hatches, sensor clusters, diagnostic ports, drone sockets, sparse inspection light. | `First Wake` |
| Machine Room | <a id="printer-bay-or-repair-cell"></a>Printer Bay Or Repair Cell | Compact machine-facing room for fabricating parts, servicing drones, and preparing small repair assemblies; not a human workshop unless placed in a human maintenance area. | Unrestored, Restored, Locked Down, Hostile | Feedstock cartridges, part queues, compact fabrication heads, tool ports, calibration jigs, drone sockets, manipulator tracks, conveyor pockets, service cassettes; no desks, chairs, hand-tool benches, or human console layouts. | `The Unrestored Room`, `The First Restored Room` |
| Machine Room | <a id="relay-cabinet-room"></a>Relay Cabinet Room | Small local data, power, or command-routing room; oppressive because machines need only rack, shutter, cable-trunk, and service clearance. | Unrestored, Restored, Locked Down, Hostile, Rebooting | Relay cabinets, breaker columns, armored command racks, cable trunks, sealed panels, diagnostic ports, isolation shutters, route indicators, lockout clamps; hostility appears as denial of access rather than spectacle. | `The Locked Control Node` |
| Machine Room | <a id="service-junction"></a>Service Junction | Medium intersection where multiple machine systems meet; larger only because routing, drone movement, heat, and maintenance clearance require it. | Restored, Locked Down, Hostile, Rebooting, Failed | Drone rails, pipe manifolds, breaker columns, conduit bundles, access shafts, heavy shutters, multiple paths and layers, no unnecessary open floor. | `Sector Reboot` |
| Machine Room | Quarantine Duct Or Cable Trunk | Narrow infrastructure route for cables, fluid lines, data fiber, and micro-drone access; battles here are small-scale and system-oriented. | Unrestored, Hostile, Failed | Cable bundles, conduit clamps, dust, residue, sensor nodes, blocked vents, quarantine motes, lock crawlers, cut lines, jammed shutters, inspection strips, tool lamps, emergency markers. |  |
| Machine Room | Drone Staging Niche | Compact holding, charging, and dispatch area for maintenance drones, often attached to printer bays, relay rooms, or service junctions. | Unrestored, Restored, Locked Down, Hostile | Cradle sockets, charging contacts, tool cartridges, rail docks, service cassettes; empty or occupied drone slots show state; restored versions show purposeful motion rather than decoration. |  |
| Transport Route | Package Conduit | Small route for samples, parts, tools, data cartridges, biological material, or sealed service cassettes; often too small for humans. | Unrestored, Restored, Locked Down, Hostile, Failed | Narrow tubes, cartridge rails, pneumatic locks, sorting flaps, sensor gates, maintenance sockets, blockages, jammed cassettes, quarantined samples, cut signal lines, stalled micro-carriers, inspection pinlights. |  |
| Transport Route | Drone Route | Compact route for maintenance drones, crawlers, inspection units, and small repair frames; may run above, below, or beside human spaces without being built for human comfort. | Unrestored, Restored, Locked Down, Hostile, Rebooting | Rail sockets, magnetic guide strips, docking notches, charge contacts, traffic shutters, local bypasses, small-scale conflicts between repair drones, lock crawlers, quarantine motes, and denial devices. |  |
| Transport Route | Service Corridor | Human-passable maintenance route that remains machine-first; exists so humans, drones, and larger tools can reach otherwise inaccessible systems. | Unrestored, Restored, Locked Down, Hostile, Failed | Low ceilings, exposed conduits, pipe racks, cable trays, access hatches, pressure doors, floor grates, sparse inspection and safety lighting, security frames or sealed shutters. |  |
| Transport Route | Freight Shaft | Large vertical or horizontal route for bulk matter, modules, machinery, and assembled components; scale follows cargo clearance, loading, safety, heat, vibration, and maintenance access. | Unrestored, Restored, Locked Down, Hostile, Rebooting, Failed | Heavy rails, cargo locks, lift tracks, module clamps, transfer cradles, braking systems, inspection gantries; large empty volumes only where cargo or machinery must pass. |  |
| Transport Route | Megastructure Transit Spine | Huge route, potentially hundreds of meters across, for massive modules, habitat segments, engine parts, construction frames, or entire machine assemblies. | Unrestored, Restored, Locked Down, Hostile, Failed | Nested lanes, structural ribs, immense clamps, tug drones, magnetic guide fields, cargo cradles, distant gantries, safety voids; vast but legibly infrastructural, not decorative. |  |
| Human Or Mixed Room | <a id="habitat-segment"></a>Habitat Segment | Human-livable area embedded in the megastructure; justifies more light, air, orientation, decoration, and public scale because humans were meant to live there. | Human-Preserved, Locked Down, Hostile, Failed | Apartments, schools, plazas, trees, medical points, public signage, railings, windows, comfort lighting, landmarks, human-scale paths; absence hurts because the space was prepared for people. | `Quarantine Swarm In The Habitat Ring` |
| Human Or Mixed Room | <a id="command-deck"></a>Command Deck | Human decision space built around operator coordination, ceremony, visibility, and command authority, prepared for operators who never arrived. | Human-Preserved, Locked Down, Hostile | Operator stations, handrails, sealed welcome kits, observation panes, command displays, more designed lighting than pure machine rooms, restrained by age and absence. | `The Empty Command Deck` |
| Human Or Mixed Room | <a id="archive-chamber"></a>Archive Chamber | Mixed machine-human information space; can be larger if humans accessed, interpreted, or ceremonially preserved records, while pure machine archives remain dense and rack-like. | Restored, Locked Down, Hostile, Human-Preserved | Data monoliths, storage racks, memory cores, retrieval arms, projection surfaces where human access is needed, abstract record fragments without readable text, contradiction without excessive holographic ornament. | `Archive Of Contradictory Orders` |
| Large Machine Room | Engine Gallery | Large room justified by machinery scale, heat, vibration, magnetic clearance, or service movement. | Unrestored, Restored, Locked Down, Hostile, Failed | Massive housings, clearance gaps, cooling channels, structural ribs, lift tracks, shielded access spines; scale comes from function, not empty grandeur. |  |
| Large Machine Room | Freight Or Construction Field | Large logistics or assembly space for moving matter, modules, drones, and large components. | Unrestored, Restored, Hostile, Failed | Heavy rails, gantries, module sockets, cargo locks, fabrication scaffolds, storage lanes; larger battles fit because larger machines have a reason to exist. |  |
| Large Machine Room | <a id="factory-hall"></a>Factory Hall | Factory-sized production space for repeated fabrication, assembly, recycling, or component finishing; large because materials, modules, arms, gantries, and transport systems move continuously. | Unrestored, Restored, Locked Down, Hostile, Rebooting, Failed | Assembly lanes, suspended manipulators, machine rails, feedstock trunks, part buffers, sorting gates, recycling mouths, clearance volumes only where arms, cargo, heat, fumes, or modules require them. | `The Unrestored Factory Hall` |
| Large Machine Room | Agricultural Bay | Large life-support and food-production space; openness, softer light, moisture, and visual care are justified by plants, water, pollination systems, harvest drones, and human food needs. | Unrestored, Restored, Locked Down, Hostile, Human-Preserved, Failed | Growing racks, hydroponic troughs, nutrient pipes, humidity systems, harvest rails, pollination drones, seed vaults, grow lights, algae tanks, root mats, dry beds, mold, overgrowth, nutrient leaks. |  |
| Large Machine Room | <a id="deep-access-seal"></a>Deep Access Seal | Permission threshold for deep systems, root authority, or high-level infrastructure; large only if the locking machinery, service clearance, or security mechanisms require it. | Locked Down, Hostile, Rebooting | Heavy seal rings, command racks, lockout clamps, armored shutters, authority relays, security frames, sparse warning beacons, and restricted service paths. | `Root Sentinel At The Deep Seal` |
| Human Or Mixed Room | <a id="observation-spine"></a>Observation Spine | Human-facing observation habitat or lounge for seeing the Works' cosmic purpose; machines do not need panorama views, so large windows imply human purpose, comfort, memory, or ceremony. | Restored, Human-Preserved, Locked Down | Reinforced panorama windows, seating alcoves, handrails, personal-scale walkways, planters, comfort lighting, dust, derelict public-space architecture, exterior machinery and surrounding nothing beyond the glass. | `The Cosmic Engine Window` |

## Principles

Room design starts from function:

- What does the room do?
- What machines need to access it?
- How much clearance, light, cooling, storage, or transport does that function require?
- Was the space built for machines, humans, or both?
- What visible change does restoration, lockdown, or hostile control cause?

Most machine rooms are compact, dense, and utilitarian. Large spaces exist only when large machinery, logistics, heat,
transport, storage, agriculture, or human assembly require them.

The same rule applies to routes that are not rooms. Transport shafts, ducts, rails, and freight corridors can range from
package-sized conduits to passages hundreds of meters across, but their scale should always follow what they move and how
that movement is maintained.

## Visual States

| State | Description | Visual Cues |
|---|---|---|
| <a id="unrestored"></a>Unrestored | Useful machinery is present but unavailable. | Locked slots, stalled rails, sealed cassettes, dead diodes, dust, frost, grime, clogged filters, weak emergency light. |
| <a id="restored"></a>Restored | Function has returned without spectacle. | Task lights, active ports, moving trays, calibrated tool heads, working conduits, purposeful drone motion. |
| <a id="locked-down"></a>Locked Down | A system is active but refuses change. | Closed shutters, clamped panels, warning beacons, isolated routes, denial frames, access hatches sealed by command. |
| <a id="hostile"></a>Hostile | Local systems actively contest repair or authority. | Security frames, quarantine motes, lock crawlers, jammed relays, defensive cable routing, active denial devices. |
| <a id="rebooting"></a>Rebooting | A controlled shutdown or restart is in progress. | Light and motion returning in sequence, partial darkness at edges, relays cycling, diagnostics waking, shutters unlatching. |
| <a id="human-preserved"></a>Human-Preserved | A human space remains intact but was never occupied. | More legible lighting, comfort surfaces, personal scale, preserved furniture, public orientation, absence emphasized. |
| <a id="failed"></a>Failed | Restoration cannot proceed without deeper intervention. | Burned conduits, fused locks, collapsed supports, ruptured pipes, broken rails, hazard containment, missing modules. |

## Artwork Workflow

Before generating or commissioning an image, choose:

1. Room type.
2. Visual state.
3. Whether the space is machine-only, human-only, or mixed.
4. Functional reason for the room's scale.
5. Active visual change, such as repair, lockdown, battle, reboot, or discovery.

The image prompt should then emphasize the room's function and state before adding mood, lighting, or composition.
