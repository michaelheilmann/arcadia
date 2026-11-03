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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_NURBS3DIFFERENTIALSREAL64_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_NURBS3DIFFERENTIALSREAL64_H_INCLUDED

#include "Arcadia/Math/Implementation/NURBS3Real64.h"

// Exact parametric first derivative of a rational B-spline curve. The output
// may alias a control point. Workspace: 4 * (degree + 1) scalar elements.
// Input validation and domain semantics match NURBS3Real64_evaluateCurve.
Arcadia_BooleanValue
Arcadia_Math_NURBS3Real64_evaluateCurveTangent
  (
    const Arcadia_Real64Value (*points)[3], const Arcadia_Real64Value* weights,
    Arcadia_SizeValue count, Arcadia_SizeValue degree,
    const Arcadia_Real64Value* knots, Arcadia_SizeValue knotCount,
    Arcadia_Real64Value parameter,
    Arcadia_Real64Value* workspace, Arcadia_SizeValue workspaceElements,
    Arcadia_Real64Value tangent[3]
  );

// Simultaneous surface position and exact partial derivatives dP/du, dP/dv.
// Outputs must be distinct from each other; the position may alias an input
// control point, since all active controls are read before outputs are stored.
// Workspace: 4 * (degreeU + degreeV + 2) scalar elements.
Arcadia_BooleanValue
Arcadia_Math_NURBS3Real64_evaluateSurfaceDifferentials
  (
    const Arcadia_Real64Value (*points)[3], const Arcadia_Real64Value* weights,
    Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    Arcadia_SizeValue degreeU, Arcadia_SizeValue degreeV,
    const Arcadia_Real64Value* knotsU, Arcadia_SizeValue knotCountU,
    const Arcadia_Real64Value* knotsV, Arcadia_SizeValue knotCountV,
    Arcadia_Real64Value u, Arcadia_Real64Value v,
    Arcadia_Real64Value* workspace, Arcadia_SizeValue workspaceElements,
    Arcadia_Real64Value position[3],
    Arcadia_Real64Value tangentU[3], Arcadia_Real64Value tangentV[3]
  );

// Uniform parameter-space grid over [knotsU[degreeU], knotsU[countU]] by
// [knotsV[degreeV], knotsV[countV]], including endpoints. Storage is u-major:
// result[uIndex * samplesV + vIndex]. Both sample counts must be at least 2.
// Optional unit normals point along cross(dP/du,dP/dv); NULL disables them.
// Singular/degenerate tangents reject normals. On a late error, earlier samples
// may already have been written; outputs must not alias inputs or workspace.
// Workspace: 4 * (degreeU + degreeV + 2) scalar elements.
Arcadia_BooleanValue
Arcadia_Math_NURBS3Real64_sampleSurface
  (
    const Arcadia_Real64Value (*points)[3], const Arcadia_Real64Value* weights,
    Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    Arcadia_SizeValue degreeU, Arcadia_SizeValue degreeV,
    const Arcadia_Real64Value* knotsU, Arcadia_SizeValue knotCountU,
    const Arcadia_Real64Value* knotsV, Arcadia_SizeValue knotCountV,
    Arcadia_SizeValue samplesU, Arcadia_SizeValue samplesV,
    Arcadia_Real64Value (*positions)[3], Arcadia_Real64Value (*normals)[3],
    Arcadia_Real64Value* workspace, Arcadia_SizeValue workspaceElements
  );

#endif // ARCADIA_MATH_IMPLEMENTATION_NURBS3DIFFERENTIALSREAL64_H_INCLUDED
