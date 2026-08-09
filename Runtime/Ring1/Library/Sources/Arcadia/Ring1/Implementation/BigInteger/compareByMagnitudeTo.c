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
#include "Arcadia/Ring1/Implementation/BigInteger/compareTo.h"

#include "Arcadia/Ring1/Include.h"

Arcadia_Integer8Value
Arcadia_BigInteger_compareByMagnitudeTo
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger* other
  )
{
  if (self->numberOfLimps < other->numberOfLimps) {
    // self has the smaller magnitude and hence comes after other.
    return Arcadia_Integer8Value_Literal(-1);
  } else if (self->numberOfLimps > other->numberOfLimps) {
    // self has the greater magnitude and hence comes behind other.
    return Arcadia_Integer8Value_Literal(+1);
  } else {
  #if Arcadia_BigInteger_LimpOrder == Arcadia_BigInteger_LimpOrder_LittleEndian
    for (Arcadia_SizeValue i = self->numberOfLimps; i > 0; --i) {
      if (self->limps[i - 1] != other->limps[i - 1]) {
        if (self->limps[i - 1] < other->limps[i - 1]) {
          return Arcadia_Integer8Value_Literal(-1);
        } else if (self->limps[i - 1] > other->limps[i - 1]) {
          return Arcadia_Integer8Value_Literal(+1);
        }
      }
    }
    // Both magnitudes are zero.
    return Arcadia_Integer8Value_Literal(0);
  #elif Arcadia_BigInteger_LimpOrder == Arcadia_BigInteger_LimpOrder_BigEndian
    for (Arcadia_SizeValue i = 0; i < self->numberOfLimps; ++i) {
      if (self->limps[i] != other->limps[i]) {
        if (self->limps[i] < other->limps[i]) {
          return Arcadia_Integer8Value_Literal(-1);
        } else /*if (self->limps[i] > other->limps[i])*/ {
          return Arcadia_Integer8Value_Literal(+1);
        }
      }
    }
    // Both magnitudes are zero.
    return Arcadia_Integer8Value_Literal(0);
  #else
    #error("unknown/unsupported big integer limp order")
  #endif
  }
  return Arcadia_Integer8Value_Literal(0);
}
