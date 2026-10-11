
<section class="cxx entity typedef">

  <h1 id="Arcadia_Integer*Value">Arcadia_Integer*Value</h1>

  <my-signature><code>
  typedef <my-mv>implementation detail</my-mv> Arcadia_Integer<my-mv>Suffix</my-mv>Value
  </code></my-signature>

  <my-summary>
  Two's complment integer number types
  where <my-mv>Suffix</my-mv> denotes the number of Bits of the two's complement integer and can be one of <code>8</code>, <code>16</code>, <code>32</code>, or <code>64</code>.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_Integer*Value_NumberOfBits">Arcadia_Integer*Value_NumberOfBits</h1>

  <my-signature><code>
  #define Arcadia_Integer<my-mv>Suffix</my-mv>Value_NumberOfBits <my-mv>implementation detail</my-mv>
  </code><my-signature>

  <my-summary>
  The number of Bits of a value of the type <code>Arcadia_Integer<my-mv>Suffix</my-mv></code> occupies
  where <my-mv>Suffix</my-mv> denotes the number of Bits of the two's complement integer and can be one of <code>8</code>, <code>16</code>, <code>32</code>, or <code>64</code>.
  The value is always equal to the value of the respective <my-mv>Suffix</my-mv>.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_Integer*Value_NumberOfBytes">Arcadia_Integer*Value_NumberOfBytes</h1>
  <my-signature><code>
  #define Arcadia_Integer<my-mv>Suffix</my-mv>Value_NumberOfBytes <my-mv>implementation detail</my-mv>
  </code><my-signature>

  <my-summary>
  The number of Bytes a value of the type <code>Arcadia_Integer<my-mv>Suffix</my-mv>Value</code> occupies
  where <my-mv>Suffix</my-mv> denotes the number of Bits of the two's complement integer and can be one of <code>8</code>, <code>16</code>, <code>32</code>, or <code>64</code>.
  The value is always equal to <code>Arcadia_Integer<my-mv>Suffix</my-mv>Value_NumberOfBits</code> divided by 8.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_Integer*Value_Maximum">Arcadia_Integer*Value_Maximum</h1>

  <my-signature><code>
  #define Arcadia_Integer<my-mv>Suffix</my-mv>Value_Maximum <my-mv>implementation detail</my-mv>
  </code><my-signature>

  <my-summary>
  The greatest value representable by a the <code>Arcadia_Integer<my-mv>Suffix</my-mv>Value</code> type
  where <my-mv>Suffix</my-mv> denotes the number of Bits of the two's complement integer and can be one of <code>8</code>, <code>16</code>, <code>32</code>, or <code>64</code>.
  </my-summary>

  <p>
  The following table denotes the value of the constant for the respective <my-mv>Suffix</my-mv>
  </p>
  <table>
  <tr><td><my-mv>Suffix</my-mv></td><td>Value</td></tr>
  <tr><td>8                    </td><td>127</td></tr>
  <tr><td>16                   </td><td>32767</td></tr>
  <tr><td>32                   </td><td>2147483647</td></tr>
  <tr><td>64                   </td><td>9223372036854775807</td></tr>
  </table>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_Integer*Value_Minimum">Arcadia_Integer*Value_Minimum</h1>

  <my-signature><code>
  #define Arcadia_Integer<my-mv>Suffix</my-mv>Value_Minimum <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  The least value representable by a the <code>Arcadia_Integer<my-mv>Suffix</my-mv>Value</code> type
  where <my-mv>Suffix</my-mv> denotes the number of Bits of the two's complement integer and can be one of <code>8</code>, <code>16</code>, <code>32</code>, or <code>64</code>.
  </my-summary>

  <p>
  The following table denotes the value of the constant for the respective <my-mv>Suffix</my-mv>
  </p>
  <table>
  <tr><td><my-mv>Suffix</my-mv></td><td>Value</td></tr>
  <tr><td>8                    </td><td>-128</td></tr>
  <tr><td>16                   </td><td>-32768</td></tr>
  <tr><td>32                   </td><td>-2147483648</td></tr>
  <tr><td>64                   </td><td>-9223372036854775808</td></tr>
  </table>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_Integer*Value_Literal">Arcadia_Integer*Value_Literal</h1>

  <my-signature><code>
  #define Arcadia_Integer<my-mv>Suffix</my-mv>Value_Literal(x) <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  Macro used to write a literal of the type <code>Arcadia_Integer<my-mv>Suffix</my-mv>Value</code> in C source code
  where <my-mv>Suffix</my-mv> denotes the number of Bits of the two's complement integer and can be one of <code>8</code>, <code>16</code>, <code>32</code>, or <code>64</code>.
  </my-summary>

  <p>
  The following table denotes  the expansions of the macro:
  </p>
  <table>
  <tr><td><my-mv>Suffix</my-mv></td><td>Value</td></tr>
  <tr><td>8                    </td><td>x</td></tr>
  <tr><td>16                   </td><td>x</td></tr>
  <tr><td>32                   </td><td>x</td></tr>
  <tr><td>64                   </td><td>x##LL</td></tr>
  </table>

</section>
