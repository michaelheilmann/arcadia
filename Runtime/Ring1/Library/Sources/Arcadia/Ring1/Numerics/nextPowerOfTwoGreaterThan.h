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

#if !defined(ARCADIA_RING1_NUMERICS_NEXTPOWEROFTWOGREATERTHAN_H_INCLUDED)
#define ARCADIA_RING1_NUMERICS_NEXTPOWEROFTWOGREATERTHAN_H_INCLUDED

#include "Arcadia/Ring1/Integer16.h"
#include "Arcadia/Ring1/Integer32.h"
#include "Arcadia/Ring1/Integer64.h"
#include "Arcadia/Ring1/Integer8.h"

#include "Arcadia/Ring1/Natural16.h"
#include "Arcadia/Ring1/Natural32.h"
#include "Arcadia/Ring1/Natural64.h"
#include "Arcadia/Ring1/Natural8.h"

#include "Arcadia/Ring1/Size.h"

#define Define(Type, Suffix) \
  Type##Value \
  Arcadia_nextPowerOfTwoGreaterThan##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Type##Value x \
    );

Define(Arcadia_Integer16, Integer16)
Define(Arcadia_Integer32, Integer32)
Define(Arcadia_Integer64, Integer64)
Define(Arcadia_Integer8, Integer8)

Define(Arcadia_Natural16, Natural16)
Define(Arcadia_Natural32, Natural32)
Define(Arcadia_Natural64, Natural64)
Define(Arcadia_Natural8, Natural8)

Define(Arcadia_Size, Size)

#undef Define

#endif // ARCADIA_RING1_NUMERICS_NEXTPOWEROFTWOGREATERTHAN_H_INCLUDED
