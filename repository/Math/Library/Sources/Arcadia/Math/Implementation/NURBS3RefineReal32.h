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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_NURBS3REFINEREAL32_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_NURBS3REFINEREAL32_H_INCLUDED

#include "Arcadia/Math/Implementation/NURBS3Real32.h"

// Insert one interior knot into a 3D NURBS curve without changing its shape.
// The curve's degree is unchanged; output has count+1 control points,
// knotCount+1 knots, and (optionally) count+1 weights.
// The input weight array may be NULL to represent unit weights. If it is
// non-NULL, newWeights is required; if NULL, newWeights may be NULL.
// Input and output arrays must be pairwise disjoint (including from each
// other); no heap allocation or managed objects. Output is fully unchanged
// on invalid input or numerical conversion failure.
// This operation inserts knots strictly inside (knots[degree], knots[count]);
// existing interior multiplicity may be at most degree. Degree zero is
// supported for piecewise-constant curves.
// Both output capacities are measured in elements, not bytes. The arrays
// newPoints and newKnots must have capacities count+1 and knotCount+1.
Arcadia_BooleanValue
Arcadia_Math_NURBS3RefineReal32_insertKnot(
    const Arcadia_Real32Value (*points)[3], const Arcadia_Real32Value* weights,
    Arcadia_SizeValue count, Arcadia_SizeValue degree,
    const Arcadia_Real32Value* knots, Arcadia_SizeValue knotCount,
    Arcadia_Real32Value parameter,
    Arcadia_Real32Value (*newPoints)[3], Arcadia_Real32Value* newWeights,
    Arcadia_SizeValue pointCapacity,
    Arcadia_Real32Value* newKnots, Arcadia_SizeValue knotCapacity);

#endif // ARCADIA_MATH_IMPLEMENTATION_NURBS3REFINEREAL32_H_INCLUDED
