<section class="cxx entity class">
<h1 id="Arcadia_ADL_Definitions">Arcadia_ADL_Definitions</h1>
<my-signature><code>
Arcadia_ADL_Definitions
</code></my-signature>

<my-summary>
A container of definitions, mapping definition names to definitions.
</my-summary>

<my-remarks>
<p>
An application reads all of its assets into one container and links the container
once the assets have been read.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_Definitions_create">Arcadia_ADL_Definitions_create</h1>
<my-signature><code>
Arcadia_ADL_Definitions* Arcadia_ADL_Definitions_create
&nbsp;&nbsp;(Arcadia_Thread* thread)
</code></my-signature>

<my-summary>
Create an empty container of definitions.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
An empty container of definitions.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the container cannot be allocated.</div>
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_Definitions_getDefinitionOrNull">Arcadia_ADL_Definitions_getDefinitionOrNull</h1>
<my-signature><code>
Arcadia_ADL_Definition* Arcadia_ADL_Definitions_getDefinitionOrNull
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* self,
&nbsp;&nbsp;&nbsp;Arcadia_String* name)
</code></my-signature>

<my-summary>
Get the definition with the given name, or a null pointer if no definition of that
name is stored.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>self</div>
  <div>The container.</div>
</div>
<div>
  <div>name</div>
  <div>The name of the definition.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition with the given name, or a null pointer if there is none.
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_Definitions_link">Arcadia_ADL_Definitions_link</h1>
<my-signature><code>
void Arcadia_ADL_Definitions_link
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* self)
</code></my-signature>

<my-summary>
Link every definition of the container.
</my-summary>

<my-remarks>
<p>
Linking the container links the whole graph of definitions, because linking a
definition also links the definitions it references.
</p>
</my-remarks>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>self</div>
  <div>The container.</div>
</div>
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises an error if a referenced definition of any stored definition does not exist or does not have the required type.</div>
</section>
</section>

</section>

</section>