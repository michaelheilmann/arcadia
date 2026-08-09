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
#include "Arcadia/Ring1/Implementation/BigInteger/countSignificandBits.h"

#include "Arcadia/Ring1/Include.h"

Arcadia_SizeValue
Arcadia_BigInteger_countSignificandBits
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self
  )
{
  if (Arcadia_BigInteger_isZero(thread, self)) {
    return Arcadia_SizeValue_Literal(0);
  }
  return Arcadia_BigInteger_BitsPerLimp * (self->numberOfLimps - 1) + Arcadia_countSignificandBitsNatural32Value(thread, self->limps[self->numberOfLimps - 1]);
}
