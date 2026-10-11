<section class="cxx entity procedure">

  <h1 id="Arcadia_Real%2aValue_isFinite">Arcadia_Real*Value_isFinite</h1>

  <my-signature><code>
  Arcadia_BooleanValue<br>
  Arcadia_Real<my-mv>Bits</my-mv>Value_isFinite<br>
    (<br>
      Arcadia_Thread* thread,<br>
      Arcadia_Real<my-mv>Bits</my-mv>Value self<br>
    )
  </code></my-signature>

  <my-summary>
  Get if a <code>Arcadia_Real<my-mv>Suffix</my-mv>Value</code> is finite,
  that is, it is a normal, denormal, or zero <code>Arcadia_Real<my-mv>Suffix</my-mv>Value</code>
  (which also implies that it is not NaN or Inf).
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
    <p><code>Arcadia_BooleanValue_True</code> if <code>self</code> is finite. <code>Arcadia_BooleanValue_False</code> otherwise.</p>
  </section>

</section>
