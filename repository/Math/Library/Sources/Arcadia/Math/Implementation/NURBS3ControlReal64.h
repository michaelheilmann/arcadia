// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#if !defined(ARCADIA_MATH_IMPLEMENTATION_NURBS3CONTROLREAL64_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_NURBS3CONTROLREAL64_H_INCLUDED

#include "Arcadia/Math/Implementation/Affine3Real64.h"

// Native NURBS control-lattice helpers. Points are u-major packed triplets:
// point(i,j) = points[i * countV + j]. All inputs must be finite.
// Counts must be positive and countU*countV must fit Arcadia_SizeValue.
// No allocation or managed objects. AABB is the control hull, not the tight
// bounding box of the evaluated rational surface.
Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlReal64_computeAABB(const Arcadia_Real64Value (*points)[3],
    Arcadia_SizeValue countU, Arcadia_SizeValue countV, Arcadia_Math_AABB3Real64* result);

// Map the longest axis of the control hull to [-1,+1], preserving aspect ratio.
// Center coincides with the origin. Identical controls normalize to the origin.
// Output may be the input array; non-overlapping separate buffers are also valid.
// On failure all output points are left untouched. Capacity is in vertices.
Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlReal64_normalize(const Arcadia_Real64Value (*points)[3],
    Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    Arcadia_Real64Value (*result)[3], Arcadia_SizeValue resultCapacity);

// Apply an affine mapping to each control. The matrix must contain
// finite coefficients. Output may equal input. Failure leaves output untouched.
Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlReal64_transform(const Arcadia_Real64Value (*points)[3],
    Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    const Arcadia_Math_Affine3Real64* transform, Arcadia_Real64Value (*result)[3],
    Arcadia_SizeValue resultCapacity);

// Pick a control point inside a world-space sphere of given radius along the
// forward ray. Choose nearest nonnegative ray t, then lowest u-major index on
// a tie. Ray direction need not be unit length. On miss or invalid input,
// outputs (including optional t) stay unchanged. Sphere radius must be > 0.
Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlReal64_pickControl(const Arcadia_Real64Value (*points)[3],
    Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    const Arcadia_Math_Ray3Real64* ray, Arcadia_Real64Value radius,
    Arcadia_SizeValue* index, Arcadia_Real64Value* t);

#endif // ARCADIA_MATH_IMPLEMENTATION_NURBS3CONTROLREAL64_H_INCLUDED
