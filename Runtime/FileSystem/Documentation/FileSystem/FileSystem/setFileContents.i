<section class="cxx entity method">

  <h1 id="Arcadia_FileSystem_setFileContents">Arcadia_FileSystem_setFileContents</h1>

  <my-signature><code>
  void<br>
  Arcadia_FileSystem_setFileContents<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileSystem* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FilePath* path,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_ByteBuffer* contents<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Set the contents of a file.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div><a href="#">Arcadia_FileSystem</a>* self</div>
      <div>A pointer to this <code>Arcadia_FileSystem</code> object.</div>
    </div>
    <div>
      <div>Arcadia_FilePath* path</div>
      <div>The file path of the file.</div>
    </div>
    <div>
      <div>Arcadia_ByteBuffer* contents</div>
      <div>A poiner to the Byte buffer with the file contents.</div>
    </div>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>self</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>path</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>contents</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_OperationFailed</div>
      <div>Opening the file failed.</div>
    </div>
  </section>

</section>
