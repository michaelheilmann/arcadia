<section class="cxx entity procedure">

  <h1 id="Arcadia_safeAddFull%2a">Arcadia_safeAddFull*</h1>

  <my-signature><code>
  void<br>
  Arcadia_safeAddFull<my-mv>Suffix</my-mv><br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv> augend,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv> addend,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv>* sumHigh,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv>* sumLow<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>
  Compute the sum of two values.
  </my-summary>

  <p>
  The following table lists the combinations of <my-mv>Type</my-mv> and <my-mv>Suffix</my-mv> for which the above functions are defined
  </p>

  <table>
    <tr><td><my-mv>Suffix</my-mv></td>      <td><my-mv>Type</my-mv></td></tr>
    <tr><td><code>Natural16Value</code></td><td><code>Arcadia_Natural16Value</code></td></tr>
    <tr><td><code>Natural32Value</code></td><td><code>Arcadia_Natural32Value</code></td></tr>
    <tr><td><code>Natural64Value</code></td><td><code>Arcadia_Natural64Value</code></td></tr>
    <tr><td><code>Natural8Value</code></td> <td><code>Arcadia_Natural8Value</code></td></tr>
    <tr><td><code>SizeValue</code></td>     <td><code>Arcadia_SizeValue</code></td></tr>
  </table>

  <section class="cxx parameters">

    <div><div>Arcadia_Thread* thread</div><div>A pointer to the <code>Arcadia_Thread</code> object.</div></div>
    <div><div><my-mv>Type</my-mv> augend</div>  <div>The augend value.</div></div>
    <div><div><my-mv>Type</my-mv> addend</div>  <div>The addend value.</div></div>
    <div><div><my-mv>Type</my-mv>* sumHigh</div><div>A pointer to a variable. The variable is assigned the high bits of the result.</div></div>
    <div><div><my-mv>Type</my-mv>* sumLow</div> <div>A pointer to a variable. The variable is assigned the low bits of the result.</div></div>

  </section>

</section>
