<section class="cxx entity enumeration">
<h1 id="Arcadia_Media_SampleFormat">Arcadia_Media_SampleFormat</h1>
<my-signature><code>
typedef enum Arcadia_Media_SampleFormat Arcadia_Media_SampleFormat;
</code></my-signature>

<my-summary>
An enumeration of sample formats.
</my-summary>

<section class="cxx enumeration-elements">

  <h1>Elements</h1>

  <div>
    <div>
      <code>Arcadia_Media_SampleFormat_Integer16</code>
    </div>
    <div>
    One Arcadia_Integer16Value.
    Maximal amplitude at \(+2^{16-1}-1\), minimal amplitude at \(-2^{16-1}\), silence at \(0\).
    This is an asymmetricc format as the distance of the minimal amplitude value and the distance of the maximal amplitude value from the silence value are not equal.
    </div>
  </div>

  <div>
    <div>
      <code>Arcadia_Media_SampleFormat_Integer8</code>
    </div>
    <div>
    One Arcadia_Integer8Value.
    Maximal amplitude at \(+2^{8-1}-1\), minimal amplitude at \(-2^{8-1}\), silence at \(0\).
    This is an asymmetricc format as the distance of the minimal amplitude value and the distance of the maximal amplitude value from the silence value are not equal.
    </div>
  </div>

  <div>
    <div>
      <code>Arcadia_Media_SampleFormat_Natural16</code>
    </div>
    <div>
    One Arcadia_Natural8Value.
    Maximal amplitude at \(2^{16}-1\), minimal amplitude at \(0\). Silence at \(\textit{ceil}\left(\frac{2^{16}-1}{2}\right)\).
    This is a symmetric format at the distance of the minimal amplitude value and the maximal amplitude value from the silience value are equal.
    </div>
  </div>

  <div>
    <div>
      <code>Arcadia_Media_SampleFormat_Natural8</code>
    </div>
    <div>
    One Arcadia_Natural8Value.
    Maximal amplitude at \(2^{8}-1\), minimal amplitude at \(0\). Silence at \(\textit{ceil}\left(\frac{2^{8}-1}{2}\right)\).
    This is a symmetric format at the distance of the minimal amplitude value and the maximal amplitude value from the silience value are equal.
    </div>
  </div>

</section>

</section>
