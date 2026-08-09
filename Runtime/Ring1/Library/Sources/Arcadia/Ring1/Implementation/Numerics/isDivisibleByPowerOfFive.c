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
#include "Arcadia/Ring1/Implementation/Numerics/isDivisibleByPowerOfFive.h"

Arcadia_BooleanValue
Arcadia_isDivisibleByPowerOfFiveNatural16
  (
    Arcadia_Thread* thread,
    const uint16_t v,
    const uint16_t p
  )
{
  #define Arcadia_Configuration_isPowerOfFiveNatural16_Implementation (0)

#if Arcadia_Configuration_isPowerOfFiveNatural16_Implementation == 0
  if (v == 0) {
    // v = 0 can never be any power of 5.
    return Arcadia_BooleanValue_False;
  }
  // Determine the greatest power of 5 by which v != 0 is divisible.
  uint16_t c = 0;
  uint16_t u = v;
  // If v % 5 is non-zero, then value is no power of 5.
  // Otherwise repeatedly divide by 5 to determine the power.
  while (1) {
    uint16_t q = u / 5;
    uint16_t r = u - 5 * q;
    if (r) {
      break;
    }
    u = q;
    c++;
  }
  return c >= p;
#elif Arcadia_Configuration_isPowerOfFiveNatural16_Implementation == 1
  // The maximal power of 5 that fits into an uint16_t is floor(log2(2^16 - 1)) = floor(log5(65535)) = 6.
  uint16_t q = 0;
  switch (v) {
    case 1: {
      return 0 >= p;
    } break;
    case 5: {
      return 1 >= p;
    } break;
    case 5*5: {
      return 2 >= p;
    } break;
    case 5*5*5: {
      return 3 >= p;
    } break;
    case 5*5*5*5: {
      return 4 >= p;
    } break;
    case 5*5*5*5*5: {
      return 5 >= p;
    } break;
    case 5*5*5*5*5*5: {
      return 6 >= p;
    } break;
    default: /* v > 5^6, v % 5 != 0, v = 0 */{
      return Arcadia_BooleanValue_False;
    } break;
  };
#else
  #error("unknown/unsupported implementation")
#endif

  #undef Arcadia_Configuration_isPowerOfFiveNatural16_Implementation
}

Arcadia_BooleanValue
Arcadia_isDivisibleByPowerOfFiveNatural32
  (
    Arcadia_Thread* thread,
    const uint32_t v,
    const uint32_t p
  )
{
  #define Arcadia_Configuration_isPowerOfFiveNatural32_Implementation (0)

#if Arcadia_Configuration_isPowerOfFiveNatural32_Implementation == 0
  if (v == 0) {
    // v = 0 can never be any power of 5.
    return Arcadia_BooleanValue_False;
  }
  // Determine the greatest power of 5 by which v != 0 is divisible.
  uint32_t c = 0;
  uint32_t u = v;
  // If v % 5 is non-zero, then value is no power of 5.
  // Otherwise repeatedly divide by 5 to determine the power.
  while (1) {
    uint32_t q = u / 5;
    uint32_t r = u - 5 * q;
    if (r) {
      break;
    }
    u = q;
    c++;
  }
  return c >= p;
#else
  #error("unknown/unsupported implementation")
#endif

  #undef Arcadia_Configuration_isPowerOfFiveNatural32_Implementation
}

Arcadia_BooleanValue
Arcadia_isDivisibleByPowerOfFiveNatural64
  (
    Arcadia_Thread* thread,
    const uint64_t v,
    const uint64_t p
  )
{
  #define Arcadia_Configuration_isPowerOfFiveNatural64_Implementation (0)

#if Arcadia_Configuration_isPowerOfFiveNatural64_Implementation == 0
  if (v == 0) {
    // v = 0 can never be any power of 5.
    return Arcadia_BooleanValue_False;
  }
  // Determine the greatest power of 5 by which v != 0 is divisible.
  uint64_t c = 0;
  uint64_t u = v;
  // If v % 5 is non-zero, then value is no power of 5.
  // Otherwise repeatedly divide by 5 to determine the power.
  while (1) {
    uint64_t q = u / 5;
    uint64_t r = u - 5 * q;
    if (r) {
      break;
    }
    u = q;
    c++;
  }
  return c >= p;
#else
  #error("unknown/unsupported implementation")
#endif

  #undef Arcadia_Configuration_isPowerOfFiveNatural64_Implementation
}

Arcadia_BooleanValue
Arcadia_isDivisibleByPowerOfFiveNatural8
  (
    Arcadia_Thread* thread,
    const uint8_t v,
    const uint8_t p
  )
{
#define Arcadia_Configuration_isPowerOfFiveNatural8_Implementation (0)

#if Arcadia_Configuration_isPowerOfFiveNatural8_Implementation == 0
  if (v == 0) {
    // v = 0 can never be any power of 5.
    return Arcadia_BooleanValue_False;
  }
  // Determine the greatest power of 5 by which v != 0 is divisible.
  uint32_t c = 0;
  uint32_t u = v;
  // If v % 5 is non-zero, then value is no power of 5.
  // Otherwise repeatedly divide by 5 to determine the power.
  while (1) {
    uint32_t q = u / 5;
    uint32_t r = u - 5 * q;
    if (r) {
      break;
    }
    u = q;
    c++;
  }
  return c >= p;
#elif Arcadia_Configuration_isPowerOfFiveNatural8_Implementation == 1
  switch (v) {
    case 1: {
      return 0 >= p;
    } break;
    case 5: {
      return 1 >= p;
    } break;
    case 5*5: {
      return 2 >= p;
    } break;
    default: /* v > 5*5, v % 5 != 0, v = 0 */{
      return Arcadia_BooleanValue_False;
    } break;
  };
#else
  #error("unknown/unsupported implementation")
#endif

  #undef Arcadia_Configuration_isPowerOfFiveNatural8_Implementation
}
