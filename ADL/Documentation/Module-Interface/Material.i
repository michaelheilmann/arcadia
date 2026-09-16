<section class="cxx entity class">
<h1 id="Arcadia_ADL_MaterialDefinition">Arcadia_ADL_MaterialDefinition</h1>
<my-signature><code>
Arcadia_ADL_MaterialDefinition
</code></my-signature>

<my-summary>
The definition of the material a model is rendered with.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"Material"</code>. The material holds a reference to the texture of
its ambient-color source, resolved when the definition is linked; the referenced
definition must be a <code>Texture</code> definition.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_MaterialDefinition_create">Arcadia_ADL_MaterialDefinition_create</h1>
<my-signature><code>
Arcadia_ADL_MaterialDefinition* Arcadia_ADL_MaterialDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_MaterialType materialType,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_AmbientColorSource ambientColorSource,
&nbsp;&nbsp;&nbsp;Arcadia_String* ambientColorTextureName,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_BlendFunction blendSourceFunction,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_BlendFunction blendDestinationFunction)
</code></my-signature>

<my-summary>
Create the definition of a material.
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
  <div>materialType</div>
  <div>The material type, an element of the <code>Arcadia_ADL_MaterialType</code> enumeration.</div>
</div>
<div>
  <div>ambientColorSource</div>
  <div>The source of the ambient color, an element of the <code>Arcadia_ADL_AmbientColorSource</code> enumeration.</div>
</div>
<div>
  <div>ambientColorTextureName</div>
  <div>The name of the referenced texture definition.</div>
</div>
<div>
  <div>blendSourceFunction</div>
  <div>The source blend function, an element of the <code>Arcadia_ADL_BlendFunction</code> enumeration. The element <code>None</code> selects the default of the engine.</div>
</div>
<div>
  <div>blendDestinationFunction</div>
  <div>The destination blend function, an element of the <code>Arcadia_ADL_BlendFunction</code> enumeration. The element <code>None</code> selects the default of the engine.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the material.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

</section>

</section>