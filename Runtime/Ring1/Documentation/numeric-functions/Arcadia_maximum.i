<section class="cxx entity procedure">

  <h1 id="Arcadia_maximum%2a">Arcadia_maximum*</h1>

  <my-signature><code>
  <my-mv>Type</my-mv><br>
  Arcadia_maximum<my-mv>Suffix</my-mv><br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv> x,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv> y<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>Return the maximum of two values.</my-summary>

  <p>The following table lists the combinations of <my-mv>Type</my-mv> and <my-mv>Suffix</my-mv> for which the above functions are defined</p>

  <table>
  <tr><td><my-mv>Suffix</my-mv></td> <td><my-mv>Type</my-mv></td></tr>
  <tr><td><code>Integer8Value</code></td><td><code>Arcadia_Integer8Value</code></td></tr>
  <tr><td><code>Integer16Value</code></td><td><code>Arcadia_Integer16Value</code></td></tr>
  <tr><td><code>Integer32Value</code></td><td><code>Arcadia_Integer32Value</code></td></tr>
  <tr><td><code>Integer64Value</code></td><td><code>Arcadia_Integer64Value</code></td></tr>
  <tr><td><code>Natural8Value</code></td><td><code>Arcadia_Natural8Value</code></td></tr>
  <tr><td><code>Natural16Value</code></td><td><code>Arcadia_Natural16Value</code></td></tr>
  <tr><td><code>Natural32Value</code></td><td><code>Arcadia_Natural32Value</code></td></tr>
  <tr><td><code>Natural64Value</code></td><td><code>Arcadia_Natural64Value</code></td></tr>
  <tr><td><code>SizeValue</code></td><td><code>Arcadia_SizeValue</code></td></tr>
  </table>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div><div>Arcadia_Thread* thread</div><div>A pointer to the <code>Arcadia_Thread</code> object.</div></div>
    <div><div><my-mv>Type</my-mv> x </div><div>The first value.</div></div>
    <div><div><my-mv>Type</my-mv> y </div><div>The second value.</div></div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>Return the maximum of <code>x</code> and <code>y</code>.</p>
  </section>
</section>
