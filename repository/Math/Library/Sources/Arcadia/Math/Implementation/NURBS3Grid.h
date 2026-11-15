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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_NURBS3GRID_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_NURBS3GRID_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

// Geometry grid companion to NURBS3Real{32,64}_sampleSurface.
// All arrays are u-major: vertex(i,j) = i * samplesV + j.
// Both sample dimensions must be >= 2. Caller owns all output storage.
// On invalid dimensions or insufficient capacity, output arrays remain unchanged.
// Arrays must not overlap each other. No allocations or managed objects.
// getCounts returns exact element counts, rejecting Arcadia_SizeValue overflow.
Arcadia_BooleanValue
Arcadia_Math_NURBS3Grid_getCounts
  (
    Arcadia_SizeValue samplesU,
    Arcadia_SizeValue samplesV,
    Arcadia_SizeValue* vertexCount,
    Arcadia_SizeValue* triangleCount
  );

// Two counterclockwise triangles per quad when a surface's normal is
// cross(dP/du, dP/dv). Each triangle is three zero-based vertex indices.
// Required capacity: triangleCount from getCounts (not index count).
Arcadia_BooleanValue
Arcadia_Math_NURBS3Grid_makeTriangles
  (
    Arcadia_SizeValue samplesU,
    Arcadia_SizeValue samplesV,
    Arcadia_SizeValue (*triangles)[3],
    Arcadia_SizeValue triangleCapacity
  );

// Regular normalized parameter-space UV coordinates, including (0,0) and
// (1,1). These work with arbitrary (nonzero-width) NURBS knot domains;
// they represent normalized sampling coordinates rather than distances.
// Required capacity: vertexCount from getCounts (not scalar count).
Arcadia_BooleanValue
Arcadia_Math_NURBS3Grid_makeUVReal32
  (
    Arcadia_SizeValue samplesU,
    Arcadia_SizeValue samplesV,
    Arcadia_Real32Value (*uv)[2],
    Arcadia_SizeValue vertexCapacity
  );

Arcadia_BooleanValue
Arcadia_Math_NURBS3Grid_makeUVReal64
  (
    Arcadia_SizeValue samplesU,
    Arcadia_SizeValue samplesV,
    Arcadia_Real64Value (*uv)[2],
    Arcadia_SizeValue vertexCapacity
  );

#endif // ARCADIA_MATH_IMPLEMENTATION_NURBS3GRID_H_INCLUDED
