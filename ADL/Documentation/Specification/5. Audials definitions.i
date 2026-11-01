      <h2 id="audials">5 Audials definitions</h2>
      <p>
      The audials definitions describe sample buffers and the digital signal
      processing definitions that synthesize them, along with finite and
      streaming sound-source nodes.
      </p>

      <h3>SampleBuffer</h3>
      <p>Type name: <code>"SampleBuffer"</code>. A sample buffer synthesized by a digital signal processing definition.</p>
      <pre>
"type"     : "SampleBuffer"
"name"     : &lt;string&gt;
"duration" : &lt;number&gt;   // Natural32, the duration in seconds
"dsp"      : &lt;string&gt;   // reference to a DSP definition
      </pre>
      <p>
      While linking, the <code>dsp</code> reference is resolved and is linked
      recursively.
      </p>

      <h3>DSPStreamProvider</h3>
      <p>Type name: <code>"DSPStreamProvider"</code>. An incrementally rendered DSP source.</p>
      <pre>
"type"       : "DSPStreamProvider"
"name"       : &lt;string&gt;
"dsp"        : &lt;string&gt;   // reference to a DSP definition
"sampleRate" : &lt;number&gt;   // Natural32, frames per second
"duration"   : &lt;number&gt;   // optional Natural32, seconds; absent means infinite
      </pre>
      <p>
      When present, <code>duration</code> limits the provider to that many
      seconds of frames at <code>sampleRate</code>. When absent, the provider
      keeps producing frames until its consumer is stopped. The <code>dsp</code>
      reference is resolved and linked recursively.
      </p>

      <h3>SoundSourceNode</h3>
      <p>Type name: <code>"SoundSourceNode"</code>. A runtime node playing a pre-rendered sample buffer.</p>
      <pre>
"type"         : "SoundSourceNode"
"name"         : &lt;string&gt;
"sampleBuffer" : &lt;string&gt;   // reference to a SampleBuffer definition
"isLooping"    : &lt;boolean&gt;  // optional, default false
"volume"       : &lt;number&gt;   // optional Real32, default 1.0
      </pre>
      <p>The <code>sampleBuffer</code> reference is resolved and linked recursively.</p>

      <h3>StreamingSoundSourceNode</h3>
      <p>Type name: <code>"StreamingSoundSourceNode"</code>. A runtime node pulling audio chunks from a provider.</p>
      <pre>
"type"     : "StreamingSoundSourceNode"
"name"     : &lt;string&gt;
"provider" : &lt;string&gt;   // reference to a DSPStreamProvider definition
"volume"   : &lt;number&gt;   // optional Real32, default 1.0
      </pre>
      <p>
      The provider reference is resolved and linked recursively. The node plays
      until its provider reaches end-of-stream or playback is stopped; a provider
      with no duration does not reach end-of-stream on its own.
      </p>

      <h3>Digital signal processing definitions</h3>
      <p>
      A digital signal processing definition describes a signal. It is referenced by
      the <code>dsp</code> entry of a <code>"SampleBuffer"</code> definition.
      </p>
      <p>Type name: <code>"DSP.Constant"</code>. A constant signal.</p>
      <pre>
"type"  : "DSP.Constant"
"name"  : &lt;string&gt;
"value" : &lt;number&gt;   // Real32
      </pre>
      <p>Type name: <code>"DSP.SineWave"</code>. A sine wave.</p>
      <pre>
"type"      : "DSP.SineWave"
"name"      : &lt;string&gt;
"frequency" : &lt;number&gt;   // strictly positive Integer32, frequency in hertz
      </pre>
      <p>Type name: <code>"DSP.SawtoothWave"</code>. A sawtooth wave.</p>
      <pre>
"type"      : "DSP.SawtoothWave"
"name"      : &lt;string&gt;
"frequency" : &lt;number&gt;   // strictly positive Integer32, frequency in hertz
      </pre>
      <p>
      Both oscillator asset readers require frequency in <code>1..INT32_MAX</code>.
      Zero is not silence and negative values do not request reversed phase:
      nonpositive Integer32 frequencies raise <code>Arcadia_Status_SemanticalError</code>
      during ADL reading, before definition construction or Engine DSP creation.
      This also applies to generated definitions read after graph expansion.
      The class constructors/factories still represent the full signed Integer32
      domain; reader validation does not change their ABI or native Media's
      separate nonpositive-frequency <code>ArgumentValueInvalid</code> guard.
      </p>
      <p>Type name: <code>"DSP.WhiteNoise"</code>. White noise. Has no extra entries.</p>
      <pre>
"type" : "DSP.WhiteNoise"
"name" : &lt;string&gt;
      </pre>

      <h3>DSP.Gain, DSP.LowPass and DSP.Mix</h3>
      <p>
      These ordinary assets share one operator definition and reader. Each requires
      string <code>type</code> and <code>name</code> entries. Gain requires a string
      <code>input</code> reference and numeric <code>gain</code>; LowPass requires
      a string <code>input</code> reference and numeric <code>cutoff</code>. Mix
      requires a nonempty <code>inputs</code> list of string references, preserving
      order and duplicates. Entries belonging to another variant, unknown entries,
      duplicate entries and wrong entry types are semantical errors.
      </p>
      <pre>
{ type: "DSP.Gain", name: "Quiet", input: "Tone", gain: 0.25 }
{ type: "DSP.LowPass", name: "Filtered", input: "Quiet", cutoff: 850 }
{ type: "DSP.Mix", name: "Combined", inputs: ["Tone", "Filtered"] }
      </pre>
      <p>
      Mix uses unnormalized unity-gain summation in the defined input order.
      It does not divide by the input count, normalize to a peak, clamp, compress,
      limit or apply automatic gain compensation. One input passes through;
      duplicate entries are rendered and summed individually. Two inputs each
      producing 0.75 produce 1.5; samples outside [-1, +1] are valid inside the
      floating-point graph. Native Media Mix with zero inputs emits silence,
      but the ADL asset still requires a nonempty inputs list. Output conversion
      and any future optional gain/limiter node are separate concerns.
      </p>
      <p>
      DSP repeatability is scoped to identical state, topology, inputs, seeds,
      sample rate/configuration and execution environment (binary, compiler/FP
      settings and runtime). Fresh deterministic renders must match sample bit
      patterns, not just numerically equal promoted values. Supported streaming
      topologies preserve their sequence across chunk partitions. Cross-platform
      numerical equivalence requires operator-specific reference tolerances and
      actual platform gates; cross-platform bitwise identity is not promised.
      Non-finite inputs and NaN payload behavior need explicit operator policy.
      Native shared-state rendering/seed/reset semantics and tested topology
      limits are documented in the Media DSP reproducibility profile; ADL source
      expansion does not add a runtime scheduling/cache mechanism.
      </p>
      <p>
      Parameters are converted to Real32, without reader-side positivity or
      clamping restrictions. Zero gain is valid; cutoff positivity is checked
      later by the native Media LowPass constructor. References need not exist
      while reading; unresolved names fail during linking with NotFound. Ordinary
      operator linking resolves references recursively but does not validate their
      DSP family; unsupported DSP definitions are rejected later by the Engine DSP
      factory. This does not grant graph-local Gain/LowPass support or change the
      separate graph-local Mix validation rules.
      </p>
