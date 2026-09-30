<section class="cxx entity class">
<h1 id="Arcadia_ADL_Reader">Arcadia_ADL_Reader</h1>
<my-signature><code>
Arcadia_ADL_Reader
</code></my-signature>

<my-summary>
A reader validates the assets of a definition type against their schema and
constructs the resulting definitions.
</my-summary>

<my-remarks>
<p>
The <code>Arcadia.ADL.Reader</code> type is the base type of the concrete readers of
the module, one per definition type. A reader lives in the context registry and is
looked up by the string <code>type</code> entry of an asset. Applications normally do
not call the methods of a reader directly; they read assets through a context.
</p>
<p>
Each concrete reader owns the schema of its type, given in the Arcadia Data
Definition Schema Language (Arcadia DDLS), and validates every asset against it
before constructing the definition.
</p>
</my-remarks>

</section>