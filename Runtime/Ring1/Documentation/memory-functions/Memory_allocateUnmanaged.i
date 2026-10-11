<section class="cxx entity procedure">

  <h1 id="Arcadia_Memory_allocateUnmanaged">Arcadia_Memory_allocateUnmanaged</h1>

  <my-signature><code>
  void*<br>
  Arcadia_Memory_allocateUnmanaged<br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>Allocate an unnamanged memory region.</my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div>Arcadia_Thread* thread</div><div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_SizeValue size</div>
      <div>The size, in Bytes, of the memory region to allocate. Memory regions of zero Bytes are supported.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>
    A pointer to the first Byte of the memory region.
    </p>
  </section>

  <section class="cxx errors">

    <h1>Errors</h1>

    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>

  </ section>

</section>
