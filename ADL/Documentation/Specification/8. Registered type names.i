      <h2 id="registered-type-names">8 Registered type names</h2>
      <p>
      The <code>Arcadia.ADL</code> module registers exactly one reader per type name
      below. A <code>type</code> entry not listed here is a semantical error. The
      table is complete as of this draft and is the authoritative inventory of asset
      types; the sections that specify the types give the entry lists.
      </p>
      <table>
        <thead><tr><th>Type name</th><th>Section</th></tr></thead>
        <tbody>
          <tr><td><code>"Color"</code></td><td>4 Visuals definitions</td></tr>
          <tr><td><code>"Mesh"</code></td><td>4 Visuals definitions</td></tr>
          <tr><td><code>"BoxMeshGenerator"</code></td><td>4 Visuals definitions</td></tr>
          <tr><td><code>"PlaneMeshGenerator"</code></td><td>4 Visuals definitions</td></tr>
          <tr><td><code>"SphereMeshGenerator"</code></td><td>4 Visuals definitions</td></tr>
          <tr><td><code>"PixelBuffer"</code></td><td>4 Visuals definitions</td></tr>
          <tr><td><code>"PixelBufferOperations.FillOperation"</code></td><td>4 Visuals definitions</td></tr>
          <tr><td><code>"PixelBufferOperations.CheckerboardFillOperation"</code></td><td>4 Visuals definitions</td></tr>
          <tr><td><code>"Texture"</code></td><td>4 Visuals definitions</td></tr>
          <tr><td><code>"Material"</code></td><td>4 Visuals definitions</td></tr>
          <tr><td><code>"Model"</code></td><td>4 Visuals definitions</td></tr>
          <tr><td><code>"SampleBuffer"</code></td><td>5 Audials definitions</td></tr>
          <tr><td><code>"DSP.Constant"</code></td><td>5 Audials definitions</td></tr>
          <tr><td><code>"DSP.SineWave"</code></td><td>5 Audials definitions</td></tr>
          <tr><td><code>"DSP.SawtoothWave"</code></td><td>5 Audials definitions</td></tr>
          <tr><td><code>"DSP.WhiteNoise"</code></td><td>5 Audials definitions</td></tr>
          <tr><td><code>"UI.Button"</code></td><td>6 User interface definitions</td></tr>
          <tr><td><code>"UI.Label"</code></td><td>6 User interface definitions</td></tr>
          <tr><td><code>"UI.StackPanel"</code></td><td>6 User interface definitions</td></tr>
          <tr><td><code>"UI.SegmentedChoice"</code></td><td>6 User interface definitions</td></tr>
          <tr><td><code>"UI.Slider"</code></td><td>6 User interface definitions</td></tr>
          <tr><td><code>"UI.Toggle"</code></td><td>6 User interface definitions</td></tr>
        </tbody>
      </table>
      <p>
      Twenty-two type names are registered. The base type <code>MeshGenerator</code>
      is not among them, so no asset may use it as its <code>type</code> entry; it
      occurs only in the requirement on the <code>mesh</code> entry of a
      <code>"Model"</code> definition in section 4. The definition types
      <code>"Canvas"</code>, <code>"Container"</code>, and <code>"ScrollPanel"</code>
      of section 6 have no reader and therefore do not appear.
      </p>
