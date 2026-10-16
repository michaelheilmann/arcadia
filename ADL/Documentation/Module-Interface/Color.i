<section class="cxx entity class">
<h1 id="Arcadia_ADL_ColorDefinition">Arcadia_ADL_ColorDefinition</h1>
<my-signature><code>
Arcadia_ADL_ColorDefinition
</code></my-signature>

<my-summary>
The definition of a color, with straight/unassociated red, green, blue and alpha components.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"Color"</code>. Its public scalar fields are <code>red</code>,
<code>green</code>, <code>blue</code>, then <code>alpha</code>, each
<code>Arcadia_Natural8Value</code>. RGB asset fields are required; alpha is optional,
defaulting to 255. Explicit alpha 0 is preserved. The definition adds no reference
fields and links trivially; its native parent still retains definitions/name.
</p>
<p>
The existing five-argument factory remains unchanged and initializes alpha to 255.
Native callers may assign the public alpha field afterward. The PDL constructor
owns initialization and uses a narrow native typed-255 helper, since ordinary PDL
integer literals are not Natural8 values. No new getter or dispatch slot is added.
Nonintegral/out-of-range numeric asset alpha raises <code>Arcadia_Status_ConversionFailed</code>,
as for RGB; a non-number raises <code>Arcadia_Status_SemanticalError</code> during schema validation.
</p>
<p>
Stored alpha does not enable blending, premultiply RGB or change RGB-only consumers.
Media fills retain pixel alpha 255; mesh-generator color conversion and current
example clear-color/text-run conversions retain alpha 1; UI Button color state
retains RGB-only copying. Material blend settings remain separate.
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
Create the definition of a color with the supplied RGB and alpha initialized to 255.
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
