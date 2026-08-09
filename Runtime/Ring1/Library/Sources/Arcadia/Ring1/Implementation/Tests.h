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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_TESTS_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_TESTS_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Boolean.h"
#include "Arcadia/Ring1/Implementation/Diagnostics.h"
#include "Arcadia/Ring1/Implementation/Process.h"
#include "Arcadia/Ring1/Implementation/Thread.h"

#define Arcadia_Tests_assertTrue(thread, expression) \
  if (!(expression)) { \
    Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: test assertion true == `%s` failed\n", __FILE__, __LINE__, #expression); \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed); \
    Arcadia_Thread_jump(thread); \
  }

#define Arcadia_Tests_assertFalse(thread, expression) \
  if ((expression)) { \
    Arcadia_logf(Arcadia_LogFlags_Error, "%s:%d: test assertion false == `%s` failed\n", __FILE__, __LINE__, #expression); \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed); \
    Arcadia_Thread_jump(thread); \
  }

static inline Arcadia_BooleanValue
Arcadia_Tests_safeExecute
  (
    void (*f)(Arcadia_Thread*)
  )
{
  Arcadia_BooleanValue result = Arcadia_BooleanValue_True;
  Arcadia_Process* process = NULL;
  if (Arcadia_Process_get(&process)) {
    result = Arcadia_BooleanValue_False;
    return result;
  }
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(Arcadia_Process_getThread(process), &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    (*f)(Arcadia_Process_getThread(process));
  } else {
    result = Arcadia_BooleanValue_False;
  }
  Arcadia_Thread_popJumpTarget(Arcadia_Process_getThread(process));
  Arcadia_Status status = Arcadia_Thread_getStatus(Arcadia_Process_getThread(process));
  Arcadia_Process_relinquish(process);
  process = NULL;
  if (status) {
    result = Arcadia_BooleanValue_False;
  }
  return result;
}

#endif // ARCADIA_RING1_IMPLEMENTATION_TESTS_H_INCLUDED
