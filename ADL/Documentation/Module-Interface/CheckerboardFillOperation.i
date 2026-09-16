<section class="cxx entity class">
<h1 id="Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition">Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition</h1>
<my-signature><code>
Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition
</code></my-signature>

<my-summary>
The definition of a fill operation that fills a pixel buffer with a checkerboard pattern.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"PixelBufferOperations.CheckerboardFillOperation"</code>. The fill
operation holds a reference to the color of the first checker and a reference to
the color of the second checker, both resolved when the definition is linked; the
referenced definitions must be <code>Color</code> definitions.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_create">Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_create</h1>
<my-signature><code>
Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_Integer32Value checkerWidth,
&nbsp;&nbsp;&nbsp;Arcadia_Integer32Value checkerHeight,
&nbsp;&nbsp;&nbsp;Arcadia_String* firstCheckerColorName,
&nbsp;&nbsp;&nbsp;Arcadia_String* secondCheckerColorName)
</code></my-signature>

<my-summary>
Create the definition of a checkerboard fill operation.
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
  <div>checkerWidth</div>
  <div>The width, in pixels, of a checker of the pattern.</div>
</div>
<div>
  <div>checkerHeight</div>
  <div>The height, in pixels, of a checker of the pattern.</div>
</div>
<div>
  <div>firstCheckerColorName</div>
  <div>The name of the referenced color definition of the first checker.</div>
</div>
<div>
  <div>secondCheckerColorName</div>
  <div>The name of the referenced color definition of the second checker.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the checkerboard fill operation.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

</section>

</section>