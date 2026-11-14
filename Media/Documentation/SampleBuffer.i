<section class="cxx entity object">
<h1 id="Arcadia_Media_SampleBuffer">Arcadia_Media_SampleBuffer</h1>
<p><code>Arcadia_Media_SampleBuffer</code> extends <code>Arcadia_Object</code>.</p>
<p>
<code>Arcadia_Media_SampleBuffer</code> is a one-dimensional array of samples.
</p>
<p>Sample data is owned by a fixed-size, mutable, contiguous
<code>Arcadia_ByteArray</code> held in <code>byteStorage</code> and traced by
the garbage collector. Samples remain in native packed PCM format;
quantization and native audio backends operate on contiguous bytes without
per-sample virtual dispatch. Creating a clone copies its PCM bytes into a
distinct owning ByteArray.</p>
<p>Native C callers may use <code>Arcadia_Media_SampleBuffer_getSampleBytes</code>
to access the managed owner or the existing <code>bytes</code> field as a
borrowed pointer. The latter is not an allocation owner and must never
be freed directly. Keep the SampleBuffer rooted while using borrowed
bytes, including during OpenAL uploads; do not pass a raw pointer through
<code>Arcadia.Value</code> or PDL. The buffer does not support resizing
its backing ByteArray.</p>
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
