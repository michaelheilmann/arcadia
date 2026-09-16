<section class="cxx entity enumeration">
<h1 id="Arcadia_ADL_BlendFunction">Arcadia_ADL_BlendFunction</h1>
<my-signature><code>
typedef enum Arcadia_ADL_BlendFunction Arcadia_ADL_BlendFunction;
</code></my-signature>

<my-summary>
An enumeration of blend functions.
A blend function identifies the factor the source or the destination color is multiplied with when blending is enabled.
The values correspond to the blend functions of the engine and to the blend factors of OpenGL.
</my-summary>

<my-remarks>
<p>
The value <code>None</code> is used as the default value and indicates that blending is
disabled and hence no blend functions are applied. In ADL assets, each element is
identified by a string literal of the same name, e.g. <code>"Zero"</code> or
<code>"SourceAlpha"</code>.
</p>
</my-remarks>

<section class="cxx enumeration-elements">

  <h1>Elements</h1>

  <div>
    <div><code>Arcadia_ADL_BlendFunction_None</code></div>
    <div>
    Blending is disabled. This is the default value.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_BlendFunction_Zero</code></div>
    <div>
    The factor (0,0,0,0).
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_BlendFunction_One</code></div>
    <div>
    The factor (1,1,1,1).
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_BlendFunction_SourceColor</code></div>
    <div>
    The factor is the color of the source.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_BlendFunction_OneMinusSourceColor</code></div>
    <div>
    The factor is one minus the color of the source.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_BlendFunction_DestinationColor</code></div>
    <div>
    The factor is the color of the destination.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_BlendFunction_OneMinusDestinationColor</code></div>
    <div>
    The factor is one minus the color of the destination.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_BlendFunction_SourceAlpha</code></div>
    <div>
    The factor is the alpha of the source.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_BlendFunction_OneMinusSourceAlpha</code></div>
    <div>
    The factor is one minus the alpha of the source.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_BlendFunction_DestinationAlpha</code></div>
    <div>
    The factor is the alpha of the destination.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_BlendFunction_OneMinusDestinationAlpha</code></div>
    <div>
    The factor is one minus the alpha of the destination.
    </div>
  </div>

</section>

</section>

<section class="cxx entity enumeration">
<h1 id="Arcadia_ADL_AmbientColorSource">Arcadia_ADL_AmbientColorSource</h1>
<my-signature><code>
typedef enum Arcadia_ADL_AmbientColorSource Arcadia_ADL_AmbientColorSource;
</code></my-signature>

<my-summary>
An enumeration of ambient color sources.
An ambient color source identifies where the ambient color of a material is taken from.
</my-summary>

<my-remarks>
<p>
In ADL assets, each element is identified by a string literal of the same name, e.g.
<code>"Mesh"</code> or <code>"Texture"</code>.
</p>
</my-remarks>

<section class="cxx enumeration-elements">

  <h1>Elements</h1>

  <div>
    <div><code>Arcadia_ADL_AmbientColorSource_Mesh</code></div>
    <div>
    The ambient color is the ambient color of the mesh.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_AmbientColorSource_Vertex</code></div>
    <div>
    The ambient color is taken from the vertices of the mesh.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_AmbientColorSource_Texture</code></div>
    <div>
    The ambient color is sampled from a texture.
    </div>
  </div>

</section>

</section>

<section class="cxx entity enumeration">
<h1 id="Arcadia_ADL_MaterialType">Arcadia_ADL_MaterialType</h1>
<my-signature><code>
typedef enum Arcadia_ADL_MaterialType Arcadia_ADL_MaterialType;
</code></my-signature>

<my-summary>
An enumeration of material types.
A material type identifies the illumination model a material is evaluated with.
</my-summary>

<my-remarks>
<p>
In ADL assets, the element <code>BlinnPhong</code> is identified by the string literal
<code>"Blinn-Phong"</code>.
</p>
</my-remarks>

<section class="cxx enumeration-elements">

  <h1>Elements</h1>

  <div>
    <div><code>Arcadia_ADL_MaterialType_BlinnPhong</code></div>
    <div>
    The material is evaluated with the Blinn-Phong illumination model.
    </div>
  </div>

</section>

</section>

<section class="cxx entity enumeration">
<h1 id="Arcadia_ADL_TextureFilter">Arcadia_ADL_TextureFilter</h1>
<my-signature><code>
typedef enum Arcadia_ADL_TextureFilter Arcadia_ADL_TextureFilter;
</code></my-signature>

<my-summary>
An enumeration of texture magnification and minification filters.
A texture filter identifies a technique to compute the pixel value closest to the specified uv coordinates.
The values correspond to the texture filters of the engine.
</my-summary>

<my-remarks>
<p>
The value <code>None</code> is used as the default value and indicates that no filter is
specified and the engine's default filter is used. In ADL assets, each element is
identified by a string literal of the same name, e.g. <code>"Linear"</code> or
<code>"Nearest"</code>.
</p>
</my-remarks>

<section class="cxx enumeration-elements">

  <h1>Elements</h1>

  <div>
    <div><code>Arcadia_ADL_TextureFilter_None</code></div>
    <div>
    No filter is specified. This is the default value.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_TextureFilter_Linear</code></div>
    <div>
    Linear filtering, e.g. bilinear interpolation.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_TextureFilter_Nearest</code></div>
    <div>
    Nearest-neighbor filtering.
    </div>
  </div>

</section>

</section>

<section class="cxx entity enumeration">
<h1 id="Arcadia_ADL_TextureAddressMode">Arcadia_ADL_TextureAddressMode</h1>
<my-signature><code>
typedef enum Arcadia_ADL_TextureAddressMode Arcadia_ADL_TextureAddressMode;
</code></my-signature>

<my-summary>
An enumeration of texture address modes for the U and V texture coordinates.
A texture address mode identifies a technique for resolving uv coordinates that are outside of the boundaries of a texture.
The values correspond to the texture address modes of the engine.
</my-summary>

<my-remarks>
<p>
The value <code>None</code> is used as the default value and indicates that no address mode
is specified and the engine's default address mode is used. In ADL assets, each element is
identified by a string literal of the same name, e.g. <code>"ClampToEdge"</code> or
<code>"Repeat"</code>.
</p>
</my-remarks>

<section class="cxx enumeration-elements">

  <h1>Elements</h1>

  <div>
    <div><code>Arcadia_ADL_TextureAddressMode_None</code></div>
    <div>
    No address mode is specified. This is the default value.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_TextureAddressMode_ClampToBorder</code></div>
    <div>
    Texture coordinates are clamped to the border.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_TextureAddressMode_ClampToEdge</code></div>
    <div>
    Texture coordinates are clamped to the edge.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_TextureAddressMode_MirroredRepeat</code></div>
    <div>
    Texture coordinates are mirrored and then repeated.
    </div>
  </div>
  <div>
    <div><code>Arcadia_ADL_TextureAddressMode_Repeat</code></div>
    <div>
    Texture coordinates are repeated.
    </div>
  </div>

</section>

</section>