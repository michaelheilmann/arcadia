<section class="cxx entity enumeration">
<h1 id="Arcadia_Engine_Visuals_VertexElementSemantics">Arcadia_Engine_Visuals_VertexElementSemantics</h1>
<my-signature><code>
typedef enum Arcadia_Engine_Visuals_VertexElementSemantics Arcadia_Engine_Visuals_VertexElementSemantics;
</code></my-signature>

<my-summary>
An enumeration of pixel formats.
</my-summary>

<section class="cxx enumeration-elements">
  <h1>Elements</h1>

  <div>
    <div>
      <code>Arcadia_Engine_Visuals_VertexElementSemantics_PositionXyz</code>
    </div>
    <div>
    The vertex element denotes a position coordinate.
    If the vertex syntax is Arcadia_Engine_Visuals_VertexElementSyntax_Real32Real32, then this the position coordinate is in XY space (model, local, or global).
    If the vertex syntax is Arcadia_Engine_Visuals_VertexElementSyntax_Real32Real32Real32, then this the position coordinate is in XYZ space (model, local, or global).
    </div>
  </div>

  <div>
    <div>
      <code>Arcadia_Engine_Visuals_VertexElementSemantics_AmbientRgba</code>
    </div>
    <div>
    The vertex element denotes an ambient color coordinate.
    <ul>
      <li>If the vertex syntax is Arcadia_Engine_Visuals_VertexElementSyntax_Real32Real32Real32, then the color is in [0,1] RGB space.
      The first Arcadia_Real32Value indicates the intensity of the red component,
      the second Arcadia_Real32Value indicates the intensity of the green component,
      the third Arcadia_Real32Value indicates the intensity of the blue component.
      0 indicates minimum intensity, 1 indicates maximum intensity.
      </li>
      <li>If the vertex syntax is Arcadia_Engine_Visuals_VertexElementSyntax_Real32Real32Real32Real32, then the color is in [0,1] RGBA space.
      The first Arcadia_Real32Value indicates the intensity of the red component,
      the second Arcadia_Real32Value indicates the intensity of the green component,
      the third Arcadia_Real32Value indicates the intensity of the blue component.
      0 indicates minimum intensity, 1 indicates maximum intensity.
      The fourth Arcadia_Real32Value indicates the intensity of the alpha component.
      0 indicates fully transparent, 1 indicates fully opaque.
      </li>
    </ul>
    </</div>
  </div>

  <div>
    <div>
      <code>Arcadia_Engine_Visuals_VertexElementSemantics_AmbientUv</code>
    </div>
    <div>
    The vertex element denotes a texture coordinates for an ambient texture.
    If the vertex syntax is Arcadia_Engine_Visuals_VertexElementSyntax_Real32Real32, then this the texture coordinate is in UV space.
    </div>
  </div>

</section>

</section>
