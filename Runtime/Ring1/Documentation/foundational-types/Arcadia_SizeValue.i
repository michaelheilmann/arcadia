
<section class="cxx entity typedef">

  <h1 id="Arcadia_SizeValue">Size Type</h3>

  <my-signature><code>
  typedef <my-mv>implementation detail</my-mv> Arcadia_SizeValue
  </code></my-signature>

  <my-summary>
  An alias for the builtin <code>size_t</code> type.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_SizeValue_NumberOfBits">Arcadia_SizeValue_NumberOfBits</h1>

  <my-signature><code>
  #define Arcadia_SizeValue_NumberOfBits <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  The number of Bits of a value of the type <code>Arcadia_SizeValue</code> occupies.
  This is a positive multiple of 8.
  This number must one of 32 or 64, otherwise the environment is not supported by <em>Arcadia Ring 1</em>.
  This restriction may be relaxed in future versions of <em>Arcadia Ring 1</em>.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_SizeValue_NumberOfBytes">Arcadia_SizeValue_NumberOfBytes</h1>

  <my-signature><code>
  #define Arcadia_SizeValue_NumberOfBytes <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  The number of Bytes a value of the type <code>Arcadia_SizeValue</code> occupies.
  The value is always equal to <code>Arcadia_SizeValue</code> divided by 8.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_SizeValue_Maximum">Arcadia_SizeValue_Maximum</h1>

  <my-signature><code>
  #define Arcadia_SizeValue_Maximum <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  The greatest value representable by a the <code>Arcadia_SizeValue</code> type.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_SizeValue_Minimum">Arcadia_SizeValue_Minimum</h1>

  <my-signature><code>
  #define Arcadia_SizeValue_Minimum <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-signature>
  The least value representable by a the <code>Arcadia_SizeValue</code> type.
  </my-signature>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_SizeValue_Literal">Arcadia_SizeValue_Literal</h1>

  <my-signature><code>
  #define Arcadia_SizeValue_Literal(x) <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  Macro used to write a literal of the type <code>Arcadia_SizeValue</code> in C source code.
  </my-summary>

</section>
