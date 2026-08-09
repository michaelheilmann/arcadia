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

#include "Arcadia/Math/Implementation/setTranslation.h"

void
Arcadia_Math_Matrix4x4Real32_setTranslation
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real32* self,
    Arcadia_Real32Value x,
    Arcadia_Real32Value y,
    Arcadia_Real32Value z
  )
{
  self->elements[0][0] = 1.f;
  self->elements[1][0] = 0.f;
  self->elements[2][0] = 0.f;
  self->elements[3][0] = 0.f;

  self->elements[0][1] = 0.f;
  self->elements[1][1] = 1.f;
  self->elements[2][1] = 0.f;
  self->elements[3][1] = 0.f;

  self->elements[0][2] = 0.f;
  self->elements[1][2] = 0.f;
  self->elements[2][2] = 1.f;
  self->elements[3][2] = 0.f;

  self->elements[0][3] = x;
  self->elements[1][3] = y;
  self->elements[2][3] = z;
  self->elements[3][3] = 1.f;
}

void
Arcadia_Math_Matrix4x4Real64_setTranslation
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real64* self,
    Arcadia_Real64Value x,
    Arcadia_Real64Value y,
    Arcadia_Real64Value z
  )
{
  self->elements[0][0] = 1.;
  self->elements[1][0] = 0.;
  self->elements[2][0] = 0.;
  self->elements[3][0] = 0.;

  self->elements[0][1] = 0.;
  self->elements[1][1] = 1.;
  self->elements[2][1] = 0.;
  self->elements[3][1] = 0.;

  self->elements[0][2] = 0.;
  self->elements[1][2] = 0.;
  self->elements[2][2] = 1.;
  self->elements[3][2] = 0.;

  self->elements[0][3] = x;
  self->elements[1][3] = y;
  self->elements[2][3] = z;
  self->elements[3][3] = 1.;
}
