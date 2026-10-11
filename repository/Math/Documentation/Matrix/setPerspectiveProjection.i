<section class="cxx entity method">

  <h1 id="Arcadia_Math_Matrix*x**_setPerspectiveProjection">Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv>_setPerspectiveProjection</h1>

  <my-signature><code>
  void<br>
  Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv>_setOrthographicProjection<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv>* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> fieldOfView,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> aspectRatio,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> near,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> far<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  <p>Assign the elements of this <code>Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv></code> object the values of a perspective projection matrix.
  This function is defined for \(4 \times 4\).</p>

  <p>The projection matrix is defined as</p>
  \[
	\begin{matrix}
	\frac{f}{\textit{aspectRatio}} & 0 & 0                                                                 & 0                                                                             \\
	0                              & f & 0                                                                 & 0                                                                             \\
	0                              & 0 & \frac{\textit{near} + \textit{far}}{\textit{near} - \textit{far}} & \frac{2 \cdot \textit{near} \cdot \textit{far}}{\textit{near} - \textit{far}} \\
	0                              & 0 & -1                                                                & 0
	\end{matrix}
  \]
  where \(f = \cot\left(\frac{\textit{fieldOfView}}{2}\right)\).</p>
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
      <div><my-mv>ElementType</my-mv> fieldOfView</div>
      <div>
      The field of view angle, in degrees, in the y-direction. Must be positive.
      In other terms: the angle between a plane passing through the camera position as well as the top of your canvas and another plane passing through the camera position and the bottom of the canvas.
      The bigger this angle is the more you can see of the world - but at the same time, the object you can see become smaller.
      </div>
    </div>
    <div>
      <div><my-mv>ElementType</my-mv> aspectRatio</div>
      <div>The aspect ratio which determines the field of view in the x-direction. The aspect ratio is the ratio of width and height. Must be positive. Typical canvas have aspect ratios of 4:3 or 16:9.</div>
    </div>
    <div>
      <div><my-mv>ElementType</my-mv> near</div>
      <div>The distance of the viewer to the near clipping plane. Must be positive. Must be less than <code>far</code> or nothing is displayed</div>
    </div>
    <div>
      <div><my-mv>ElementType</my-mv> far</div>
      <div>The distance of the viewer to the far clipping plane. Must be positive. Must be greater than <code>near</code>.</div>
    </div>
  </section>

</section>
