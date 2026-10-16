<section class="cxx entity procedure">
<h1 id="Arcadia_ADL_Graphs_readFromNodes">Arcadia_ADL_Graphs_readFromNodes</h1>
<my-signature><code>
void Arcadia_ADL_Graphs_readFromNodes<br>
&nbsp;&nbsp;(Arcadia_Thread* thread,<br>
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Context* context,<br>
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,<br>
&nbsp;&nbsp;&nbsp;Arcadia_List* nodes)
</code></my-signature>
<my-summary>
Collect graph templates and instances from a batch of DDL root maps and expand
them into ordinary ADL definitions. Include <code>Arcadia/ADL/Include.h</code>.
</my-summary>
<section class="cxx parameters">
<h1>Parameters</h1>
<div><div>thread</div><div>The calling runtime thread.</div></div>
<div><div>context</div><div>The ADL context used to read ordinary batch definitions.</div></div>
<div><div>definitions</div><div>A fresh destination definitions container.</div></div>
<div><div>nodes</div><div>An ordered list of DDL map-node object references, including ordinary assets and graph front-end forms.</div></div>
</section>
<my-remarks>
<p>
The function does not link or render the resulting definitions. Call
<code>Arcadia_ADL_Definitions_link</code> after successful expansion. Generated
nodes are currently read using <code>Arcadia_ADL_Context_getOrCreate</code> in the
expander; the explicit context argument is used for ordinary batch nodes.
</p>
<p>
Graph shape/binding errors raise <code>Arcadia_Status_SemanticalError</code>;
allocation and ordinary reader/link operations retain their own error behavior.
Do not assume the destination is unchanged on failure: ordinary definitions are
read before instance staging, and generated definitions are published individually.
</p>
<p>
Supported local audio nodes are <code>DSP.SineWave</code> and <code>DSP.Mix</code>.
Mix's required nonempty <code>inputs</code> list is rewritten in its original
order to generated local names, preserving repeated references. Each input must
name a supported local DSP node; unresolved names, wrong families, references to
<code>Graph.Output</code>, and local cycles raise
<code>Arcadia_Status_SemanticalError</code>. A mix can be published as the single
<code>Graph.Output</code> with <code>referenceType: "DSP.Mix"</code> and consumed
by ordinary <code>SampleBuffer.dsp</code> after linking. Reference parameters
remain Color-only; no output/source type-equality check is added.
</p>
<p>
See <a href="@{siteAddress}/Arcadia/ADL/Specification/#graph-composition">Graph
composition</a> for accepted local types, parameter bindings, generated names and
the current reference-validation limitations. That section describes the current
source implementation, not a broader historical graph-composition proposal.
</p>
</my-remarks>
</section>
