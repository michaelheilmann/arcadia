<section class="cxx entity method">

  <h1 id="Arcadia_FileSystem_getFileType">Arcadia_FileSystem_getFileType</h1>

  <my-signature><code>
  Arcadia_FileType<br>
  Arcadia_FileSystem_getFileType<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileSystem* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FilePath* path<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Get the type of an existing file-system entry.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><code>Arcadia_Thread* thread</code></div>
      <div>The current thread.</div>
    </div>
    <div>
      <div><code>Arcadia_FileSystem* self</code></div>
      <div>The file-system instance.</div>
    </div>
    <div>
      <div><code>Arcadia_FilePath* path</code></div>
      <div>The path of the file-system entry to inspect.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>
    An <a href="#Arcadia_FileType"><code>Arcadia_FileType</code></a> value indicating
    a regular file, directory, or unknown file type.
    </p>
  </section>

  <section class="cxx remarks">
    <h1>Remarks</h1>
    <p>
    This query does not distinguish symbolic links or reparse points as a separate
    <code>Arcadia_FileType</code> value. On Linux the default implementation uses
    <code>stat</code>, which follows symbolic links; Windows uses
    <code>GetFileAttributes</code>, whose attributes may differ for reparse points.
    Do not rely on this operation to decide whether recursive traversal should
    follow an entry. Use <a href="#Arcadia_FileSystem_isSymbolicLink"><code>Arcadia_FileSystem_isSymbolicLink</code></a>
    first when such a distinction matters.
    </p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div><code>Arcadia_Status_NotFound</code> or <code>Arcadia_Status_EnvironmentFailed</code></div>
      <div>The entry cannot be found or examined; the exact status depends on the
      operating system and underlying failure.</div>
    </div>
  </section>

</section>
