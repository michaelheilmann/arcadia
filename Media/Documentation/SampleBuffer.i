<section class="cxx entity object">
<h1 id="Arcadia_Media_SampleBuffer">Arcadia_Media_SampleBuffer</h1>
<p><code>Arcadia_Media_SampleBuffer</code> extends <code>Arcadia_Object</code>.</p>
<p>
<code>Arcadia_Media_SampleBuffer</code> is a one-dimensional array of samples.
</p>
<p>PCM bytes are owned by a fixed-size, mutable, contiguous
<code>Arcadia_ByteArray</code> referenced by <code>byteStorage</code> and
retained through GC visitation. Native DSP and audio processing uses direct,
packed PCM memory without per-sample boxing. Clones own independent storage.</p>
<p>Native C consumers obtain the owning ByteArray using
<code>Arcadia_Media_SampleBuffer_getSampleBytes</code>, then borrow contiguous
storage using <code>Arcadia_ByteArray_getBytes</code> or
<code>Arcadia_ByteArray_getMutableBytes</code>. Acquire the pointer only when
needed; keep the SampleBuffer rooted while using it. No separate raw-pointer
field is stored, and pointers must not be transported through Arcadia.Value or
PDL. Do not resize SampleBuffer's ByteArray.</p>
<p>The following terms and definitions are used througout its documentation:</p>
<ul>
  <li><em>sample</em>: An integral value representing an amplitude value.
  A sample is uniquely identified by a zero-based integral index.
  </li>
  <li>
  <em>length</em>: The length, in seconds, of the sample buffer.
  </li>
  <li>
  <em>sample rate</em>: The sample rate of the sample buffer.
  </li>
  <li>
  <em>sample format</em>: The sample format of the samples of the sample buffer as defined by <a href="@{siteAddress}/Arcadia/Media/#Arcadia_Media_SampleFormat">Arcadia_Media_SampleFormat</a>.
  </li>
</ul>
</section>
