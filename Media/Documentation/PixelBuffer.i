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
<h2>ByteArray-backed pixel storage</h2>
<p>A PixelBuffer owns exactly one fixed-size, mutable, contiguous
<code>Arcadia_ByteArray</code>. The ByteArray owns the allocation; the
PixelBuffer's legacy <code>bytes</code> field is only a native-C compatibility
pointer into that storage. It must never be freed or reallocated independently.
The PixelBuffer's GC visitor retains the ByteArray.</p>
<p><code>Arcadia_Media_PixelBuffer_getPixelBytes(thread, buffer)</code> returns
the owning <code>Arcadia_ByteArray*</code>. Its capabilities are mutable,
non-resizable and contiguous. The allocated byte count is
<code>(width * bytes per pixel + line padding) * height</code>, with
non-negative dimensions/padding and checked allocation arithmetic.
The line padding is included in each allocated row but is not itself a pixel.
ByteArray access is native C only; raw pointers are never carried through
PDL or Arcadia.Value. The legacy signed 32-bit
<code>Arcadia_Media_PixelBuffer_getLineStride</code> API rejects strides
whose multiplication or padding addition cannot be represented by
<code>Arcadia_Integer32Value</code>, reporting
<code>Arcadia_Status_ArgumentValueInvalid</code> instead of overflowing.</p>
<p>The native <code>bytes</code> pointer is borrowed: its lifetime depends on
the PixelBuffer/ByteArray remaining reachable, and its address can change
when pixel format, line padding or dimensions change. Always reacquire it
after these operations. Format conversion may replace ByteArray storage;
swapping transfers storage ownership together with the legacy pointer.
Cloning and assignment create independent storage. The native pixel and
image-I/O hot loops continue to work on contiguous byte spans, not boxed
per-pixel accesses.</p>
<p>Changing width or height preserves only the retained pixel rectangle,
copied by row spans; padding is not copied as pixels. Newly introduced
pixels are initialized as opaque black. Changing the pixel format preserves
logical RGBA components, with alpha set to 255 where the source has none.
A pixel read or write requires
<code>0 &lt;= column &lt; width</code> and
<code>0 &lt;= row &lt; height</code>; a zero-sized buffer has no valid pixel
coordinates. Native bulk consumers, including image writers and texture
uploaders, must root the PixelBuffer during pointer use.</p>
</section>
