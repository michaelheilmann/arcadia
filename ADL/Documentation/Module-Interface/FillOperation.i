<section class="cxx entity class">
<h1 id="Arcadia_ADL_PixelBufferOperations_FillOperationDefinition">Arcadia_ADL_PixelBufferOperations_FillOperationDefinition</h1>
<my-signature><code>
Arcadia_ADL_PixelBufferOperations_FillOperationDefinition
</code></my-signature>

<my-summary>
The definition of a fill operation that fills a pixel buffer with a single color.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"PixelBufferOperations.FillOperation"</code>. The fill operation
holds a reference to the color it fills the pixel buffer with, resolved when the
definition is linked; the referenced definition must be a <code>Color</code>
definition.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_create">Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_create</h1>
<my-signature><code>
Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_String* colorName)
</code></my-signature>

<my-summary>
Create the definition of a fill operation.
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
  <div>colorName</div>
  <div>The name of the referenced color definition.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the fill operation.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

</section>

</section>