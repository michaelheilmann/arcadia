<section class="cxx entity class">
<h1 id="Arcadia_ADL_Definition">Arcadia_ADL_Definition</h1>
<my-signature><code>
Arcadia_ADL_Definition
</code></my-signature>

<my-summary>
A definition, the base type of all definitions of the Arcadia asset definition
language.
</my-summary>

<my-remarks>
<p>
A definition has a name and belongs to a container of definitions. Some definitions
hold references to other definitions; such references are resolved when the
definition is linked (see <code>Arcadia_ADL_Definition_link</code> and
<code>Arcadia_ADL_Definitions_link</code>).
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_Definition_getName">Arcadia_ADL_Definition_getName</h1>
<my-signature><code>
Arcadia_String* Arcadia_ADL_Definition_getName
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definition* self)
</code></my-signature>

<my-summary>
Get the name of the definition.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>self</div>
  <div>The definition.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The name of the definition.
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_Definition_link">Arcadia_ADL_Definition_link</h1>
<my-signature><code>
void Arcadia_ADL_Definition_link
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definition* self)
</code></my-signature>

<my-summary>
Link the definition: resolve its references and check their types.
</my-summary>

<my-remarks>
<p>
This is a virtual method. Link implementations resolve every reference of the
concrete definition, check that the referenced definition has the required type,
and link referenced definitions recursively. Linking is idempotent.
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
  <div>The definition.</div>
</div>
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises an error if a referenced definition does not exist or does not have the required type.</div>
</section>
</section>

</section>

</section>