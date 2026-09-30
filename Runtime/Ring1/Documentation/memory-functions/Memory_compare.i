<section class="cxx entity procedure">

  <h1 id="Arcadia_Memory_compare">Arcadia_Memory_compare</h1>

  <my-signature><code>
  Arcadia_Integer8Value<br>
  Arcadia_Memory_compare<br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;const void* p,<br>
  &nbsp;&nbsp;const void* q,<br>
  &nbsp;&nbsp;Arcadia_SizeValue n<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>Compare two memory regions.</my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div><div>Arcadia_Thread* thread</div><div>A pointer to the <code>Arcadia_Thread</code> object.</div></div>
    <div><div>const void* p</div><div>A pointer to the first Byte of the first memory region.</div></div>
    <div><div>const void* q</div><div>A pointer to the first Byte of the second memory region.</div></div>
    <div><div>Arcadia_SizeValue n</div><div>The number of Bytes to compare.</div></div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>
    A positive value if the first memory region is greater than the first memory region.
    A negative value if the first memory region is less than the second memory region.
    Zero if both memory regions are equal.
    </p>
  </section>

  <section class="cxx remarks">
    <h1>Remarks</h1>
    <p>
    A memory region <code>(p,n)</code> is equal to a memory region <code>(q,n)</code> if for all <code>0&lte; i &lt;</code> <code>p[i] = q[i]</code> holds.
    Otherwise there exists a least <code>0&lte; i &lt; n</code> such that <code>p[i] &neq; q[i]</code> holds and consequently either <code>p[i] &lt; q[i]</code> or <code>p[i] &gt; q[i]</code> holds:
    if <code>p[i] &neq; q[i]</code> holds then p is less than q and if <code>p[i] &gt; q[i]</code> holds then p is greater than q.
    </p>
  </section>

</section>
