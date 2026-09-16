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
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Arcadia/Ring1/Include.h"

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_WeakReference* weakReference =
    Arcadia_WeakReference_create
      (
        thread,
        Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, u8"x", sizeof(u8"x") - 1))
      );
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  Arcadia_Object_lock(thread, (Arcadia_Object*)weakReference);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_Value v;

    v = Arcadia_WeakReference_getValue(thread, weakReference);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isRuntimeUTF8StringValue(&v));

    Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false);

    v = Arcadia_WeakReference_getValue(thread, weakReference);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&v));

    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)weakReference);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)weakReference);
    Arcadia_Thread_jump(thread);
  }
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&test)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
