<section class="cxx entity class">
<h1 id="Arcadia_ADL_StreamingSoundSourceNodeDefinition">Arcadia_ADL_StreamingSoundSourceNodeDefinition</h1>
<my-signature><code>Arcadia_ADL_StreamingSoundSourceNodeDefinition</code></my-signature>
<my-summary>A runtime streaming sound-source description.</my-summary>
<my-remarks>
<p>The ADL type name is <code>StreamingSoundSourceNode</code>. It references a
provider definition and has optional <code>volume</code> (default
<code>1.0</code>). Playback ends when the provider reaches end-of-stream or the
runtime node is stopped.</p>
</my-remarks>
<section class="cxx methods">
<h1>Methods</h1>
<section class="cxx method">
<h1 id="Arcadia_ADL_StreamingSoundSourceNodeDefinition_create">Arcadia_ADL_StreamingSoundSourceNodeDefinition_create</h1>
<my-signature><code>Arcadia_ADL_StreamingSoundSourceNodeDefinition* Arcadia_ADL_StreamingSoundSourceNodeDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Reference* provider,
&nbsp;&nbsp;&nbsp;Arcadia_Real32Value volume)</code></my-signature>
<my-summary>Create a streaming sound-source definition.</my-summary>
<p><code>provider</code> references the stream provider used by the runtime node.</p>
</section>
</section>
</section>
