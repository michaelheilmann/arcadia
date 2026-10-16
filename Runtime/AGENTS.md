# Runtime Invariants

- `Runtime/ARMS`, `Runtime/Ring1`, `Runtime/Collections`, and `Runtime/FileSystem` must remain hand-written C: do not port them to PDL or regenerate their production headers/bodies with PDLC. The full protected set, test/import exceptions and explicit-approval requirement are defined by the [root native-C invariant](../AGENTS.md#start-here).
- Collections' opaque native imports retain defining-module ownership; do not duplicate them in dependent modules or generate collection implementations. Existing native build/configuration generation is not a production PDL migration.
- Follow the root `AGENTS.md` and any module-specific instructions for builds, source style, testing, and version control.
