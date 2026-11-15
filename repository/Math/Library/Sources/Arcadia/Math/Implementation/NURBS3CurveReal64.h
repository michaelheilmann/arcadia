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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_NURBS3CURVEREAL64_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_NURBS3CURVEREAL64_H_INCLUDED

#include "Arcadia/Math/Implementation/NURBS3DifferentialsReal64.h"

// Native, allocation-free render-ready line segments for 3D NURBS curves.
// Sampled endpoints include both knot-domain boundaries; output order follows
// increasing curve parameter. Segments contain 2 separate endpoints each.
// All buffers are caller-owned. positions and optional tangents must not
// overlap controls, knots, weights, workspace, or each other.
// On preflight failure outputs remain unchanged. A late numerical failure
// may leave a prefix of output vertices written, as for NURBS surface meshes.
// Workspace requires 4 * (degree + 1) scalars when tangents are requested,
// otherwise 3 * (degree + 1) scalars. Tangents are parametric derivatives,
// NOT normalized unit vectors; zero derivatives are allowed.
Arcadia_BooleanValue
Arcadia_Math_NURBS3CurveReal64_getVertexCount(Arcadia_SizeValue samples,
    Arcadia_SizeValue* vertexCount);

Arcadia_BooleanValue
Arcadia_Math_NURBS3CurveReal64_sampleLines(
    const Arcadia_Real64Value (*points)[3], const Arcadia_Real64Value* weights,
    Arcadia_SizeValue count, Arcadia_SizeValue degree,
    const Arcadia_Real64Value* knots, Arcadia_SizeValue knotCount,
    Arcadia_SizeValue samples,
    Arcadia_Real64Value (*positions)[3], Arcadia_Real64Value (*tangents)[3],
    Arcadia_SizeValue vertexCapacity,
    Arcadia_Real64Value* workspace, Arcadia_SizeValue workspaceElements);

// Approximate arc length by summing sample-to-sample distances in double
// precision, with finite/representability checks. No output modification on
// failure. Uses caller-owned sampled-point storage (capacity >= samples).
Arcadia_BooleanValue
Arcadia_Math_NURBS3CurveReal64_approximateLength(
    const Arcadia_Real64Value (*positions)[3], Arcadia_SizeValue samples,
    Arcadia_Real64Value* result);

#endif // ARCADIA_MATH_IMPLEMENTATION_NURBS3CURVEREAL64_H_INCLUDED
