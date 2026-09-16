<section class="cxx entity class">
<h1 id="Arcadia_ADL_ModelDefinition">Arcadia_ADL_ModelDefinition</h1>
<my-signature><code>
Arcadia_ADL_ModelDefinition
</code></my-signature>

<my-summary>
The definition of a model: a mesh rendered with a material.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"Model"</code>. The model holds a reference to a mesh and a
reference to a material, both resolved when the definition is linked.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_ModelDefinition_create">Arcadia_ADL_ModelDefinition_create</h1>
<my-signature><code>
Arcadia_ADL_ModelDefinition* Arcadia_ADL_ModelDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_String* meshName,
&nbsp;&nbsp;&nbsp;Arcadia_String* materialName)
</code></my-signature>

<my-summary>
Create the definition of a model.
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
  <div>meshName</div>
  <div>The name of the referenced mesh definition.</div>
</div>
<div>
  <div>materialName</div>
  <div>The name of the referenced material definition.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the model.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

</section>

</section>