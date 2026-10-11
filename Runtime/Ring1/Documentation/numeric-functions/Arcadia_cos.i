<section class="cxx entity procedure">

  <h1 id="Arcadia_cos%2a">Arcadia_cos*</h1>

  <my-signature><code>
  <my-mv>Type</my-mv><br>
  Arcadia_cos<my-mv>Suffix</my-mv><br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv> x<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>Compute the cosine of <code>x</code>.</my-summary>

  <p>The following table lists the combinations of <my-mv>Type</my-mv> and <my-mv>Suffix</my-mv> for which the above functions are defined</p>

  <table>
  <tr><td><my-mv>Suffix</my-mv></td><td><my-mv>Type</my-mv></td></tr>
  <tr><td><code>Real32Value</code>    </td><td><code>Arcadia_Real32Value</code></td></tr>
  <tr><td><code>Real64Value</code>    </td><td><code>Arcadia_Real64Value</code></td></tr>
  </table>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div><div>Arcadia_Thread* thread</div><div>A pointer to the <code>Arcadia_Thread</code> object.</div></div>
    <div><div><my-mv>Type</my-mv> x</div><div>The value to compute the cosine of.</div></div>
  </section>

  <p>
  This function returns the cosine of <code>x</code>.
  If <code>x</code> is ±0 then the result is 1.
  If <code>x</code> is ±∞ or not a number then the result is not a number.
  </p>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>The cosine of <code>x</code>.</p>
  </section>

</section>
