
<section class="cxx entity typedef">

  <h1 id="Arcadia_ObjectReferenceValue">Arcadia_ObjectReferenceValue</h1>

  <my-signature><code>
  typedef <my-mv>implementation detail</my-mv> Arcadia_ObjectReferenceValue
  </code></my-signature>

  <my-summary>
  The <code>Arcadia_ObjectReferenceValue</code> type is a pointer which assume the value Arcadia_ObjectReferenceValue_Null or point to a Arcadia_Object or derived type value.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_ObjectReferenceValue_Null">Arcadia_ObjectReferenceValue_Null</h1>

  <my-signature><code>
  #define Arcadia_ObjectReferenceValue_Null <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  The Arcadia_ObjectReferenceValue_Null value indicating the absence of a an Arcadia_Object or derived type value.
  This usually evaluates to <code>((void*)0)</code>.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_ObjectReferenceValue_NumberOfBits">Arcadia_ObjectReferenceValue_NumberOfBits</h1>

  <my-signature><code>
  #define Arcadia_ObjectReferenceValue_NumberOfBits <my-mv>implementation detail</my-mv>
  </code><my-signature>

  <my-summary>
  Denotes the number of Bits of the type.
  </my-summary>

</section>

<section class="cxx entity define">

  <h1 id="Arcadia_ObjectReferenceValue_NumberOfBytes">Arcadia_ObjectReferenceValue_NumberOfBytes</h1>

  <my-signature><code>
  #define Arcadia_ObjectReferenceValue_NumberOfBytes <my-mv>implementation detail</my-mv>
  </code><my-signature>

  <my-summary>
  Denotes the number of Bytes of the type which is usually <code>Arcadia_ObjectReferenceValue_NumberOfBits</code> divided by 8.
  </my-summary>

</section>
