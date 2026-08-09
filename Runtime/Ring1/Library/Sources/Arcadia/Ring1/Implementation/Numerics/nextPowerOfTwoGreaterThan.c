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
#include "Arcadia/Ring1/Implementation/Numerics/nextPowerOfTwoGreaterThan.h"

#include "Arcadia/Ring1/Include.h"

#define Define(Type, Suffix, Bits) \
Type##Value \
Arcadia_nextPowerOfTwoGreaterThan##Suffix##Value \
  ( \
    Arcadia_Thread* thread, \
    Type##Value x \
  ) \
{ \
  /* i is Bits is x is 0. i is Bits - 1 if x is 1. i is Bits - 2 if x is 2 or 3. ... i is 0 if x >= greatestPowerOfTwo. Consequently the shift is Bits - i.*/ \
  Arcadia_SizeValue numberOfLeadingZeroes = Arcadia_countLeadingZeroes##Suffix##Value(thread, x); \
  if (!numberOfLeadingZeroes) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NotExists); \
    Arcadia_Thread_jump(thread); \
  } \
  Arcadia_SizeValue shift = Bits - numberOfLeadingZeroes; \
  Type##Value t = Type##Value_Literal(1) << shift; \
  return t; \
}

Define(Arcadia_Natural8, Natural8, Arcadia_Natural8Value_NumberOfBits)
Define(Arcadia_Natural16, Natural16, Arcadia_Natural16Value_NumberOfBits)
Define(Arcadia_Natural32, Natural32, Arcadia_Natural32Value_NumberOfBits)
Define(Arcadia_Natural64, Natural64, Arcadia_Natural64Value_NumberOfBits)

#undef Define

Arcadia_SizeValue
R_nextPowerOfTwoGreaterThanSizeValue
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue x
  )
{
#if Arcadia_Configuration_InstructionSetArchitecture == Arcadia_Configuration_InstructionSetArchitecture_X64
  return Arcadia_nextPowerOfTwoGreaterThanNatural64Value(thread, x);
#elif Arcadia_Configuration_InstructionSetArchitecture == Arcadia_Configuration_InstructionSetArchitecture_X86
  return Arcadia_nextPowerOfTwoGreaterThanNatural32Value(thread, x);
#else
  #error("environment not (yet) supported")
#endif
}
