<section class="cxx entity class">
<h1 id="Arcadia_ADL_PixelBufferDefinition">Arcadia_ADL_PixelBufferDefinition</h1>
<my-signature><code>
Arcadia_ADL_PixelBufferDefinition
</code></my-signature>

<my-summary>
The definition of a pixel buffer with a width, a height, and a list of fill operations.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"PixelBuffer"</code>. The definition holds a list of references
to its fill operations; linking the definition resolves the references.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_PixelBufferDefinition_create">Arcadia_ADL_PixelBufferDefinition_create</h1>
<my-signature><code>
Arcadia_ADL_PixelBufferDefinition* Arcadia_ADL_PixelBufferDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_Natural32Value width,
&nbsp;&nbsp;&nbsp;Arcadia_Natural32Value height)
</code></my-signature>

<my-summary>
Create the definition of a pixel buffer.
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
  <div>The width, in pixels, of the pixel buffer.</div>
</div>
<div>
  <div>height</div>
  <div>The height, in pixels, of the pixel buffer.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the pixel buffer.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

</section>

</section>