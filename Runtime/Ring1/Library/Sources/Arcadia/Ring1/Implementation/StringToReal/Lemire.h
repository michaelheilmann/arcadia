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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_STRINGTOREAL_LEMIRE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_TOREAL_LEMIRE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Boolean.h"
#include "Arcadia/Ring1/Implementation/Integer64.h"
#include "Arcadia/Ring1/Implementation/Natural64.h"
#include "Arcadia/Ring1/Implementation/Real64.h"
#include "Arcadia/Ring1/Implementation/Size.h"
#include "Arcadia/Ring1/Implementation/StringToReal/Result.h"
typedef struct Arcadia_BigInteger Arcadia_BigInteger;

// For q in (0,350), we have that
//   f = (((152170 + 65536) * q ) >> 16);
// is equal to
//   floor(p) + q
// where
//   p = log(5**q)/log(2) = q * log(5)/log(2)
//
// For negative values of q in (-400,0), we have that
//   f = (((152170 + 65536) * q ) >> 16);
// is equal to
//   -ceil(p) + q
// where
//  p = log(5**-q)/log(2) = -q * log(5)/log(2)
//
// Warning:
// Undefined behavior if q not in (-400,350).
// In particular, undefined if q 0.
static inline Arcadia_Integer32Value
Arcadia_power
  (
    Arcadia_Integer32Value q
  )
{ return (((152170 + 65536) * q) >> 16) + 63; }

void
Arcadia_computeProductApproximation
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value w,
    Arcadia_Integer64Value q,
    Arcadia_Natural64Value* upper,
    Arcadia_Natural64Value* lower
  );

// Lemire's method.
void
Arcadia_lemireReal64
  (
    Arcadia_Thread* thread,
    Arcadia_ToReal64_Result* result,
    State* state
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_TOREAL_LEMIRE_H_INCLUDED
