<section class="cxx entity enumeration">
<h1 id="Arcadia_Engine_Visuals_DepthCompareFunction">Arcadia_Engine_Visuals_DepthCompareFunction</h1>
<my-signature><code>
typedef enum Arcadia_Engine_Visuals_DepthCompareFunction Arcadia_Engine_Visuals_DepthCompareFunction;
</code></my-signature>

<my-summary>
An enumeration of depth compare functions.
A depth compare function identifies a technique that compares depth value of an incoming fragment to its corresponding depth value in the depth buffer.
</my-summary>

<section class="cxx enumeration-elements">

  <h1>Elements</h1>

  <div>
    <div><code>Arcadia_Engine_Visuals_DepthComparefunction_Always</code></div>
    <div>
    The incoming fragment always passes.
    </div>
  </div>

  <div>
    <div><code>Arcadia_Engine_Visuals_DepthCompareFunction_Never</code></div>
    <div>
    The incoming fragment never passes.
    </div>
  </div>

  <div>
    <div><code>Arcadia_Engine_Visuals_DepthCompareFunction_EqualTo</code></div>
    <div>
    The incoming fragment passes if its depth value is equal to its corresponding depth value in the depth buffer.
    </div>
  </div>

  <div>
    <div><code>Arcadia_Engine_Visuals_DepthCompareFunction_NotEqualTo</code></div>
    <div>
    The incoming fragment passes if its depth value is not equal to its corresponding depth value in the depth buffer.
    </div>
  </div>

  <div>
    <div><code>Arcadia_Engine_Visuals_DepthCompareFunction_LessThan</code></div>
    <div>
    The incoming fragment passes if its depth value is less than its corresponding depth value in the depth buffer.
    </div>
  </div>

  <div>
    <div><code>Arcadia_Engine_Visuals_DepthCompareFunction_LessThanOrEqualTo</code></div>
    <div>
    The incoming fragment passes if its depth value is less than or equal to its corresponding depth value in the depth buffer.
    </div>
  </div>

  <div>
    <div><code>Arcadia_Engine_Visuals_DepthCompareFunction_GreaterThan</code></div>
    <div>
    The incoming fragment passes if its depth value is greater than its corresponding depth value in the depth buffer.
    </div>
  </div>

  <div>
    <div><code>Arcadia_Engine_Visuals_DepthCompareFunction_GreaterThanOrEqualTo</code></div>
    <div>
    The incoming fragment passes if its depth value is greater than or equal to its corresponding depth value in the depth buffer.
    </div>
  </div>

</section>

</section>
