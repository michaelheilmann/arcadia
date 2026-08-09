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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_NUMERICS_ISPOWEROFTWO_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_NUMERICS_ISPOWEROFTWO_H_INCLUDED

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

#include "Arcadia/Ring1/Implementation/Size.h"

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_isPowerOfTwo
Arcadia_BooleanValue
Arcadia_isPowerOfTwoInteger8Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer8Value x
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_isPowerOfTwo
Arcadia_BooleanValue
Arcadia_isPowerOfTwoInteger16Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer16Value x
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_isPowerOfTwo
Arcadia_BooleanValue
Arcadia_isPowerOfTwoInteger32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value x
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_isPowerOfTwo
Arcadia_BooleanValue
Arcadia_isPowerOfTwoInteger64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value x
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_isPowerOfTwo
Arcadia_BooleanValue
Arcadia_isPowerOfTwoNatural8Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value x
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_isPowerOfTwo
Arcadia_BooleanValue
Arcadia_isPowerOfTwoNatural16Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value x
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_isPowerOfTwo
Arcadia_BooleanValue
Arcadia_isPowerOfTwoNatural32Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value x
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_isPowerOfTwo
Arcadia_BooleanValue
Arcadia_isPowerOfTwoNatural64Value
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value x
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_isPowerOfTwo
Arcadia_BooleanValue
Arcadia_isPowerOfTwoSizeValue
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue x
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_NUMERICS_ISPOWEROFTWO_H_INCLUDED
