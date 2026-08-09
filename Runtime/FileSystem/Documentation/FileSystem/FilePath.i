<section class="cxx entity object">
<h1 id="Arcadia_FilePath">Arcadia_FilePath</h1>
<p><code>Arcadia_FilePath</code> extends <code>Arcadia_Object</code>.</p>
<p><code>Arcadia_FilePath</code> represents a file path.</p>
</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity method">

  <h1 id="Arcadia_FilePath_create">create</h1>

  <my-signature><code>
  Arcadia_FilePath*<br>
  Arcadia_FilePath_create<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Create the empty file path.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>A pointer to the file path.</p>
  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

@{include("FileSystem/FilePath/parseGeneric.i")}
@{include("FileSystem/FilePath/parseNative.i")}
@{include("FileSystem/FilePath/parseUnix.i")}
@{include("FileSystem/FilePath/parseWindows.i")}

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity method">

  <h1 id="Arcadia_FilePath_toNative">Arcadia_FilePath_toNative</h1>

  <my-signature><code>
  Arcadia_String*<br>
  Arcadia_FilePath_toNative<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_FilePath* self<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Convert a file path to the native format.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div>self</div>
      <div>A pointer to this file path.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>A pointer to the string.</p>
  </section>

</section>
