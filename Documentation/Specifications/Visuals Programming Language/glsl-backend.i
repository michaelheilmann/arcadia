      <h2 id="glsl-backend">12. GLSL 420 core backend</h2>
      <p><code>Arcadia_VPL_Backends_GLSL_compile(thread, publishedModule, diagnostics, version)</code>
      materializes and verifies an independent snapshot and returns a <code>ProgramArtifact</code>.
      The implemented configuration is version 420 core; another version is a target error.
      The backend receives no legacy expression AST or symbol graph and has no caller-owned mapping
      or fragment-name out-parameters.</p>

      <h3 id="glsl-artifact">12.1 Artifact and bindings</h3>
      <p>The artifact owns immutable vertexCode and fragmentCode strings, uniform bindings,
      vertexInterface and fragmentInterface bindings, version, and the resolved primary
      fragmentColorOutput and fragmentColorLocation. Code strings contain no extra terminator;
      GL consumers pass their explicit byte lengths. Binding glName strings include one trailing
      zero byte for native GL name queries.</p>
      <p>Each binding records its logical name, exact GLSL name, type, source-list slot, and resolved
      location. Uniform bindings additionally record the canonical uniform kind and a blockBinding
      point (-1 for non-blocks). Interface bindings record direction and semantics; their uniform
      kind/block fields are not meaningful.</p>
      <p>Record uniforms become std140 interface blocks. Their fields keep canonical declaration
      order and are named vpl_m0, vpl_m1, and so on; nested records use deterministic vpl_R names.
      Block names are vpl_Block followed by source uniform slot, and instance names vpl_u followed
      by slot. Block binding points are assigned in record-uniform order, independently of driver
      optimization. Other uniforms use vpl_u names and preserve declared array lengths. Reflection
      describes root declarations, not a table of every active leaf location; array/record uploads
      may query selectors such as [0].vpl_m0 using the reflected type.</p>

      <h3 id="glsl-locations">12.2 Names and interface locations</h3>
      <p>Explicit interface locations are preserved. Unspecified locations are allocated in source
      order, avoiding explicit and previously assigned ranges within each direction. Linked fragment
      inputs inherit the resolved vertex-output location. Matrices consume one location per column;
      arrays and records consume their recursive spans. Target overlap/range errors are rejected.</p>
      <p>Vertex inputs use vpl_attribute names, linked varyings vpl_varying names, and fragment
      outputs vpl_output names, suffixed by location. Thus linked declarations use consistent names
      even when their logical names differ. Position maps to gl_Position in vertex output and
      gl_FragCoord in fragment input, with no user declaration. Integer-containing varyings use flat
      interpolation. Canonical names cannot inject GLSL identifiers: emitted functions, registers,
      parameters, records, fields, blocks, and bindings use deterministic generated names.</p>

      <h3 id="glsl-operations">12.3 Operations and control flow</h3>
      <p>Result-producing instructions assign statically typed function-scope register variables.
      Loads, stores, constructors, extraction, calls, comparisons, and arithmetic follow the canonical
      operations without reassociating matrix products. Parameters are invocation-local by-value
      function parameters. Whole block-uniform loads explicitly construct a record value from block
      fields; an interface-block instance is not treated as an ordinary struct value.</p>
      <p>One-block return/discard functions emit directly. General graphs use a function-local
      integer block selector, a while loop, and a switch with one case per block. Branches set the
      selector and leave the switch iteration; returns leave the function, and discard abandons the
      fragment. A default return value in non-void dispatch fallback code is unreachable in the
      verified graph. A main wrapper invokes the explicitly selected canonical entry function.
      Forward declarations permit source-independent helper ordering.</p>
      <p>Finite float literals are emitted with a private C numeric locale and nine significant
      digits, preserving binary32 values and signed zero. Minimum integer spelling avoids an
      out-of-range positive signed literal. Arithmetic and comparisons use GLSL 420 operators.
      Signed add/subtract/multiply therefore wrap to low 32 bits. Runtime conversion, rounding,
      zero signs, subnormal flushing, NaN behavior, and contraction follow GLSL 4.20 core rather
      than an emulated CPU IEEE contract. Cross-driver bit-identical floating arithmetic is not
      promised, and VPL registers do not imply the GLSL precise qualifier.</p>
      <p>Texture emits native texture(sampler, coordinates). Vertex implicit base LOD is zero;
      fragment implicit LOD and addressing/filtering use the driver and external sampler state.
      Dispatch loops do not provide a portable implicit-derivative reconvergence promise. No
      uniformity analysis or explicit gradient/LOD emulation is inserted.</p>

      <h3 id="glsl-capabilities">12.4 Initial target restrictions</h3>
      <ul>
        <li>Empty records, arrays of arrays, aggregate-contained samplers, and array lengths beyond
        signed GL integer range are rejected. Arrays of nonempty records, including record fields
        that are one-dimensional arrays, are supported.</li>
        <li>Boolean/opaque interface types are rejected. Vertex attributes support numeric scalars,
        vectors, and floating matrices. Ordinary varyings support numeric aggregates; fragment
        outputs support numeric scalars/vectors and one-dimensional arrays of those.</li>
        <li>Opaque sampler registers are static aliases rather than assignable GLSL locals. A used
        sampler register has one load/copy/local-store alias definition whose chain ends at a
        sampler uniform or sampler parameter. Multiple definitions, cycles, sampler return values,
        or mutation of sampler parameter storage are target errors. Unused sampler declarations
        do not require definitions.</li>
      </ul>
      <p>These restrictions are checked after target-neutral verification and produce structured
      target diagnostics. Hardware-specific limits and active-resource optimization remain driver
      compile/link checks. The target does not silently emit placeholder code for unsupported types.</p>

      <h3 id="glsl-engine">12.5 Engine integration and verification</h3>
      <p>Programs_create selects one complete checked-in canonical description from
      <code>VPL/Library/Assets/Programs/</code>: mesh-ambient-color.vpl, vertex-ambient-color.vpl,
      texture-ambient-color.vpl, or texture-vertex-ambient-color.vpl. CMake embeds the exact source
      bytes into a private BuiltinPrograms.h in the external build tree. Changes to a program,
      embedding script, or header template regenerate that header and rebuild the library.</p>
      <p>The runtime selects the embedded bytes, reads/binds/verifies them through Text_read with
      the source filename as its diagnostic label, and publishes VerifiedModule. No runtime file
      lookup, working-directory convention, or source-tree availability is required for built-ins.
      Arbitrary external source files still use the ordinary text-reading interface. The embedded
      data is a build product; the .vpl files are the editable source of truth.</p>
      <p>Material nodes and backend resource creation pass VerifiedModule.
      The OpenGL resource compiles one artifact, uploads both code strings with explicit lengths,
      binds the resolved fragment output before linking, and uses artifact reflection for uniform
      blocks and scalar locations. Deterministic block binding points remain available to model
      resources even when a driver optimizes an unused block away.</p>
      <p>Canonical goldens cover all four variants and reflection. The Windows GLSL.Driver test
      creates a hidden 4.2 core context, compiles/links, checks std140 block sizes and output/attribute
      locations, and renders each variant into an RGBA8 framebuffer. Additional loop variants check
      dispatch execution with expected pixels. A general fixture compiles/links record/array
      uniforms, integer-containing varyings, calls, storage selectors, and terminating discard.
      Driver tests report skipped status if no suitable core context is available.</p>
      <p>Numeric/sampling conformance renders a pass color only when native signed wrapping,
      negative integer division, exact runtime conversion, finite arithmetic, and zero comparison
      behave as specified. Sampler cases distinguish repeat from clamp addressing and verify
      fragment implicit mip selection versus vertex base LOD zero using different mip colors.
      Tests do not assert nonportable NaN payloads, zero-sign arithmetic, subnormal retention,
      correctly-rounded shader division, or implicit derivatives in divergent flow.</p>
