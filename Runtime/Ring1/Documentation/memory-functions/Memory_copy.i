<section class="cxx entity procedure">

  <h1 id="Arcadia_Memory_copy">Arcadia_Memory_copy</h1>

  <my-signature><code>
  void<br>
  Arcadia_Memory_copy<br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;void* target,<br>
  &nbsp;&nbsp;const void* source,<br>
  &nbsp;&nbsp;Arcadia_SizeValue numberOfBytes<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>Copy the Bytes from one memory region to another memory region. The memory regions may overlap.</my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div><div>Arcadia_Thread* thread</div><div>A pointer to the <code>Arcadia_Thread</code> object.</div></div>
    <div><div>void* target</div><div>A pointer to the first Byte of the target memory region.</div></div>
    <div><div>const void* source</div><div>A pointer to the first Byte of the source memory region.</div></div>
    <div><div>Arcadia_SizeValue numberOfBytes</div><div>The number of Bytes to copy.</div></div>
  </section>

</section>
