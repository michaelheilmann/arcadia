<section class="cxx entity method">

  <h1 id="Arcadia_Math_Matrix*x**_setRotationX">Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv>_setRotationX</h1>

  <my-signature><code>
  void<br>
  Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv>_setRotationX<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv>* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> angle<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  <p>Assign the elements of this <code>Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv></code> object the values of a rotation matrix.
  This matrix represents a counter-clockwise rotation by <code>angle</code> degrees around the x-axis.
  This function is defined for \(4 \times 4\).</p>

  <p>The rotation matrix is defined as
  \[
	\begin{matrix}
	 1 &  0 &  0 &  0 \\
	 0 &  c & -s &  0 \\
	 0 &  s &  c &  0 \\
	 0 &  0 &  0 &  1
	\end{matrix}
  \]
  where \(c=\cos(a)\) and \(s=\sin(a)\).</p>

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
      <div><my-mv>ElementType</my-mv> angle</div>
	  <div>The angle of rotation, in degrees.</div>
    </div>
  </section>

</section>
