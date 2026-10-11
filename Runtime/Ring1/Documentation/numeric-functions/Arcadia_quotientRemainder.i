<section class="cxx entity procedure">

  <h1 id="Arcadia_quotientRemainder%2a">Arcadia_quotientRemainder*</h1>

  <my-signature><code>
  void<br>
  Arcadia_quotientRemainder<my-mv>Suffix</my-mv><br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv> dividend,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv> divisor,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv>* quotient,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv>* remainder<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>
  Return the number of significand Bits of the binary representation of a value.
  </my-summary>

  <p>
  The following table lists the combinations of <my-mv>Type</my-mv> and <my-mv>Suffix</my-mv> for which the above functions are defined
  </p>

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

  <p>
  This function computes the quotient <code><my-mv>q</my-mv> = dividend / divisor</code> and the remainder <code><my-mv>r</my-mv> = dividend - (q * divisor)</code>.
  The quotient is stored in <code>*quotient</code> and the divisor is stored in <code>*remainder</code>.
  </p>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div><div>Arcadia_Thread* thread</div><div>A pointer to the <code>Arcadia_Thread</code> object.</div></div>
    <div><div><my-mv>Type</my-mv> dividend</div><div>The dividend.</div></div>
    <div><div><my-mv>Type</my-mv> divisor</div><div>The divisor.</div></div>
    <div><div><my-mv>Type</my-mv> quotient</div><div>Variable to store the quotient in.</div></div>
    <div><div><my-mv>Type</my-mv> remainder</div><div>Variable to store the remainder in.</div></div>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div><div>Arcadia_Status_ArgumentValueInvalid</div><div><code>quotient</code> or <code>remainder</code> is zero.</div></div>
    <div><div>Arcadia_Status_DivisionByZero</div><div>The divisor is zero.</div></div>
  </section>

</section>
