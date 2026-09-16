<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity object">

  <h1 id="Arcadia_Visuals_CanvasActivationStateChangedEvent">[MIL] Arcadia.Visuals.CanvasActivationStateChangedEvent</h1>

  <my-signature><code>
  class Arcadia.Visuals.CanvasActivationStateChangedEvent extends Arcadia.Visuals.CanvasEvent { ... }
  </code></my-signature>

  <my-summary>
  A canvas activation state changed event.
  </my-summary>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<!-- TODO: This should be "mil" scope not "cxx" scope. -->
<section class="cxx entity method">

  <h1 id="Arcadia.Visuals.CanvasActivationStateChangedEvent.construct">
  [MIL] Arcadia.Visuals.CanvasActivationStateChangedEvent.construct
  </h1>

  <my-signature><code>
    void<br>
    Arcadia.Visuals.CanvasActivationStateChangedEvent.construct<br>
    &nbsp;&nbsp;(<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia.Natural64 timeStamp,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia.Boolean active<br>
    &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Construct this canvas activation state changed event.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div>
      <div>Arcadia.Natural64 timeStamp</div>
      <div>The timestamp of this event.</div>
    </div>

    <div>
      <div>Arcadia.Boolean active</div>
      <div><code>Arcadia.Boolean.True</code> if the canvas activation state is active. <code>Arcadia.Boolean.False</code> if the canvas activation state is inactive.</div>
    </div>

  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
