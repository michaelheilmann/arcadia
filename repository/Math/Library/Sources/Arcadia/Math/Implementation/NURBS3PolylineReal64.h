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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_NURBS3POLYLINEREAL64_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_NURBS3POLYLINEREAL64_H_INCLUDED

#include "Arcadia/Math/Implementation/RayGeometry3Real64.h"

// Native utilities for sampled NURBS curves. Points are 3D consecutive
// vertices, not an unindexed GL_LINES stream. All arrays are caller-owned.
// Distances use chord lengths; they approximate, not integrate, arc length.
// Inputs must be finite. On validation/numeric failure, results stay intact.
// Output buffers must not overlap inputs (except query/result alias in closest).
// No allocation, managed objects, or rendering dependency.

// Cumulative distances start at zero; repeated adjacent points are valid.
// The output contains count values. Input count must be at least 2.
Arcadia_BooleanValue
Arcadia_Math_NURBS3PolylineReal64_cumulativeLengths(const Arcadia_Real64Value (*points)[3],
    Arcadia_SizeValue count, Arcadia_Real64Value* distances, Arcadia_SizeValue capacity);

// Regular fractions of the cumulative length, inclusive of both endpoints.
// Requires a nondecreasing finite cumulative array starting at zero; final
// length strictly positive. Repeated distances are permitted. Samples >= 2.
Arcadia_BooleanValue
Arcadia_Math_NURBS3PolylineReal64_resampleByLength(const Arcadia_Real64Value (*points)[3],
    const Arcadia_Real64Value* distances, Arcadia_SizeValue count, Arcadia_SizeValue samples,
    Arcadia_Real64Value (*result)[3], Arcadia_SizeValue capacity);

// Nearest point on the sampled polyline to a point. Segment index and
// interpolation fraction are optional. Ties select the lowest segment index.
// Squared distance is computed in double; scalar result is Euclidean distance.
Arcadia_BooleanValue
Arcadia_Math_NURBS3PolylineReal64_closestPoint(const Arcadia_Real64Value (*points)[3], Arcadia_SizeValue count,
    const Arcadia_Real64Value query[3], Arcadia_Real64Value result[3], Arcadia_SizeValue* segment,
    Arcadia_Real64Value* fraction, Arcadia_Real64Value* distance);

// Find closest approach of a forward ray to any polyline segment within radius.
// Returns segment index, ray parameter and segment fraction. Ray parameter
// uses origin+t*direction, and direction need not be normalized. A ray at
// t=0 is valid. Ties select lowest segment index. All outputs are optional
// except segment; radius must be nonnegative. Miss preserves all outputs.
Arcadia_BooleanValue
Arcadia_Math_NURBS3PolylineReal64_pickRay(const Arcadia_Real64Value (*points)[3], Arcadia_SizeValue count,
    const Arcadia_Math_Ray3Real64* ray, Arcadia_Real64Value radius,
    Arcadia_SizeValue* segment, Arcadia_Real64Value* rayT, Arcadia_Real64Value* fraction);

#endif // ARCADIA_MATH_IMPLEMENTATION_NURBS3POLYLINEREAL64_H_INCLUDED
