<section class="cxx entity typedef">

  <h1 id="Arcadia_ForeignProcedureValue">Arcadia_ForeignProcedureValue</h1>

  <my-signature><code>
  typedef <my-mv>implementation detail</my-mv> Arcadia_ForeignProcedureValue;
  </code></my-signature>

  <my-summary>
  A pointer to a function adhering to the calling conventions for C functions.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_ForeignProcedureValue_Null">Arcadia_ForeignProcedureValue_Null</h1>

  <my-signature><code>
  #define Arcadia_ForeignProcedureValue_Null <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  The Arcadia_ForeignProcedureValue value indicating the absence of a function.
  This usually evaluates to <code>((void*)0)</code>.
  </my-summary>

</section>

<section class="cxx entity typedef">

  <h1 id="Arcadia_ForeignProcedureValue_NumberOfBits">Arcadia_ForeignProcedureValue_NumberOfBits</h1>

  <my-signature><code>
  #define Arcadia_ForeignProcedureValue_NumberOfBits <my-mv>implementation detail</my-mv>
  </code><my-signature>

  <my-summary>
  The number of Bits an Arcadia_ForeignProcedureValue occupies.
  This is a value is a positive multiple of 8.
  </my-summary>

</section>

<section class="cxx entity typedef">

  <h1 id="Arcadia_ForeignProcedureValue_NumberOfBytes">Arcadia_ForeignProcedureValue_NumberOfBytes</h1>

  <my-signature><code>
  #define Arcadia_ForeignProcedureValue_NumberOfBytes <my-mv>implementation detail</my-mv>
  </code><my-signature>

  <my-summary>
  The number of Bytes of the type which is usually <code>Arcadia_ForeignProcedureValue_NumberOfBits</code> divided by 8.
  </my-summary>

</section>
