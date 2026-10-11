<section class="cxx entity typedef">

  <h1 id="Arcadia_Real*Value">Arcadia_Real*Value</h1>

  <my-signature><code>
  typedef <my-mv>implementation detail</my-mv> Arcadia_Real<my-mv>Suffix</my-mv>Value
  </code></my-signature>

  <my-summary>
  IEEE 754 binary real number type where <my-mv>Suffix</my-mv> denotes the number of Bits of the type and can be one of <code>32</code> or <code>64</code>.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_Real*Value_NumberOfBits">Arcadia_Real*Value_NumberOfBits</h1>

  <p>The macros in this section all include the <my-mv>Suffix</my-mv> metavariable and are defined for the
  values <code>32</code> and <code>64</code>.</p>

  <my-signature><code>
  #define Arcadia_Real<my-mv>Suffix</my-mv>Value_NumberOfBits <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  The number of Bits of a value of the type <code>Arcadia_VoidValue</code> occupies
  where <my-mv>Suffix</my-mv> denotes the number of Bits of the type and can be one of <code>32</code> or <code>64</code>.
  This is a positive multiple of 8.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_Real%2aValue_NumberOfBytes">Arcadia_Real*Value_NumberOfBytes</h1>

  <my-signature><code>
  #define Arcadia_Real<my-mv>Suffix</my-mv>Value_NumberOfBytes <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  The number of Bytes a value of the type <code>Arcadia_Real<my-mv>Suffix</my-mv>Value</code> occupies
  where <my-mv>Suffix</my-mv> denotes the number of Bits of the type and can be one of <code>32</code> or <code>64</code>.
  The value is always equal to <code>Arcadia_Real<my-mv>Suffix</my-mv>Value_NumberOfBits</code> divided by 8.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_Real*Value_Maximum">Arcadia_Real*Value_Maximum</h1>

  <my-signature><code>
  #define Arcadia_Real<my-mv>Suffix</my-mv>Value_Maximum <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  The greatest value representable by a the <code>Arcadia_Real<my-mv>Suffix</my-mv>Value</code> type.
  </my-summary>

  <p>
  The following table denotes the value of the constant for the respective <code><span>Suffix</span></code>
  </p>
  <table>
  <tr><td><code><span>Constant</span></code></td><td>Value</td></tr>
  <tr><td>32                                </td><td><code>+FLT_MAX</code></td></tr>
  <tr><td>64                                </td><td><code>+DBL_MAX</code></td></tr>
  </table>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_Real*Value_Minimum">Arcadia_Real*Value_Minimum</h1>

  <my-signature><code>
  #define Arcadia_Real<my-mv>Suffix</my-mv>Value_Minimum <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  The least value representable by a the <code>Arcadia_Real<my-mv>Suffix</my-mv>Value</code> type
  where <my-mv>Suffix</my-mv> denotes the number of Bits of the type and can be one of <code>32</code> or <code>64</code>.
  </my->summary>

  <p>
  The following table denotes the value of the constant for the respective <code><my-mv>Suffix</my-mv></code>
  </p>
  <table>
  <tr><td><my-mv>Suffix</my-mv></td><td>Value</td></tr>
  <tr><td>32                   </td><td><code>-FLT_MAX</code></td></tr>
  <tr><td>64                   </td><td><code>-DBL_MAX</code></td></tr>
  </table>

</section>
