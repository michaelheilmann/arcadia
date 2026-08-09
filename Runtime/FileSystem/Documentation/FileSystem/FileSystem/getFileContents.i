<section class="cxx entity method">

  <h1 id="Arcadia_FileSystem_getFileContents">Arcadia_FileSystem_getFileContents</h1>

  <my-signature><code>
  Arcadia_ByteBuffer*<br>
  Arcadia_FileSystem_getFileContents<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileSystem* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FilePath* path<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Get the contents of a file.
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
      <div><a href="#">Arcadia_FilePath</a>* path</div>
      <div>The file path of the file.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>
    A pointer to a <code>Arcadia_ByteBuffer</code> object with the file contents.
    </p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div><a href="#">Arcadia_Status_ArgumentValueInvalid</a></div>
      <div><code>self</code> is a null pointer.</div>
    </div>
    <div>
      <div><a href="#">Arcadia_Status_ArgumentValueInvalid</a></div>
      <div><code>path</code> is a null pointer.</div>
    </div>
    <div>
      <div><a href="#">Arcadia_Status_OperationFailed</a></div>
      <div>Opening the file failed.</div>
    </div>
  </section>

</section>
