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
#include "Arcadia/Ring1/Implementation/Numerics/countLeadingZeroes.h"

#include "Arcadia/Ring1/Include.h"

#if Arcadia_Configuration_CompilerC == Arcadia_Configuration_CompilerC_Msvc
  #include <intrin.h>
#endif

Arcadia_SizeValue
Arcadia_countLeadingZeroesInteger8Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer8Value x
  )
{
  // Cast is defined behavior: Two's complete cast signed to unsigned is Bit reinterpretation when 2-complement.
  return Arcadia_countLeadingZeroesNatural8Value(thread, (Arcadia_Natural8Value)x);
}

Arcadia_SizeValue
Arcadia_countLeadingZeroesInteger16Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer16Value x
  )
{
  // Cast is defined behavior: Two's complete cast signed to unsigned is Bit reinterpretation when 2-complement.
  return Arcadia_countLeadingZeroesNatural16Value(thread, (Arcadia_Natural16Value)x);
}

Arcadia_SizeValue
Arcadia_countLeadingZeroesInteger32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value x
  )
{
  // Cast is defined behavior: Two's complete cast signed to unsigned is Bit reinterpretation when 2-complement.
  return Arcadia_countLeadingZeroesNatural32Value(thread, (Arcadia_Natural32Value)x);
}

Arcadia_SizeValue
Arcadia_countLeadingZeroesInteger64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value x
  )
{
  // Cast is defined behavior: Two's complete cast signed to unsigned is Bit reinterpretation when 2-complement.
  return Arcadia_countLeadingZeroesNatural64Value(thread, (Arcadia_Natural64Value)x);
}

Arcadia_SizeValue
Arcadia_countLeadingZeroesNatural8Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value x
  )
{
  // Maps a value from 0000 (0x0) to 1111 (0xf) the value's number of leading zero bits.
  static const short lookup[16] = {
    4, //  0: 0000
    3, //  1: 0001
    2, //  2: 0010
    2, //  3: 0011
    1, //  4: 0100
    1, //  5: 0101
    1, //  6: 0110
    1, //  7: 0111
    0, //  8: 1000
    0, //  9: 1001
    0, // 10: 1010
    0, // 11: 1011
    0, // 12: 1100
    0, // 13: 1101
    0, // 14: 1110
    0, // 15: 1111
  };
  Arcadia_Natural8Value upper = (x & 0xf0) >> 4;
  Arcadia_Natural8Value lower = (x & 0x0f) >> 0;
  return upper ? lookup[upper] : 4 + lookup[lower];
}

Arcadia_SizeValue
Arcadia_countLeadingZeroesNatural16Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value x
  )
{
  Arcadia_Natural16Value upper = (x & 0xff00) >> 8;
  if (upper) {
    return Arcadia_countLeadingZeroesNatural8Value(thread, (Arcadia_Natural8Value)upper);
  } else {
    Arcadia_Natural16Value lower = (x & 0x00ff) >> 0;
    return 8 + Arcadia_countLeadingZeroesNatural8Value(thread, (Arcadia_Natural8Value)lower);
  }
}

Arcadia_SizeValue
Arcadia_countLeadingZeroesNatural32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value x
  )
{
#if Arcadia_Configuration_CompilerC == Arcadia_Configuration_CompilerC_Msvc
  unsigned long n;
  if (_BitScanReverse(&n, x)) {
    // x was not zero.
    return (short)32 - ((short)n + 1);
  } else {
    // x was zero.
    return (short)32;
  }
#else
  Arcadia_Natural32Value upper = (x & 0xffff0000) >> 16;
  if (upper) {
    return Arcadia_countLeadingZeroesNatural16Value(thread, (Arcadia_Natural16Value)upper);
  } else {
    Arcadia_Natural32Value lower = (x & 0x0000ffff) >> 0;
    return 16 + Arcadia_countLeadingZeroesNatural16Value(thread, (Arcadia_Natural16Value)lower);
  }
#endif
}

Arcadia_SizeValue
Arcadia_countLeadingZeroesNatural64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value x
  )
{
  Arcadia_Natural32Value upper = (x & 0xffffffff00000000) >> 32;
  if (upper) {
    return Arcadia_countLeadingZeroesNatural32Value(thread, upper);
  } else {
    Arcadia_Natural32Value lower = (x & 0x00000000ffffffff) >> 0;
    return 32 + Arcadia_countLeadingZeroesNatural32Value(thread, lower);
  }
}

Arcadia_SizeValue
Arcadia_countLeadingZeroesSizeValue
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue x
  )
{
#if Arcadia_Configuration_InstructionSetArchitecture == Arcadia_Configuration_InstructionSetArchitecture_X64
  return Arcadia_countLeadingZeroesNatural64Value(thread, x);
#elif Arcadia_Configuration_InstructionSetArchitecture == Arcadia_Configuration_InstructionSetArchitecture_X86
  return Arcadia_countLeadingZeroesNatural32Value(thread, x);
#else
  #error("environment not (yet) supported")
#endif
}
