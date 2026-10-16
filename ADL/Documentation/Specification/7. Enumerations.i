      <h2 id="enumerations">7 Enumerations</h2>
      <p>
      Several string entries are enumerated. The literal of an enumeration element is
      a string, and an unknown literal is a semantical error. In the C interface of
      the <code>Arcadia.ADL</code> module each enumeration is a type of its own, and
      the <code>None</code> element is its default. The literals are:
      </p>
      <ul>
        <li><code>BlendFunction</code>: <code>"Zero"</code>, <code>"One"</code>, <code>"SourceColor"</code>, <code>"OneMinusSourceColor"</code>, <code>"DestinationColor"</code>, <code>"OneMinusDestinationColor"</code>, <code>"SourceAlpha"</code>, <code>"OneMinusSourceAlpha"</code>, <code>"DestinationAlpha"</code>, <code>"OneMinusDestinationAlpha"</code>.</li>
        <li><code>AmbientColorSource</code>: <code>"Mesh"</code>, <code>"Vertex"</code>, <code>"Texture"</code>, <code>"TextureVertex"</code>.</li>
        <li><code>MaterialType</code>: <code>"Blinn-Phong"</code>.</li>
        <li><code>TextureFilter</code>: <code>"Linear"</code>, <code>"Nearest"</code>.</li>
        <li><code>TextureAddressMode</code>: <code>"ClampToBorder"</code>, <code>"ClampToEdge"</code>, <code>"MirroredRepeat"</code>, <code>"Repeat"</code>.</li>
      </ul>
      <p>
      The alignment strings of section 6 are <i>not</i> drawn from an enumeration at
      the language level: the user-interface reader stores whatever string it is
      given. Boolean string values are likewise accepted as spelled by the DDL
      scanner, that is, as <code>"true"</code> and <code>"false"</code>.
      </p>
