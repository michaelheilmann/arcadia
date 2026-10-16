<section class="cxx entity class">
<h1 id="Arcadia_ADL_MeshGenerator">Arcadia_ADL_MeshGenerator</h1>
<my-signature><code>
Arcadia_ADL_MeshGenerator
</code></my-signature>

<my-summary>
The base class of the mesh generator definitions.
</my-summary>

<my-remarks>
<p>
A mesh generator describes a mesh procedurally. It carries the ambient color of
the mesh it generates. The ambient color is held as a reference, resolved when
the definition is linked; the referenced definition must be a <code>Color</code>
definition.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_MeshGenerator_getAmbientColor">Arcadia_ADL_MeshGenerator_getAmbientColor</h1>
<my-signature><code>
Arcadia_ADL_ColorDefinition* Arcadia_ADL_MeshGenerator_getAmbientColor
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_MeshGenerator* self)
</code></my-signature>

<my-summary>
Get the ambient color of this mesh generator.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>self</div>
  <div>This mesh generator.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The ambient color definition of this mesh generator.
</section>
</section>

</section>

</section>