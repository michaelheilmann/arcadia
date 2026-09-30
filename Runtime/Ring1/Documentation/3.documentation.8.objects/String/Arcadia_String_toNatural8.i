<section class="cxx entity method">

  <h1 id="Arcadia_String_toNatural8">Arcadia_String_toNatural8</h1>

  <my-signature><code>
  Arcadia_Natural8Value<br>
  Arcadia_String_toNatural8<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_String const* self<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Interprete the symbols of this string as a decimal natural literal and convert the number represented by that literal into an <code>Arcadia_Natural8Value</code>.
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
    <p>The natural value.</p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentTypeInvalid</div>
      <div><code>self</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_ConversionFailed</td>
      <div>The symbols of this string cannot be interpreted as a decimal natural literal or
      the number represented by the literal cannot be represented a value of type <code>Arcadia_Natural8Value</code>.</div>
    </div>
  </section>

</section>
