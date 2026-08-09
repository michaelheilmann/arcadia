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
#include "Arcadia/Ring1/Implementation/StringToReal/Result.h"

#include "Arcadia/Ring1/Include.h"

void
Arcadia_ToReal64_BitsToValue
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value* target,
    Arcadia_ToReal64_Result* source
  )
{
  Arcadia_Natural64Value temporary = source->significand | (((Arcadia_Natural64Value)source->exponent) << Arcadia_Real64Value_ExponentBitsShift);
  if (source->negative) {
    temporary |= Arcadia_Natural64Value_Literal(1) << Arcadia_Real64Value_SignBitsShift;
  }
  Arcadia_Memory_copy(thread, target, &temporary, Arcadia_Real64Value_NumberOfBytes);
}
