<section class="cxx entity class">
<h1 id="Arcadia_ADL_SoundSourceNodeDefinition">Arcadia_ADL_SoundSourceNodeDefinition</h1>
<my-signature><code>Arcadia_ADL_SoundSourceNodeDefinition</code></my-signature>
<my-summary>A runtime finite-buffer sound source description.</my-summary>
<my-remarks>
<p>The ADL type name is <code>SoundSourceNode</code>. It references a
<code>SampleBuffer</code> and has optional <code>isLooping</code> (default
<code>false</code>) and <code>volume</code> (default <code>1.0</code>) entries.
The Engine audials node factory turns the linked definition into a runtime
<code>SoundSourceNode</code>.</p>
</my-remarks>
<section class="cxx methods">
<h1>Methods</h1>
<section class="cxx method">
<h1 id="Arcadia_ADL_SoundSourceNodeDefinition_create">Arcadia_ADL_SoundSourceNodeDefinition_create</h1>
<my-signature><code>Arcadia_ADL_SoundSourceNodeDefinition* Arcadia_ADL_SoundSourceNodeDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Reference* sampleBuffer,
&nbsp;&nbsp;&nbsp;Arcadia_BooleanValue isLooping,
&nbsp;&nbsp;&nbsp;Arcadia_Real32Value volume)</code></my-signature>
<my-summary>Create a finite-buffer sound-source definition.</my-summary>
<p><code>sampleBuffer</code> references the finite buffer to play. <code>isLooping</code> and
<code>volume</code> are the node settings represented in the ADL description.</p>
</section>
</section>
</section>
