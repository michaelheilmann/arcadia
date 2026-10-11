<section class="cxx entity procedure">

  <h1 id="Arcadia_Real%2aValue_getBits">Arcadia_Real*Value_getBits</h1>

  <my-signature><code>
  Arcadia_Natural<my-mv>Bits</my-mv><br>
  Arcadia_Real<my-mv>Bits</my-mv>Value_getBits<br>
    (<br>
      Arcadia_Thread* thread,<br>
      Arcadia_Real<my-mv>Bits</my-mv>Value self<br>
    )
  </code></my-signature>

  <my-summary>
  Get the bits of an <code>Arcadia_Real<my-mv>Suffix</my-mv>Value</code>
  where <my-mv>Suffix</my-mv> denotes the number of Bits of the type and can be one of <code>32</code> or <code>64</code>.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div>Arcadia_Thread* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_Real<my-mv>Bits</my-mv> self</div>
      <div>The Real value.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>Return the bits of the Real value as a Natural value.</p>
  </section>

</section>
