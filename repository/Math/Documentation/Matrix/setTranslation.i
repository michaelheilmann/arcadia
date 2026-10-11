<section class="cxx entity method">

  <h1 id="Arcadia_Math_Matrix*x**_setTranslation">Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv>_setTranslation</h1>

  <my-signature><code>
  void<br>
  Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv>_setTranslation<br>
  &nbsp;&nbsp;(<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Thread* thread,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv>Value* self,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> x,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> y,<br>
  &nbsp;&nbsp;&nbsp;&nbsp;<my-mv>ElementType</my-mv> z<br>
  &nbsp;&nbsp;)
  </code></my-signature>

  <my-summary>
  Assign the elements of this <code>Arcadia_Math_Matrix<my-mv>NumberOfColumns</my-mv>x<my-mv>NumberOfRows</my-mv><my-mv>ElementTypeSuffix</my-mv>Value</code> object the values of a translation matrix.
  This function is defined for \(4 \times 4\).
  The translation matrix for \f(4 \times \) matrices is defined as</p>
  \[
	\begin{matrix}
	1 & 0 & 0 & x \\
	0 & 1 & 0 & y \\
	0 & 0 & 1 & z \\
	0 & 0 & 0 & 1
	\end{matrix}
  \]
  <p>where \(x\), \(y\), and \(z\) is the translation along the x, y, and z axis, respectively.</p>

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
      <div><my-mv>ElementType</my-mv> x</div>
      <div>The translation along the x-axis.</div>
    </div>
    <div>
      <div><my-mv>ElementType</my-mv> y</div>
      <div>The translation along the y-axis.</div>
    </div>
    <div>
      <div><my-mv>ElementType</my-mv> z</div>
      <div>The translation along the z-axis.</div>
    </div>
  </section>

</section>
