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
#include "Arcadia/Ring1/Implementation/Numerics/isPowerOfTwo.h"

#include "Arcadia/Ring1/Include.h"

Arcadia_BooleanValue
Arcadia_isPowerOfTwoInteger8Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer8Value x
  )
{
  if (x <= 0) return Arcadia_BooleanValue_False;
  else return 0 == (x & (x - 1));
}

Arcadia_BooleanValue
Arcadia_isPowerOfTwoInteger16Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer16Value x
  )
{
  if (x <= 0) return Arcadia_BooleanValue_False;
  else return 0 == (x & (x - 1));
}

Arcadia_BooleanValue
Arcadia_isPowerOfTwoInteger32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value x
  )
{
  if (x <= 0) return Arcadia_BooleanValue_False;
  else return 0 == (x & (x - 1));
}

Arcadia_BooleanValue
Arcadia_isPowerOfTwoInteger64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value x
  )
{
  if (!x) return Arcadia_BooleanValue_False;
  else return 0 == (x & (x - 1));
}

Arcadia_BooleanValue
Arcadia_isPowerOfTwoNatural8Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value x
  )
{
  if (!x) return Arcadia_BooleanValue_False;
  else return 0 == (x & (x - 1));
}

Arcadia_BooleanValue
Arcadia_isPowerOfTwoNatural16Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value x
  )
{
  if (!x) return Arcadia_BooleanValue_False;
  else return 0 == (x & (x - 1));
}

Arcadia_BooleanValue
Arcadia_isPowerOfTwoNatural32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value x
  )
{
  if (!x) return Arcadia_BooleanValue_False;
  else return 0 == (x & (x - 1));
}

Arcadia_BooleanValue
Arcadia_isPowerOfTwoNatural64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value x
  )
{
  if (!x) return Arcadia_BooleanValue_False;
  else return 0 == (x & (x - 1));
}

Arcadia_BooleanValue
Arcadia_isPowerOfTwoSizeValue
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue x
  )
{
#if Arcadia_Configuration_InstructionSetArchitecture == Arcadia_Configuration_InstructionSetArchitecture_X64
  return Arcadia_isPowerOfTwoNatural64Value(thread, (Arcadia_Natural64Value)x);
#elif Arcadia_Configuration_InstructionSetArchitecture == Arcadia_Configuration_InstructionSetArchitecture_X86
  return Arcadia_isPowerOfTwoNatural32Value(thread, (Arcadia_Natural32Value)x);
#else
  #error("instruction set architecture not (yet) supported")
#endif
}
