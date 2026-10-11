<section class="cxx entity procedure">

  <h1 id="Arcadia_clamp%2a">Arcadia_clamp*</h1>

  <my-signature><code>
  <my-mv>Type</my-mv><br>
  Arcadia_clamp<my-mv>Suffix</my-mv><br>
  &nbsp;(<br>
  &nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv> value,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv> lower,<br>
  &nbsp;&nbsp;<my-mv>Type</my-mv> upper<br>
  &nbsp;)
  </code></my-signature>

  <my-summary>Clamp a value to a range.</my-summary>

  <p>The following table lists the combinations of <my-mv>Type</my-mv> and <my-mv>Suffix</my-mv> for which the above functions are defined</p>

  <table>
  <tr><td><my-mv>Suffix</my-mv></td><td><my-mv>Type</my-mv></td></tr>
  <tr><td><code>Integer8Value</code>  </td><td><code>Arcadia_Integer8Value</code> </td></tr>
  <tr><td><code>Integer16Value</code> </td><td><code>Arcadia_Integer16Value</code></td></tr>
  <tr><td><code>Integer32Value</code> </td><td><code>Arcadia_Integer32Value</code></td></tr>
  <tr><td><code>Integer64Value</code> </td><td><code>Arcadia_Integer64Value</code></td></tr>
  <tr><td><code>Natural8Value</code>  </td><td><code>Arcadia_Natural8Value</code> </td></tr>
  <tr><td><code>Natural16Value</code> </td><td><code>Arcadia_Natural16Value</code></td></tr>
  <tr><td><code>Natural32Value</code> </td><td><code>Arcadia_Natural32Value</code></td></tr>
  <tr><td><code>Natural64Value</code> </td><td><code>Arcadia_Natural64Value</code></td></tr>
  <tr><td><code>Real32Value</code>    </td><td><code>Arcadia_Real32Value</code>   </td></tr>
  <tr><td><code>Real64Value</code>    </td><td><code>Arcadia_Real64Value</code>   </td></tr>
  <tr><td><code>SizeValue</code>      </td><td><code>Arcadia_SizeValue</code>     </td></tr>
  </table>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div><div>Arcadia_Thread* thread</div><div>A pointer to the <code>Arcadia_Thread</code> object.</div></div>
    <div><div><my-mv>Type</my-mv> value</div><div>The value to clamp.</div></div>
    <div><div><my-mv>Type</my-mv> lower</div><div>The lower bound (inclusive).</div></div>
    <div><div><my-mv>Type</my-mv> upper</div><div>The upper bound (inclusive).</div></div>
  </section>

  <p>This function returns the value <code>value</code> clamped to the range <code>[lower, upper]</code>.</p>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div><div>Arcadia_Status_ArgumentValueInvalid</div><div><code>lower &gt; upper</code></div></div>
    <div><div>Arcadia_Status_EncodingInvalid     </div><div>The sequence of Bytes does not represented a UTF-8-NO-BOM string.</div></div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>The value <code>value</code> clamped to the range <code>[lower, upper]</code>.</p>
  </section>

</section>
