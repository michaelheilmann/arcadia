<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity object">

  <h1 id="Arcadia_Visuals_CanvasSizeChangedEvent">[MIL] Arcadia.Visuals.CanvasSizeChangedEvent</h1>

  <my-signature><code>
  class Arcadia.Visuals.CanvasSizeChangedEvent extends Arcadia.Visuals.CanvasEvent { ... }
  </code></my-signature>

  <my-summary>
  A canvas size changed event.
  </my-summary>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<!-- TODO: This should be "mil" scope not "cxx" scope. -->
<section class="cxx entity method">

  <h1 id="Arcadia.Visuals.CanvasSizeChangedEvent.construct">
  [MIL] Arcadia.Visuals.CanvasSizeChangedEvent.construct
  </h1>

  <my-signature><code>
    void<br>
    Arcadia.Visuals.CanvasSizeChangedEvent.construct<br>
    &nbsp;&nbsp;(<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia.Natural64 timeStamp,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia.Integer32 horizontalSize,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;Arcadia.Integer32 verticalSize<br>
    &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Construct this canvas size changed event.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div>
      <div>Arcadia.Natural64 timeStamp</div>
      <div>The timestamp of this event.</div>
    </div>

    <div>
      <div>Arcadia.Integer32 horizontalSize</div>
      <div>The horizontal size, in pixels, of the canvas.</div>
    </div>

    <div>
      <div>Arcadia.Integer32 verticalSize</div>
      <div>The vertical size, in pixels, of the canvas.</div>
    </div>

  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
