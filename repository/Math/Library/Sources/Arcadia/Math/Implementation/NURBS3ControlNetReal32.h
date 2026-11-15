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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_NURBS3CONTROLNETREAL32_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_NURBS3CONTROLNETREAL32_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

// Native control-polygon and control-net overlay geometry.
// All inputs are packed xyz vertices. Surface grids use u-major indexing:
// index(u,v) = u * countV + v. A curve emits its consecutive edges.
// A surface first emits all u-edges in increasing (u,v) order, then all
// v-edges in increasing (u,v) order; each edge becomes two distinct vertices.
// No wraparound and no skipped degenerate edges. Caller owns output buffers.
// Optional sourcePairs maps each output line to its original point indices.
// Arrays must not overlap their input points or one another. On validation
// failure all outputs are unchanged. No heap/GC/renderer dependencies.
Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlNetReal32_countCurve(Arcadia_SizeValue count, Arcadia_SizeValue* lineCount, Arcadia_SizeValue* vertexCount);

Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlNetReal32_countSurface(Arcadia_SizeValue countU, Arcadia_SizeValue countV,
    Arcadia_SizeValue* lineCount, Arcadia_SizeValue* vertexCount);

Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlNetReal32_emitCurve(const Arcadia_Real32Value (*points)[3], Arcadia_SizeValue count,
    Arcadia_Real32Value (*vertices)[3], Arcadia_SizeValue vertexCapacity,
    Arcadia_SizeValue (*sourcePairs)[2], Arcadia_SizeValue pairCapacity);

Arcadia_BooleanValue
Arcadia_Math_NURBS3ControlNetReal32_emitSurface(const Arcadia_Real32Value (*points)[3], Arcadia_SizeValue countU,
    Arcadia_SizeValue countV, Arcadia_Real32Value (*vertices)[3], Arcadia_SizeValue vertexCapacity,
    Arcadia_SizeValue (*sourcePairs)[2], Arcadia_SizeValue pairCapacity);

#endif // ARCADIA_MATH_IMPLEMENTATION_NURBS3CONTROLNETREAL32_H_INCLUDED
