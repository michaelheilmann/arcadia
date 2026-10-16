<section class="cxx entity class">
<h1 id="Arcadia_ADL_BoxMeshGenerator">Arcadia_ADL_BoxMeshGenerator</h1>
<my-signature><code>
Arcadia_ADL_BoxMeshGenerator
</code></my-signature>

<my-summary>
The definition of a mesh generator that generates a box.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"BoxMeshGenerator"</code>. The generator describes a box of the
given width, height, and depth. It holds a reference to the ambient color of the
box, resolved when the definition is linked; the referenced definition must be a
<code>Color</code> definition.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_BoxMeshGenerator_create">Arcadia_ADL_BoxMeshGenerator_create</h1>
<my-signature><code>
Arcadia_ADL_BoxMeshGenerator* Arcadia_ADL_BoxMeshGenerator_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_Real32Value width,
&nbsp;&nbsp;&nbsp;Arcadia_Real32Value height,
&nbsp;&nbsp;&nbsp;Arcadia_Real32Value depth,
&nbsp;&nbsp;&nbsp;Arcadia_String* ambientColorName)
</code></my-signature>

<my-summary>
Create the definition of a box mesh generator.
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
  <div>width</div>
  <div>The width of the box.</div>
</div>
<div>
  <div>height</div>
  <div>The height of the box.</div>
</div>
<div>
  <div>depth</div>
  <div>The depth of the box.</div>
</div>
<div>
  <div>ambientColorName</div>
  <div>The name of the referenced color definition.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the box mesh generator.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_BoxMeshGenerator_getWidth">Arcadia_ADL_BoxMeshGenerator_getWidth</h1>
<my-signature><code>
Arcadia_Real32Value Arcadia_ADL_BoxMeshGenerator_getWidth
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_BoxMeshGenerator* self)
</code></my-signature>

<my-summary>
Get the width of this box mesh generator.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>self</div>
  <div>This box mesh generator.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The width of the box.
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_BoxMeshGenerator_getHeight">Arcadia_ADL_BoxMeshGenerator_getHeight</h1>
<my-signature><code>
Arcadia_Real32Value Arcadia_ADL_BoxMeshGenerator_getHeight
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_BoxMeshGenerator* self)
</code></my-signature>

<my-summary>
Get the height of this box mesh generator.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>self</div>
  <div>This box mesh generator.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The height of the box.
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_BoxMeshGenerator_getDepth">Arcadia_ADL_BoxMeshGenerator_getDepth</h1>
<my-signature><code>
Arcadia_Real32Value Arcadia_ADL_BoxMeshGenerator_getDepth
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_BoxMeshGenerator* self)
</code></my-signature>

<my-summary>
Get the depth of this box mesh generator.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>self</div>
  <div>This box mesh generator.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The depth of the box.
</section>
</section>

</section>

</section>