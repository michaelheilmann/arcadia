<section class="cxx entity procedure">

  <h1 id="Arcadia_Memory_fill">Arcadia_Memory_fill</h1>

  <my-signature><code>
  void<br>
  Arcadia_Memory_fill<br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;void* bytes,<br>
  &nbsp;&nbsp;Arcadia_SizeValue numberOfBytes<br>
  &nbsp;&nbsp;Arcadia_Natural8Value value<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>Assignt the Bytes of a memory region a value.</my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div><div>Arcadia_Thread* thread</div><div>A pointer to the <code>Arcadia_Thread</code> object.</div></div>
    <div><div>void* p</div><div>A pointer to the first Byte of the target memory region.</div></div>
    <div><div>Arcadia_SizeValue n</div><div>The size, in Bytes, of the memory region.</div></div>
    <div><div>Arcadia_Natural8Value v</div><div>The value to assign to each Byte of the memory region.</div></div>
  </section>

</section>
