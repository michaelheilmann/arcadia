<p>
The functions in this section extend <code>Arcadia_String</code>, <code>Arcadia_StringBuilder</code>, and <code>Arcadia_ByteArrayBuilder</code> with conversions to and from C++ (C) values and null-terminated C-strings.
</p>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity method">

  <h1 id="Arcadia_String_createFromCxxInt">Arcadia_String_createFromCxxInt</h1>

  <my-signature><code>
  Arcadia_String*<br>
  Arcadia_String_createFromCxxInt<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;int x<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Create a string from a C++ int value.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>int x</div>
      <div>The value.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>A pointer to the <code>Arcadia_String</code> object.</p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>
  </section>

</section>

<section class="cxx entity method">

  <h1 id="Arcadia_String_toCxxInt">Arcadia_String_toCxxInt</h1>

  <my-signature><code>
  int<br>
  Arcadia_String_toCxxInt<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_String* self<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Get the C++ int value of this string.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_String* self</div>
      <div>A pointer to this string.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>The C++ int value of this string.</p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ConversionFailed</div>
      <div>This string does not hold the decimal representation of an integer, or the integer is outside the range of a C++ int.</div>
    </div>
  </section>

</section>

<section class="cxx entity method">

  <h1 id="Arcadia_String_createFromCxxString">Arcadia_String_createFromCxxString</h1>

  <my-signature><code>
  Arcadia_String*<br>
  Arcadia_String_createFromCxxString<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;const char *x<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Create a string from a null-terminated C-string.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>const char *x</div>
      <div>An array of Bytes. Must be a null pointer or a pointer to the first Byte of a null-terminated sequence of Bytes.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>A pointer to the <code>Arcadia_String</code> object.</p>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>x</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>
  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity method">

  <h1 id="Arcadia_StringBuilder_insertBackCxxInt">Arcadia_StringBuilder_insertBackCxxInt</h1>

  <my-signature><code>
  void<br>
  Arcadia_StringBuilder_insertBackCxxInt<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_StringBuilder* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;int x<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Insert the string representation of a C++ int value at the back of this string buffer.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_StringBuilder* self</div>
      <div>A pointer to this string buffer.</div>
    </div>
    <div>
      <div>int x</div>
      <div>The value.</div>
    </div>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>self</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>
  </section>

</section>

<section class="cxx entity method">

  <h1 id="Arcadia_StringBuilder_insertBackCxxString">Arcadia_StringBuilder_insertBackCxxString</h1>

  <my-signature><code>
  void<br>
  Arcadia_StringBuilder_insertBackCxxString<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_StringBuilder* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;const char *x<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Insert a null-terminated C-string at the back of this string buffer.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_StringBuilder* self</div>
      <div>A pointer to this string buffer.</div>
    </div>
    <div>
      <div>const char *x</div>
      <div>An array of Bytes. Must be a null pointer or a pointer to the first Byte of a null-terminated sequence of Bytes.</div>
    </div>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>self</code> or <code>x</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>
  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity method">

  <h1 id="Arcadia_StringBuilder_insertFrontString">Arcadia_StringBuilder_insertFrontString</h1>

  <my-signature><code>
  void<br>
  Arcadia_StringBuilder_insertFrontString<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_StringBuilder* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_String* x<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Insert a string at the front of this string buffer.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_StringBuilder* self</div>
      <div>A pointer to this string buffer.</div>
    </div>
    <div>
      <div>Arcadia_String* x</div>
      <div>A pointer to the string to insert.</div>
    </div>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>self</code> or <code>x</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>
  </section>

</section>

<section class="cxx entity method">

  <h1 id="Arcadia_StringBuilder_insertBackString">Arcadia_StringBuilder_insertBackString</h1>

  <my-signature><code>
  void<br>
  Arcadia_StringBuilder_insertBackString<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_StringBuilder* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_String* x<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Insert a string at the back of this string buffer.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_StringBuilder* self</div>
      <div>A pointer to this string buffer.</div>
    </div>
    <div>
      <div>Arcadia_String* x</div>
      <div>A pointer to the string to insert.</div>
    </div>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>self</code> or <code>x</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>
  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity method">

  <h1 id="Arcadia_String_insertByteBuffer">Arcadia_String_insertByteBuffer</h1>

  <my-signature><code>
  void<br>
  Arcadia_String_insertByteBuffer<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_String* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_SizeValue index,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_ByteArrayBuilder* target<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Insert the Bytes of this string into the Byte buffer at the specified index.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_String* self</div>
      <div>A pointer to this string.</div>
    </div>
    <div>
      <div>Arcadia_SizeValue index</div>
      <div>The index at which to insert the Bytes. Must be within the bounds of <code>[0,n)</code> where <code>n</code> is the size, in Bytes, of the Byte buffer.</div>
    </div>
    <div>
      <div>Arcadia_ByteArrayBuilder* target</div>
      <div>A pointer to the Byte buffer.</div>
    </div>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>self</code> or <code>target</code> is a null pointer, or <code>index</code> is outside the bounds of the Byte buffer.</div>
    </div>
    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>
  </section>

</section>

<section class="cxx entity method">

  <h1 id="Arcadia_String_insertFrontByteBuffer">Arcadia_String_insertFrontByteBuffer</h1>

  <my-signature><code>
  void<br>
  Arcadia_String_insertFrontByteBuffer<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_String* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_ByteArrayBuilder* target<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Insert the Bytes of this string into the Byte buffer at the front.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_String* self</div>
      <div>A pointer to this string.</div>
    </div>
    <div>
      <div>Arcadia_ByteArrayBuilder* target</div>
      <div>A pointer to the Byte buffer.</div>
    </div>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>self</code> or <code>target</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>
  </section>

</section>

<section class="cxx entity method">

  <h1 id="Arcadia_String_insertBackByteBuffer">Arcadia_String_insertBackByteBuffer</h1>

  <my-signature><code>
  void<br>
  Arcadia_String_insertBackByteBuffer<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_String* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_ByteArrayBuilder* target<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Insert the Bytes of this string into the Byte buffer at the back.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>Arcadia_String* self</div>
      <div>A pointer to this string.</div>
    </div>
    <div>
      <div>Arcadia_ByteArrayBuilder* target</div>
      <div>A pointer to the Byte buffer.</div>
    </div>
  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div><code>self</code> or <code>target</code> is a null pointer.</div>
    </div>
    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>
  </section>

</section>