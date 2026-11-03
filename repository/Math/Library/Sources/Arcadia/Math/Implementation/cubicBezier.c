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

#include "Arcadia/Math/Implementation/cubicBezier.h"

#include <assert.h>

void
Arcadia_Math_Vector3Real32_cubicBezier
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real32* p0,
    Arcadia_Math_Vector3Real32* p1,
    Arcadia_Math_Vector3Real32* p2,
    Arcadia_Math_Vector3Real32* p3,
    Arcadia_Real32Value t,
    Arcadia_Math_Vector3Real32* result
  )
{
  assert(thread && p0 && p1 && p2 && p3 && result);
  Arcadia_Real32Value s = 1.f - t;
  Arcadia_Real32Value a = s * s * s;
  Arcadia_Real32Value b = 3.f * s * s * t;
  Arcadia_Real32Value c = 3.f * s * t * t;
  Arcadia_Real32Value d = t * t * t;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    result->elements[i] = a * p0->elements[i] + b * p1->elements[i] +
                          c * p2->elements[i] + d * p3->elements[i];
  }
}

void
Arcadia_Math_Vector3Real64_cubicBezier
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real64* p0,
    Arcadia_Math_Vector3Real64* p1,
    Arcadia_Math_Vector3Real64* p2,
    Arcadia_Math_Vector3Real64* p3,
    Arcadia_Real64Value t,
    Arcadia_Math_Vector3Real64* result
  )
{
  assert(thread && p0 && p1 && p2 && p3 && result);
  Arcadia_Real64Value s = 1. - t;
  Arcadia_Real64Value a = s * s * s;
  Arcadia_Real64Value b = 3. * s * s * t;
  Arcadia_Real64Value c = 3. * s * t * t;
  Arcadia_Real64Value d = t * t * t;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    result->elements[i] = a * p0->elements[i] + b * p1->elements[i] +
                          c * p2->elements[i] + d * p3->elements[i];
  }
}
