<section class="cxx entity enumeration">
<h1 id="Arcadia_Engine_Visuals_CullMode">Arcadia_Engine_Visuals_CullMode</h1>
<my-signature><code>
typedef enum Arcadia_Engine_Visuals_CullMode Arcadia_Engine_Visuals_CullMode;
</code></my-signature>

<my-summary>
An enumeration of cull modes.
A cull mode identifies a technique for deciding wether a fragment is removed based on its facing towards the viewer.
</my-summary>

<section class="cxx enumeration-elements">

  <h1>Elements</h1>

  <div>
    <div><code>Arcadia_Engine_Visuals_CullMode_None</code></div>
    <div>Neither front-facing nor back-facing fragments are culled.</div>
  </div>
  <div>
    <div><code>Arcadia_Engine_Visuals_CullMode_Front</code></div>
    <div>Front-facing fragments are culled.</div>
  </div>
  <div>
    <div><code>Arcadia_Engine_Visuals_CullMode_Back</code></div>
    <div>Back-facing fragments are culled.</div>
  </div>
  <div>
    <div><code>Arcadia_Engine_Visuals_CullMode_FrontAndBack</code></div>
    <div>
    Both front-facing and back-facing fragments are culled.
    This is an alias of <code>Arcadia_Engine_Visuals_CullMode_BackAndFront</code>.
    </div>
  </div>
  <div>
    <div><code>Arcadia_Engine_Visuals_CullMode_BackAndFront</code></div>
    <div>
    Both back-facing and front-facing fragments are culled.
    This is an alias of <code>Arcadia_Engine_Visuals_CullMode_FrontAndBack</code>.
    </div>
  </div>

</section>

</section>
