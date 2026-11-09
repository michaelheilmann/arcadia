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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_NURBS3REAL32_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_NURBS3REAL32_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

// Native, allocation-free non-uniform rational B-spline evaluation.
// Control points are packed triplets. Surface points use u-major order:
// points[uIndex * countV + vIndex]. weights may be NULL (all weights = 1).
// Non-NULL weights must be finite and strictly positive at active points.
// Degrees must be less than their control-point counts. Knot arrays contain
// count + degree + 1 finite, nondecreasing values and have a nonempty domain
// [knots[degree], knots[count]]. Parameters outside this domain are rejected.
// Repeated knots are allowed. Endpoints, including the final knot, are valid.
// On failure the result is left unchanged. Output may alias a control point.
// Workspace must NOT overlap inputs/outputs; the caller owns all storage.
//
// curve workspace: at least 3*(degree + 1) scalar entries.
// surface workspace: at least 3*(degreeU + degreeV + 2) scalar entries.
// No managed values, temporary heap allocations, or renderer dependencies.

Arcadia_BooleanValue
Arcadia_Math_NURBS3Real32_makeOpenUniformKnots
  (
    Arcadia_SizeValue count,
    Arcadia_SizeValue degree,
    Arcadia_Real32Value* knots,
    Arcadia_SizeValue knotCount
  );

Arcadia_BooleanValue
Arcadia_Math_NURBS3Real32_evaluateCurve
  (
    const Arcadia_Real32Value (*points)[3],
    const Arcadia_Real32Value* weights,
    Arcadia_SizeValue count,
    Arcadia_SizeValue degree,
    const Arcadia_Real32Value* knots,
    Arcadia_SizeValue knotCount,
    Arcadia_Real32Value parameter,
    Arcadia_Real32Value* workspace,
    Arcadia_SizeValue workspaceElements,
    Arcadia_Real32Value result[3]
  );

Arcadia_BooleanValue
Arcadia_Math_NURBS3Real32_evaluateSurface
  (
    const Arcadia_Real32Value (*points)[3],
    const Arcadia_Real32Value* weights,
    Arcadia_SizeValue countU,
    Arcadia_SizeValue countV,
    Arcadia_SizeValue degreeU,
    Arcadia_SizeValue degreeV,
    const Arcadia_Real32Value* knotsU,
    Arcadia_SizeValue knotCountU,
    const Arcadia_Real32Value* knotsV,
    Arcadia_SizeValue knotCountV,
    Arcadia_Real32Value u,
    Arcadia_Real32Value v,
    Arcadia_Real32Value* workspace,
    Arcadia_SizeValue workspaceElements,
    Arcadia_Real32Value result[3]
  );

#endif // ARCADIA_MATH_IMPLEMENTATION_NURBS3REAL32_H_INCLUDED
