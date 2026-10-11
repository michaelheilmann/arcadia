<section class="cxx entity procedure">

  <h1 id="Arcadia_Unicode_isUTF8">Arcadia_Unicode_isUTF8</h1>

  <my-signature><code>
  Arcadia_BooleanValue<br>
  Arcadia_Unicode_isUTF8<br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;const void* bytes,<br>
  &nbsp;&nbsp;Arcadia_SizeValue numberOfBytes,<br>
  &nbsp;&nbsp;Arcadia_SizeValue* numberOfSymbols<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>Get if a sequence of Bytes is an UTF-8 sequence of Bytes and count its number of symbols.</my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div><div>Arcadia_Thread* thread</div><div>A pointer to the <code>Arcadia_Thread</code> object.</div></div>
    <div><div>const void* bytes</div><div>A pointer to an array of <code>numberOfBytes</code> Bytes.</div></div>
    <div><div>Arcadia_SizeValue numberOfBytes</div><div>The number of Bytes in the array of Bytes pointed to by <code>bytes</code>.</div></div>
    <div><div>Arcadia_SizeValue* numberOfSymbols</div><div>
    The number of Bytes in the array of Bytes pointed to by <code>bytes</code>.
    If this function terminates and this pointer is not null, the variable is assigned the number of symbols decoded successfully so far.
    </div></div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>
    <code>Arcadia_BooleanValue_True</code> if the sequence of <code>numberOfBytes</code> Bytes pointed to by <code>bytes</code> is an UTF-8 sequence of Bytes.
    <code>Arcadia_BooleanValue_False</code> otherwise.
    </p>
  </section>

</section>
