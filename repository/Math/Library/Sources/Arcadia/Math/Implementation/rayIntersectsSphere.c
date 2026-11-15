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

#include "Arcadia/Math/Implementation/rayIntersectsSphere.h"

#include <assert.h>

Arcadia_BooleanValue
Arcadia_Math_Vector3Real32_rayIntersectsSphere
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real32* origin,
    Arcadia_Math_Vector3Real32* direction,
    Arcadia_Math_Vector3Real32* center,
    Arcadia_Real32Value radius
  )
{
  assert(thread && origin && direction && center);
  if (radius < 0.f) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Real32Value dx = direction->elements[0];
  Arcadia_Real32Value dy = direction->elements[1];
  Arcadia_Real32Value dz = direction->elements[2];
  Arcadia_Real32Value lengthSquared = dx * dx + dy * dy + dz * dz;
  if (lengthSquared == 0.f) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Real32Value wx = center->elements[0] - origin->elements[0];
  Arcadia_Real32Value wy = center->elements[1] - origin->elements[1];
  Arcadia_Real32Value wz = center->elements[2] - origin->elements[2];
  Arcadia_Real32Value projection = wx * dx + wy * dy + wz * dz;
  Arcadia_Real32Value distanceSquared = wx * wx + wy * wy + wz * wz;
  Arcadia_Real32Value radiusSquared = radius * radius;
  if (distanceSquared <= radiusSquared) {
    return Arcadia_BooleanValue_True;
  }
  if (projection < 0.f) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Real32Value perpendicularSquared = distanceSquared - (projection * projection) / lengthSquared;
  return perpendicularSquared <= radiusSquared ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
}

Arcadia_BooleanValue
Arcadia_Math_Vector3Real64_rayIntersectsSphere
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real64* origin,
    Arcadia_Math_Vector3Real64* direction,
    Arcadia_Math_Vector3Real64* center,
    Arcadia_Real64Value radius
  )
{
  assert(thread && origin && direction && center);
  if (radius < 0.) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Real64Value dx = direction->elements[0];
  Arcadia_Real64Value dy = direction->elements[1];
  Arcadia_Real64Value dz = direction->elements[2];
  Arcadia_Real64Value lengthSquared = dx * dx + dy * dy + dz * dz;
  if (lengthSquared == 0.) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Real64Value wx = center->elements[0] - origin->elements[0];
  Arcadia_Real64Value wy = center->elements[1] - origin->elements[1];
  Arcadia_Real64Value wz = center->elements[2] - origin->elements[2];
  Arcadia_Real64Value projection = wx * dx + wy * dy + wz * dz;
  Arcadia_Real64Value distanceSquared = wx * wx + wy * wy + wz * wz;
  Arcadia_Real64Value radiusSquared = radius * radius;
  if (distanceSquared <= radiusSquared) {
    return Arcadia_BooleanValue_True;
  }
  if (projection < 0.) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Real64Value perpendicularSquared = distanceSquared - (projection * projection) / lengthSquared;
  return perpendicularSquared <= radiusSquared ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
}
