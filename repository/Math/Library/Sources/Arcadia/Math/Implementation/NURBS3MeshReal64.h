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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_NURBS3MESHREAL64_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_NURBS3MESHREAL64_H_INCLUDED

#include "Arcadia/Math/Implementation/NURBS3DifferentialsReal64.h"
#include "Arcadia/Math/Implementation/NURBS3Grid.h"

// Draw-arrays bridge for a NURBS surface. The caller owns all output arrays
// and the existing NURBS differential workspace. No managed objects or heap.
// Vertex order is the same as NURBS3Grid_makeTriangles: two triangles per
// (u,v) cell, each with three unshared vertices. UV is normalized parameter
// space, with exact 0 and 1 at the grid endpoints.
//
// getVertexCount checks every multiplication before storing any output.
// On an early validation/capacity failure no output is modified; numerical
// errors during evaluation can leave a prefix of vertices written. Inputs,
// outputs and workspace must not overlap. With normals == NULL, degenerate
// surfaces are accepted; with normals present they are rejected.
Arcadia_BooleanValue
Arcadia_Math_NURBS3MeshReal64_getVertexCount(
    Arcadia_SizeValue samplesU, Arcadia_SizeValue samplesV,
    Arcadia_SizeValue* vertexCount);

// The caller allocates vertexCount items in positions and uv, and (optionally)
// normals. Capacity is measured in vertices. Workspace needs
// 4*(degreeU+degreeV+2) scalar elements; see NURBS3DifferentialsReal64.h.
Arcadia_BooleanValue
Arcadia_Math_NURBS3MeshReal64_sampleTriangles(
    const Arcadia_Real64Value (*points)[3], const Arcadia_Real64Value* weights,
    Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    Arcadia_SizeValue degreeU, Arcadia_SizeValue degreeV,
    const Arcadia_Real64Value* knotsU, Arcadia_SizeValue knotCountU,
    const Arcadia_Real64Value* knotsV, Arcadia_SizeValue knotCountV,
    Arcadia_SizeValue samplesU, Arcadia_SizeValue samplesV,
    Arcadia_Real64Value (*positions)[3], Arcadia_Real64Value (*normals)[3],
    Arcadia_Real64Value (*uv)[2], Arcadia_SizeValue vertexCapacity,
    Arcadia_Real64Value* workspace, Arcadia_SizeValue workspaceElements);

#endif // ARCADIA_MATH_IMPLEMENTATION_NURBS3MESHREAL64_H_INCLUDED
