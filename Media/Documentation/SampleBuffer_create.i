<section class="cxx entity method">

  <h1 id="Arcadia_Media_SampleBuffer_create">Arcadia_Media_SampleBuffer_create</h1>

  <my-signature><code>
  Arcadia_Media_SampleBuffer*<br>
  Arcadia_Media_SampleBuffer_create<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Integer32Value length,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Integer32Value sampleRate,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Media_SampleFormat sampleFormat,<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Create a sample buffer of the specified length, sample rate, and sample format.
  It is filled with silence.
  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>

    <div>
      <div><a href="#">Arcadia_Integer32Value</a> length</div>
      <div>The length of the sample buffer, in seconds. Must be non-negative.</div>
    </div>

    <div>
      <div><a href="#">Arcadia_Integer32Value</a> sampleRate</div>
      <div>The sample rate, in Hertz, of the sample buffer. Must be non-negative.</div>
    </div>

    <div>
      <div><a href="#">Arcadia_Media_SampleFormat</a> sampleFormat</div>
      <div>The sample format of the sample buffer.</div>
    </div>

  </section>

  <section class="cxx errors">
    <h1>Errors</h1>
    <div>
      <div>Arcadia_Status_AllocationFailed</div>
      <div>An allocation failed.</div>
    </div>
    <div>
      <div>Arcadia_Status_ArgumentValueInvalid</div>
      <div>At least one of <code>length</code>, <code>sampleRate</code> is negative.</div>
    </div>
  </section>

  <section class="cxx return-value">
    <h1>Return value</h1>
    <p>A pointer to the <code>Arcadia_Media_SampleBuffer</code> object.</p>
  </section>

</section>
