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

#if !defined(ARCADIA_MATH_IMPLEMENTATION_AABB3REAL64_H_INCLUDED)
#define ARCADIA_MATH_IMPLEMENTATION_AABB3REAL64_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

// Native axis-aligned 3D bounds. Empty is a separate state, not an invalid
// floating-point sentinel. A newly declared value must be initialized.
typedef struct Arcadia_Math_AABB3Real64 {
  Arcadia_Real64Value minimum[3];
  Arcadia_Real64Value maximum[3];
  Arcadia_BooleanValue empty;
} Arcadia_Math_AABB3Real64;

void Arcadia_Math_AABB3Real64_setEmpty(Arcadia_Math_AABB3Real64* self);
// Normalizes each min/max pair, so the result is always nonempty and ordered.
void Arcadia_Math_AABB3Real64_setMinMax(Arcadia_Math_AABB3Real64* self, const Arcadia_Real64Value minimum[3], const Arcadia_Real64Value maximum[3]);
void Arcadia_Math_AABB3Real64_includePoint(Arcadia_Math_AABB3Real64* self, const Arcadia_Real64Value point[3]);
// All operations permit aliasing of result with either input box.
void Arcadia_Math_AABB3Real64_union(Arcadia_Math_AABB3Real64* result, const Arcadia_Math_AABB3Real64* a, const Arcadia_Math_AABB3Real64* b);
// Touching faces/edges/vertices count as a nonempty intersection.
void Arcadia_Math_AABB3Real64_intersection(Arcadia_Math_AABB3Real64* result, const Arcadia_Math_AABB3Real64* a, const Arcadia_Math_AABB3Real64* b);
Arcadia_BooleanValue Arcadia_Math_AABB3Real64_containsPoint(const Arcadia_Math_AABB3Real64* self, const Arcadia_Real64Value point[3]);
void Arcadia_Math_AABB3Real64_translate(Arcadia_Math_AABB3Real64* self, Arcadia_Real64Value x, Arcadia_Real64Value y, Arcadia_Real64Value z);

#endif // ARCADIA_MATH_IMPLEMENTATION_AABB3REAL64_H_INCLUDED
