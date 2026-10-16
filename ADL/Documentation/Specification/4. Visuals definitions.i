      <h2 id="visuals">4 Visuals definitions</h2>
      <p>
      The visuals definitions describe colors, geometry, pixels, materials, and the
      models composed from them.
      </p>

      <h3>Color</h3>
      <p>
      Type name: <code>"Color"</code>. A color with red, green, and blue components.
      Holds no references and therefore links trivially.
      </p>
      <pre>
"type"  : "Color"
"name"  : &lt;string&gt;
"red"   : &lt;number&gt;    // Natural8, the red component
"green" : &lt;number&gt;    // Natural8, the green component
"blue"  : &lt;number&gt;    // Natural8, the blue component
      </pre>

      <h3>Mesh</h3>
      <p>Type name: <code>"Mesh"</code>. A polygonal mesh given explicitly by its vertex data.</p>
      <pre>
"type"                                : "Mesh"
"name"                                : &lt;string&gt;
"ambientColor"                        : &lt;string&gt;   // reference to a Color definition
"vertexPositions"                     : &lt;list of numbers&gt;   // Real32
"vertexAmbientColors"                 : &lt;list of numbers&gt;   // Real32
"vertexAmbientColorTextureCoordinates" : &lt;list of numbers&gt;   // Real32
      </pre>
      <p>
      The number of vertices is derived, not written: it is one third of the length of
      <code>vertexPositions</code>. There is no <code>numberOfVertices</code> entry.
      The three lists are read into byte arrays and must satisfy the following; any
      violation is a semantical error:
      </p>
      <ul>
        <li>the length of <code>vertexPositions</code> is a multiple of three,</li>
        <li>the length of <code>vertexAmbientColors</code> is four times the number of vertices, and</li>
        <li>the length of <code>vertexAmbientColorTextureCoordinates</code> is twice the number of vertices.</li>
      </ul>
      <p>
      The <code>ambientColor</code> entry is stored as a name; the current
      implementation does not resolve it while linking.
      </p>

      <h3>Mesh generators</h3>
      <p>
      A mesh generator describes a mesh procedurally. <code>"Mesh"</code> and the
      three generator types below share the base type <code>MeshGenerator</code>, so a
      reference required to denote a mesh generator accepts any of the four.
      </p>
      <p>
      Every mesh generator carries the ambient color of the mesh it generates. As with
      <code>"Mesh"</code>, the <code>ambientColor</code> entry is stored as a name and
      is not resolved while linking.
      </p>
      <p>Type name: <code>"BoxMeshGenerator"</code>. Generates a box with the given width, height, and depth.</p>
      <pre>
"type"         : "BoxMeshGenerator"
"name"         : &lt;string&gt;
"width"        : &lt;number&gt;   // Real32
"height"       : &lt;number&gt;   // Real32
"depth"        : &lt;number&gt;   // Real32
"ambientColor" : &lt;string&gt;   // reference to a Color definition
      </pre>
      <p>Type name: <code>"PlaneMeshGenerator"</code>. Generates a plane with the given width and height.</p>
      <pre>
"type"         : "PlaneMeshGenerator"
"name"         : &lt;string&gt;
"width"        : &lt;number&gt;   // Real32
"height"       : &lt;number&gt;   // Real32
"ambientColor" : &lt;string&gt;   // reference to a Color definition
      </pre>
      <p>Type name: <code>"SphereMeshGenerator"</code>. Generates a sphere with the given radius and number of segments.</p>
      <pre>
"type"         : "SphereMeshGenerator"
"name"         : &lt;string&gt;
"radius"       : &lt;number&gt;   // Real32
"segments"     : &lt;number&gt;   // Natural32, the number of segments
"ambientColor" : &lt;string&gt;   // reference to a Color definition
      </pre>

      <h3>PixelBuffer</h3>
      <p>Type name: <code>"PixelBuffer"</code>. A rectangular buffer of pixels.</p>
      <pre>
"type"       : "PixelBuffer"
"name"       : &lt;string&gt;
"width"      : &lt;number&gt;   // Natural32
"height"     : &lt;number&gt;   // Natural32
"operations" : &lt;list of strings&gt;   // references to pixel-buffer operation definitions
      </pre>
      <p>
      While linking, each operation reference is resolved, must be a pixel-buffer
      operation definition, and is linked recursively.
      </p>

      <h3>Pixel-buffer operations</h3>
      <p>
      Pixel-buffer operations produce the content of a pixel buffer. They are
      referenced by the <code>operations</code> entry of a <code>"PixelBuffer"</code>
      definition.
      </p>
      <p>Type name: <code>"PixelBufferOperations.FillOperation"</code>. Fills a pixel buffer with a color.</p>
      <pre>
"type"  : "PixelBufferOperations.FillOperation"
"name"  : &lt;string&gt;
"color" : &lt;string&gt;   // reference to a Color definition
      </pre>
      <p>
      While linking, the <code>color</code> reference is resolved and must be a
      <code>"Color"</code> definition.
      </p>
      <p>Type name: <code>"PixelBufferOperations.CheckerboardFillOperation"</code>. Fills a pixel buffer with a checkerboard pattern.</p>
      <pre>
"type"               : "PixelBufferOperations.CheckerboardFillOperation"
"name"               : &lt;string&gt;
"firstCheckerColor"  : &lt;string&gt;   // reference to a Color definition
"secondCheckerColor" : &lt;string&gt;   // reference to a Color definition
"checkerWidth"       : &lt;number&gt;   // Integer32
"checkerHeight"      : &lt;number&gt;   // Integer32
      </pre>
      <p>
      While linking, both color references are resolved and each must be a
      <code>"Color"</code> definition.
      </p>

      <h3>Texture</h3>
      <p>Type name: <code>"Texture"</code>. A texture; a pixel buffer together with sampling properties.</p>
      <pre>
"type"                : "Texture"
"name"                : &lt;string&gt;
"pixelBuffer"         : &lt;string&gt;   // reference to a PixelBuffer definition
"magnificationFilter" : &lt;string&gt;   // optional, a TextureFilter literal
"minificationFilter"  : &lt;string&gt;   // optional, a TextureFilter literal
"addressModeU"        : &lt;string&gt;   // optional, a TextureAddressMode literal
"addressModeV"        : &lt;string&gt;   // optional, a TextureAddressMode literal
      </pre>
      <p>
      An omitted sampling property is the <code>None</code> element of its
      enumeration; the engine then uses its own default. An unknown literal is a
      semantical error. While linking, the <code>pixelBuffer</code> reference is
      resolved and must be a <code>"PixelBuffer"</code> definition.
      </p>

      <h3>Material</h3>
      <p>Type name: <code>"Material"</code>. The material a model is rendered with.</p>
      <pre>
"type"                    : "Material"
"name"                    : &lt;string&gt;
"materialType"            : &lt;string&gt;   // a MaterialType literal
"ambientColorSource"      : &lt;string&gt;   // an AmbientColorSource literal
"ambientColorTexture"     : &lt;string&gt;   // reference to a Texture definition
"blendSourceFunction"     : &lt;string&gt;   // optional, a BlendFunction literal
"blendDestinationFunction" : &lt;string&gt;   // optional, a BlendFunction literal
      </pre>
      <p>
      An omitted blend function is the <code>None</code> element of the
      <code>BlendFunction</code> enumeration. While linking, the
      <code>ambientColorTexture</code> reference is resolved and must be a
      <code>"Texture"</code> definition.
      </p>

      <h3>Model</h3>
      <p>Type name: <code>"Model"</code>. A model, a mesh rendered with a material.</p>
      <pre>
"type"     : "Model"
"name"     : &lt;string&gt;
"mesh"     : &lt;string&gt;   // reference to a Mesh or mesh generator definition
"material" : &lt;string&gt;   // reference to a Material definition
      </pre>
      <p>
      While linking, the <code>mesh</code> reference is resolved and must be a
      definition of the base type <code>MeshGenerator</code>, that is, a
      <code>"Mesh"</code>, <code>"BoxMeshGenerator"</code>,
      <code>"PlaneMeshGenerator"</code>, or <code>"SphereMeshGenerator"</code>
      definition; and the <code>material</code> reference is resolved and must be a
      <code>"Material"</code> definition.
      </p>
