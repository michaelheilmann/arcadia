<section class="cxx entity method">

  <h1 id="Arcadia_FileSystem_isSymbolicLink">Arcadia_FileSystem_isSymbolicLink</h1>

  <my-signature><code>
  Arcadia_BooleanValue<br>
  Arcadia_FileSystem_isSymbolicLink<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileSystem* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FilePath* path<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Determine whether a file-system entry is a symbolic link or, on Windows, a reparse point.
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
      <div>The path of an existing file-system entry to inspect.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>
    <code>Arcadia_BooleanValue_True</code> if the final path component is a symbolic link
    on Linux, or has the <code>FILE_ATTRIBUTE_REPARSE_POINT</code> attribute on Windows.
    <code>Arcadia_BooleanValue_False</code> otherwise.
    </p>
  </section>

  <section class="cxx remarks">
    <h1>Remarks</h1>
    <p>
    On Linux, the default implementation uses <code>lstat</code> and does not follow
    a symbolic link at the final path component, including a dangling symbolic link.
    On Windows, the default implementation uses <code>GetFileAttributesA</code> and
    returns true for any reparse point, including directory junctions; this is a
    broader category than symbolic links.
    </p>
    <p>
    Unlike an existence query, this operation requires the path entry to exist and
    be accessible. A missing or inaccessible entry causes an error, not a false result.
    Other path components may themselves be links; the operation is not a general
    path-confinement or race-prevention mechanism. A caller using it during recursive
    traversal must still account for changes between the check and later file access.
    </p>
    <p>
    See also <a href="#Arcadia_FileSystem_getFileType"><code>Arcadia_FileSystem_getFileType</code></a>.
    </p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div><code>Arcadia_Status_OperationFailed</code></div>
      <div>The operating system could not obtain the entry's metadata, for example
      because the path does not exist or the operation is denied.</div>
    </div>
  </section>

</section>
