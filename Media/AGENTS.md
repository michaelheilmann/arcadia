# Media Maintenance Notes

## Scope And Migration Boundaries

- Public consumers use `Arcadia/Media/Include.h`; preserve defining-module aggregate dependencies on ADL, Collections and Math. Native imports do not transfer implementation ownership. Follow `Engine/AGENTS.md` for per-class header/implementation migration gates and `AGENTS/Testing.md` for tests.
- Media DSP is excluded from current migration scope: base/node/Gain/Graph/Buffer scaffolding, storage and sample algorithms require separate maintainer authorization. ADL DSP definition/configuration objects are not Media DSP implementation ports.
- PixelBuffer, SampleBuffer, DSP.Buffer and VertexBuffer storage/destructors/pointer APIs, Quantization, pixel/sample loops and mesh-generation algorithms remain native behind ownership, executable-equivalence and performance gates. Do not transport raw pointers through Arcadia.Value or replace hot loops with scalar boxing.
- DSP's ownerGraph back-reference has no visitor; an ordinary generated managed field would change lifetime semantics. Do not make it strong as a scaffolding cleanup. Graph also requires membership/pointer identity, publication/rollback, ordered reset/reseed and exact seed derivation audits.

## Geometry And Descriptor Compatibility

- Geometry.pdl is currently opaque-only; its presence is not a completed scaffolding migration. Further porting requires separate authorization. Preserve vertexBuffer/ambientColor field order, factory/constructor/visitor ABI and the entire public header surface, including createFromMeshGenerator.
- getAmbientColor returns the stored object; getVertexBuffer allocates an independent mutable vertex copy sharing the descriptor. Never model the latter as a field getter. Keep clone/evaluation and raw copying behind native boundaries, including GC rooting and failures.
- Geometry's public factory boxes null as void, but its runtime constructor uses checked object-reference reads. Those branches do not establish a nullable constructor contract; verify actual status/tag behavior before modeling it.
- VertexDescriptor.pdl owns declarations only (`native implementation`). Keep VertexDescriptor.c and its module-private factory native. Current generated concrete classes expose a public factory; declaring this descriptor abstract or hand-stripping generated declarations does not model its concrete/private-factory contract.
- Preserve legacy constructor List/ArrayList transport despite the public ImmutableList field/getter declaration. Generated ImmutableList checks at parameter/field assignment would narrow accepted native inputs; a wider List parameter alone does not remove the field-store check. Keep full-width stride and independent Natural32 vertexSize wrapping.
- VertexElementDescriptor's runtime name is Arcadia.Engine.Visuals.VertexElementDescriptor while its C name is Arcadia_Media_VertexElementDescriptor. Its public factory offset is Natural8; runtime constructor/storage/getter use Natural32. Preserve Integer32 runtime enum transport and Object's value-stack equality override; scalar-only layout is not evidence of executable migration readiness.
- VertexElementSemantics and VertexElementSyntactics are PDL-owned enums with ordinary .h/.c output. Preserve semantics 1/2/3 and syntactics 1/2/3/4. Descriptor storage/implementation ownership is a separate decision.

## Regression Coverage

- Geometry/VertexEnums checks enum ABI/descriptors/boxing and native transport; VertexBuilder checks offsets, sizes/stride, snapshot retention, legacy descriptor transport and GC. Keep both: builder consumer coverage alone is not enum registration coverage.
- Completed scaffolding migrations retain ordinary production-linked cases in existing Geometry/PixelBuffer/SampleBuffer suites; no permanent comparison triples or per-type executables. Native helpers/consumer probes are intentional.
- PixelBuffer selectors include Operations, Fills, FormatMetadata, FormatQueries, FormatConsumers, Resize and BulkResize. Formats and the assertion-free EmptyBoundary are retired; SampleBuffer's Formats selector is still valid. Do not resurrect no-op aliases.
- Keep same-format row-span resizing and padded growth/shrink/alpha/default-area regressions. Do not restore per-pixel copying through generated format queries; that caused first-use texture stalls. Pixel/sample storage and conversion loops are not migrated by format metadata or operation-base adoption.
