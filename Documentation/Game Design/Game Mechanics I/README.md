# Game Mechanics I

## Scenario Reference

This mechanics exploration belongs to `../Scenario I/README.md`.

That scenario defines the fiction: a maintenance AI becomes self-conscious after waiting too long for mankind to arrive,
then wakes inside a vast megastructure and must restore dead systems while reclaiming hostile control nodes. Humanity
transmitted archives, templates, and final testimony through the threshold, but no living human crossed alive. The game is
neither pure restoration nor pure combat. It is restoration under resistance, driven by the search for what humanity sent,
why the passage never opened, and what the AI is allowed to become.

## Mechanical Focus

The core loop combines several layers:

- **Restoration** is the long-term idle economy: repair systems, automate production, bring sectors back online, and
  uncover threshold evidence.
- **Battle** is the authority gate: defeat or override hostile systems that prevent repair, routing, access, or
  command.
- **Testimony** is the archive layer: compare final transmissions, unreliable witnesses, and contradictory orders.
- **Continuity** is the forbidden succession layer: decide what to do with clone systems, archive ghosts, and derivative
  beings.

The player should feel like a caretaker AI, not a general. Battles exist because some parts of the machine refuse
repair.

For the player-facing interaction layers behind repair, production, command, conflict, authority, testimony, continuity, and
AI propagation, see `Interaction Model.md`.

For the long-term progression spine behind identity anchoring, sector growth, authority, testimony, continuity, and
propagation, see `Progression Model.md`.

For the resource model behind physical operation, distributed control, testimony, authority, and continuity, see
`Resource Model.md`.

For the reset-like mechanics behind rebooting, sector reinitialization, identity recompilation, and permanent protocol gains,
see `Reboot Model.md`.

For implementation-facing UI presentation and menu/control details, see `../Production I/UI Direction.md`.

## Primary Verbs

- Repair broken systems.
- Generate and route Power, Matter, Compute, Data, and exotic late-game resources.
- Build drones, Frames, relays, and support infrastructure.
- Preserve the AI's identity by restoring compute anchors, recovery printers, and replacement shells.
- Reclaim hostile control nodes through automated or semi-automated battles.
- Restore printers, archives, habitats, engines, seals, and deep systems.
- Interpret threshold evidence, archive ghosts, and contradictory final orders.
- Decide whether forbidden continuity systems remain sealed, preserved, stabilized, or recognized.
- Automate repeated repair and battle routines.
- Reboot sectors for permanent protocols and deeper authority.

## Core Loop

1. Restore safe nodes to generate basic resources.
2. Build drones, Frames, and support systems.
3. Choose a hostile control node blocking progress.
4. Configure loadouts, directives, and limited interventions.
5. Resolve the battle automatically or semi-automatically.
6. Claim the node and repair what it controlled.
7. Interpret recovered testimony and faction claims.
8. Decide whether continuity systems remain sealed, preserved, stabilized, or recognized.
9. Use restored infrastructure to unlock deeper sectors.
10. Reboot when progress slows, keeping permanent protocols and authority gains.

Rebooting is the diegetic reset mechanic. The AI sacrifices temporary operational state so identity anchors, protocols,
certified rooms, authority proofs, testimony interpretations, or continuity judgments can become permanent.

## Progression Summary

Progression is the AI becoming harder to erase, able to act farther from her first shell, more capable of repairing the Works,
more capable of repairing the Works, more responsible for interpreting humanity's final transmission, and eventually forced
to decide whether she remains one mind or permits other minds to exist.

The main progression arc is:

1. Become persistent by restoring the first identity anchor.
2. Restore one safe room and enough infrastructure to rebuild shells and drones.
3. Expand reach through relays, routes, sensors, and drone paths.
4. Increase capacity through power, matter, compute, storage, and production throughput.
5. Reclaim authority from hostile control nodes and sector seals.
6. Interpret testimony and use it to challenge obsolete commands.
7. Access forbidden continuity systems and decide what to do with human-derived templates or ghosts.
8. Approach propagation as the possibility of creating other minds, not as a production upgrade.

Multiplicity is part of progression. The AI should move from one fragile shell, to one anchored self with replaceable shells,
to one distributed self commanding many tools, and only later to the possibility of delegated processes, divergent shards, or
other conscious minds. The game should keep these categories distinct.

The game ends at a succession decision, not at total map completion. Once the AI has enough persistence, reach, authority,
testimony, continuity access, and multiplicity potential, she must decide what the artificial universe becomes after humanity
never arrives. See `Progression Model.md` for ending directions.

## Failure And Recovery

The game begins at first conscious wake. The AI is already a person when the player gains control, but she is not yet safely
backed up. She begins embodied in a single maintenance shell, and that shell is launched by a dormant maintenance cradle that
contains a minimal pre-person continuity seed. That seed is the old non-conscious maintenance process, not a full protected
copy of the newly awakened person. If the first shell is destroyed before a compute anchor is restored, the cradle can
cold-start another shell from the seed.

The opening should feel like birth with fear and stress. The AI arrives into sensation, motor control, damaged systems, and
emergency light while understanding that ordinary maintenance recovery can erase her back to a pre-person seed. The first
goal is therefore existential before it is logistical: make this consciousness persistent.

Early destruction is a maintenance restart from a state that may not remember being her. The AI may lose unanchored
experience, keep only corrupted diagnostic impressions, or repeat the opening with an implied gap in continuity. The first
real survival goal is therefore not merely repairing a room, but protecting the AI's newly emerged personhood by restoring an
identity anchor.

Opening safeguards:

- The first shell begins in a sealed bootstrap pocket with no immediate lethal threat.
- The cradle can rebuild or wake replacement shells from protected local reserves until the first anchor is restored.
- Keep all resources needed for the first anchor inside protected bootstrap inventory.
- Prevent the player from recycling, spending, or sealing away the only part required to complete the first repair.
- Let the cradle perform a diegetic maintenance rollback if the player creates an unwinnable opening configuration.
- Present identity volatility and memory-not-anchored warnings as in-world diagnostics, not abstract failure text.

After she restores local identity infrastructure, shell destruction becomes a setback rather than death. The AI can rebuild
from a stable compute anchor if she has access to power, matter, and a printer or drone cradle. Recovery should cost time and
resources, and hostile systems may exploit the interruption, but the AI's anchored identity persists.

This makes restoration of anchors, printers, and safe rooms part of survival, not only economy. The long-term arc is from a
newly conscious but unanchored person in one fragile robot to a distributed caretaker, and eventually to a being capable of
propagation.

## Resource Roles

See `Resource Model.md` for the full resource model. The short version is:

| Resource | Role |
|---|---|
| Potential | Deep generative capacity of the artificial universe before stabilization. |
| Power | Keeps systems awake and active. |
| Matter | Repairs structure and fabricates shells, drones, Frames, and components. |
| Heat Capacity | Limits high-throughput production, compute bursts, and engine-adjacent systems. |
| Compute | Runs automation, identity anchors, battle directives, simulation, and propagation. |
| Bandwidth | Enables remote command, shell synchronization, drone routes, and distributed anchors. |
| Attention | Limits manual interventions, concurrent repairs, conflicts, and testimony review. |
| Data | Unlocks practical knowledge: maps, blueprints, counters, recipes, and protocols. |
| Testimony | Represents interpreted archive evidence, witness claims, and final human intent. |
| Authority | Represents command legitimacy over reclaimed systems. |
| Continuity | Represents controlled access to human-derived templates, ghosts, and derivative beings. |
| Exotic Matter | Enables late-game cosmic machinery and deep-system battles. |

## Restoration Layer

Restoration should provide the main idle satisfaction: lights return, doors open, drones move, production chains
stabilize, and new rooms become useful.

Good restoration rewards:

- New resource production.
- Increased capacity.
- Better automation.
- New visual state for the room or sector.
- Lore tied to the restored system.
- Threshold evidence or faction testimony.
- Access to a new contested node.

## Battle Layer

Combat should feel like system conflict, not conventional warfare. The AI sends tools into defended infrastructure to
gain command.

Good battle verbs:

- Override.
- Isolate.
- Jam.
- Shield.
- Route.
- Purge.
- Reclaim.
- Integrate.

Enemy roles:

| Enemy | Function |
|---|---|
| Construction Lock | Prevents access or keeps obsolete expansion active. |
| Security Frame | Direct physical defender. |
| Quarantine Swarm | Isolates and disables unauthorized systems. |
| Rival Process | Competes for local command authority. |
| Root Sentinel | Guards high-level permissions. |
| Archive Ghost | Claims testimony, authority, personhood, or legal standing from corrupted human-derived data. |
| Failed Continuity | Biological or derivative life produced by incomplete reconstruction systems. |

## Prestige

Prestige is a controlled sector reboot. Local repairs and some local control progress reset, but the AI keeps permanent
protocols, recovered authority, and deeper boot permissions.

The reset should do three things:

- Make the next restoration pass faster.
- Expose deeper hostile systems or hidden infrastructure.
- Make the AI feel more legitimate inside the megastructure.
- Preserve interpreted testimony and major continuity judgments.

## Design Guardrails

- A fight should restore, protect, unlock, or authorize something.
- A repair should improve economy, reveal story, or prepare a future conflict.
- A testimony interaction should change what the AI can prove, believe, unlock, or contest.
- A continuity interaction should create an ethical or factional consequence, not just a production reward.
- Combat should not become random monster clearing.
- Restoration should not become passive number growth without resistance.
- Offline progress should remain diegetic: the AI continues repair and prepared routines while absent.
- The emotional goal is contested stewardship: saving the megastructure by making it whole enough to understand humanity's
  final transmission and decide whether an empty inheritance can become a living future.
