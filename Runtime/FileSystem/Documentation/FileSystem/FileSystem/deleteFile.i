<section class="cxx entity method">

  <h1 id="Arcadia_FileSystem_deleteFile">Arcadia_FileSystem_deleteFile</h1>

  <my-summary><code>
  void<br>
  Arcadia_FileSystem_deleteFile<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileSystem* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FilePath* path<br>
  &nbsp;&nbsp;)
  </code></my-summary>

  <my-summary>
  Delete a regular file.
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
      <div>The path to the file.</div>
    </div>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_AccessDenied</div>
      <div><code>self</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>self</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>path</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_AccessDenied</div>
      <div>Access to the file was denied.</div>
    </div>
    <div>
      <div>Arcadia_Status_NotFound</div>
      <div>The file was not found.</div>
    </div>
    <div>
      <div>Arcadia_Status_OperationFailed</div>
      <div>Deletion failed.</div>
    </div>
  </section>

</section>
