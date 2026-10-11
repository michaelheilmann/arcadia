<section class="cxx entity method">

  <h1 id="Arcadia_Math_Matrix*x**_setOrthographicProjection">Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv>_setOrthographicProjection</h1>

  <my-signature><code>
  void<br>
  Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv>_setOrthographicProjection<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv>* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> left,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> right,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> bottom,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> top,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> near,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> far<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  <p>Assign the elements of this <code>Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv></code> object the values of an orthographic projection matrix.
  This function is defined for \(4 \times 4\).</p>

  <p>The projection matrix is defined as</p>
  \[
	\begin{matrix}
	a & 0 & 0 & u \\
	0 & b & 0 & v \\
	0 & 0 & c & w \\
	0 & 0 & 0 & 1
	\end{matrix}
  \]
  where</p>
  <ul>
	<li>\(a =  \frac{2}{right-left}\)</li>
	<li>\(b =  \frac{2}{top-bottom}\)</li>
	<li>\(c = -\frac{2}{far - near}\)</li>
	<li>\(u = -\frac{right + left}{right - left}\)</li>
	<li>\(v = -\frac{top + bottom}{top - bottom}\)</li>
	<li>\(w = -\frac{far + near}{far - near}\)</li>
  </ul>
  <p>The resulting matrix sets up the axes as follows:</p>
  <ul>
	<li>the positive z-axis points out of the canvas (negative z-axis points into the canvas)</li>
	<li>the positive x-axis points to the right</li>
	<li>the positive y-axis points to the top</li>
  </ul>
  <p>
  <my-mv>ElementTypeSuffix</my-mv> indicates the <em>Arcadia</em> type of the elements and parameters denoted by <my-mv>ElementType</my-mv>.
  The following table lists the <my-mv>ElementTypeSuffix</my-mv> and the corresponding <my-mv>ElementType</my-mv>.
  </p>

  <table>
    <tr><td><my-mv>ElementTypeSuffix</my-mv> </td><td><my-mv>ElementType</my-mv>      </td></tr>
    <tr><td><code>Real32Value</code>         </td><td><code>Arcadia_Real32Value</code></td></tr>
    <tr><td><code>Real64Value</code>         </td><td><code>Arcadia_Real64Value</code></td></tr>
  </table>

  </my-summary>

  <section class="cxx parameters">
    <h1>Parameters</h1>
    <div>
      <div><a href="#">Arcadia_Thread</a>* thread</div>
      <div>A pointer to the <code>Arcadia_Thread</code> object.</div>
    </div>
    <div>
      <div><code>Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv></code>* self</div>
      <div>A pointer to this <code>Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv></code> object.</div>
    </div>
    <div>
      <div><my-mv>ElementType</my-mv> left</div>
	  <div>The coordinate of the left clipping plane.</div>
    </div>
    <div>
      <div><my-mv>ElementType</my-mv> right</div>
	  <div>The coordinate of the right clipping plane.</div>
    </div>
    <div>
      <div><my-mv>ElementType</my-mv> bottom</div>
	  <div>The coordinate of the bottom clipping plane.</div>
    </div>
    <div>
      <div><my-mv>ElementType</my-mv> top</div>
	  <div>The coordinate of the top clipping plane.</div>
    </div>
    <div>
      <div><my-mv>ElementType</my-mv> near</div>
	  <div>The coordinate of the near clipping plane.</div>
    </div>
    <div>
      <div><my-mv>ElementType</my-mv> far</div>
	  <div>The coordinate of the far clipping plane.</div>
    </div>
  </section>

</section>
