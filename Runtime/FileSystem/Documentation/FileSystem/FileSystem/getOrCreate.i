<section class="cxx entity method">

  <h1 id="Arcadia_FileSystem_getOrCreate">Arcadia_FileSystem_getOrCreate</h1>

  <my-signature><code>
  Arcadia_FileSystem*<br>
  Arcadia_FileSystem_getOrCreate<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Get or create the file system singleton.
  </my-summary>

  <section clas="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>
    A pointer to a <code>Arcadia_FileSystem</code> object.
    </p>
  </section>

</section>
