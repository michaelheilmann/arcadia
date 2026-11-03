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

#include "Arcadia/Math/Implementation/quadraticBSplineSegment.h"

#include <assert.h>

void
Arcadia_Math_Vector3Real32_quadraticBSplineSegment
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real32* p0,
    Arcadia_Math_Vector3Real32* p1,
    Arcadia_Math_Vector3Real32* p2,
    Arcadia_Real32Value t,
    Arcadia_Math_Vector3Real32* result
  )
{
  assert(thread && p0 && p1 && p2 && result);
  (void)thread;
  // Uniform quadratic B-spline weights: 0.5(1-t)^2,
  // 0.5 + t - t^2, 0.5 t^2. Sum is one for every t.
  Arcadia_Real32Value s = 1.f - t;
  Arcadia_Real32Value w0 = 0.5f * s * s;
  Arcadia_Real32Value w1 = 0.5f + t - t * t;
  Arcadia_Real32Value w2 = 0.5f * t * t;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    result->elements[i] = w0 * p0->elements[i] +
                          w1 * p1->elements[i] +
                          w2 * p2->elements[i];
  }
}

void
Arcadia_Math_Vector3Real64_quadraticBSplineSegment
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real64* p0,
    Arcadia_Math_Vector3Real64* p1,
    Arcadia_Math_Vector3Real64* p2,
    Arcadia_Real64Value t,
    Arcadia_Math_Vector3Real64* result
  )
{
  assert(thread && p0 && p1 && p2 && result);
  (void)thread;
  // Uniform quadratic B-spline weights: 0.5(1-t)^2,
  // 0.5 + t - t^2, 0.5 t^2. Sum is one for every t.
  Arcadia_Real64Value s = 1. - t;
  Arcadia_Real64Value w0 = 0.5 * s * s;
  Arcadia_Real64Value w1 = 0.5 + t - t * t;
  Arcadia_Real64Value w2 = 0.5 * t * t;
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    result->elements[i] = w0 * p0->elements[i] +
                          w1 * p1->elements[i] +
                          w2 * p2->elements[i];
  }
}
