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

#include "Arcadia/Math/Implementation/rayIntersectsPlane.h"

#include <assert.h>
#include <math.h>

Arcadia_BooleanValue
Arcadia_Math_Vector3Real32_rayIntersectsPlane
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real32* origin,
    Arcadia_Math_Vector3Real32* direction,
    Arcadia_Math_Vector3Real32* normal,
    Arcadia_Real32Value offset,
    Arcadia_Real32Value* parameter
  )
{
  assert(thread && origin && direction && normal && parameter);
  Arcadia_Real32Value denominator = normal->elements[0] * direction->elements[0] +
                                    normal->elements[1] * direction->elements[1] +
                                    normal->elements[2] * direction->elements[2];
  if (denominator == 0.f || !isfinite(denominator)) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Real32Value numerator = -(normal->elements[0] * origin->elements[0] +
                                    normal->elements[1] * origin->elements[1] +
                                    normal->elements[2] * origin->elements[2] + offset);
  Arcadia_Real32Value t = numerator / denominator;
  if (!(t >= 0.f) || !isfinite(t)) {
    return Arcadia_BooleanValue_False;
  }
  *parameter = t;
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_Math_Vector3Real64_rayIntersectsPlane
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real64* origin,
    Arcadia_Math_Vector3Real64* direction,
    Arcadia_Math_Vector3Real64* normal,
    Arcadia_Real64Value offset,
    Arcadia_Real64Value* parameter
  )
{
  assert(thread && origin && direction && normal && parameter);
  Arcadia_Real64Value denominator = normal->elements[0] * direction->elements[0] +
                                    normal->elements[1] * direction->elements[1] +
                                    normal->elements[2] * direction->elements[2];
  if (denominator == 0. || !isfinite(denominator)) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Real64Value numerator = -(normal->elements[0] * origin->elements[0] +
                                    normal->elements[1] * origin->elements[1] +
                                    normal->elements[2] * origin->elements[2] + offset);
  Arcadia_Real64Value t = numerator / denominator;
  if (!(t >= 0.) || !isfinite(t)) {
    return Arcadia_BooleanValue_False;
  }
  *parameter = t;
  return Arcadia_BooleanValue_True;
}
