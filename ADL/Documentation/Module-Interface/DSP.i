<section class="cxx entity class">
<h1 id="Arcadia_ADL_ConstantDefinition">Arcadia_ADL_ConstantDefinition</h1>
<my-signature><code>
Arcadia_ADL_ConstantDefinition
</code></my-signature>

<my-summary>
The definition of a digital signal processing (DSP) generating a constant signal.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"DSP.Constant"</code>.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_ConstantDefinition_create">Arcadia_ADL_ConstantDefinition_create</h1>
<my-signature><code>
Arcadia_ADL_ConstantDefinition* Arcadia_ADL_ConstantDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_Real32Value value)
</code></my-signature>

<my-summary>
Create the definition of a constant-signal DSP.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>definitions</div>
  <div>The container the definition belongs to.</div>
</div>
<div>
  <div>name</div>
  <div>The name of the definition.</div>
</div>
<div>
  <div>value</div>
  <div>The constant value of the signal.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the constant-signal DSP.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

</section>

</section>

<section class="cxx entity class">
<h1 id="Arcadia_ADL_SineWaveDefinition">Arcadia_ADL_SineWaveDefinition</h1>
<my-signature><code>
Arcadia_ADL_SineWaveDefinition
</code></my-signature>

<my-summary>
The definition of a digital signal processing (DSP) generating a sine wave signal of a constant frequency.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"DSP.SineWave"</code>.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_SineWaveDefinition_create">Arcadia_ADL_SineWaveDefinition_create</h1>
<my-signature><code>
Arcadia_ADL_SineWaveDefinition* Arcadia_ADL_SineWaveDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_Integer32Value frequency)
</code></my-signature>

<my-summary>
Create the definition of a sine wave DSP.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>definitions</div>
  <div>The container the definition belongs to.</div>
</div>
<div>
  <div>name</div>
  <div>The name of the definition.</div>
</div>
<div>
  <div>frequency</div>
  <div>The frequency, in Hertz, of the sine wave.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the sine wave DSP.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

</section>

</section>

<section class="cxx entity class">
<h1 id="Arcadia_ADL_SawtoothWaveDefinition">Arcadia_ADL_SawtoothWaveDefinition</h1>
<my-signature><code>
Arcadia_ADL_SawtoothWaveDefinition
</code></my-signature>

<my-summary>
The definition of a digital signal processing (DSP) generating a sawtooth wave signal of a constant frequency.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"DSP.SawtoothWave"</code>.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_SawtoothWaveDefinition_create">Arcadia_ADL_SawtoothWaveDefinition_create</h1>
<my-signature><code>
Arcadia_ADL_SawtoothWaveDefinition* Arcadia_ADL_SawtoothWaveDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_Integer32Value frequency)
</code></my-signature>

<my-summary>
Create the definition of a sawtooth wave DSP.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>definitions</div>
  <div>The container the definition belongs to.</div>
</div>
<div>
  <div>name</div>
  <div>The name of the definition.</div>
</div>
<div>
  <div>frequency</div>
  <div>The frequency, in Hertz, of the sawtooth wave.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the sawtooth wave DSP.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

</section>

</section>

<section class="cxx entity class">
<h1 id="Arcadia_ADL_WhiteNoiseDefinition">Arcadia_ADL_WhiteNoiseDefinition</h1>
<my-signature><code>
Arcadia_ADL_WhiteNoiseDefinition
</code></my-signature>

<my-summary>
The definition of a digital signal processing (DSP) generating a white noise signal.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"DSP.WhiteNoise"</code>.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_WhiteNoiseDefinition_create">Arcadia_ADL_WhiteNoiseDefinition_create</h1>
<my-signature><code>
Arcadia_ADL_WhiteNoiseDefinition* Arcadia_ADL_WhiteNoiseDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name)
</code></my-signature>

<my-summary>
Create the definition of a white noise DSP.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>definitions</div>
  <div>The container the definition belongs to.</div>
</div>
<div>
  <div>name</div>
  <div>The name of the definition.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the white noise DSP.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

</section>

</section>