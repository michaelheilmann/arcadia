<section class="cxx entity class">
<h1 id="Arcadia_ADL_MeshDefinition">Arcadia_ADL_MeshDefinition</h1>
<my-signature><code>
Arcadia_ADL_MeshDefinition
</code></my-signature>

<my-summary>
The definition of a polygonal mesh.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"Mesh"</code>. The mesh holds a reference to the color of its
vertices; linking the definition resolves this reference and requires the
referenced definition to be a <code>Color</code> definition.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_MeshDefinition_create">Arcadia_ADL_MeshDefinition_create</h1>
<my-signature><code>
Arcadia_ADL_MeshDefinition* Arcadia_ADL_MeshDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_SizeValue numberOfVertices,
&nbsp;&nbsp;&nbsp;Arcadia_RuntimeByteArray* vertexPositions,
&nbsp;&nbsp;&nbsp;Arcadia_RuntimeByteArray* vertexAmbientColors,
&nbsp;&nbsp;&nbsp;Arcadia_RuntimeByteArray* vertexAmbientTextureCoordinates,
&nbsp;&nbsp;&nbsp;Arcadia_String* ambientColorName)
</code></my-signature>

<my-summary>
Create the definition of a mesh.
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
  <div>numberOfVertices</div>
  <div>The number of vertices of the mesh.</div>
</div>
<div>
  <div>vertexPositions</div>
  <div>The positions of the vertices.</div>
</div>
<div>
  <div>vertexAmbientColors</div>
  <div>The ambient colors of the vertices.</div>
</div>
<div>
  <div>vertexAmbientTextureCoordinates</div>
  <div>The ambient texture coordinates of the vertices.</div>
</div>
<div>
  <div>ambientColorName</div>
  <div>The name of the referenced color definition.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the mesh.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

</section>

</section>