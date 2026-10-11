<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity object">

  <h1 id="Arcadia_Media_VertexElementDescriptor">[MIL] Arcadia.Media.VertexElementDescriptor</h1>

  <my-signature><code>
  class Arcadia.Media.VertexElementDesscriptor extends Arcadia.Object { ... }
  </code></my-signature>

  <my-summary>
  A application event.
  </my-summary>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity method">

  <h1 id="Arcadia.Media.VertexElementDescriptor.construct">
  [MIL] Arcadia.Media.VertexElementDescriptor.construct
  </h1>

  <my-signature><code>
    Arcadia.Media.VertexElementDescriptor.construct<br>
    &nbsp;&nbsp;(<br>
    &nbsp;&nbsp;&nbsp;&nbsp;offset : Arcadia.Natural64,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;semantics : Arcadia.Media.VertexElementSemantics,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;syntactics : Arcadia.Media.VertexElementSyntactics<br>
    &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Construct this vertex element descriptor.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div>
      <div>offset : Arcadia.Natural8</div>
      <div>The offset, in Bytes, of the vertex element from the beginning of the vertex.</div>
    </div>

    <div>
      <div>semantics : Arcadia.Media.VertexElementSemantics</div>
      <div>The semantics of the vertex element.</div>
    </div>

    <div>
      <div>syntactics : Arcadia.Media.VertexElementSyntactics</div>
      <div>The syntactics of the vertex element.</div>
    </div>

  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity method">

  <h1 id="Arcadia.Media.VertexElementDescriptor.isEqualTo">
  [MIL] Arcadia.Media.VertexElementDescriptor.isEqualTo
  </h1>

  <my-signature><code>
    <br>
    method isEqualTo<br>
    &nbsp;&nbsp;(<br>
    &nbsp;&nbsp;&nbsp;&nbsp;other : Arcadia.Object<br>
    &nbsp;&nbsp;) : Arcadia.Boolean
  </code></my-signature>

  <my-summary>
  Compare this vertex element descriptor to another object.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div>
      <div>other : Arcadia.Object</div>
      <div>The other object.</div>
    </div>

  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p><code>Arcadia.Boolean.True</code> if the objects are equal, <code>Arcadia.Boolean.False</code> otherwise.</p>
  </section>


</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
