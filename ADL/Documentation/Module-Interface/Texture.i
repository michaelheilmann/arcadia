<section class="cxx entity class">
<h1 id="Arcadia_ADL_TextureDefinition">Arcadia_ADL_TextureDefinition</h1>
<my-signature><code>
Arcadia_ADL_TextureDefinition
</code></my-signature>

<my-summary>
The definition of a texture: a pixel buffer sampled with a magnification filter,
a minification filter, and a texture address mode for each texture coordinate.
</my-summary>

<my-remarks>
<p>
The type name of the definition, used as the <code>type</code> entry of an ADL
asset, is <code>"Texture"</code>. The texture holds a reference to the pixel
buffer it samples, resolved when the definition is linked; the referenced
definition must be a <code>PixelBuffer</code> definition.
</p>
<p>
The sampling properties <code>magnificationFilter</code>, <code>minificationFilter</code>,
<code>addressModeU</code>, and <code>addressModeV</code> are optional. The value
<code>None</code> is the default value and indicates that the sampling property is not
specified, in which case the default of the engine is used.
</p>
</my-remarks>

<section class="cxx methods">
<h1>Methods</h1>

<section class="cxx method">
<h1 id="Arcadia_ADL_TextureDefinition_create">Arcadia_ADL_TextureDefinition_create</h1>
<my-signature><code>
Arcadia_ADL_TextureDefinition* Arcadia_ADL_TextureDefinition_create
&nbsp;&nbsp;(Arcadia_Thread* thread,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_Definitions* definitions,
&nbsp;&nbsp;&nbsp;Arcadia_String* name,
&nbsp;&nbsp;&nbsp;Arcadia_String* pixelBufferName,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_TextureFilter magnificationFilter,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_TextureFilter minificationFilter,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_TextureAddressMode addressModeU,
&nbsp;&nbsp;&nbsp;Arcadia_ADL_TextureAddressMode addressModeV)
</code></my-signature>

<my-summary>
Create the definition of a texture.
</my-summary>

<section class="cxx parameters">
<h1>Parameters</h1>
<div>
  <div>thread</div>
  <div>The thread.</div>
</div>
<div>
  <div>definitions</div>
  <div>The container the definition belongs to.</div>
</div>
<div>
  <div>name</div>
  <div>The name of the definition.</div>
</div>
<div>
  <div>pixelBufferName</div>
  <div>The name of the referenced pixel buffer definition.</div>
</div>
<div>
  <div>magnificationFilter</div>
  <div>The magnification filter, an element of the <code>Arcadia_ADL_TextureFilter</code> enumeration. The element <code>None</code> selects the default of the engine.</div>
</div>
<div>
  <div>minificationFilter</div>
  <div>The minification filter, an element of the <code>Arcadia_ADL_TextureFilter</code> enumeration. The element <code>None</code> selects the default of the engine.</div>
</div>
<div>
  <div>addressModeU</div>
  <div>The texture address mode of the U texture coordinate, an element of the <code>Arcadia_ADL_TextureAddressMode</code> enumeration. The element <code>None</code> selects the default of the engine.</div>
</div>
<div>
  <div>addressModeV</div>
  <div>The texture address mode of the V texture coordinate, an element of the <code>Arcadia_ADL_TextureAddressMode</code> enumeration. The element <code>None</code> selects the default of the engine.</div>
</div>
</section>

<section class="cxx return-value">
<h1>Return value</h1>
The definition of the texture.
</section>

<section class="cxx errors">
<h1>Errors</h1>
<div>Raises <code>Arcadia_Status_AllocationFailed</code> if the definition cannot be allocated.</div>
</section>
</section>

</section>

</section>