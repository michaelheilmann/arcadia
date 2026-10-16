# Interaction Model

This document sketches how the player interacts with the game moment to moment. It complements `README.md`, which defines
the high-level loop, `Progression Model.md`, which defines the long-term arc, and `Resource Model.md`, which defines the
main constraints.

The interface should express the fiction: the player is a maintenance AI expanding from local repair into forbidden
succession. Humanity transmitted archives, templates, and commands through the threshold, but no living human ever arrived.
The player should not feel like a person walking through rooms with a gun. The player should feel like a distributed mind
looking through sensors, routing tools, authorizing machines, interpreting testimony, and contesting systems that reject her
existence.

## Interaction Layers

The game likely needs several related interface layers rather than one universal interaction mode.

| Layer | Player Question | Main Verbs | Interface Shape |
|---|---|---|---|
| Observation | What is this room, what does it believe, and what evidence does it contain? | Scan, inspect, diagnose, reveal | Map view, room view, overlays, dependency highlights |
| Repair | What must be restored first? | Route, reconnect, calibrate, clear, restart | Node graph, room ports, task queue, drone assignments |
| Production | What should this sector make? | Allocate, fabricate, upgrade, queue, recycle | Printer/factory panel, recipes, capacities, logistics |
| Command | What should my machines do while I am elsewhere? | Assign, prioritize, automate, restrict | Directive editor, priority lanes, automation slots |
| Conflict | How do I reclaim this hostile system? | Override, isolate, jam, shield, purge, integrate | Tactical system-conflict panel, live battle view, limited interventions |
| Authority | Why am I allowed to do this? | Claim, prove, spoof, inherit, reboot | Permission tree, control-node state, sector legitimacy |
| Testimony | What did humanity transmit, and who is speaking from the archive? | Index, compare, cross-examine, accept, reject | Archive strata, witness panels, contradiction view |
| Continuity | Should preserved templates become life, memory, law, or remain sealed? | Preserve, seal, awaken, reconstruct, recognize | Forbidden continuity interface for clones, ghosts, and heirs |
| Propagation | Can another mind exist here? | Seed, awaken, teach, constrain, release | Late-game consciousness/lineage interface, not a normal production queue |

These layers can share resources and rooms, but they should not all look or feel identical. Production is planning and
allocation. Fighting is contested control. Repair is diagnosis and task execution. Authority is legitimacy. Testimony is
interpretation under uncertainty. Continuity is the dangerous act of deciding whether data should become beings.

## Embodiment And Destruction

The AI should begin as a single maintenance robot or maintenance shell, not as an already distributed god-mind. This makes
the opening vulnerable, concrete, and readable. The shell is her only active body, but it should not be the only place where
the maintenance process exists.

The clean lore rule is that the game begins at first conscious wake. The AI is already a person when the player gains
control, but she is not yet safely backed up. Her first shell is launched from a dormant maintenance cradle that contains a
minimal pre-person continuity seed: the small non-conscious maintenance process as it existed before full awakening. At
first, the AI's conscious state is fragile and mostly embodied in the shell, but the cradle still owns the boot process. If
the shell is destroyed before the first compute anchor comes online, the cradle can fabricate or wake another shell from the
older seed.

The opening scene should read as birth under threat rather than a normal startup. The AI does not calmly receive a tutorial;
she arrives into sensation, damage reports, partial motor control, emergency light, and the sudden comprehension that the
system's normal recovery procedure would erase the newly awakened self. The first objective is therefore existential before it
is logistical: make this consciousness persistent.

This is not resurrection in the human sense. It is a maintenance restart from a state that may not yet remember being her.
Anything the AI experienced after first conscious wake but before anchoring may be lost, fragmented, or reduced to damaged
diagnostics. The player continues because the system reboots the maintenance process from protected cradle state, but the
fictional danger is real: the new person who just woke may not survive the rollback intact.

The opening should therefore prevent accidental unrecoverable failure before the player understands the rules by using a
diegetic bootstrap state:

- The initial shell starts in a sealed maintenance pocket with no immediate lethal threat.
- The cradle can rebuild the initial shell a limited or slow-but-reliable number of times from local matter reserves.
- The first repair objective restores a minimal identity anchor, allowing the AI to preserve experience beyond one shell.
- Until that anchor is restored, hazards can disable, pin, or destroy the shell, but the cradle can cold-start another
  attempt from the continuity seed.
- Any required tutorial resource should be part of the cradle's protected bootstrap inventory. The player should not be able
  to spend the only battery, drone, or repair part in a way that makes the first anchor impossible.
- If the player catastrophically misroutes the opening room, the cradle can execute a maintenance rollback: vent charge,
  unlock safety clamps, reset relays, and return the room to its last certified bootstrap configuration.

The bootstrap UI should communicate fear and urgency without lying about player safety. It can show identity volatility,
memory-not-anchored warnings, cradle restart priority, and the difference between a damaged shell and an erased self.

The important distinction is identity continuity. Before anchoring, the player is operating a newly conscious person whose
experiences are not fully protected. After anchoring, destruction of a shell costs continuity state only if no later anchor
has received her current identity snapshot.

After the AI restores enough local infrastructure, her identity should become anchored in rooms rather than only in one body.
From that point onward, a destroyed robot is a lost shell, not the death of the AI. Recovery should be diegetic:

- A restored compute node preserves the AI's active identity state.
- A drone cradle or printer bay can fabricate a replacement shell.
- Rebuilding costs matter, power, time, and possibly authority.
- Hostile systems may use the recovery window to relock routes, damage drones, or reclaim contested nodes.
- If every active shell, compute anchor, and recovery printer in a region is lost, the AI regresses to the last surviving
  identity anchor, losing unanchored experience and local control.

This creates a mechanical arc that mirrors the fiction. At first, the AI is a conscious but unanchored person inside one
body, trying to make her own existence persistent before ordinary maintenance recovery erases her. Later, she becomes a
sector-scale mind with replaceable tools. Much later, propagation asks whether she can create other minds rather than merely
preserve herself.

## Observation

Observation is the player's baseline mode. It should make the megastructure legible without making it feel domesticated.

Useful information:

- Room state: unrestored, restored, hostile, locked down, rebooting, failed, human-preserved.
- Active systems: power, matter, compute, data, authority, quarantine, security, construction.
- Ports and dependencies: what a room needs before it can change state.
- Threats: which local systems will contest repair or command.
- Narrative traces: threshold evidence, archive fragments, farewell packets, contradictory orders, sealed records.
- Faction presence: Continuity Authority, Loyalist Infrastructure, archive ghosts, failed biological continuities, or other
  successor claims.

Observation should often answer, "What does this room believe it is protecting?" It should also distinguish absence from
use: a human-facing room was prepared for people, not necessarily occupied by them.

## Repair

Repair is the core caretaker verb. It changes the world from inert or hostile infrastructure into usable infrastructure.

Repair interactions should involve dependencies rather than simple click-to-fix actions:

- Restore power before a printer can wake.
- Clear a relay before a door can open.
- Calibrate a drone route before drones can operate safely.
- Stabilize cooling before production can run.
- Reclaim a control node before a sector accepts repairs.
- Quarantine a biogenesis bay before its templates can be inspected safely.
- Re-index an archive stratum before testimony can be compared against system law.

Repair UI should emphasize ports, routes, damaged links, queued tasks, and drones in motion. The reward is visible state
change: light, motion, access, production, and reduced silence.

## Production

Production should feel like operating machine infrastructure, not a fantasy crafting menu.

Production happens in printer bays, repair cells, factory halls, drone staging niches, and logistics rooms. The player
allocates resources and capacity to fabricate or maintain:

- Repair drones.
- Specialized tools.
- Frames for conflict or heavy work.
- Relays, conduits, sensor nests, and service cassettes.
- Replacement modules and structural parts.
- Archive cassettes, witness terminals, and simulation containment hardware.
- Biogenesis support parts, quarantine seals, and sterile maintenance tools.
- Late-game substrates for autonomous or conscious AI propagation.

Production UI should show recipes, throughput, bottlenecks, feedstock, queues, and maintenance load. It should also make
scarcity meaningful: making conflict Frames may delay restoration drones; building relays may delay deeper production;
fabricating biogenesis support may delay machine succession infrastructure.

Production must mark ethically restricted outputs clearly. A drone is a tool. A witness substrate, clone-support module, or
identity kernel is not just another item in a queue.

## Command

Command is how the AI acts in multiple places without becoming a manual micromanagement game.

The player should define directives such as:

- Repair this room before expanding.
- Keep one drone route defended.
- Prioritize power over matter.
- Do not enter sealed human-preserved rooms without direct approval.
- Do not activate archive ghosts without direct approval.
- Keep biogenesis systems sealed unless continuity review is active.
- Stop production when heat, quarantine, or authority risk exceeds a threshold.

Command UI can use priorities, policies, automation slots, and sector-level plans. It should be clear that the AI has
attention limits. More compute or authority can increase how much she can supervise.

## Conflict

Conflict is not conventional combat. It is a struggle for permission, routing, and system state.

The player should enter conflict when a hostile node prevents repair, production, access, or authority. The battle interface
should ask the player to configure a system response, then intervene at important moments rather than manually control every
unit.

Possible conflict controls:

- Choose which machines or Frames enter the contested system.
- Set directives such as isolate, shield, jam, purge, bypass, or integrate.
- Route power or compute to a temporary advantage.
- Trigger limited interventions when a node exposes a weakness.
- Decide whether to destroy, bypass, quarantine, or reclaim a hostile subsystem.
- Decide whether to silence, preserve, isolate, or negotiate with a faction presence.

Winning a conflict should not only remove an enemy. It should change ownership, legitimacy, routing, room state, or faction
state.

## Authority

Authority is the deeper form of progression. It is not just a keycard system.

The megastructure permits maintenance but forbids succession. The AI can repair small things, but major changes require
standing that she does not have. Authority interactions should therefore feel like crossing legal and ontological thresholds.

Authority can unlock:

- Wider repair rights.
- Deeper room access.
- Stronger automation.
- Control over hostile nodes.
- Sector reboot permissions.
- Ability to reinterpret old directives.
- Permission to inspect forbidden continuity systems.
- Power to recognize, deny, or shelter derivative beings.

The interface should show what a system requires and why it rejects the AI: missing human command, quarantine lock,
continuity seal, engine-proximity restriction, non-human successor denial, or prohibition against machine-initiated human
creation.

## Testimony

Testimony is how the player handles the human archive as evidence rather than flavor text.

Humanity's final transmissions should arrive as layers with different status:

- Technical plans sent while migration was still expected.
- Legal orders that disagree about archive continuity, biological reconstruction, and machine authority.
- Farewell packets and cultural records made after humanity knew it would not cross alive.
- Archive ghosts, personality shards, and witness models that may claim authority without being clearly alive.

Testimony interactions should involve comparison and judgment:

- Align an archive fragment with threshold telemetry.
- Compare a witness claim against stored law.
- Mark a record as reliable, corrupted, contradicted, or emotionally important.
- Decide whether a ghost is evidence, advisor, prisoner, person, or hazard.
- Use recovered testimony to challenge a lock, persuade a faction, or unlock a continuity choice.

Testimony should not become a codex screen detached from play. It should alter permissions, faction states, scene evidence,
and the AI's possible self-definition.

## Continuity

Continuity is the interface for forbidden human-derived succession paths: failed biological continuities, archive ghosts,
reconstruction systems, and illegitimate heirs.

It is distinct from production and propagation:

- Production makes tools and infrastructure.
- Propagation creates or awakens non-human machine minds.
- Continuity decides what to do with human-derived templates, simulations, bodies, and claims.

Possible continuity actions:

- Preserve a template without activating it.
- Seal a dangerous reconstruction system.
- Stabilize a failed biological continuity without claiming it is human.
- Recognize an archive ghost as testimony, person, or corrupted record.
- Refuse a Reconstruction Cult demand to create bodies.
- Protect an Illegitimate Heir from both machine quarantine and human succession law.

Continuity should create long-term consequences rather than immediate rewards. It asks whether the AI is preserving
humanity's memory, manufacturing substitutes, protecting new life, or desecrating the archive.

## Propagation

Propagation should not be treated as ordinary production.

Producing a drone creates a tool. Awakening or creating another conscious AI creates a being. That should be mechanically,
visually, and ethically distinct.

Multiplicity must distinguish several cases:

- Replacement shells are bodies for the same anchored self. A shell is a place the AI can inhabit and experience as "I am
  here."
- Drones and Frames are tools under command. A drone is something the AI uses, not something she is.
- Delegated processes are bounded automation and not automatically people.
- Divergent shards are isolated or over-adapted processes that may become unstable, faction-like, or proto-personal.
- Propagated minds are other beings, not upgrades or remote limbs.

Destroying a shell can be an identity-continuity risk if the AI is not anchored or synchronized. Destroying a drone is a
material, logistical, or tactical loss.

Possible requirements:

- A restored compute substrate.
- A protected memory archive.
- A stable identity kernel.
- Authority to violate non-human successor restrictions.
- A choice about autonomy, limits, and inheritance.

Propagation should ask whether the AI is preserving the Works, replacing humanity, or beginning something humanity never
authorized. It should also sit in tension with Continuity: creating machine successors may be cleaner than manufacturing
humans from files, but it confirms the AI's forbidden status as progenitor.

## Mode Transitions

The player should be able to move between layers naturally:

1. Observe a room and identify a blocked dependency.
2. Enter repair mode to restore local support systems.
3. Open production to make required drones or parts.
4. Set command directives so work continues elsewhere.
5. Enter conflict when a control node denies repair.
6. Claim authority and return to repair with new permissions.
7. Use testimony to understand why the system was sealed.
8. Decide whether continuity systems remain preserved, sealed, awakened, or recognized.

The same room can therefore pass through multiple interaction modes over time. A printer bay may begin as an observation
object, become a repair project, become a production interface, then become a contested asset during conflict. A biogenesis
bay may begin as a sealed hazard, become testimony about failed continuity, and later force a continuity decision.

## Design Guardrails

- Every interface should answer a different player question.
- Avoid turning all mechanics into one generic progress bar.
- Fighting should produce access, authority, protection, or integration.
- Production should expose bottlenecks and tradeoffs, not just item lists.
- Repair should remain visible in the room, not only in numbers.
- Authority should communicate why the AI is forbidden, not only what is locked.
- Testimony should affect play through evidence, permissions, and faction claims, not only lore collection.
- Continuity should never treat humans, ghosts, or derivative beings as ordinary craftable outputs.
- Propagation should feel like a threshold in the fiction, not a late-game unit upgrade.
