<section class="cxx entity method">

  <h1 id="Arcadia_FileSystem_getConfigurationDirectory">Arcadia_FileSystem_getConfigurationDirectory</h1>

  <my-summary><code>
  Arcadia_FilePath*<br>
  Arcadia_FileSystem_getConfigurationDirectory<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileSystem* self<br>
  &nbsp;&nbsp;)
  </code></my-summary>

  <my-summary>
  Get the path to the "configuration" directory.
  Get the directory in which configuration files are stored.
  The following table lists the values for a given operating system
  <ul>
    <li>Windows: <code>C:\Users\&lt;Username&gt;\AppData\Local\&lt;Organization Name&gt;\&lt;Game Name&gt;</code></li>
    <li>Linux: <code>&lt;Home&gt;\&lt;Organization Name&gt;\&lt;Game Name&gt;</code></li>
  </ul>
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
    A pointer to the the path of the "configuration" directory.
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
