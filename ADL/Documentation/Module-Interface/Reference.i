<section class="cxx entity class">
<h1 id="Arcadia_ADL_Reference">Arcadia_ADL_Reference</h1>
<my-signature><code>
Arcadia_ADL_Reference
</code></my-signature>

<my-summary>
A reference of a definition to another definition, by name.
</my-summary>

<my-remarks>
<p>
While an asset is being read, a reference holds the name of the referenced
definition but not the definition itself. Resolving the reference looks the name up
in the container of definitions. References are resolved when the owning definition
is linked.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_Reference_create">Arcadia_ADL_Reference_create</h1>
<my-signature><code>
Arcadia_ADL_Reference* Arcadia_ADL_Reference_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* definitionName)
</code></my-signature>

<my-summary>
Create a reference to the definition with the given name.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>definitions</div>
  <div>The container the referenced definition is looked up in.</div>
</div>
<div>
  <div>definitionName</div>
  <div>The name of the referenced definition.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
A reference to the definition with the given name.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the reference cannot be allocated.</div>
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_Reference_getDefinitionName">Arcadia_ADL_Reference_getDefinitionName</h1>
<my-signature><code>
Arcadia_String* Arcadia_ADL_Reference_getDefinitionName
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Reference* self)
</code></my-signature>

<my-summary>
Get the name of the referenced definition.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>self</div>
  <div>The reference.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The name of the referenced definition.
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_Reference_resolve">Arcadia_ADL_Reference_resolve</h1>
<my-signature><code>
void Arcadia_ADL_Reference_resolve
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Reference* self)
</code></my-signature>

<my-summary>
Resolve the reference: look the referenced definition up in the container.
</my-summary>

<my-remarks>
<p>
Resolving is idempotent: a reference that was already resolved is left unchanged.
If the referenced definition does not exist, an error is raised.
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
  <div>The reference.</div>
</div>
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_NotFound</code> if the referenced definition does not exist.</div>
</section>
</section>

</section>

</section>