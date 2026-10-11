<section class="cxx entity procedure">

  <h1 id="Arcadia_EncodeCodePointCallbackFunction">Arcadia_EncodeCodePointCallbackFunction</h1>

  <my-signature><code>
  typedef void<br>
  (Arcadia_EncodeCodePointCallbackFunction)<br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;Arcadia_EncodeCodePointCallbackContext* context,<br>
  &nbsp;&nbsp;const Arcadia_Natural8Value* bytes,<br>
  &nbsp;&nbsp;Arcadia_SizeValue numberOfBytes<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>The type of a function receiving an encoding of an Unicode code point.</my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div><div>Arcadia_Thread* thread</div><div>A pointer to the <code>Arcadia_Thread</code> object.</div></div>
    <div><div>Arcadia_EncodeCodePointCallbackContext* context</div><div>An opaque context pointer passed to invocations of the callback function.</div></div>
    <div><div>const Arcadia_Natural8Value* bytes</div><div>A pointer to an array of <code>numberOfBytes</code> Bytes.</div></div>
    <div><div>Arcadia_SizeValue numberOfBytes</div><div>The number of Bytes in the array of Bytes pointed to by <code>bytes</code>.</div></div>
  </section>

</section>
