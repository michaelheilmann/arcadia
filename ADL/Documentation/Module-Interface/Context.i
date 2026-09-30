<section class="cxx entity class">
<h1 id="Arcadia_ADL_Context">Arcadia_ADL_Context</h1>
<my-signature><code>
Arcadia_ADL_Context
</code></my-signature>

<my-summary>
A context is a registry of readers, mapping the string type of an ADL asset to the
reader that processes it.
</my-summary>

<my-remarks>
<p>
A context is a singleton: there is exactly one context per process. It is created on
demand by <code>Arcadia_ADL_Context_getOrCreate</code>. Reading an asset whose
<code>type</code> entry is not served by any registered reader raises a semantical
error.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_Context_getOrCreate">Arcadia_ADL_Context_getOrCreate</h1>
<my-signature><code>
Arcadia_ADL_Context* Arcadia_ADL_Context_getOrCreate
&nbsp;&nbsp;(Arcadia_Thread* thread)
</code></my-signature>

<my-summary>
Get the context of the process, creating it on the first call.
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
The context of the process.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the context cannot be allocated.</div>
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_Context_readFromString">Arcadia_ADL_Context_readFromString</h1>
<my-signature><code>
Arcadia_ADL_Definition* Arcadia_ADL_Context_readFromString
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Context* self,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* input,
&nbsp;&nbsp;&nbsp;Arcadia_BooleanValue skipExisting)
</code></my-signature>

<my-summary>
Read an ADL asset from a string, storing the resulting definition in the container
of definitions.
</my-summary>

<my-remarks>
<p>
The string is parsed with the default DDL reader into a DDL map node and then
processed as a node (see <code>Arcadia_ADL_Context_readFromNode</code>).
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
  <div>The context.</div>
</div>
<div>
  <div>definitions</div>
  <div>The container of definitions the resulting definition is stored in.</div>
</div>
<div>
  <div>input</div>
  <div>The string that contains the ADL asset.</div>
</div>
<div>
  <div>skipExisting</div>
  <div>If an asset with the same name as the resulting definition was read before: skip the new definition (<code>skipExisting</code> is true) or raise an exists error (it is false).</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The resulting definition, or the already stored definition of the same name if
<code>skipExisting</code> is true.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises an error if the asset is not a DDL map, if its <code>type</code> entry is not served by a registered reader, if the asset violates the schema of its type, or if a definition of the same name already exists and <code>skipExisting</code> is false.</div>
</section>
</section>

<section class="cxx method">
<h1 id="Arcadia_ADL_Context_readFromNode">Arcadia_ADL_Context_readFromNode</h1>
<my-signature><code>
Arcadia_ADL_Definition* Arcadia_ADL_Context_readFromNode
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Context* self,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_DDL_Node* input,
&nbsp;&nbsp;&nbsp;Arcadia_BooleanValue skipExisting)
</code></my-signature>

<my-summary>
Read an ADL asset from a DDL map node, storing the resulting definition in the
container of definitions.
</my-summary>

<my-remarks>
<p>
The node must be a DDL map node; otherwise a semantical error is raised. The
<code>type</code> entry selects the reader, which validates the asset against its
schema and constructs the definition.
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
  <div>The context.</div>
</div>
<div>
  <div>definitions</div>
  <div>The container of definitions the resulting definition is stored in.</div>
</div>
<div>
  <div>input</div>
  <div>The DDL map node that contains the ADL asset.</div>
</div>
<div>
  <div>skipExisting</div>
  <div>If an asset with the same name as the resulting definition was read before: skip the new definition (<code>skipExisting</code> is true) or raise an exists error (it is false).</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The resulting definition, or the already stored definition of the same name if
<code>skipExisting</code> is true.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises an error if the asset is not a DDL map, if its <code>type</code> entry is not served by a registered reader, if the asset violates the schema of its type, or if a definition of the same name already exists and <code>skipExisting</code> is false.</div>
</section>
</section>

</section>

</section>