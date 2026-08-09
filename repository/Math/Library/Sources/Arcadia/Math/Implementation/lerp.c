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

#include "Arcadia/Math/Implementation/lerp.h"

void
Arcadia_Math_Color4Real32Value_lerp
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Color4Real32* self,
    Arcadia_Math_Color4Real32* other,
    Arcadia_Real32Value t,
    Arcadia_Math_Color4Real32* result
  )
{
  if (t == 0.f) {
    Arcadia_Math_Color4Real32_assign(thread, result, self);
  } else if (t == 1.f) {
    Arcadia_Math_Color4Real32_assign(thread, result, other);
  } else {
    Arcadia_Real32Value s = 1.f - t;
    result->components[0] = Arcadia_clampReal32Value(thread, s * (self->components[0]) + t * (other->components[0]), 0.f, 1.f);
    result->components[1] = Arcadia_clampReal32Value(thread, s * (self->components[1]) + t * (other->components[1]), 0.f, 1.f);
    result->components[2] = Arcadia_clampReal32Value(thread, s * (self->components[2]) + t * (other->components[2]), 0.f, 1.f);
    result->components[3] = Arcadia_clampReal32Value(thread, s * (self->components[3]) + t * (other->components[3]), 0.f, 1.f);
  }
}

void
Arcadia_Math_Color4Real64Value_lerp
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Color4Real64* self,
    Arcadia_Math_Color4Real64* other,
    Arcadia_Real64Value t,
    Arcadia_Math_Color4Real64* result
  )
{
  if (t == 0.) {
    Arcadia_Math_Color4Real64_assign(thread, result, self);
  } else if (t == 1.) {
    Arcadia_Math_Color4Real64_assign(thread, result, other);
  } else {
    Arcadia_Real64Value s = 1. - t;
    result->components[0] = Arcadia_clampReal64Value(thread, s * (self->components[0]) + t * (other->components[0]), 0.f, 1.f);
    result->components[1] = Arcadia_clampReal64Value(thread, s * (self->components[1]) + t * (other->components[1]), 0.f, 1.f);
    result->components[2] = Arcadia_clampReal64Value(thread, s * (self->components[2]) + t * (other->components[2]), 0.f, 1.f);
    result->components[3] = Arcadia_clampReal64Value(thread, s * (self->components[3]) + t * (other->components[3]), 0.f, 1.f);
  }
}

void
Arcadia_Math_Vector3Real32Value_lerp
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real32* self,
    Arcadia_Math_Vector3Real32* other,
    Arcadia_Real32Value t,
    Arcadia_Math_Vector3Real32* result
  )
{
  if (t == 0.f) {
    Arcadia_Math_Vector3Real32_assign(thread, result, self);
  } else if (t == 1.f) {
    Arcadia_Math_Vector3Real32_assign(thread, result, other);
  } else {
    Arcadia_Real32Value s = 1.f - t;
    result->elements[0] = s * (self->elements[0]) + t * (other->elements[0]);
    result->elements[1] = s * (self->elements[1]) + t * (other->elements[1]);
    result->elements[2] = s * (self->elements[2]) + t * (other->elements[2]);
  }
}

void 
Arcadia_Math_Vector4Real32Value_lerp
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector4Real32* self,
    Arcadia_Math_Vector4Real32* other,
    Arcadia_Real32Value t,
    Arcadia_Math_Vector4Real32* result
  )
{
  if (t == 0.f) {
    Arcadia_Math_Vector4Real32_assign(thread, result, self);
  } else if (t == 1.f) {
    Arcadia_Math_Vector4Real32_assign(thread, result, other);
  } else {
    Arcadia_Real32Value s = 1.f - t;
    result->elements[0] = s * (self->elements[0]) + t * (other->elements[0]);
    result->elements[1] = s * (self->elements[1]) + t * (other->elements[1]);
    result->elements[2] = s * (self->elements[2]) + t * (other->elements[2]);
    result->elements[3] = s * (self->elements[3]) + t * (other->elements[3]);
  } 
}

void
Arcadia_Math_Vector3Real64Value_lerp
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real64* self,
    Arcadia_Math_Vector3Real64* other,
    Arcadia_Real64Value t,
    Arcadia_Math_Vector3Real64* result
  )
{
  if (t == 0.) {
    Arcadia_Math_Vector3Real64_assign(thread, result, self);
  } else if (t == 1.) {
    Arcadia_Math_Vector3Real64_assign(thread, result, other);
  } else {
    Arcadia_Real64Value s = 1. - t;
    result->elements[0] = s * (self->elements[0]) + t * (other->elements[0]);
    result->elements[1] = s * (self->elements[1]) + t * (other->elements[1]);
    result->elements[2] = s * (self->elements[2]) + t * (other->elements[2]);
  }
}

void
Arcadia_Math_Vector4Real64Value_lerp
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector4Real64* self,
    Arcadia_Math_Vector4Real64* other,
    Arcadia_Real64Value t,
    Arcadia_Math_Vector4Real64* result
  )
{
  if (t == 0.) {
    Arcadia_Math_Vector4Real64_assign(thread, result, self);
  } else if (t == 1.) {
    Arcadia_Math_Vector4Real64_assign(thread, result, other);
  } else {
    Arcadia_Real64Value s = 1. - t;
    result->elements[0] = s * (self->elements[0]) + t * (other->elements[0]);
    result->elements[1] = s * (self->elements[1]) + t * (other->elements[1]);
    result->elements[2] = s * (self->elements[2]) + t * (other->elements[2]);
    result->elements[3] = s * (self->elements[3]) + t * (other->elements[3]);
  }
}
