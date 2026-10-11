<section class="cxx entity procedure">

  <h1 id="Arcadia_Memory_reallocateUnmanaged">Arcadia_Memory_reallocateUnmanaged</h1>

  <my-signature><code>
  void*<br>
  Arcadia_Memory_reallocateUnmanaged<br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>Reallocate a memory region.</my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div>Arcadia_Thread* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>void* p</div>
      <div>A pointer to the first Byte of the old memory region.</div>
    </div>
    <div>
      <div>Arcadia_SizeValue n</div>
      <div>The size, in Bytes, of the new memory region.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>
    A pointer to the first Byte of the new memory region.
    </p>
  </section>

  <section class="cxx errors">

    <h1>Errors</h1>

    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>

  </ section>

  <section class="cxx remarks">
    <h1>Remarks</h1>
    <p>
    The succesful reallocation of a memory region (p,n) to a memory region (q,m) effectively means:
    The old memory region p of size n was deallocated, the new memory region q of size m was allocated.
    Furthermore, the first bytes <code>k = min(n,m)</code> Bytes of the old memory region and the new memory region have the same values.
    </p>
    <p>If the reallocation fails, then the memory region (p,n) is not modified.</p>
  </section>


</section>
