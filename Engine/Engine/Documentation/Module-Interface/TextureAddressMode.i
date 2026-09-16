<section class="cxx entity enumeration">
<h1 id="Arcadia_Engine_Visuals_TextureAddressMode">Arcadia_Engine_Visuals_TextureAddressMode</h1>
<my-signature><code>
typedef enum Arcadia_Engine_Visuals_TextureAddressMode Arcadia_Engine_Visuals_TextureAddressMode;
</code></my-signature>

<my-summary>
An enumeration of texture address modes.
A texture address mode identifies a technique for resolving uv coordinates that are outside of the boundaries of a texture.
</my-summary>

<section class="cxx enumeration-elements">

  <h1>Elements</h1>

  <div>
    <div><code>Arcadia_Engine_Visuals_TextureAddressMode_ClampToEdge</code></div>
    <div>
    uv coordinates outside of the range of [0,1] are assigned the color of the uv coordinate at 0 or 1, respectively.
    This usually corresponds to GL_TEXTURE_WRAP_S/GL_TEXTURE_WRAP_T with GL_CLAMP_TO_EDGE in OpenGL and D3D12_TEXTURE_ADDRESS_MODE_CLAMP in Direct3D 12.
    </div>
  </div>

  <div>
    <div><code>Arcadia_Engine_Visuals_TextureAddressMode_ClampToBorder</code></div>
    <div>
    uv coordinates outside of the range of [0,1] are assigned the border color of the texture.
    This usually corresponds to GL_TEXTURE_WRAP_S/GL_TEXTURE_WRAP_T with GL_CLAMP_TO_BORDER in OpenGL and D3D12_TEXTURE_ADDRESS_MODE_BORDER in Direct3D 12.
    </div>
  </div>

  <div>
    <div><code>Arcadia_Engine_Visuals_TextureAddressMode_Repeat</code></div>
    <div>
    Tile the texture at every uv coordinate integer junction.
    For example, for the u coordinates between 0 and 3, the texture is repeated three times.
    This usually corresponds to GL_TEXTURE_WRAP_S/GL_TEXTURE_WRAP_T with GL_REPEAT in OpenGL and D3D12_TEXTURE_ADDRESS_MODE_WRAP in Direct3D 12.
    </div>
  </div>

  <div>
    <div><code>Arcadia_Engine_Visuals_TextureAddressMode_MirroredRepeat</code></div>
    <div>
    Tile and flip the texture at every uv coordinate integer junction.
    For example, for the u coordinates between 0 and 3, the texture is repeated three times.
    For the u coordinates between 0 and 1 and 2 and 3 the texture is address normally.
    For the u coordinates between 1 and 2, the texture flipped.
    This usually corresponds to GL_TEXTURE_WRAP_S/GL_TEXTURE_WRAP_T with GL_MIRRORED_REPEAT in OpenGL, D3D12_TEXTURE_ADDRESS_MODE_MIRROR in Direct3D 12.
    </div>
  </div>

</section>

</section>
