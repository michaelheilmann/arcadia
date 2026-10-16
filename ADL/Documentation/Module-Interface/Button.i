<section class="cxx entity class">
<h1 id="Arcadia_ADL_UI_ButtonDefinition">Arcadia_ADL_UI_ButtonDefinition</h1>
<my-signature><code>
Arcadia_ADL_UI_ButtonDefinition
</code></my-signature>

<my-summary>
The definition of a rectangular user-interface button.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"UI.Button"</code>. The definition stores the button rectangle,
alignment strings, and background color. It holds no references and therefore
links trivially.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_UI_ButtonDefinition_create">Arcadia_ADL_UI_ButtonDefinition_create</h1>
<my-signature><code>
Arcadia_ADL_UI_ButtonDefinition* Arcadia_ADL_UI_ButtonDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_Integer32Value left,
&nbsp;&nbsp;&nbsp;Arcadia_Integer32Value bottom,
&nbsp;&nbsp;&nbsp;Arcadia_Natural32Value width,
&nbsp;&nbsp;&nbsp;Arcadia_Natural32Value height,
&nbsp;&nbsp;&nbsp;Arcadia_String* text,
&nbsp;&nbsp;&nbsp;Arcadia_String* horizontalAlignment,
&nbsp;&nbsp;&nbsp;Arcadia_String* verticalAlignment,
&nbsp;&nbsp;&nbsp;Arcadia_Natural8Value backgroundRed,
&nbsp;&nbsp;&nbsp;Arcadia_Natural8Value backgroundGreen,
&nbsp;&nbsp;&nbsp;Arcadia_Natural8Value backgroundBlue)
</code></my-signature>

<my-summary>
Create the definition of a user-interface button.
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
  <div>left</div>
  <div>The lower-left x coordinate of the button rectangle.</div>
</div>
<div>
  <div>bottom</div>
  <div>The lower-left y coordinate of the button rectangle.</div>
</div>
<div>
  <div>width</div>
  <div>The width of the button rectangle.</div>
</div>
<div>
  <div>height</div>
  <div>The height of the button rectangle.</div>
</div>
<div>
  <div>text</div>
  <div>The button label.</div>
</div>
<div>
  <div>horizontalAlignment</div>
  <div>The horizontal alignment string.</div>
</div>
<div>
  <div>verticalAlignment</div>
  <div>The vertical alignment string.</div>
</div>
<div>
  <div>backgroundRed</div>
  <div>The red component of the button background.</div>
</div>
<div>
  <div>backgroundGreen</div>
  <div>The green component of the button background.</div>
</div>
<div>
  <div>backgroundBlue</div>
  <div>The blue component of the button background.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the user-interface button.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

</section>

</section>
