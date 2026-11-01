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

#include "Arcadia/Math/Implementation/transformPoint.h"

void
Arcadia_Math_Matrix4x4Real32_transformPoint
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real32* self,
    Arcadia_Math_Vector3Real32* point
  )
{
  Arcadia_Real32Value x = point->elements[0], y = point->elements[1], z = point->elements[2];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    point->elements[i] = self->elements[i][0] * x + self->elements[i][1] * y
                      + self->elements[i][2] * z + self->elements[i][3];
  }
}

void
Arcadia_Math_Matrix4x4Real64_transformPoint
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Matrix4Real64* self,
    Arcadia_Math_Vector3Real64* point
  )
{
  Arcadia_Real64Value x = point->elements[0], y = point->elements[1], z = point->elements[2];
  for (Arcadia_SizeValue i = 0; i < 3; ++i) {
    point->elements[i] = self->elements[i][0] * x + self->elements[i][1] * y
                      + self->elements[i][2] * z + self->elements[i][3];
  }
}
