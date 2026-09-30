<section class="cxx entity method">

  <h1 id="Arcadia_String_toInteger16">Arcadia_String_toInteger16</h1>

  <my-signature><code>
  Arcadia_Integer16Value<br>
  Arcadia_String_toInteger16<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_String const* self<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Interprete the symbols of this string as decimal integer literal and convert the number represented by that integer literal into an <code>Arcadia_Integer16Value</code>.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <tr><td><a href="#">Arcadia_Thread</a>* thread</td><td>A pointer to the <code>Arcadia_Thread</code> object.</td></tr>
    <tr><td>Arcadia_String* self</td><td>A pointer to this string.</td></tr>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>The integer value.</p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentTypeInvalid</div>
      <div><code>self</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_ConversionFailed</div>
      <div>The symbols of this string cannot be interpreted as a decimal integer literal or
      the number represented by the literal cannot be represented a value of type <code>Arcadia_Integer16Value</code>.</div>
    </div>
  </section>

</section>
