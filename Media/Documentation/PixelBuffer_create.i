<section class="cxx entity method">

  <h1 id="Arcadia_Media_PixelBuffer_create">Arcadia_Media_PixelBuffer_create</h1>

  <my-signature><code>
  Arcadia_Media_PixelBuffer*<br>
  Arcadia_Media_PixelBuffer_create<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Integer32Value linePadding,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Integer32Value numberOfColumns,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Integer32Value numberOfRows,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Media_PixelFormat pixelFormat,<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Create a pixel buffer of the specified line padding, number of columns, number of rows, and pixel format.
  It is filled with opaque black.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>

    <div>
      <div><a href="#">Arcadia_Integer32Value</a> linePadding</div>
      <div>The number of Bytes after the end of a line. Must be non-negative.</div>
    </div>

    <div>
      <div><a href="#">Arcadia_Integer32Value</a> numberOfColumns</div>
      <div>The number of columns of the pixel buffer. Must be non-negative.</div>
    </div>

    <div>
      <div><a href="#">Arcadia_Integer32Value</a> numberOfRows</div>
      <div>The number of rows of the pixel buffer. Must be non-negative.</div>
    </div>

    <div>
      <div><a href="#">Arcadia_Media_PixelFormat</a> pixelFormat</div>
      <div>The pixel format of the pixel buffer.</div>
    </div>

  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div>At least one of <code>linePadding</code>, <code>numberOfColumns</code>, <code>numberOfRows</code> is negative.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>A pointer to the <code>Arcadia_Media_PixelBuffer</code> object.</p>
  </section>

</section>
