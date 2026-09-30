<section class="cxx entity method">

  <h1 id="Arcadia_String_createFromVoid">
    Arcadia_String_createFromVoid
  </h1>

  <my-signature><code>
    Arcadia_String*<br>
    Arcadia_String_createFromBoolean<br>
    &nbsp;&nbsp;(<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_VoidValue voidValue<br>
    &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
    Create a string from an <code>Arcadia_VoidValue</code> value.
    The <code>Arcadia_VoidValue</code> value is converted to the string <code>void</code>.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <tr><td><a href="#">Arcadia_Thread</a>* thread</td><td>A pointer to the <code>Arcadia_Thread</code> object.</td></tr>
    <tr><td>Arcadia_VoidValue voidValue</td><td>The <code>Arcadia_VoidValue</code> to be converted into the string value.</td></tr>

  </section>

  <section class="cxx return-value">

    <h1>Return value</h1>
    <p>A pointer to the <code>Arcadia_String</code> object.</p>

  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
  </section>

</section>
