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

#if !defined(ARCADIA_RING1_BIGINTEGER_TOTWOSCOMPLEMENT_H_INCLUDED)
#define ARCADIA_RING1_BIGINTEGER_TOTWOSCOMPLEMENT_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/BigInteger/BigInteger.h"

/// Convert to an array (*limps) of length (*numberOfLimps) of Arcadia_BigInteger_Limp values.
/// The array elements contain the two's complement representation of the value of the big integer in little endian form.
/// The array must be deallocated using Arcadia_Process_deallocateMemory when no longer required.
void
Arcadia_BigInteger_toTwosComplement
  (
    Arcadia_Thread* thread,
    Arcadia_BigInteger* self,
    Arcadia_BigInteger_Limp** limps,
    Arcadia_SizeValue* numberOfLimps
  );

#endif // ARCADIA_RING1_BIGINTEGER_TOTWOSCOMPLEMENT_H_INCLUDED
