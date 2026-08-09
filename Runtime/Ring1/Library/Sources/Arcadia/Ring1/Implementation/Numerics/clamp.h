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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_NUMERICS_CLAMP_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_NUMERICS_CLAMP_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Implementation/Integer16.h"
#include "Arcadia/Ring1/Implementation/Integer32.h"
#include "Arcadia/Ring1/Implementation/Integer64.h"
#include "Arcadia/Ring1/Implementation/Integer8.h"

#include "Arcadia/Ring1/Implementation/Natural16.h"
#include "Arcadia/Ring1/Implementation/Natural32.h"
#include "Arcadia/Ring1/Implementation/Natural64.h"
#include "Arcadia/Ring1/Implementation/Natural8.h"

#include "Arcadia/Ring1/Implementation/Real32.h"
#include "Arcadia/Ring1/Implementation/Real64.h"

#include "Arcadia/Ring1/Implementation/Thread.h"

#include "Arcadia/Ring1/Implementation/Size.h"

#define Define(Type, Suffix) \
  static inline Type##Value \
  Arcadia_clamp##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Type##Value value, \
      Type##Value lower, \
      Type##Value upper  \
    ) \
  { \
    if (lower > upper) { \
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); \
      Arcadia_Thread_jump(thread); \
    } \
    if (value < lower) value = lower; \
    else if (value > upper) value = upper; \
    return value; \
  }

Define(Arcadia_Integer16, Integer16)
Define(Arcadia_Integer32, Integer32)
Define(Arcadia_Integer64, Integer64)
Define(Arcadia_Integer8, Integer8)

Define(Arcadia_Natural16, Natural16)
Define(Arcadia_Natural32, Natural32)
Define(Arcadia_Natural64, Natural64)
Define(Arcadia_Natural8, Natural8)

Define(Arcadia_Size, Size)

Define(Arcadia_Real32, Real32)
Define(Arcadia_Real64, Real64)

#undef Define

#endif // ARCADIA_RING1_IMPLEMENTATION_NUMERICS_CLAMP_H_INCLUDED
