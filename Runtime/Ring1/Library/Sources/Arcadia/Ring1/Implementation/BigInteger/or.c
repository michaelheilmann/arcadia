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
#include "Arcadia/Ring1/Implementation/BigInteger/or.h"

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring1/Implementation/BigInteger/toTwosComplement.h"
#include <assert.h>

void
Arcadia_BigInteger_or3
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* result,
    Arcadia_BigInteger* a,
    Arcadia_BigInteger* b
  )
{
  if (a == b) {
    Arcadia_BigInteger_copy(thread, result, a);
  } else if (Arcadia_BigInteger_isZero(thread, a)) {
    Arcadia_BigInteger_copy(thread, result, b);
  } else if (Arcadia_BigInteger_isZero(thread, b)) {
    Arcadia_BigInteger_copy(thread, result, a);
  } else {
    Arcadia_BigInteger_Limp* x = NULL,
                           * y = NULL,
                           * z = NULL;
    Arcadia_SizeValue xl, yl, zl;

    Arcadia_BigInteger_Limp xextension = (a->sign < 0) ? Arcadia_BigInteger_Limp_Maximum : Arcadia_BigInteger_Limp_Literal(0);
    Arcadia_BigInteger_Limp yextension = (b->sign < 0) ? Arcadia_BigInteger_Limp_Maximum : Arcadia_BigInteger_Limp_Literal(0);

    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      Arcadia_BigInteger_toTwosComplement(thread, a, &x, &xl);
      Arcadia_BigInteger_toTwosComplement(thread, b, &y, &yl);
      zl = Arcadia_maximumSizeValue(thread, xl, yl);
      Arcadia_Memory_reallocateUnmanaged(thread, (void**)&z, sizeof(Arcadia_BigInteger_Limp) * zl);
      for (Arcadia_SizeValue i = 0; i < zl; ++i) {
        Arcadia_BigInteger_Limp xu = i < xl ? x[i] : xextension;
        Arcadia_BigInteger_Limp yu = i < yl ? y[i] : yextension;
        z[i] = xu | yu;
      }
      Arcadia_BigInteger_fromTwosComplement(thread, result, z, zl);
      Arcadia_Memory_deallocateUnmanaged(thread, z);
      z = NULL;
      Arcadia_Memory_deallocateUnmanaged(thread, y);
      y = NULL;
      Arcadia_Memory_deallocateUnmanaged(thread, x);
      x = NULL;
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      if (z) {
        Arcadia_Memory_deallocateUnmanaged(thread, z);
        z = NULL;
      }
      if (y) {
        Arcadia_Memory_deallocateUnmanaged(thread, y);
        y = NULL;
      }
      if (x) {
        Arcadia_Memory_deallocateUnmanaged(thread, x);
        x = NULL;
      }
      Arcadia_Thread_jump(thread);
    }
  }
}

void
Arcadia_BigInteger_or
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger* other
  )
{ Arcadia_BigInteger_or3(thread, self, self, other); }
