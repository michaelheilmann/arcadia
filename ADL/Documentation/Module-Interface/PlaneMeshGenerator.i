<section class="cxx entity class">
<h1 id="Arcadia_ADL_PlaneMeshGenerator">Arcadia_ADL_PlaneMeshGenerator</h1>
<my-signature><code>
Arcadia_ADL_PlaneMeshGenerator
</code></my-signature>

<my-summary>
The definition of a mesh generator that generates a plane.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"PlaneMeshGenerator"</code>. The generator describes a plane of
the given width and height. It holds a reference to the ambient color of the
plane, resolved when the definition is linked; the referenced definition must be
a <code>Color</code> definition.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_PlaneMeshGenerator_create">Arcadia_ADL_PlaneMeshGenerator_create</h1>
<my-signature><code>
Arcadia_ADL_PlaneMeshGenerator* Arcadia_ADL_PlaneMeshGenerator_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_Real32Value width,
&nbsp;&nbsp;&nbsp;Arcadia_Real32Value height,
&nbsp;&nbsp;&nbsp;Arcadia_String* ambientColorName)
</code></my-signature>

<my-summary>
Create the definition of a plane mesh generator.
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
  <div>The width of the plane.</div>
</div>
<div>
  <div>height</div>
  <div>The height of the plane.</div>
</div>
<div>
  <div>ambientColorName</div>
  <div>The name of the referenced color definition.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the plane mesh generator.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_PlaneMeshGenerator_getWidth">Arcadia_ADL_PlaneMeshGenerator_getWidth</h1>
<my-signature><code>
Arcadia_Real32Value Arcadia_ADL_PlaneMeshGenerator_getWidth
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_PlaneMeshGenerator* self)
</code></my-signature>

<my-summary>
Get the width of this plane mesh generator.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>self</div>
  <div>This plane mesh generator.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The width of the plane.
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_PlaneMeshGenerator_getHeight">Arcadia_ADL_PlaneMeshGenerator_getHeight</h1>
<my-signature><code>
Arcadia_Real32Value Arcadia_ADL_PlaneMeshGenerator_getHeight
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_PlaneMeshGenerator* self)
</code></my-signature>

<my-summary>
Get the height of this plane mesh generator.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>self</div>
  <div>This plane mesh generator.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The height of the plane.
</section>
</section>

</section>

</section>