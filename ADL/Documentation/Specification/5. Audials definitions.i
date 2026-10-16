      <h2 id="audials">5 Audials definitions</h2>
      <p>
      The audials definitions describe sample buffers and the digital signal
      processing definitions that synthesize them.
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
"frequency" : &lt;number&gt;   // Integer32, the frequency in hertz
      </pre>
      <p>Type name: <code>"DSP.SawtoothWave"</code>. A sawtooth wave.</p>
      <pre>
"type"      : "DSP.SawtoothWave"
"name"      : &lt;string&gt;
"frequency" : &lt;number&gt;   // Integer32, the frequency in hertz
      </pre>
      <p>Type name: <code>"DSP.WhiteNoise"</code>. White noise. Has no extra entries.</p>
      <pre>
"type" : "DSP.WhiteNoise"
"name" : &lt;string&gt;
      </pre>
