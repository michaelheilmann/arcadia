<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity object">

  <h1 id="Arcadia_Visuals_CanvasDPIChangedEvent">[MIL] Arcadia.Visuals.CanvasDPIChangedEvent</h1>

  <my-signature><code>
  class Arcadia.Visuals.CanvasDPIChangedEvent extends Arcadia.Visuals.CanvasEvent { ... }
  </code></my-signature>

  <my-summary>
  A canvas DPI changed event.
  </my-summary>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<!-- TODO: This should be "mil" scope not "cxx" scope. -->
<section class="cxx entity method">

  <h1 id="Arcadia.Visuals.CanvasDPIChangedEvent.construct">
  [MIL] Arcadia.Visuals.CanvasDPIChangedEvent.construct
  </h1>

  <my-signature><code>
    void<br>
    Arcadia.Visuals.CanvasDPIChangedEvent.construct<br>
    &nbsp;&nbsp;(<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia.Natural64 timeStamp,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia.Natural32 horizontalDpi,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia.Natural32 verticalDpi<br>
    &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Construct this canvas DPI changed event.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div>
      <div>Arcadia.Natural64 timeStamp</div>
      <div>The timestamp of this event.</div>
    </div>

    <div>
      <div>Arcadia.Integer32 horizontalDpi</div>
      <div>The horizontal DPI of the canvas.</div>
    </div>

    <div>
      <div>Arcadia.Integer32 verticalDpi</div>
      <div>The vertical DPI of the canvas.</div>
    </div>

  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
