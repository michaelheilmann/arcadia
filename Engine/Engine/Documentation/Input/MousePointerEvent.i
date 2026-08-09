<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity object">

  <h1 id="Arcadia_Engine_Input_MouseButtonEvent">
  [MIL] Arcadia.Engine.Input.MouseButtonEvent
  </h1>

  <my-signature><code>
  class Arcadia.Engine.Input.MouseButtonEvent extends Arcadia.Engine.Event { ... }
  </code></my-signature>

  <my-summary>
  A mouse button event.
  </my-summary>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<!-- TODO: This should be "mil" scope not "cxx" scope. -->
<section class="cxx entity method">

  <h1 id="Arcadia_Engine_Input_MouseButtonEvent_constructor">
  [MIL] constructor
  </h1>

  <my-signature><code>
    constructor<br>
    &nbsp;&nbsp;(<br>
    &nbsp;&nbsp;&nbsp;&nbsp;timestamp : Arcadia.Natural64 ,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;action : Arcadia.Visuals.MousePointerAction,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;x : Arcadia.Integer32,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;y : Arcadia.Integer32<br>
    &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Construct this mouse button event.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div>
      <div>timestamp : Arcadia.Natural64</div>
      <div>The timestamp of this event.</div>
    </div>

    <div>
      <div>action : Arcadia.Engine.Input.MousePointerAction</div>
      <div>The mouse pointer action.</div>
    </div>

    <div>
      <div>x : Arcadia.Integer32</div>
      <div>The position along the x-axis of the mouse pointer.</div>
    </div>

    <div>
      <div>y : Arcadia.Integer32</div>
      <div>The position along the y-axis of the mouse pointer.</div>
    </div>

  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
