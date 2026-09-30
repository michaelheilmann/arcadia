<section class="cxx entity object">
<h1 id="Arcadia_Media_PixelBuffer">Arcadia_Media_PixelBuffer</h1>
<p><code>Arcadia_Media_PixelBuffer</code> extends <code>Arcadia_Object</code>.</p>
<p>
<code>Arcadia_Media_PixelBuffer</code> is a two-dimensional array of pixels.
</p>
<p>The following terms and definitions are used througout its documentation:</p>
<ul>
  <li><em>pixel</em>: A pixel is uniquely adressed by the index of its column and its row.
  </li>
  <li>
  <em>number of columns</em>: The number of columns of the pixel rectangle.
  A column is adressed by zero-based consecutive integral ascending indices (0,1,2,...).
  The number of columns is also called the <em>width</em> of the pixel rectangle.
  </li>
  <li>
  <em>number of rows</em>: The number of rows of the pixel rectangle.
  A row is adressed by zero-based consecutive integral ascending indices (0,1,2,...).
  The number of rows is also called the <em>height</em> of the pixel rectangle.
  </li>
  <li>
  <em>pixel format</em>: The pixel format of the pixels of the pixel buffer as defined by <a href="@{siteAddress}/Arcadia/Media/#Arcadia_Media_PixelFormat">Arcadia_Media_PixelFormat</a>.
  </li>
  <li>
  <em>line padding</em>: The number of Bytes after a line.
  </li>
  <li>
  <em>line stride</em>: The number of Bytes of a line which comprises the Bytes of the actual pixels plus the line padding.
  <code>&lt;number of columns&gt; * &lt;bytes per pixel&gt; + &lt;line padding&gt;</code>
  </li>
</ul>
</section>
