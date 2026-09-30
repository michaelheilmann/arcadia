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

#if !defined(ARCADIA_RING1_NUMERICS_ISDIVISIBLEBYPOWEROFFIVE_H_INCLUDED)
#define ARCADIA_RING1_NUMERICS_ISDIVISIBLEBYPOWEROFFIVE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Integer16.h"
#include "Arcadia/Ring1/Integer32.h"
#include "Arcadia/Ring1/Integer64.h"
#include "Arcadia/Ring1/Integer8.h"

#include "Arcadia/Ring1/Natural16.h"
#include "Arcadia/Ring1/Natural32.h"
#include "Arcadia/Ring1/Natural64.h"
#include "Arcadia/Ring1/Natural8.h"

#include "Arcadia/Ring1/Process.h"
#include "Arcadia/Ring1/Thread.h"

#include "Arcadia/Ring1/Size.h"

// Returns true if <code>v</code> is divisible by <code>5^p</code>.
// In particular, this function returns logically false if <code>v = 0</code>.
Arcadia_BooleanValue
Arcadia_isDivisibleByPowerOfFiveNatural16
  (
    Arcadia_Thread* thread,
    const uint16_t v,
    const uint16_t p
  );

// Returns true if <code>v</code> is divisible by <code>5^p</code>.
// In particular, this function returns logically false if <code>v = 0</code>.
Arcadia_BooleanValue
Arcadia_isDivisibleByPowerOfFiveNatural32
  (
    Arcadia_Thread* thread,
    const uint32_t v,
    const uint32_t p
  );

// Returns true if <code>v</code> is divisible by <code>5^p</code>.
// In particular, this function returns logically false if <code>v = 0</code>.
Arcadia_BooleanValue
Arcadia_isDivisibleByPowerOfFiveNatural64
  (
    Arcadia_Thread* thread,
    const uint64_t v,
    const uint64_t p
  );

// Returns true if <code>v</code> is divisible by <code>5^p</code>.
// In particular, this function returns logically false if <code>v = 0</code>.
Arcadia_BooleanValue
Arcadia_isDivisibleByPowerOfFiveNatural8
  (
    Arcadia_Thread* thread,
    const uint8_t v,
    const uint8_t p
  );

#endif // ARCADIA_RING1_NUMERICS_ISDIVISIBLEBYPOWEROFFIVE_H_INCLUDED
