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

#include "Arcadia/Math/Implementation/setOrthographicProjection.h"

void
Arcadia_Math_Matrix4x4Real32_setOrthographicProjection
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real32* self,
    Arcadia_Real32Value left,
    Arcadia_Real32Value right,
    Arcadia_Real32Value bottom,
    Arcadia_Real32Value top,
    Arcadia_Real64Value near,
    Arcadia_Real64Value far
  )
{
  Arcadia_Real32Value a = +2.f / (right - left);
  Arcadia_Real32Value b = +2.f / (top - bottom);
  Arcadia_Real32Value c = -2.f / (far - near);
  Arcadia_Real32Value u = -(right + left) / (right - left);
  Arcadia_Real32Value v = -(top + bottom) / (top - bottom);
  Arcadia_Real32Value w = -(far + near) / (far - near);

  self->elements[0][0] = a;
  self->elements[1][0] = 0.f;
  self->elements[2][0] = 0.f;
  self->elements[3][0] = 0.f;

  self->elements[0][1] = 0.f;
  self->elements[1][1] = b;
  self->elements[2][1] = 0.f;
  self->elements[3][1] = 0.f;

  self->elements[0][2] = 0.f;
  self->elements[1][2] = 0.f;
  self->elements[2][2] = c;
  self->elements[3][2] = 0.f;

  self->elements[0][3] = u;
  self->elements[1][3] = v;
  self->elements[2][3] = w;
  self->elements[3][3] = 1.f;
}

void
Arcadia_Math_Matrix4x4Real64_setOrthographicProjection
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real64* self,
    Arcadia_Real64Value left,
    Arcadia_Real64Value right,
    Arcadia_Real64Value bottom,
    Arcadia_Real64Value top,
    Arcadia_Real64Value near,
    Arcadia_Real64Value far
  )
{
  Arcadia_Real64Value a = +2. / (right - left);
  Arcadia_Real64Value b = +2. / (top - bottom);
  Arcadia_Real64Value c = -2. / (far - near);
  Arcadia_Real64Value u = -(right + left) / (right - left);
  Arcadia_Real64Value v = -(top + bottom) / (top - bottom);
  Arcadia_Real64Value w = -(far + near) / (far - near);

  self->elements[0][0] = a;
  self->elements[1][0] = 0.;
  self->elements[2][0] = 0.;
  self->elements[3][0] = 0.;

  self->elements[0][1] = 0.;
  self->elements[1][1] = b;
  self->elements[2][1] = 0.;
  self->elements[3][1] = 0.;

  self->elements[0][2] = 0.;
  self->elements[1][2] = 0.;
  self->elements[2][2] = c;
  self->elements[3][2] = 0.;

  self->elements[0][3] = u;
  self->elements[1][3] = v;
  self->elements[2][3] = w;
  self->elements[3][3] = 1.;
}
