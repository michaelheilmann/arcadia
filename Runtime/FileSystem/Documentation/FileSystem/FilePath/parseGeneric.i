<section class="cxx entity method">

  <h1 id="Arcadia_FilePath_parseGeneric">Arcadia_FilePath_parseGeneric</h1>

  <my-signature><code>
  Arcadia_FilePath*<br>
  Arcadia_FilePath_parseGeneric<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;void const* bytes,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_SizeValue numberOfBytes<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Parse a file path in the generic format.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>bytes</div>
      <div>A pointer to an array of <code>numberOfBytes</code> Bytes.</div>
    </div>
    <div>
      <div>numberOfBytes</div>
      <div>The number of Bytes in the array pointed to by <code>bytes</code> Bytes.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>A pointer to the file path.</p>
  </section>

  <section class="cxx errors">

    <h1>Errors</h1>

    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>self</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>bytes</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_FileSystemOperationFailed</div>
      <div>Opening the file failed.</div>
    </div>

  </ section>

</section>
