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

#include "Arcadia/Math/Implementation/crossProduct.h"

Arcadia_Math_Vector3Real32*
Arcadia_Math_Vector3Real32_crossProduct
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real32* u,
    Arcadia_Math_Vector3Real32* v
  )
{
  Arcadia_Real32Value values[] =
    {
      u->elements[1] * v->elements[2] - u->elements[2] * v->elements[1],
      u->elements[2] * v->elements[0] - u->elements[0] * v->elements[2],
      u->elements[0] * v->elements[1] - u->elements[1] * v->elements[0],
    };
  return Arcadia_Math_Vector3Real32_createWithValues(thread, values);
}

Arcadia_Math_Vector3Real64*
Arcadia_Math_Vector3Real64_crossProduct
  (
    Arcadia_Thread* thread,
    Arcadia_Math_Vector3Real64* u,
    Arcadia_Math_Vector3Real64* v
  )
{
  Arcadia_Real64Value values[] =
  {
    u->elements[1] * v->elements[2] - u->elements[2] * v->elements[1],
    u->elements[2] * v->elements[0] - u->elements[0] * v->elements[2],
    u->elements[0] * v->elements[1] - u->elements[1] * v->elements[0],
  };
  return Arcadia_Math_Vector3Real64_createWithValues(thread, values);
}
