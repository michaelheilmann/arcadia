<section class="cxx entity method">

  <h1 id="Arcadia_String_toBoolean">Arcadia_String_toBoolean</h1>

  <my-signature><code>
  Arcadia_BooleanValue<br>
  Arcadia_String_toBoolean<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_String const* self<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Interprete the symbols of this string as boolean literal and convert the boolean represented by that literal into an <code>Arcadia_BooleanValue</code>.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_String* self</div>
      <div>A pointer to this string.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>The boolean value.</p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <tr>
      <td>Arcadia_Status_ArgumentTypeInvalid</td>
      <td><code>self</code> is a null pointer.</td>
    </tr>
    <tr>
      <td>Arcadia_Status_ConversionFailed</td>
      <td>The symbols of this string cannot be interpreted as a boolean literal. Valid boolean literals are <code>true</code> and <code>false</code>, both case sensitve.</td>
    </tr>
  </<section>

</section>
