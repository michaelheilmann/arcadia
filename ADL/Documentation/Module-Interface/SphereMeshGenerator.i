<section class="cxx entity class">
<h1 id="Arcadia_ADL_SphereMeshGenerator">Arcadia_ADL_SphereMeshGenerator</h1>
<my-signature><code>
Arcadia_ADL_SphereMeshGenerator
</code></my-signature>

<my-summary>
The definition of a mesh generator that generates a sphere.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"SphereMeshGenerator"</code>. The generator describes a sphere of
the given radius and number of segments. It holds a reference to the ambient
color of the sphere, resolved when the definition is linked; the referenced
definition must be a <code>Color</code> definition.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_SphereMeshGenerator_create">Arcadia_ADL_SphereMeshGenerator_create</h1>
<my-signature><code>
Arcadia_ADL_SphereMeshGenerator* Arcadia_ADL_SphereMeshGenerator_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_Real32Value radius,
&nbsp;&nbsp;&nbsp;Arcadia_Natural32Value segments,
&nbsp;&nbsp;&nbsp;Arcadia_String* ambientColorName)
</code></my-signature>

<my-summary>
Create the definition of a sphere mesh generator.
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
  <div>radius</div>
  <div>The radius of the sphere.</div>
</div>
<div>
  <div>segments</div>
  <div>The number of segments of the sphere.</div>
</div>
<div>
  <div>ambientColorName</div>
  <div>The name of the referenced color definition.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the sphere mesh generator.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_SphereMeshGenerator_getRadius">Arcadia_ADL_SphereMeshGenerator_getRadius</h1>
<my-signature><code>
Arcadia_Real32Value Arcadia_ADL_SphereMeshGenerator_getRadius
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_SphereMeshGenerator* self)
</code></my-signature>

<my-summary>
Get the radius of this sphere mesh generator.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>self</div>
  <div>This sphere mesh generator.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The radius of the sphere.
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_SphereMeshGenerator_getSegments">Arcadia_ADL_SphereMeshGenerator_getSegments</h1>
<my-signature><code>
Arcadia_Natural32Value Arcadia_ADL_SphereMeshGenerator_getSegments
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_SphereMeshGenerator* self)
</code></my-signature>

<my-summary>
Get the number of segments of this sphere mesh generator.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>self</div>
  <div>This sphere mesh generator.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The number of segments of the sphere.
</section>
</section>

</section>

</section>