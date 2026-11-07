# Spatial value salvage from `michaelheilmann/toport`

This migration is based on `nyx/modules/m0/matrix33.c/.h` and
`nyx/modules/m0/aabb.c/.h` from `toport/main` at
`e917f18242dcd64138a78956465751a384a31a99`.

Both concepts now exist in Arcadia.Math as native C values in Real32 and
Real64 variants. They are deliberately not `Arcadia.Object` instances and
cannot increase GC pressure. No runtime implementation is copied from the
obsolete M0 object model.

## Matrix3

- Row-major `elements[3][3]`, column-vector multiplication.
- Identity, scaling, matrix product, sum, difference, transpose, value-copy,
  and vector transformation.
- In-place output is supported for either or both operands.
- The old `M0_Matrix33_SetIdentity` and `SetScale` wrote index 3 to
  a three-column array; the new operations use only indices 0..2.
- The old `M0_Matrix33_Mul` performed addition across a 4x4 loop rather than
  a 3x3 matrix product. The new operation computes the actual 3x3 product.

## AABB3

- Axis-aligned bounds with inclusive min/max and a distinct empty state.
- Normalized min/max input, point accumulation, union, intersection,
  inclusive point containment, and translation.
- Empty bounds never implicitly contain the origin. Union and intersection
  support in-place output.
- Old `M0_AABB` allocated two managed vectors; these replacements have no
  owned allocations and do not require a GC root.

Run `Arcadia.Math.Tests.SpatialValueTests` to exercise both precisions and
aliasing. The original `toport` source remains unchanged; Arcadia owns the
salvaged implementations and their regression coverage.
