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

#include "Arcadia/Math/Implementation/setLookAt.h"

#include <math.h>

void
Arcadia_Math_Matrix4x4Real32_setLookAt
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real32* self,
    Arcadia_Math_Vector3Real32* source,
    Arcadia_Math_Vector3Real32* target,
    Arcadia_Math_Vector3Real32* up
  )
{
  Arcadia_Real32Value forward[3], right[3], correctedUp[3], normalizedUp[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    forward[i] = target->elements[i] - source->elements[i];
  }
  Arcadia_Real32Value length = hypotf(hypotf(forward[0], forward[1]), forward[2]);
  Arcadia_Real32Value upLength = hypotf(hypotf(up->elements[0], up->elements[1]), up->elements[2]);
  if (length == 0.f || upLength == 0.f || !isfinite(length) || !isfinite(upLength)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    forward[i] /= length;
    normalizedUp[i] = up->elements[i] / upLength;
  }
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Arcadia_SizeValue j = (i + 1) % 3, k = (i + 2) % 3;
    right[i] = forward[j] * normalizedUp[k] - forward[k] * normalizedUp[j];
  }
  length = hypotf(hypotf(right[0], right[1]), right[2]);
  if (length == 0.f || !isfinite(length)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    right[i] /= length;
  }
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Arcadia_SizeValue j = (i + 1) % 3, k = (i + 2) % 3;
    correctedUp[i] = right[j] * forward[k] - right[k] * forward[j];
  }
  // All validation precedes destination writes, so failed calls preserve self.
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    self->elements[0][i] = right[i];
    self->elements[1][i] = correctedUp[i];
    self->elements[2][i] = -forward[i];
    self->elements[3][i] = 0.f;
  }
  self->elements[0][3] = -(right[0] * source->elements[0] + right[1] * source->elements[1] + right[2] * source->elements[2]);
  self->elements[1][3] = -(correctedUp[0] * source->elements[0] + correctedUp[1] * source->elements[1] + correctedUp[2] * source->elements[2]);
  self->elements[2][3] = forward[0] * source->elements[0] + forward[1] * source->elements[1] + forward[2] * source->elements[2];
  self->elements[3][3] = 1.f;
}

void
Arcadia_Math_Matrix4x4Real64_setLookAt
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real64* self,
    Arcadia_Math_Vector3Real64* source,
    Arcadia_Math_Vector3Real64* target,
    Arcadia_Math_Vector3Real64* up
  )
{
  Arcadia_Real64Value forward[3], right[3], correctedUp[3], normalizedUp[3];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    forward[i] = target->elements[i] - source->elements[i];
  }
  Arcadia_Real64Value length = hypot(hypot(forward[0], forward[1]), forward[2]);
  Arcadia_Real64Value upLength = hypot(hypot(up->elements[0], up->elements[1]), up->elements[2]);
  if (length == 0. || upLength == 0. || !isfinite(length) || !isfinite(upLength)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    forward[i] /= length;
    normalizedUp[i] = up->elements[i] / upLength;
  }
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Arcadia_SizeValue j = (i + 1) % 3, k = (i + 2) % 3;
    right[i] = forward[j] * normalizedUp[k] - forward[k] * normalizedUp[j];
  }
  length = hypot(hypot(right[0], right[1]), right[2]);
  if (length == 0. || !isfinite(length)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    right[i] /= length;
  }
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    Arcadia_SizeValue j = (i + 1) % 3, k = (i + 2) % 3;
    correctedUp[i] = right[j] * forward[k] - right[k] * forward[j];
  }
  // All validation precedes destination writes, so failed calls preserve self.
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    self->elements[0][i] = right[i];
    self->elements[1][i] = correctedUp[i];
    self->elements[2][i] = -forward[i];
    self->elements[3][i] = 0.;
  }
  self->elements[0][3] = -(right[0] * source->elements[0] + right[1] * source->elements[1] + right[2] * source->elements[2]);
  self->elements[1][3] = -(correctedUp[0] * source->elements[0] + correctedUp[1] * source->elements[1] + correctedUp[2] * source->elements[2]);
  self->elements[2][3] = forward[0] * source->elements[0] + forward[1] * source->elements[1] + forward[2] * source->elements[2];
  self->elements[3][3] = 1.;
}
