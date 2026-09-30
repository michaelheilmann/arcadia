<section class="cxx entity method">

  <h1 id="Arcadia_String_getByteAt">Arcadia_String_getByteAt</h1>

  <my-signature><code>
  Arcadia_Natural8Value<br>
  Arcadia_String_getByteAt<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_String const* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_SizeValue index<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Get the Byte value at the specified index.
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
    <div>
      <div>Arcadia_SizeValue index</div>
      <div>The index. Must be within the bounds <code>[0,n)</code> where <code>n</code> is the size, in Bytes, of this string.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>The Byte value.</p>
  </section>

</section>
