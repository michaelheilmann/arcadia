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
#include "Arcadia/Ring1/Implementation/BigInteger/getHigh64.h"

#include "Arcadia/Ring1/Include.h"

// shl = clz(a)
// truncated = false
// return a << shl
static Arcadia_Natural64Value
high64_64_1
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value a,
    Arcadia_BooleanValue* truncated
  );

// shl = clz(a0)
// if shl = 0 then
//   truncated = true
//   return a0
// else
//   shr = 64 - shl
//   truncated = a1 << shl /* determine if there a bits we are not able to shift in */
//   return (a0 << shl) || (a1 >> shr)
// endif
static Arcadia_Natural64Value
high64_64_2
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value a0,
    Arcadia_Natural64Value a1,
    Arcadia_BooleanValue* truncated
  );

// return hi64_64_1(a, truncated)
static Arcadia_Natural64Value
high64_32_1
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value a,
    Arcadia_BooleanValue* truncated
  );

// x0 = (n64)a0
// x1 = (n64)a1
// return hi64_64_1((x0 << 32) | x1, truncated);
static Arcadia_Natural64Value
high64_32_2
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value a0,
    Arcadia_Natural32Value a1,
    Arcadia_BooleanValue* truncated
  );

// x0 = (n64)a0
// x1 = (n64)a1
// x2 = (n64)a2
// return hi64_64_2(x0, (x1 << 32) | x2, truncated);
static Arcadia_Natural64Value
high64_32_3
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value a0,
    Arcadia_Natural32Value a1,
    Arcadia_Natural32Value a2,
    Arcadia_BooleanValue* truncated
  );

static Arcadia_Natural64Value
high64_64_1
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value a,
    Arcadia_BooleanValue* truncated
  )
{
  Arcadia_SizeValue leadingZeroes = Arcadia_countLeadingZeroesNatural64Value(thread, a);
  *truncated = Arcadia_BooleanValue_False;
  return a << leadingZeroes;
}

static Arcadia_Natural64Value
high64_64_2
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value a0,
    Arcadia_Natural64Value a1,
    Arcadia_BooleanValue* truncated
  )
{
  Arcadia_SizeValue shl = Arcadia_countLeadingZeroesNatural64Value(thread, a0);
  if (shl == 0) {
    *truncated = a1 != 0;
    return a0;
  } else {
    Arcadia_SizeValue shr = 64 - shl;
    *truncated = (a1 << shl) != 0;
    return (a0 << shl) | (a1 >> shr);
  }
}

static Arcadia_Natural64Value
high64_32_1
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value a,
    Arcadia_BooleanValue* truncated
  )
{ return high64_64_1(thread, a, truncated); }

static Arcadia_Natural64Value
high64_32_2
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value a0,
    Arcadia_Natural32Value a1,
    Arcadia_BooleanValue* truncated
  )
{
  Arcadia_Natural64Value x0 = a0;
  Arcadia_Natural64Value x1 = a1;
  return high64_64_1(thread, (x0 << 32) | x1, truncated);
}

static Arcadia_Natural64Value
high64_32_3
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value a0,
    Arcadia_Natural32Value a1,
    Arcadia_Natural32Value a2,
    Arcadia_BooleanValue* truncated
  )
{
  Arcadia_Natural64Value x0 = a0;
  Arcadia_Natural64Value x1 = a1;
  Arcadia_Natural64Value x2 = a2;
  return high64_64_2(thread, x0, (x1 << 32) | x2, truncated);
}

// from the msb
Arcadia_Natural64Value
Arcadia_BigInteger_getHigh64
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BooleanValue* truncated
  )
{
  if (self->numberOfLimps == 1) {
    return high64_32_1(thread, self->limps[0], truncated);
  } else if (self->numberOfLimps == 2) {
    return high64_32_2(thread,
                       self->limps[self->numberOfLimps - 1],
                       self->limps[self->numberOfLimps - 2],
                       truncated);
  } else {
    Arcadia_Natural64Value x = high64_32_3(thread,
                                           self->limps[self->numberOfLimps - 1],
                                           self->limps[self->numberOfLimps - 2],
                                           self->limps[self->numberOfLimps - 3],
                                           truncated);
    *truncated |= self->limps[self->numberOfLimps - 1]
               || self->limps[self->numberOfLimps - 2]
               || self->limps[self->numberOfLimps - 3];
    return x;
  }
}
