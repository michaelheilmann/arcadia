<section class="cxx entity enumeration">
<h1 id="Arcadia_Engine_Visuals_TextureFilter">Arcadia_Engine_Visuals_TextureFilter</h1>
<my-signature><code>
typedef enum Arcadia_Engine_Visuals_TextureFilter Arcadia_Engine_Visuals_TextureFilter;
</code></my-signature>

<my-summary>
An enumeration of texture filters.
A texture filter identifies a technique to compute the pixel value closed to the specified uv coordinates.
</my-summary>

<section class="cxx enumeration-elements">

  <h1>Elements</h1>

  <div>
    <div><code>Arcadia_Engine_Visuals_TextureFilter_Nearest</code></div>
    <div>
    Filter using the texture element that is nearest (in Manhatten distance) to the specified uv coordinates.
    </div>
  </div>
  <div>
    <div><code>Arcadia_Engine_Visuals_TextureFilter_Linear</code></div>
    <div>
    Returns the weighted average of the four texture elements that are closest to the specified uv coordinates.
    These can include items wrapped or repeated from other parts of the texture, depending on the
    <a href="@{siteAddress}/repository/Arcadia.Visuals/#Arcadia_Engine_Visuals_TextureAddressMode">texture address mode</a>.
    </div>
  </div>

</section>

</section>
