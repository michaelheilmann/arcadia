<section class="cxx entity class">
<h1 id="Arcadia_ADL_ColorDefinition">Arcadia_ADL_ColorDefinition</h1>
<my-signature><code>
Arcadia_ADL_ColorDefinition
</code></my-signature>

<my-summary>
The definition of a color, with red, green, and blue components.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"Color"</code>. The definition holds no references and therefore
links trivially.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_ColorDefinition_create">Arcadia_ADL_ColorDefinition_create</h1>
<my-signature><code>
Arcadia_ADL_ColorDefinition* Arcadia_ADL_ColorDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_Natural8Value red,
&nbsp;&nbsp;&nbsp;Arcadia_Natural8Value green,
&nbsp;&nbsp;&nbsp;Arcadia_Natural8Value blue)
</code></my-signature>

<my-summary>
Create the definition of a color.
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
  <div>red</div>
  <div>The red component.</div>
</div>
<div>
  <div>green</div>
  <div>The green component.</div>
</div>
<div>
  <div>blue</div>
  <div>The blue component.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the color.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

</section>

</section>