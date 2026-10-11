<section class="cxx entity procedure">

  <h1 id="Arcadia_encodeCodePointsUtf8">Arcadia_encodeCodePointsUtf8</h1>

  <my-signature><code>
  void<br>
  Arcadia_encodeCodePointUtf8<br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;const Arcadia_Natural32Value* codePoints,<br>
  &nbsp;&nbsp;Arcadia_SizeValue numberOfCodePoints,<br>
  &nbsp;&nbsp;Arcadia_EncodeCodePointCallbackContext* context,<br>
  &nbsp;&nbsp;Arcadia_EncodeCodePointCallbackFunction* function<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>Encode Unicode code points as an UTF-8 Byte sequence.</my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div>Arcadia_Thread* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_Natural32Value* codePoints</div>
      <div>
      A pointer to an array of <code>numberOfCodePoints</code> <code>Arcadia_Natural32Value</code> values.
      Each <code>Arcadia_Natural32Value</code> in the range of a Unicode code point.</div>
    </div>
    <div>
      <div>Arcadia_SizeValue numberOfCodePoints</div>
      <div>
      The number of <code>Arcadia_Natural32Value</code> in the array pointed to by <code>codePoints</code>.
    </div>
    <div>
      <div>Arcadia_EncodeCodePointCallbackContext* context</div>
      <div>The callback context.</div>
    </div>
    <div>
      <div>Arcadia_EncodeCodePointCallbackFunction* function</div>
      <div>The callback function.</div>
    </div>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_EncodingInvalid</div>
      <div><code>codePoint</code> does not represent an Unicode code point</div>
    </div>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>codePoints</code> is a null pointer</div>
    </div>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>codePoints</code> is a null pointer</div>
    </div>
  </section>

</section>
