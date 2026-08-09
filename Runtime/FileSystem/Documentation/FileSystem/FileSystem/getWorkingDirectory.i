<section class="cxx entity method">

  <h1 id="Arcadia_FileSystem_getWorkingDirectory">Arcadia_FileSystem_getWorkingDirectory</h1>

  <my-summary><code>
  Arcadia_FilePath*<br>
  Arcadia_FileSystem_getWorkingDirectory<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileSystem* self<br>
  &nbsp;&nbsp;)
  </code></my-summary>

  <my-summary>
  Get the working directory of this process.
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
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>
    A pointer to the path of the working directory.
    </p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>self</code> is a null pointer.</div>
    </div>
  </section>

</section>
