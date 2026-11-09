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

#include "Arcadia/Math/Implementation/AABB3Real32.h"

void
Arcadia_Math_AABB3Real32_setEmpty(Arcadia_Math_AABB3Real32* self)
{
  self->empty = Arcadia_BooleanValue_True;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    self->minimum[i] = 0;
    self->maximum[i] = 0;
  }
}

void
Arcadia_Math_AABB3Real32_setMinMax(Arcadia_Math_AABB3Real32* self, const Arcadia_Real32Value minimum[3], const Arcadia_Real32Value maximum[3])
{
  // Capture the values first so either array may overlap the destination.
  Arcadia_Math_AABB3Real32 temporary;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Arcadia_Real32Value a = minimum[i];
    Arcadia_Real32Value b = maximum[i];
    temporary.minimum[i] = a < b ? a : b;
    temporary.maximum[i] = a > b ? a : b;
  }
  temporary.empty = Arcadia_BooleanValue_False;
  *self = temporary;
}

void
Arcadia_Math_AABB3Real32_includePoint(Arcadia_Math_AABB3Real32* self, const Arcadia_Real32Value point[3])
{
  // point may point at a component of self.
  Arcadia_Real32Value coordinates[3] = { point[0], point[1], point[2] };
  if (self->empty) {
    Arcadia_Math_AABB3Real32_setMinMax(self, coordinates, coordinates);
    return;
  }
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    if (coordinates[i] < self->minimum[i]) self->minimum[i] = coordinates[i];
    if (coordinates[i] > self->maximum[i]) self->maximum[i] = coordinates[i];
  }
}

void
Arcadia_Math_AABB3Real32_union(Arcadia_Math_AABB3Real32* result, const Arcadia_Math_AABB3Real32* a, const Arcadia_Math_AABB3Real32* b)
{
  if (a->empty) {
    *result = *b;
    return;
  }
  if (b->empty) {
    *result = *a;
    return;
  }
  Arcadia_Math_AABB3Real32 temporary;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    temporary.minimum[i] = a->minimum[i] < b->minimum[i] ? a->minimum[i] : b->minimum[i];
    temporary.maximum[i] = a->maximum[i] > b->maximum[i] ? a->maximum[i] : b->maximum[i];
  }
  temporary.empty = Arcadia_BooleanValue_False;
  *result = temporary;
}

void
Arcadia_Math_AABB3Real32_intersection(Arcadia_Math_AABB3Real32* result, const Arcadia_Math_AABB3Real32* a, const Arcadia_Math_AABB3Real32* b)
{
  if (a->empty || b->empty) {
    Arcadia_Math_AABB3Real32_setEmpty(result);
    return;
  }
  Arcadia_Math_AABB3Real32 temporary;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    temporary.minimum[i] = a->minimum[i] > b->minimum[i] ? a->minimum[i] : b->minimum[i];
    temporary.maximum[i] = a->maximum[i] < b->maximum[i] ? a->maximum[i] : b->maximum[i];
    if (temporary.minimum[i] > temporary.maximum[i]) {
      Arcadia_Math_AABB3Real32_setEmpty(result);
      return;
    }
  }
  temporary.empty = Arcadia_BooleanValue_False;
  *result = temporary;
}

Arcadia_BooleanValue
Arcadia_Math_AABB3Real32_containsPoint(const Arcadia_Math_AABB3Real32* self, const Arcadia_Real32Value point[3])
{
  if (self->empty) return Arcadia_BooleanValue_False;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    if (point[i] < self->minimum[i] || point[i] > self->maximum[i]) {
      return Arcadia_BooleanValue_False;
    }
  }
  return Arcadia_BooleanValue_True;
}

void
Arcadia_Math_AABB3Real32_translate(Arcadia_Math_AABB3Real32* self, Arcadia_Real32Value x, Arcadia_Real32Value y, Arcadia_Real32Value z)
{
  if (self->empty) return;
  const Arcadia_Real32Value translation[3] = { x, y, z };
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    self->minimum[i] += translation[i];
    self->maximum[i] += translation[i];
  }
}
