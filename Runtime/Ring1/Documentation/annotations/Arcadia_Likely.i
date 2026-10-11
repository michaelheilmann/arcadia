<section class="cxx entity define">

  <h1 id="Arcadia_Likely">Arcadia_Likely</h1>

  <my-signature><code>
  #define Arcadia_Likely(expression) <my-mv>implementation detail</my-mv>
  </code></my-signature>

  <my-summary>
  Expression annotation aiding branch prediction by indicating an expression is likely to evaluate to logically true.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div><div>expression</div><div>An expression.</div></div>
  </section>

  <section class="cxx remarks">
  <p>
  For example, given typical programs, it is likely that <code>malloc</code> for a small size of memory is very likely to succeed.
  Hence, in the following code snipped, we tell tell the compiler that the test for success of a call to <code>malloc</code> is likely to to evaluate to logically true and
  the code in branch <code>(A)</code> is likely to  be executed whilst consequently the code in branch <code>(X)</code> is not likely to be execute.
  </p>
  <p><code>
  void* p = malloc(sizeof(char));<br>
  if (Arcadia_Likely(p)) {<br>
    /* (A) Foaming with joy, continue execution. */<br>
  } else {<br>
    /* (X) In the heart with pain, abort execution. */<br>
  }
  </code></p>
  </section>

</section>
