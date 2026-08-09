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

#if !defined(ARCADIA_RING1_SAFEADD_H_INCLUDED)
#define ARCADIA_RING1_SAFEADD_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Boolean.h"

#include "Arcadia/Ring1/Implementation/Integer16.h"
#include "Arcadia/Ring1/Implementation/Integer32.h"
#include "Arcadia/Ring1/Implementation/Integer64.h"
#include "Arcadia/Ring1/Implementation/Integer8.h"

#include "Arcadia/Ring1/Implementation/Natural16.h"
#include "Arcadia/Ring1/Implementation/Natural32.h"
#include "Arcadia/Ring1/Implementation/Natural64.h"
#include "Arcadia/Ring1/Implementation/Natural8.h"

#include "Arcadia/Ring1/Implementation/Process.h"

#include "Arcadia/Ring1/Implementation/Size.h"

// See Arcadia_safeAddInteger32Value for more information.
static inline void
Arcadia_safeAddFullInteger8Value
  (
    int8_t a,
    int8_t b,
    int8_t* hi,
    uint8_t* lo
  )
{
  uint8_t ua = (uint8_t)a;
  uint8_t ub = (uint8_t)b;

  uint8_t lo1 = ua + ub;
  uint8_t carry = (lo1 < ua); // unsigned carry out

  // signed high word: sign bits + carry.
  int8_t hi1 = (a >> 7) + (b >> 7) + (int8_t)carry;

  *lo = lo1;
  *hi = hi1;
}

// See Arcadia_safeAddInteger32Value for more information.
static inline void
Arcadia_safeAddFullInteger16Value
  (
    int16_t a,
    int16_t b,
    int16_t* hi,
    uint16_t* lo
  )
{
  uint16_t ua = (uint16_t)a;
  uint16_t ub = (uint16_t)b;

  uint16_t lo1 = ua + ub;
  uint16_t carry = (lo1 < ua); // unsigned carry out

  // signed high word: sign bits + carry.
  int16_t hi1 = (a >> 15) + (b >> 15) + (int16_t)carry;

  *lo = lo1;
  *hi = hi1;
}

// See Arcadia_safeAddInteger32Value for more information.
static inline void
Arcadia_safeAddFullInteger32Value
  (
    int32_t a,
    int32_t b,
    int32_t* hi,
    uint32_t* lo
  )
{
  uint32_t ua = (uint32_t)a;
  uint32_t ub = (uint32_t)b;

  uint32_t lo1 = ua + ub;
  uint32_t carry = (lo1 < ua); // unsigned carry out

  // signed high word: sign bits + carry.
  int32_t hi1 = (a >> 31) + (b >> 31) + (int32_t)carry;

  *lo = lo1;
  *hi = hi1;
}

// Two's complement is designed so that bit-level addition works both for signed and unsigned values.
// if
//  - high = 0 and low >> 63 == 0 OR
//  - high = -1 and low >> 63 == 1
// then the actual value is low cast to (int64_t).
// Otherwise there is an overflow.
// @code
// bool narrow_int64(int64_t high, uint64_t low, int64_t *out) {
//   /* Valid if high is just sign-extension of low */
//   if (high == 0 && (low >> 63) == 0) {
//     *out = (int64_t)low;
//     return true;
//   }
//   if (high == -1 && (low >> 63) == 1) {
//     *out = (int64_t)low;
//     return true;
//   }
//   return false; /* overflow */
// }
// @endcode
static inline void
Arcadia_safeAddFullInteger64Value
  (
    int64_t a,
    int64_t b,
    int64_t* hi,
    uint64_t* lo
  )
{
  uint64_t ua = (uint64_t)a;
  uint64_t ub = (uint64_t)b;

  uint64_t low = ua + ub;
  uint64_t carry = (low < ua); // unsigned carry out

  // signed high word: sign bits + carry.
  int64_t high = (a >> 63) + (b >> 63) + (int64_t)carry;

  *lo = low;
  *hi = high;
}

static inline Arcadia_BooleanValue
Arcadia_safeAddInteger8Value
  (
    int8_t a,
    int8_t b,
    int8_t* result
  )
{
  Arcadia_Integer8Value hi; Arcadia_Natural8Value lo;
  Arcadia_safeAddFullInteger8Value(a, b, &hi, &lo);
  /* Valid if high is just sign-extension of low */
  if (hi == 0 && (lo >> 7) == 0) {
    *result = (int8_t)lo;
    return Arcadia_BooleanValue_True; // No overflow.
  }
  if (hi == -1 && (lo >> 7) == 1) {
    *result = (int8_t)lo;
    return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False; // Overflow.
}

static inline Arcadia_BooleanValue
Arcadia_safeAddInteger16Value
  (
    int16_t a,
    int16_t b,
    int16_t* result
  )
{
  Arcadia_Integer16Value hi; Arcadia_Natural16Value lo;
  Arcadia_safeAddFullInteger16Value(a, b, &hi, &lo);
  /* Valid if high is just sign-extension of low */
  if (hi == 0 && (lo >> 15) == 0) {
    *result = (int16_t)lo;
    return Arcadia_BooleanValue_True; // No overflow.
  }
  if (hi == -1 && (lo >> 15) == 1) {
    *result = (int16_t)lo;
    return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False; // Overflow.
}

static inline Arcadia_BooleanValue
Arcadia_safeAddInteger32Value
  (
    int32_t a,
    int32_t b,
    int32_t* result
  )
{
  Arcadia_Integer32Value hi; Arcadia_Natural32Value lo;
  Arcadia_safeAddFullInteger32Value(a, b, &hi, &lo);
  /* Valid if high is just sign-extension of low */
  if (hi == 0 && (lo >> 31) == 0) {
    *result = (int32_t)lo;
    return Arcadia_BooleanValue_True; // No overflow.
  }
  if (hi == -1 && (lo >> 31) == 1) {
    *result = (int32_t)lo;
    return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False; // Overflow.
}

static inline Arcadia_BooleanValue
Arcadia_safeAddInteger64Value
  (
    int64_t a,
    int64_t b,
    int64_t* result
  )
{
  Arcadia_Integer64Value hi; Arcadia_Natural64Value lo;
  Arcadia_safeAddFullInteger64Value(a, b, &hi, &lo);
  /* Valid if high is just sign-extension of low */
  if (hi == 0 && (lo >> 63) == 0) {
    *result = (int64_t)lo;
    return Arcadia_BooleanValue_True; // No overflow.
  }
  if (hi == -1 && (lo >> 63) == 1) {
    *result = (int64_t)lo;
    return Arcadia_BooleanValue_True;
  }
  return Arcadia_BooleanValue_False; // Overflow.
}

void
Arcadia_safeAddFullNatural16Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value augend,
    Arcadia_Natural16Value addend,
    Arcadia_Natural16Value* sumHigh,
    Arcadia_Natural16Value* sumLow
  );

void
Arcadia_safeAddFullNatural32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value augend,
    Arcadia_Natural32Value addend,
    Arcadia_Natural32Value* sumHigh,
    Arcadia_Natural32Value* sumLow
  );

void
Arcadia_safeAddFullNatural64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value augend,
    Arcadia_Natural64Value addend,
    Arcadia_Natural64Value* sumHigh,
    Arcadia_Natural64Value* sumLow
  );

void
Arcadia_safeAddFullNatural8Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value augend,
    Arcadia_Natural8Value addend,
    Arcadia_Natural8Value* sumHigh,
    Arcadia_Natural8Value* sumLow
  );

void
Arcadia_safeAddFullSizeValue
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue augend,
    Arcadia_SizeValue addend,
    Arcadia_SizeValue* sumHigh,
    Arcadia_SizeValue* sumLow
  );

#endif // ARCADIA_RING1_SAFEADD_H_INCLUDED
