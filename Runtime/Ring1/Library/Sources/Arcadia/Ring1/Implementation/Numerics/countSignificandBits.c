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
#include "Arcadia/Ring1/Implementation/Numerics/countSignificandBits.h"

#include "Arcadia/Ring1/Include.h"

#if Arcadia_Configuration_CompilerC == Arcadia_Configuration_CompilerC_Msvc
  #include <intrin.h>
#endif

Arcadia_SizeValue
Arcadia_countSignificandBitsInteger8Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer8Value x
  )
{
  // Cast is defined behavior: Two's complete cast signed to unsigned is Bit reinterpretation when 2-complement.
  return Arcadia_countSignificandBitsNatural8Value(thread, (Arcadia_Natural8Value)x);
}

Arcadia_SizeValue
Arcadia_countSignificandBitsInteger16Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer16Value x
  )
{
  // Cast is defined behavior: Two's complete cast signed to unsigned is Bit reinterpretation when 2-complement.
  return Arcadia_countSignificandBitsNatural16Value(thread, (Arcadia_Natural16Value)x);
}

Arcadia_SizeValue
Arcadia_countSignificandBitsInteger32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value x
  )
{
  // Cast is defined behavior: Two's complete cast signed to unsigned is Bit reinterpretation when 2-complement.
  return Arcadia_countSignificandBitsNatural32Value(thread, (Arcadia_Natural32Value)x);
}

Arcadia_SizeValue
Arcadia_countSignificandBitsInteger64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value x
  )
{
  // Cast is defined behavior: Two's complete cast signed to unsigned is Bit reinterpretation when 2-complement.
  return Arcadia_countSignificandBitsNatural64Value(thread, (Arcadia_Natural64Value)x);
}

Arcadia_SizeValue
Arcadia_countSignificandBitsNatural8Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value x
  )
{
  return 8 - Arcadia_countLeadingZeroesNatural8Value(thread, x);
}

Arcadia_SizeValue
Arcadia_countSignificandBitsNatural16Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value x
  )
{
  return 16 - Arcadia_countLeadingZeroesNatural16Value(thread, x);
}

Arcadia_SizeValue
Arcadia_countSignificandBitsNatural32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value x
  )
{
  return 32 - Arcadia_countLeadingZeroesNatural32Value(thread, x);
}

Arcadia_SizeValue
Arcadia_countSignificandBitsNatural64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value x
  )
{
  return 64 - Arcadia_countLeadingZeroesNatural64Value(thread, x);
}

Arcadia_SizeValue
Arcadia_countSignificandBitsSizeValue
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue x
  )
{
#if Arcadia_Configuration_InstructionSetArchitecture == Arcadia_Configuration_InstructionSetArchitecture_X64
  return Arcadia_countSignificandBitsNatural64Value(thread, x);
#elif Arcadia_Configuration_InstructionSetArchitecture == Arcadia_Configuration_InstructionSetArchitecture_X86
  return Arcadia_countSignificandBitsNatural32Value(thread, x);
#else
  #error("environment not (yet) supported")
#endif
}
