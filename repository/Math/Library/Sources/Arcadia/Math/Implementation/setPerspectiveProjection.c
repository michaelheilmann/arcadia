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

#include "Arcadia/Math/Implementation/setPerspectiveProjection.h"

#include "Arcadia/Math/Implementation/trigonometry.h"

void
Arcadia_Math_Matrix4x4Real32_setPerspectiveProjection
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real32* self,
    Arcadia_Real32Value fieldOfView,
    Arcadia_Real32Value aspectRatio,
    Arcadia_Real32Value near,
    Arcadia_Real32Value far
  )
{
  fieldOfView = Arcadia_Math_degreesToRadiansReal32Value(thread, fieldOfView);
  Arcadia_Real32Value f = 1 / tanf(fieldOfView / 2.f);

  self->elements[0][0] = f / aspectRatio;
  self->elements[1][0] = 0.f;
  self->elements[2][0] = 0.f;
  self->elements[3][0] = 0.f;

  self->elements[0][1] = 0.f;
  self->elements[1][1] = f;
  self->elements[2][1] = 0.f;
  self->elements[3][1] = 0.f;

  self->elements[0][2] = 0.f;
  self->elements[1][2] = 0.f;
  self->elements[2][2] = (far + near) / (near - far);
  self->elements[3][2] = -1.f;

  self->elements[0][3] = 0.f;
  self->elements[1][3] = 0.f;
  self->elements[2][3] = (2.f * far * near) / (near - far);
  self->elements[3][3] = 0.f;
}

void
Arcadia_Math_Matrix4x4Real64_setPerspectiveProjection
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real64* self,
    Arcadia_Real64Value fieldOfView,
    Arcadia_Real64Value aspectRatio,
    Arcadia_Real64Value near,
    Arcadia_Real64Value far
  )
{
  fieldOfView = Arcadia_Math_degreesToRadiansReal64Value(thread, fieldOfView);
  Arcadia_Real64Value f = 1 / tan(fieldOfView / 2.);

  self->elements[0][0] = f / aspectRatio;
  self->elements[1][0] = 0.;
  self->elements[2][0] = 0.;
  self->elements[3][0] = 0.;

  self->elements[0][1] = 0.;
  self->elements[1][1] = f;
  self->elements[2][1] = 0.;
  self->elements[3][1] = 0.;

  self->elements[0][2] = 0.;
  self->elements[1][2] = 0.;
  self->elements[2][2] = (far + near) / (near - far);
  self->elements[3][2] = -1.;

  self->elements[0][3] = 0.;
  self->elements[1][3] = 0.;
  self->elements[2][3] = (2.f * far * near) / (near - far);
  self->elements[3][3] = 0.;
}
