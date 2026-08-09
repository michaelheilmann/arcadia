<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity object">

  <h1 id="Arcadia_Engine_Input_KeyboardKeyEvent">
  [MIL] Arcadia.Engine.Input.KeyboardKeyEvent
  </h1>

  <my-signature><code>
  class Arcadia.Engine.Input.KeyboardKeyEvent extends Arcadia.Engine.Event { ... }
  </code></my-signature>

  <my-summary>
  A keyboard key event.
  </my-summary>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity method">

  <h1 id="Arcadia_Engine_Input_KeyboardKeyEvent_constructor">
  [MIL] constructor
  </h1>

  <my-signature><code>
    constructor<br>
    &nbsp;&nbsp;(<br>
    &nbsp;&nbsp;&nbsp;&nbsp;timestamp : Arcadia.Natural64,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;action : Arcadia.Engine.Input.KeyboardKeyAction ,<br>
    &nbsp;&nbsp;&nbsp;&nbsp;key : Arcadia.Engine.Input.KeyboardKey<br>
    &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Construct this keyboard key event.
  </my-summary>

  <section class="cxx parameters">

    <h1>Parameters</h1>

    <div>
      <div>timestamp : Arcadia.Natural64</div>
      <div>The timestamp of this event.</div>
    </div>

    <div>
      <div>action : Arcadia.Engine.Input.KeyboardKeyAction</div>
      <div>The keyboard key action.</div>
    </div>

    <div>
      <div>key : Arcadia.Engine.Input.KeyboardKey</div>
      <div>The keyboard key.</div>
    </div>

  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity method">

  <h1 id="Arcadia_Engine_Input_KeyboardKeyEvent_getAction">
  [MIL] Arcadia.Engine.Input.KeyboardKeyEvent.getAction
  </h1>

  <my-signature><code>
    Arcadia.Engine.Input.KeyboardKeyEvent.getAction<br>
    &nbsp;&nbsp;(<br>
    &nbsp;&nbsp;) : Arcadia.Engine.Input.KeyboardKeyAction
  </code></my-signature>

  <my-summary>
  Get the keyboard key action of this keyboard key event.
  </my-summary>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>The keyboard key action.</p>
  </section>

</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

<section class="cxx entity method">

  <h1 id="Arcadia_Engine_Input_KeyboardKeyEvent_getKey">
  [MIL] Arcadia.Engine.Input.KeyboardKeyEvent.getKey
  </h1>

  <my-signature><code>
    Arcadia.Engine.Input.KeyboardKeyEvent.getKey<br>
    &nbsp;&nbsp;(<br>
    &nbsp;&nbsp;) : Arcadia.Engine.Input.KeyboardKey
  </code></my-signature>

  <my-summary>
  Get the keyboard key of this keyboard key event.
  </my-summary>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>The keyboard key.</p>
  </section>


</section>

<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->

