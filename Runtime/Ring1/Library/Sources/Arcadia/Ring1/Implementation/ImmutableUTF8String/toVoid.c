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

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/toVoid.h"

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String.h"
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/NumeralParser.h"

Arcadia_VoidValue
_toVoid
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* immutableUTF8StringValue
  )
{
  _State state;
  _State_init(&state, Arcadia_RuntimeUTF8String_getBytes(thread, immutableUTF8StringValue),
                      Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, immutableUTF8StringValue));
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    #define check(x) \
      if (!is(&state, (x))) { \
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ConversionFailed); \
        Arcadia_Thread_jump(thread); \
      } \

    next(&state);
    check('v');
    check('o');
    check('i');
    check('d');
    check(_Unicode_CodePoint_End);

    #undef check
    _State_uninit(&state);
    Arcadia_Thread_popJumpTarget(thread);
  } else {
    _State_uninit(&state);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Thread_jump(thread);
  }

  return Arcadia_VoidValue_Void;
}
