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

#include <stdlib.h>

#include "Arcadia/Math/Include.h"

static void
test1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Real32Value axisValues[] = { 0.f, 1.f, 0.f };
  
  Arcadia_Math_QuaternionReal32* p = Arcadia_Math_QuaternionReal32_create(thread, 0.f, 0.f, 0.f, 0.f);
  Arcadia_Math_QuaternionReal32_setFromAxisAngle(thread, p, Arcadia_Math_Vector3Real32_createWithValues(thread, axisValues), 0.f);
  Arcadia_Math_Matrix4Real32* a = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_QuaternionReal32_toRotationMatrix(thread, p, a);
  
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&test1)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
