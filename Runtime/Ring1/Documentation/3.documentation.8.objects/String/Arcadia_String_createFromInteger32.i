<section class="cxx entity method">

  <h1 id="Arcadia_String_createFromInteger32">
    Arcadia_String_createFromInteger32
  </h1>

  <my-signature><code>
    Arcadia_String*<br>
    Arcadia_String_createFromInteger32<br>
    &nbsp;&nbsp;(<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Integer32Value integer32Value<br>
    &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
    Create a string from an <code>Arcadia_Integer32Value</code> value.
    The <code>Arcadia_Integer32Value</code> value is converted to the decimal integer number literal string.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <tr><td><a href="#">Arcadia_Thread</a>* thread</td><td>A pointer to the <code>Arcadia_Thread</code> object.</td></tr>
    <tr><td>Arcadia_Integer32Value integer32Value</td><td>The <code>Arcadia_Integer32Value</code> to be converted into the string value.</td></tr>

  </section>

  <section class="cxx return-value">

    <h1>Return value</h1>
    <p>A pointer to the <code>Arcadia_String</code> object.</p>

  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
  </section>

</section>
