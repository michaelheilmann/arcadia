      <h2 id="graph-composition">9 Graph composition</h2>
      <p>
      Graph composition is an opt-in source expansion pass over a batch of DDL
      map nodes. It produces ordinary ADL definitions before normal linking; it
      is not a runtime graph interpreter. Callers use
      <code>Arcadia_ADL_Graphs_readFromNodes</code>, not an ordinary asset reader,
      to process the front-end forms below.
      </p>

      <h3>Templates, instances and parameters</h3>
      <p>
      A <code>GraphDefinition</code> has a nonempty <code>name</code> and a
      <code>nodes</code> list of local maps with distinct names. A
      <code>GraphInstance</code> has a nonempty <code>name</code>, a
      <code>graph</code> naming its template, and a <code>bindings</code> list.
      Templates and instances are collected before expansion, so a template need
      not precede its instance in the batch. The name segment
      <code>__graph</code> is reserved for generated names.
      </p>
      <p>
      Each template has exactly one <code>Graph.Output</code> with
      <code>name</code>, <code>referenceType</code> and <code>source</code> strings.
      A <code>Graph.Parameter</code> declares either
      <code>referenceType: "Color"</code> or
      <code>valueType: "Integer32"</code>, not both. Reference bindings use
      <code>{ parameter: "P", source: "Existing.Definition" }</code>; value
      bindings use <code>{ parameter: "P", value: 440 }</code>. Missing,
      duplicate and unknown parameter bindings are rejected. A reference binding
      must name an existing definition; its existence alone does not establish
      that definition's type.
      </p>
      <pre>
[
  {
    type: "GraphDefinition", name: "Templates.Tone",
    nodes: [
      { type: "Graph.Parameter", name: "Frequency", valueType: "Integer32" },
      { type: "DSP.SineWave", name: "tone", frequency: "Frequency" },
      { type: "Graph.Output", name: "output", referenceType: "DSP.SineWave", source: "tone" }
    ]
  },
  {
    type: "GraphInstance", name: "Sounds.Tone", graph: "Templates.Tone",
    bindings: [ { parameter: "Frequency", value: 440 } ]
  }
]
      </pre>
      <p>
      The example generates a sine-wave definition named
      <code>Sounds.Tone.output</code>. It does not itself render audio.
      </p>
      <p>
      A dual-tone template can combine two sine nodes using the existing
      <code>DSP.Mix</code> definition. Its <code>inputs</code> is a nonempty,
      ordered list of local DSP names; each target must be a
      <code>DSP.SineWave</code> or another <code>DSP.Mix</code>.
      Repeated references remain repeated entries, not a deduplicated input list.
      </p>
      <pre>
[
  {
    type: "GraphDefinition", name: "Templates.DualTone",
    nodes: [
      { type: "Graph.Parameter", name: "Low", valueType: "Integer32" },
      { type: "Graph.Parameter", name: "High", valueType: "Integer32" },
      { type: "DSP.SineWave", name: "low", frequency: "Low" },
      { type: "DSP.SineWave", name: "high", frequency: "High" },
      { type: "DSP.Mix", name: "mix", inputs: ["high", "low"] },
      { type: "Graph.Output", name: "output", referenceType: "DSP.Mix", source: "mix" }
    ]
  },
  {
    type: "GraphInstance", name: "Sounds.DualTone", graph: "Templates.DualTone",
    bindings: [ { parameter: "Low", value: 220 }, { parameter: "High", value: 880 } ]
  },
  { type: "SampleBuffer", name: "Sounds.Buffer", duration: 1, dsp: "Sounds.DualTone.output" }
]
      </pre>
      <p>
      This generates <code>Sounds.DualTone.__graph.high</code>,
      <code>Sounds.DualTone.__graph.low</code>, and one published mix at
      <code>Sounds.DualTone.output</code>. Linking the ordinary
      <code>SampleBuffer.dsp</code> reference resolves that published mix.
      Separate instances receive distinct generated definitions. Local declaration
      order does not change generated names or the order of mix input references.
      This is source expansion into the existing native ADL/Media DSP path, not
      new sample-processing functionality or a runtime graph interpreter.
      </p>

      <h3>Current supported surface</h3>
      <table>
        <thead><tr><th>Category</th><th>Accepted forms</th></tr></thead>
        <tbody>
          <tr><td>Ordinary graph-local nodes</td><td><code>Color</code>, <code>PixelBuffer</code>, <code>PixelBufferOperations.FillOperation</code>, <code>PixelBufferOperations.CheckerboardFillOperation</code>, <code>DSP.SineWave</code>, <code>DSP.Mix</code></td></tr>
          <tr><td>Output referenceType</td><td><code>PixelBuffer</code>, <code>DSP.SineWave</code> or <code>DSP.Mix</code></td></tr>
          <tr><td>Reference-field rewriting</td><td>CheckerboardFillOperation's <code>firstCheckerColor</code>/<code>secondCheckerColor</code>; PixelBuffer's <code>operations</code> list; DSP.Mix's ordered <code>inputs</code> list</td></tr>
          <tr><td>Value-field substitution</td><td><code>DSP.SineWave.frequency</code> from an Integer32 parameter</td></tr>
        </tbody>
      </table>
      <p>
      Other string fields are not automatically treated as graph references.
      Sine frequency bindings must be Integer32 numbers and must not be negative;
      zero passes this expansion check, without guaranteeing that downstream DSP
      construction accepts it. Ordinary reader support for <code>DSP.Gain</code>
      or <code>DSP.LowPass</code> does not make those types valid graph-local
      nodes. DSP reference parameters are not supported: reference parameters
      remain Color-only. Multiple outputs, nested graph instances, general
      value types and parameter defaults are not supported by this front end.
      </p>

      <h3>Generated names and processing order</h3>
      <p>
      For instance <code>I</code>, an internal node <code>L</code> is named
      <code>I.__graph.L</code>. The output's source is instead named
      <code>I.outputName</code>; it is not also emitted under its internal name.
      Parameter and output declaration nodes are not ordinary asset definitions
      and do not receive reader registrations. The caller links the resulting
      definitions with <code>Arcadia_ADL_Definitions_link</code> afterward.
      </p>
      <p>
      Ordinary batch definitions are read first, allowing bindings to refer to
      them. All instance bindings and generated nodes are then validated/staged
      before generated-node reading starts. Publication reads those nodes one at
      a time. This is not a whole-batch transaction: an error can leave ordinary
      definitions or an already-read prefix of generated definitions in the
      destination. No rollback guarantee for reader or allocation failures is
      established by staging alone.
      </p>

      <h3>Validation limits</h3>
      <p>
      Output referenceType is allow-listed but is not cross-checked against its
      source's type. The collector rejects unresolved names in its registered
      reference fields, references to Graph.Output, incompatible reference
      families, and cycles among local reference edges with
      <code>Arcadia_Status_SemanticalError</code>. Mix inputs must target supported
      local DSP nodes, not parameters or external definition names. These checks
      cover the registered fields above, not every string field of every asset:
      in particular, FillOperation has no registered reference-field descriptor.
      Ordinary readers and linking retain their own validation responsibilities.
      </p>
      <p>
      Graph composition does not specify clipping/normalization policy, rendered
      sample equivalence or cross-platform bit-identical audio. Those require
      separate decisions and runtime verification.
      </p>
