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
alignment strings, and five Color references. Named references are resolved
and type-checked during linking. Inline Color definitions are retained directly
by their containing button; omitted optional state colors reuse its normal Color.
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
&nbsp;&nbsp;&nbsp;Arcadia_String* backgroundColor,
&nbsp;&nbsp;&nbsp;Arcadia_String* hoveredBackgroundColor,
&nbsp;&nbsp;&nbsp;Arcadia_String* pressedBackgroundColor,
&nbsp;&nbsp;&nbsp;Arcadia_String* disabledBackgroundColor,
&nbsp;&nbsp;&nbsp;Arcadia_String* focusedBackgroundColor,
&nbsp;&nbsp;&nbsp;Arcadia_Natural32Value pixelSize,
&nbsp;&nbsp;&nbsp;Arcadia_Natural32Value borderWidth,
&nbsp;&nbsp;&nbsp;Arcadia_Natural8Value borderRed,
&nbsp;&nbsp;&nbsp;Arcadia_Natural8Value borderGreen,
&nbsp;&nbsp;&nbsp;Arcadia_Natural8Value borderBlue)
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
  <div>backgroundColor</div>
  <div>Required name of the normal background Color definition (C factory interface).</div>
</div>
<div>
  <div>hoveredBackgroundColor</div>
  <div>Optional name of the hovered Color definition.</div>
</div>
<div>
  <div>pressedBackgroundColor</div>
  <div>Optional name of the pressed Color definition.</div>
</div>
<div>
  <div>disabledBackgroundColor</div>
  <div>Optional name of the disabled Color definition.</div>
</div>
<div>
  <div>focusedBackgroundColor</div>
  <div>Optional name of the focused Color definition.</div>
</div>
<div>
  <div>pixelSize</div>
  <div>The text size in pixels.</div>
</div>
<div>
  <div>borderWidth</div>
  <div>The border width in pixels.</div>
</div>
<div>
  <div>borderRed</div>
  <div>The red border component.</div>
</div>
<div>
  <div>borderGreen</div>
  <div>The green border component.</div>
</div>
<div>
  <div>borderBlue</div>
  <div>The blue border component.</div>
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
