<section class="cxx entity object">
<h1 id="Arcadia_FileHandle">Arcadia_FileHandle</h1>
<p><code>Arcadia_FileHandle</code> extends <code>Arcadia_Object</code>.</p>
<p><code>Arcadia_FileHandle</code> represents a operating system file handle.</p>
</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
<section class="cxx entity method">

  <h1 id="Arcadia_FileHandle_create">
    Arcadia_FileHandle_create
  </h1>

  <my-signature><code>
    Arcadia_FileHandle*<br>
    Arcadia_FileHandle_create<br>
    &nbsp;&nbsp;(<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileSystem* fileSystem<br>
    &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
    Create a file handle. The file handle is closed.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_FileSystem* self</div>
      <div>A pointer to the backing <code>Arcadia_FileSystem</code> object.</div>
    </div>

  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>A pointer to the file handle.</p>
  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
<section class="cxx entity method">

  <h1 id="Arcadia_FileHandle_close">Arcadia_FileHandle_close</h1>

  <my-signature><code>
  void<br>
  Arcadia_FileHandle_close<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileHandle* self<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Close this file handle.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_FileHandle* self</div>
      <div>A pointer to this file handle.</div>
    </div>

  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
<section class="cxx entity method">

  <h1 id="Arcadia_FileHandle_openForReading">Arcadia_FileHandle_openForReading</h1>

  <my-signature><code>
    void<br>
    Arcadia_FileHandle_openForReading<br>
    &nbsp;&nbsp;(<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileHandle* self,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FilePath* path<br>
    &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
    Open a file for reading. If the file is open, it is closed before trying to re-open it.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_FileHandle* self</div>
      <div>A pointer to this file handle.</div>
    </div>
    <div>
      <div>Arcadia_FilePath* path</div>
      <div>The file path of the file to read from.</div>
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
      <div>Arcadia_Status_FileSystemOperationFailed</div>
      <div>Opening the file failed.</div>
    </div>

  </ section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
<section class="cxx entity method">

  <h1 id="Arcadia_FileHandle_openForWriting">Arcadia_FileHandle_openForWriting</h1>

  <my-signature><code>
  void<br>
  Arcadia_FileHandle_openForWriting<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileHandle* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FilePath* path<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Open a file for writing. If the file is open, it is closed before trying to re-open it.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div><div>Arcadia_FileHandle* self</div><div>A pointer to this file handle.</div></div>
    <div><div>Arcadia_FilePath* path  </div><div>The file path of the file to write to.</div></div>

  </section>

  <section class="cxx errors">

    <h1>Errors</h1>

    <div><div>Arcadia_Status_ArgumentValueInvalid     </div><div><code>self</code> is a null pointer.</div></div>
    <div><div>Arcadia_Status_ArgumentValueInvalid     </div><div><code>path</code> is a null pointer.</div></div>
    <div><div>Arcadia_Status_FileSystemOperationFailed</div><div>Opening the file failed.            </div></div>

  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
<section class="cxx entity method">

  <h1 id="Arcadia_FileHandle_isClosed">Arcadia_FileHandle_isClosed</h1>

  <my-signature><code>
  Arcadia_BooleanValue<br>
  Arcadia_FileHandle_isClosed<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileHandle const* self<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Get if this file handle is closed.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div><div>Arcadia_FileHandle const* self </div><div>A pointer to this file handle.</div></div>

  </section>

  <section class="cxx return-value">

    <h1>Return value</h1>

    <p>
    <code>Arcadia_BooleanValue_True</code> if this file handle is closed.
    <code>Arcadia_BooleanValue_False</code> otherwise.
    </p>

  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
<section class="cxx entity method">

  <h1 id="Arcadia_FileHandle_isOpened">Arcadia_FileHandle_isOpened</h1>

  <my-signature><code>
  Arcadia_BooleanValue<br>
  Arcadia_FileHandle_isOpened<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileHandle const* self<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>Get if this file handle is opened.</my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div><div>Arcadia_FileHandle const* self </div><div>A pointer to this file handle.</div></div>

  </section>

  <section class="cxx return-value">

    <h1>Return value</h1>

    <p>
    <code>Arcadia_BooleanValue_True</code> if this file handle is opened.
    <code>Arcadia_BooleanValue_False</code> otherwise.
    </p>

  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
<section class="cxx entity method">

  <h1 id="Arcadia_FileHandle_isOpenedForReading">Arcadia_FileHandle_isOpenedForReading</h1>

  <my-signature><code>
  Arcadia_BooleanValue<br>
  Arcadia_FileHandle_isOpenedForReading<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileHandle const* self<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>Get if this file handle is opened for reading.</my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div><div>Arcadia_FileHandle const* self </div><div>A pointer to this file handle.</div></div>

  </section>

  <section class="cxx return-value">

    <h1>Return value</h1>

    <p>
    <code>Arcadia_BooleanValue_True</code> if this file handle is opened for reading.
    <code>Arcadia_BooleanValue_False</code> otherwise.
    </p>

  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
<section class="cxx entity method">

  <h1 id="Arcadia_FileHandle_isOpenedForWriting">Arcadia_FileHandle_isOpenedForWriting</h1>

  <my-signature><code>
  Arcadia_BooleanValue<br>
  Arcadia_FileHandle_isOpenedForWriting<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileHandle const* self<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Get if this file handle is opened for writing.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div><div>Arcadia_FileHandle const* self </div><div>A pointer to this file handle.</div></div>

  </section>

  <section class="cxx return-value">

    <h1>Return value</h1>

    <p>
    <code>Arcadia_BooleanValue_True</code> if this file handle is opened for writing.
    <code>Arcadia_BooleanValue_False</code> otherwise.
    </p>

  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
<section class="cxx entity method">

  <h1 id="Arcadia_FileHandle_write">Arcadia_FileHandle_write</h1>

  <my-signature><code>
  void<br>
  Arcadia_FileHandle_write<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FileHandle* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;void const* p,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_SizeValue bytesToWrite<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Write Bytes to this file handle.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div><div>Arcadia_FileHandle* self       </div><div>A pointer to this file handle.</div></div>
    <div><div>void const* bytes              </div><div>A pointer to an array of <code>bytesToWrite</code> Bytes.</div></div>
    <div><div>Arcadia_SizeValue bytesToWrite </div><div>The number of Bytes in the array pointed to by <code>bytes</code>.</div></div>
    <div><div>Arcadia_SizeValue* bytesWritten</div><div>A pointer to a <code>Arcadia_SizeValue</code> variable.</div></div>

  </section>

  <section class="cxx success-condition">
    <h1>Success</h1>
    <p><code>*bytesWritten</code> is assigned the actual number of Bytes written.</p>
  </section>

  <section class="cxx errors">

    <h1>Errors</h1>

    <div><div>Arcadia_Status_ArgumentValueInvalid     </div><div><code>self</code> is a null pointer. </div></div>
    <div><div>Arcadia_Status_ArgumentValueInvalid     </div><div><code>bytes</code> is a null pointer.</div></div>
    <div><div>Arcadia_Status_OperationInvalid         </div><div>The file is not opened for writing.</div></div>
    <div><div>Arcadia_Status_FileSystemOperationFailed</div><div>Writing failed.</div></div>

  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
<section class="cxx entity method">

  <h1 id="Arcadia_FileHandle_read">Arcadia_FileHandle_read</h1>

  <my-signature><code>
  void<br>
  Arcadia_FileHandle_read<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;Arcadia_FileHandle* self,<br>
  &nbsp;&nbsp;void const* bytes,<br>
  &nbsp;&nbsp;Arcadia_SizeValue bytesToRead,<br>
  &nbsp;&nbsp;Arcadia_SizeValue* bytesRead<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>Read Bytes from this file handle.</my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div><div>Arcadia_FileHandle* self     </div><div>A pointer to this file handle.</div></div>
    <div><div>void const* bytes            </div><div>A pointer to an array of <code>bytesToWrite</code> Bytes.</div></div>
    <div><div>Arcadia_SizeValue bytesToRead</div><div>The number of Bytes to read from the the array pointed to by <code>bytes</code>.</div></div>
    <div><div>Arcadia_SizeValue* bytesRead </div><div>A pointer to a <code>Arcadia_SizeValue</code> variable.</div></div>

  </section>

  <section class="cxx success-condition">

    <h1>Success</h1>

    <p>
    <code>*bytesRead</code> is assigned the actual number of Bytes read.
    The number of Bytes read is 0 if the end of the file was reached.
    </p>

  </section>

  <section class="cxx errors">

    <h1>Errors</h1>

    <div><div>Arcadia_Status_ArgumentValueInvalid     </div><div><code>self</code> is a null pointer.</div></div>
    <div><div>Arcadia_Status_ArgumentValueInvalid     </div><div><code>bytes</code> is a null pointer.</div></div>
    <div><div>Arcadia                                 </div><div><code>bytesRead</code> is a null pointer.</div></div>
    <div><div>Arcadia_Status_OperationInvalid         </div><div>The file is not opened for writing.</div></div>
    <div><div>Arcadia_Status_FileSystemOperationFailed</div><div>Writing failed.</div></div>

  </section>

</section>
