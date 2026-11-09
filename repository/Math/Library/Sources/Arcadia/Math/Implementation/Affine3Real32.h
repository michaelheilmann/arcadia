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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_AFFINE3REAL32_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_AFFINE3REAL32_H_INCLUDED

#include "Arcadia/Math/Implementation/RayGeometry3Real32.h"

// Native affine 3x4 matrix: p' = linear * p + translation, row-major.
// This matches Arcadia Math Matrix4x4 transformation conventions. The last
// homogeneous matrix row is implicitly (0, 0, 0, 1).
// Input and output geometry may alias, except outputs may not alias the matrix.
// Failure leaves output geometry unchanged; no managed/heap allocations.
typedef struct Arcadia_Math_Affine3Real32 {
  Arcadia_Real32Value elements[3][4];
} Arcadia_Math_Affine3Real32;

void Arcadia_Math_Affine3Real32_setIdentity(Arcadia_Math_Affine3Real32* self);
Arcadia_BooleanValue Arcadia_Math_Affine3Real32_setValues(Arcadia_Math_Affine3Real32* self, const Arcadia_Real32Value values[12]);
Arcadia_BooleanValue Arcadia_Math_Affine3Real32_multiply(Arcadia_Math_Affine3Real32* result, const Arcadia_Math_Affine3Real32* left, const Arcadia_Math_Affine3Real32* right);
Arcadia_BooleanValue Arcadia_Math_Affine3Real32_inverse(Arcadia_Math_Affine3Real32* result, const Arcadia_Math_Affine3Real32* source);
Arcadia_BooleanValue Arcadia_Math_Affine3Real32_transformPoint(const Arcadia_Math_Affine3Real32* matrix, const Arcadia_Real32Value point[3], Arcadia_Real32Value result[3]);
Arcadia_BooleanValue Arcadia_Math_Affine3Real32_transformDirection(const Arcadia_Math_Affine3Real32* matrix, const Arcadia_Real32Value direction[3], Arcadia_Real32Value result[3]);
Arcadia_BooleanValue Arcadia_Math_Affine3Real32_transformPlane(const Arcadia_Math_Affine3Real32* matrix, const Arcadia_Math_Plane3Real32* plane, Arcadia_Math_Plane3Real32* result);
Arcadia_BooleanValue Arcadia_Math_Affine3Real32_transformRay(const Arcadia_Math_Affine3Real32* matrix, const Arcadia_Math_Ray3Real32* ray, Arcadia_Math_Ray3Real32* result);
Arcadia_BooleanValue Arcadia_Math_Affine3Real32_transformAABB(const Arcadia_Math_Affine3Real32* matrix, const Arcadia_Math_AABB3Real32* bounds, Arcadia_Math_AABB3Real32* result);

#endif // ARCADIA_MATH_IMPLEMENTATION_AFFINE3REAL32_H_INCLUDED
