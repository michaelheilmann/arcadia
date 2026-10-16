<section class="cxx entity class">
<h1 id="Arcadia_ADL_DSPStreamProviderDefinition">Arcadia_ADL_DSPStreamProviderDefinition</h1>
<my-signature><code>Arcadia_ADL_DSPStreamProviderDefinition</code></my-signature>
<my-summary>A DSP-backed stream-provider description.</my-summary>
<my-remarks>
<p>The ADL type name is <code>DSPStreamProvider</code>. It references a DSP
definition and specifies a sample rate. Its optional duration is in seconds;
absence means that the provider is infinite. The Engine audials node factory
creates the runtime DSP graph and provider from this linked definition.</p>
</my-remarks>
<section class="cxx methods">
<h1>Methods</h1>
<section class="cxx method">
<h1 id="Arcadia_ADL_DSPStreamProviderDefinition_create">Arcadia_ADL_DSPStreamProviderDefinition_create</h1>
<my-signature><code>Arcadia_ADL_DSPStreamProviderDefinition* Arcadia_ADL_DSPStreamProviderDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Reference* dsp,
&nbsp;&nbsp;&nbsp;Arcadia_Natural32Value sampleRate,
&nbsp;&nbsp;&nbsp;Arcadia_BooleanValue hasDuration,
&nbsp;&nbsp;&nbsp;Arcadia_Natural32Value duration)</code></my-signature>
<my-summary>Create a DSP stream-provider definition.</my-summary>
<p><code>duration</code> is meaningful only when <code>hasDuration</code> is true; its unit is seconds.</p>
</section>
</section>
</section>
