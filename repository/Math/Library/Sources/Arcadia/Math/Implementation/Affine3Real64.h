// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
// details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#if !defined(ARCADIA_MATH_IMPLEMENTATION_AFFINE3REAL64_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_AFFINE3REAL64_H_INCLUDED

#include "Arcadia/Math/Implementation/RayGeometry3Real64.h"

// Native affine 3x4 matrix: p' = linear * p + translation, row-major.
// This matches Arcadia Math Matrix4x4 transformation conventions. The last
// homogeneous matrix row is implicitly (0, 0, 0, 1).
// Input and output geometry may alias, except outputs may not alias the matrix.
// Failure leaves output geometry unchanged; no managed/heap allocations.
typedef struct Arcadia_Math_Affine3Real64 {
  Arcadia_Real64Value elements[3][4];
} Arcadia_Math_Affine3Real64;

void Arcadia_Math_Affine3Real64_setIdentity(Arcadia_Math_Affine3Real64* self);
Arcadia_BooleanValue Arcadia_Math_Affine3Real64_setValues(Arcadia_Math_Affine3Real64* self, const Arcadia_Real64Value values[12]);
Arcadia_BooleanValue Arcadia_Math_Affine3Real64_multiply(Arcadia_Math_Affine3Real64* result, const Arcadia_Math_Affine3Real64* left, const Arcadia_Math_Affine3Real64* right);
Arcadia_BooleanValue Arcadia_Math_Affine3Real64_inverse(Arcadia_Math_Affine3Real64* result, const Arcadia_Math_Affine3Real64* source);
Arcadia_BooleanValue Arcadia_Math_Affine3Real64_transformPoint(const Arcadia_Math_Affine3Real64* matrix, const Arcadia_Real64Value point[3], Arcadia_Real64Value result[3]);
Arcadia_BooleanValue Arcadia_Math_Affine3Real64_transformDirection(const Arcadia_Math_Affine3Real64* matrix, const Arcadia_Real64Value direction[3], Arcadia_Real64Value result[3]);
Arcadia_BooleanValue Arcadia_Math_Affine3Real64_transformPlane(const Arcadia_Math_Affine3Real64* matrix, const Arcadia_Math_Plane3Real64* plane, Arcadia_Math_Plane3Real64* result);
Arcadia_BooleanValue Arcadia_Math_Affine3Real64_transformRay(const Arcadia_Math_Affine3Real64* matrix, const Arcadia_Math_Ray3Real64* ray, Arcadia_Math_Ray3Real64* result);
Arcadia_BooleanValue Arcadia_Math_Affine3Real64_transformAABB(const Arcadia_Math_Affine3Real64* matrix, const Arcadia_Math_AABB3Real64* bounds, Arcadia_Math_AABB3Real64* result);

#endif // ARCADIA_MATH_IMPLEMENTATION_AFFINE3REAL64_H_INCLUDED
