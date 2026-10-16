<section>
  <h1>Matrix Operations and Affine Transformations</h1>
  <p>Include <code>Arcadia/Math/Include.h</code>. All operations below are available for
  <code>Real32</code> and <code>Real64</code>; substitute the corresponding suffix and scalar type
  in the signatures. Matrix elements are indexed as <code>elements[row][column]</code>.
  Transformations multiply column vectors: <code>result = matrix * vector</code>.
  Consequently, in <code>A * B</code>, transformation B acts first.</p>

  <section>
    <h1 id="Arcadia_Math_Matrix4_transpose">Transpose</h1>
    <pre><code>void Arcadia_Math_Matrix4Real32_transpose
  (Arcadia_Thread* thread, Arcadia_Math_Matrix4Real32* self,
   Arcadia_Math_Matrix4Real32* other);</code></pre>
    <p>Assigns <code>self[i][j] = other[j][i]</code>. The destination and source may be
    the same object. With distinct objects, the source is unchanged. No managed objects are allocated.</p>
  </section>

  <section>
    <h1 id="Arcadia_Math_Matrix4_getDeterminant">Determinant</h1>
    <pre><code>Arcadia_Real32Value Arcadia_Math_Matrix4Real32_getDeterminant
  (Arcadia_Thread* thread, Arcadia_Math_Matrix4Real32* self);</code></pre>
    <p>Returns the determinant of a 4 by 4 matrix by cofactor expansion. Does not modify the matrix
    or allocate managed objects. Singular matrices are valid inputs; no division or arbitrary
    singularity tolerance is used. Floating-point rounding applies, so a computed determinant is
    not a general-purpose numerical invertibility test.</p>
  </section>

  <section>
    <h1 id="Arcadia_Math_Matrix4x4_transformPoint">Point Transformation</h1>
    <pre><code>void Arcadia_Math_Matrix4x4Real32_transformPoint
  (Arcadia_Thread* thread, Arcadia_Math_Matrix4Real32* self,
   Arcadia_Math_Vector3Real32* point);</code></pre>
    <p>Transforms the point in place by the first three components of
    <code>self * (point.x, point.y, point.z, 1)</code>. Translation participates.
    The matrix must describe an affine transformation. The function does not validate the last
    row and performs no perspective divide; it is not a projection operation.</p>
  </section>

  <section>
    <h1 id="Arcadia_Math_Matrix4x4_transformDirection">Direction Transformation</h1>
    <pre><code>void Arcadia_Math_Matrix4x4Real32_transformDirection
  (Arcadia_Thread* thread, Arcadia_Math_Matrix4Real32* self,
   Arcadia_Math_Vector3Real32* direction);</code></pre>
    <p>Transforms the direction in place by the upper-left 3 by 3 matrix, equivalent to the first
    three components of <code>self * (direction.x, direction.y, direction.z, 0)</code> for finite
    inputs. Translation is ignored. Scaling affects length; the result is not normalized.
    Normal vectors generally require an inverse-transpose transformation instead.</p>
    <p>Both point and direction operations preserve the matrix, capture input components before
    assigning the result, and allocate no managed objects.</p>
  </section>

  <section>
    <h1 id="Arcadia_Math_Matrix*x**_setLookAt">Look-at View Matrix</h1>
    <pre><code>void Arcadia_Math_Matrix4x4Real32_setLookAt
  (Arcadia_Thread* thread, Arcadia_Math_Matrix4Real32* self,
   Arcadia_Math_Vector3Real32* source, Arcadia_Math_Vector3Real32* target,
   Arcadia_Math_Vector3Real32* up);</code></pre>
    <p>Constructs a right-handed world-to-view matrix looking along negative view-space Z.
    The source maps to the origin and the target lies on the negative Z axis.
    The supplied up vector need not be unit length or perpendicular to the viewing direction.</p>
    <pre><code>f = normalize(target - source)
r = normalize(cross(f, normalize(up)))
u = cross(r, f)

V = [ r.x   r.y   r.z  -dot(r, source) ]
    [ u.x   u.y   u.z  -dot(u, source) ]
    [-f.x  -f.y  -f.z   dot(f, source) ]
    [ 0     0     0     1              ]</code></pre>
    <p>Raises <code>Arcadia_Status_ArgumentValueInvalid</code> for a zero or non-finite
    viewing/up direction or a zero cross product (parallel directions). No angular tolerance is
    imposed. Computations whose direction lengths overflow also raise this status.
    Failure leaves the destination unchanged. Input vectors are never modified and no managed
    objects are allocated.</p>
  </section>
</section>
