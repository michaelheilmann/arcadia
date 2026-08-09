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
#include "Arcadia/Ring1/Implementation/safeAdd.h"

#include "Arcadia/Ring1/Implementation/Thread.h"

void
Arcadia_safeAddFullNatural16Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value augend,
    Arcadia_Natural16Value addend,
    Arcadia_Natural16Value* sumHigh,
    Arcadia_Natural16Value* sumLow
  )
{
  if (!sumHigh | !sumLow) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
#if 0
  Arcadia_Natural16Value temporary = augend + addend;
  *sumHigh = temporary < augend ? 1 : 0; /*Alternatively temporary < addend would also work.*/
  *sumLow = temporary;
#else
  Arcadia_Natural32Value temporary = (Arcadia_Natural32Value)augend + (Arcadia_Natural32Value)addend;
  *sumHigh = (Arcadia_Natural16Value)(temporary >> 16);
  *sumLow = (Arcadia_Natural16Value)(temporary & 0x0000ffff);
#endif
}

void
Arcadia_safeAddFullNatural32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value augend,
    Arcadia_Natural32Value addend,
    Arcadia_Natural32Value* sumHigh,
    Arcadia_Natural32Value* sumLow
  )
{
  if (!sumHigh | !sumLow) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
#if 0
  Arcadia_Natural32Value temporary = augend + addend;
  *sumHigh = temporary < augend ? 1 : 0; /*Alternatively temporary < addend would also work.*/
  *sumLow = temporary;
#else
  Arcadia_Natural64Value temporary = (Arcadia_Natural64Value)augend + (Arcadia_Natural64Value)addend;
  *sumHigh = (Arcadia_Natural32Value)(temporary >> 32);
  *sumLow = (Arcadia_Natural32Value)(temporary & 0x00000000ffffffff);
#endif
}

void
Arcadia_safeAddFullNatural64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value augend,
    Arcadia_Natural64Value addend,
    Arcadia_Natural64Value* sumHigh,
    Arcadia_Natural64Value* sumLow
  )
{
  if (!sumHigh | !sumLow) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }

  // if two unsigned n>0 bit values are added then the result is at most an unsigned n+1 bit value.
  // more precisely, the maximum value of the result is 2^(n + 1) - 2.
  //
  // Proof:
  //   2^n - 1 + 2^n - 1
  // = 2 x 2^n - 2
  // = 2^(n+1) - 2
  // q.e.d.
  //
  // Given unconstrained arithmetic:
  // (x + y) / n will return the topmost bit which is either 0 or 1 and is hi.
  // (x + y) % n will return the lower 64 bit which are lo.
  // Given the rules of C/C++ uintx_t arithmetic, (x + y) % n is actually computed by the standard addition so we can simply x + y to compute lo.
  Arcadia_Natural64Value x = augend + addend;
  *sumHigh = x < augend ? 1 : 0;
  *sumLow = x;
}

void
Arcadia_safeAddFullNatural8Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value augend,
    Arcadia_Natural8Value addend,
    Arcadia_Natural8Value* sumHigh,
    Arcadia_Natural8Value* sumLow
  )
{
  if (!sumHigh | !sumLow) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
#if 0
  Arcadia_Natural8Value temporary = augend + addend;
  *sumHigh = temporary < augend ? 1 : 0; /*Alternatively temporary < addend would also work.*/
  *sumLow = temporary;
#else
  Arcadia_Natural16Value temporary = (Arcadia_Natural16Value)augend + (Arcadia_Natural16Value)addend;
  *sumHigh = (Arcadia_Natural8Value)(temporary >> 8);
  *sumLow = (Arcadia_Natural8Value)(temporary & 0x00ff);
#endif
}

void
Arcadia_safeAddFullSizeValue
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue augend,
    Arcadia_SizeValue addend,
    Arcadia_SizeValue* sumHigh,
    Arcadia_SizeValue* sumLow
  ) {
  if (!sumHigh | !sumLow) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
#if Arcadia_Configuration_CompilerC_Msvc == Arcadia_Configuration_CompilerC
  #if Arcadia_Configuration_InstructionSetArchitecture_X64 == Arcadia_Configuration_InstructionSetArchitecture
    Arcadia_StaticAssert(Arcadia_SizeValue_Maximum == Arcadia_Natural64Value_Maximum && Arcadia_SizeValue_NumberOfBits == Arcadia_Natural64Value_NumberOfBits, "environment not (yet) supported");
    Arcadia_safeAddFullNatural64Value(thread, augend, addend, (Arcadia_Natural64Value*)sumHigh, (Arcadia_Natural64Value*)sumLow);
  #elif Arcadia_Configuration_InstructionSetArchitecture_X86 == Arcadia_Configuration_InstructionSetArchitecture
    Arcadia_StaticAssert(Arcadia_SizeValue_Maximum == Arcadia_Natural32Value_Maximum && Arcadia_SizeValue_NumberOfBits == Arcadia_Natural32Value_NumberOfBits, "environment not (yet) supported");
    Arcadia_safeAddFullNatural32Value(thread, augend, addend, (Arcadia_Natural32Value*)sumHigh, (Arcadia_Natural32Value*)sumLow);
  #else
    #error ("environemnt not (yet) supported");
  #endif
#elif Arcadia_Configuration_CompilerC_Gcc == Arcadia_Configuration_CompilerC
  #if Arcadia_Configuration_InstructionSetArchitecture_X64 == Arcadia_Configuration_InstructionSetArchitecture
    Arcadia_StaticAssert(Arcadia_SizeValue_Maximum == Arcadia_Natural64Value_Maximum && Arcadia_SizeValue_NumberOfBits == Arcadia_Natural64Value_NumberOfBits, "environment not (yet) supported");
    Arcadia_safeAddFullNatural64Value(thread, augend, addend, (Arcadia_Natural64Value*)sumHigh, (Arcadia_Natural64Value*)sumLow);
  #elif Arcadia_Configuration_InstructionSetArchitecture_X86 == Arcadia_Configuration_InstructionSetArchitecture
    Arcadia_StaticAssert(Arcadia_SizeValue_Maximum == Arcadia_Natural32Value_Maximum && Arcadia_SizeValue_NumberOfBits == Arcadia_Natural32Value_NumberOfBits, "environment not (yet) supported");
    Arcadia_safeAddFullNatural32Value(thread, augend, addend, (Arcadia_Natural32Value*)sumHigh, (Arcadia_Natural32Value*)sumLow);
  #else
    #error("environemnt not (yet) supported");
  #endif
#else
  #error ("environemnt not (yet) supported");
#endif
}
