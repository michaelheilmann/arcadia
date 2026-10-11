<section class="cxx entity procedure">

  <h1 id="Arcadia_countTrailingZeroes%2a">Arcadia_countTrailingZeroes*</h1>

  <my-signature><code>
  Arcadia_SizeValue<br>
  Arcadia_countTrailingZeroes<my-mv>Suffix</my-mv><br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv> value<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>Return the number of trailing zeroes of the binary representation of a value.</my-summary>

  <p>The following table lists the combinations of <my-mv>Type</my-mv> and <my-mv>Suffix</my-mv> for which the above functions are defined</p>

  <table>
  <tr><td><my-mv>Suffix</my-mv></td><td><my-mv>Type</my-mv></td></tr>
  <tr><td><code>Integer8Value</code>  </td><td><code>Arcadia_Integer8Value</code></td></tr>
  <tr><td><code>Integer16Value</code> </td><td><code>Arcadia_Integer16Value</code></td></tr>
  <tr><td><code>Integer32Value</code> </td><td><code>Arcadia_Integer32Value</code></td></tr>
  <tr><td><code>Integer64Value</code> </td><td><code>Arcadia_Integer64Value</code></td></tr>
  <tr><td><code>Natural8Value</code>  </td><td><code>Arcadia_Natural8Value</code></td></tr>
  <tr><td><code>Natural16Value</code> </td><td><code>Arcadia_Natural16Value</code></td></tr>
  <tr><td><code>Natural32Value</code> </td><td><code>Arcadia_Natural32Value</code></td></tr>
  <tr><td><code>Natural64Value</code> </td><td><code>Arcadia_Natural64Value</code></td></tr>
  <tr><td><code>SizeValue</code>      </td><td><code>Arcadia_SizeValue</code></td></tr>
  </table>

  <p>This function returns the number of trailing zeroes of the binary representation of <code>value</code>.</p>

  <section class="cxx parameters">
    <my-parameters>Parameters</my-parameters>
    <table>
      <tr><td>Arcadia_Thread* thread</td><td>A pointer to the <code>Arcadia_Thread</code> object.</td></tr>
      <tr><td><my-mv>Type</my-mv> value</td><td>The value.</td></tr>
    </table>
  </section>

  <section class="cxx return-value">
    <my-return-value>Return value</my-return-value>
    <p>The number of leading zeroes.</p>
  </section>

</section>
