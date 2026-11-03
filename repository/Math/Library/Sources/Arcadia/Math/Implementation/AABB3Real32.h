// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
// for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#if !defined(ARCADIA_MATH_IMPLEMENTATION_AABB3REAL32_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_AABB3REAL32_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

// Native axis-aligned 3D bounds. Empty is a separate state, not an invalid
// floating-point sentinel. A newly declared value must be initialized.
typedef struct Arcadia_Math_AABB3Real32 {
  Arcadia_Real32Value minimum[3];
  Arcadia_Real32Value maximum[3];
  Arcadia_BooleanValue empty;
} Arcadia_Math_AABB3Real32;

void Arcadia_Math_AABB3Real32_setEmpty(Arcadia_Math_AABB3Real32* self);
// Normalizes each min/max pair, so the result is always nonempty and ordered.
void Arcadia_Math_AABB3Real32_setMinMax(Arcadia_Math_AABB3Real32* self, const Arcadia_Real32Value minimum[3], const Arcadia_Real32Value maximum[3]);
void Arcadia_Math_AABB3Real32_includePoint(Arcadia_Math_AABB3Real32* self, const Arcadia_Real32Value point[3]);
// All operations permit aliasing of result with either input box.
void Arcadia_Math_AABB3Real32_union(Arcadia_Math_AABB3Real32* result, const Arcadia_Math_AABB3Real32* a, const Arcadia_Math_AABB3Real32* b);
// Touching faces/edges/vertices count as a nonempty intersection.
void Arcadia_Math_AABB3Real32_intersection(Arcadia_Math_AABB3Real32* result, const Arcadia_Math_AABB3Real32* a, const Arcadia_Math_AABB3Real32* b);
Arcadia_BooleanValue Arcadia_Math_AABB3Real32_containsPoint(const Arcadia_Math_AABB3Real32* self, const Arcadia_Real32Value point[3]);
void Arcadia_Math_AABB3Real32_translate(Arcadia_Math_AABB3Real32* self, Arcadia_Real32Value x, Arcadia_Real32Value y, Arcadia_Real32Value z);

#endif // ARCADIA_MATH_IMPLEMENTATION_AABB3REAL32_H_INCLUDED
