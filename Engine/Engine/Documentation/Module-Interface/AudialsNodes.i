<section class="cxx entity">
<h1 id="audials-sound-source-nodes">Audials sound-source nodes</h1>
<p>
<code>Arcadia_Engine_Audials_SoundSourceNode</code> plays a linked, finite
<code>Arcadia_ADL_SampleBufferDefinition</code>. It supports looping and a
per-source volume in <code>[0,1]</code>.
</p>
<p>
<code>Arcadia_Engine_Audials_StreamingSoundSourceNode</code> pulls successive
chunks from an <code>Arcadia_Engine_Audials_AudioStreamProvider</code>. A finite
provider ends naturally; an infinite provider continues until the node is
stopped. Pausing suspends playback without advancing the provider. Stop discards
queued data and does not rewind the provider.
</p>
<p>
The audials <code>NodeFactory</code> constructs these runtime nodes from linked
ADL descriptions. For a streaming node it also constructs the referenced
DSP-backed provider. Render a node before invoking its playback controls.
</p>
<p>
Both nodes expose <code>isPlaying</code>, <code>play</code>,
<code>pause</code>, <code>stop</code>, and <code>setVolume</code> controls.
The finite-buffer node additionally exposes <code>getVolume</code>,
<code>setIsLooping</code>, and <code>getIsLooping</code>. Source volume is
clamped to <code>[0,1]</code>.
</p>
<pre><code>
Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeFromDefinition
  (Arcadia_Thread*, Arcadia_Engine_Audials_NodeFactory*,
   Arcadia_Engine_Audials_BackendContext*, Arcadia_ADL_SoundSourceNodeDefinition*);

Arcadia_Engine_Audials_StreamingSoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinition
  (Arcadia_Thread*, Arcadia_Engine_Audials_NodeFactory*,
   Arcadia_Engine_Audials_BackendContext*, Arcadia_ADL_StreamingSoundSourceNodeDefinition*);
</code></pre>
</section>
