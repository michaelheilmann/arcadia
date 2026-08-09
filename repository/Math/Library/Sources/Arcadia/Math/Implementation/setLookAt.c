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

#include "Arcadia/Math/Implementation/crossProduct.h"
#include "Arcadia/Math/Implementation/trigonometry.h"
#include "Arcadia/Math/Implementation/setTranslation.h"

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
  Arcadia_Math_Vector3Real32 *forward, *right, *up1, *up2;
  Arcadia_Math_Matrix4Real32 *r, *t;

  // forward := norm(target - source)
  forward = Arcadia_Math_Vector3Real32_create(thread);
  Arcadia_Math_Vector3Real32_assign(thread, forward, target);
  Arcadia_Math_Vector3Real32_subtract(thread, forward, source);
  Arcadia_Math_Vector3Real32_normalize(thread, forward);
  // right := forward x norm(up)
  up1 = Arcadia_Math_Vector3Real32_create(thread);
  Arcadia_Math_Vector3Real32_assign(thread, up1, up);
  Arcadia_Math_Vector3Real32_normalize(thread, up1);
  right = Arcadia_Math_Vector3Real32_create(thread);
  Arcadia_Math_Vector3Real32_assign(thread, right, forward);
  Arcadia_Math_Vector3Real32_crossProduct(thread, right, up1);
  // up' := right x forward
  up2 = Arcadia_Math_Vector3Real32_create(thread);
  Arcadia_Math_Vector3Real32_assign(thread, up2, right);
  Arcadia_Math_Vector3Real32_crossProduct(thread, up2, forward);

  r = Arcadia_Math_Matrix4Real32_create(thread);
  // column #1
  r->elements[0][0] = right->elements[0];
  r->elements[1][0] = up2->elements[0];
  r->elements[2][0] = -forward->elements[0];
  r->elements[3][0] = 0.f;
  // column #2
  r->elements[0][1] = right->elements[1];
  r->elements[1][1] = up2->elements[1];
  r->elements[2][1] = -forward->elements[1];
  r->elements[3][1] = 0.f;
  // column #3
  r->elements[0][2] = right->elements[2];
  r->elements[1][2] = up2->elements[2];
  r->elements[2][2] = -forward->elements[2];
  r->elements[3][2] = 0.f;
  // column #4
  r->elements[0][3] = 0.f;
  r->elements[1][3] = 0.f;
  r->elements[2][3] = 0.f;
  r->elements[3][3] = 1.f;

  t = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_Matrix4x4Real32_setTranslation(thread, t, -source->elements[0], -source->elements[1], -source->elements[2]);
 
  Arcadia_Math_Matrix4Real32_multiply(thread, r, t);
  Arcadia_Math_Matrix4Real32_assign(thread, self, r);
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
  Arcadia_Math_Vector3Real64* forward, * right, * up1, * up2;
  Arcadia_Math_Matrix4Real64* r, * t;

  // forward := norm(target - source)
  forward = Arcadia_Math_Vector3Real64_create(thread);
  Arcadia_Math_Vector3Real64_assign(thread, forward, target);
  Arcadia_Math_Vector3Real64_subtract(thread, forward, source);
  Arcadia_Math_Vector3Real64_normalize(thread, forward);
  // right := forward x norm(up)
  up1 = Arcadia_Math_Vector3Real64_create(thread);
  Arcadia_Math_Vector3Real64_assign(thread, up1, up);
  Arcadia_Math_Vector3Real64_normalize(thread, up1);
  right = Arcadia_Math_Vector3Real64_create(thread);
  Arcadia_Math_Vector3Real64_assign(thread, right, forward);
  Arcadia_Math_Vector3Real64_crossProduct(thread, right, up1);
  // up' := right x forward
  up2 = Arcadia_Math_Vector3Real64_create(thread);
  Arcadia_Math_Vector3Real64_assign(thread, up2, right);
  Arcadia_Math_Vector3Real64_crossProduct(thread, up2, forward);

  r = Arcadia_Math_Matrix4Real64_create(thread);
  // column #1
  r->elements[0][0] = right->elements[0];
  r->elements[1][0] = up2->elements[0];
  r->elements[2][0] = -forward->elements[0];
  r->elements[3][0] = 0.;
  // column #2
  r->elements[0][1] = right->elements[1];
  r->elements[1][1] = up2->elements[1];
  r->elements[2][1] = -forward->elements[1];
  r->elements[3][1] = 0.;
  // column #3
  r->elements[0][2] = right->elements[2];
  r->elements[1][2] = up2->elements[2];
  r->elements[2][2] = -forward->elements[2];
  r->elements[3][2] = 0.;
  // column #4
  r->elements[0][3] = 0.;
  r->elements[1][3] = 0.;
  r->elements[2][3] = 0.;
  r->elements[3][3] = 1.;

  t = Arcadia_Math_Matrix4Real64_create(thread);
  Arcadia_Math_Matrix4x4Real64_setTranslation(thread, t, -source->elements[0], -source->elements[1], -source->elements[2]);

  Arcadia_Math_Matrix4Real64_multiply(thread, r, t);
  Arcadia_Math_Matrix4Real64_assign(thread, self, r);
}
