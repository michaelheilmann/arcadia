<section class="cxx entity class">
<h1 id="Arcadia_Media_DSP_Mix">Arcadia_Media_DSP_Mix</h1>
<my-signature><code>Arcadia_Media_DSP_Mix</code></my-signature>
<my-summary>A native floating-point DSP mixer with ordered, unity-gain summation.</my-summary>
<my-remarks>
<p>
<code>Arcadia_Media_DSP_Mix_create(thread)</code> creates an empty mixer.
<code>Arcadia_Media_DSP_Mix_addInput(thread, mix, input)</code> appends a non-null
DSP input; a null input raises <code>Arcadia_Status_ArgumentValueInvalid</code>.
Rendering overwrites the target with silence, then renders and adds each input
in its stored order. With no inputs it emits zero-valued samples; with one input its
sample values pass through at unity gain. Duplicate entries are rendered and
added individually, not deduplicated or cached. A stateful input may advance on
each render, so duplicate entries do not promise identical source blocks.
</p>
<p>
Mix does not divide by the input count, normalize to the peak, clamp, compress,
limit or compensate gain. Two inputs each producing 0.75 produce 1.5; values
outside [-1, +1] remain valid inside the floating-point graph. Output/sample-format
conversion is a separate concern, as would be any future optional limiter node.
This contract does not introduce a general clipping policy for those consumers.
</p>
<p>
The ADL <code>DSP.Mix</code> asset reader and graph-local composition require a
nonempty input list; native zero-input silence does not relax that source rule.
Input ordering is preserved; floating-point sums need not be invariant under
reordering or bit-identical across platforms.
</p>
</my-remarks>
</section>
