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

#if !defined(ARCADIA_RING1_NUMERICS_HASH_H_INCLUDED)
#define ARCADIA_RING1_NUMERICS_HASH_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif

#include "Arcadia/Ring1/Atoms.h"

#include "Arcadia/Ring1/Boolean.h"

#include "Arcadia/Ring1/ForeignProcedure.h"

#include "Arcadia/Ring1/Integer16.h"
#include "Arcadia/Ring1/Integer32.h"
#include "Arcadia/Ring1/Integer64.h"
#include "Arcadia/Ring1/Integer8.h"

#include "Arcadia/Ring1/Natural16.h"
#include "Arcadia/Ring1/Natural32.h"
#include "Arcadia/Ring1/Natural64.h"
#include "Arcadia/Ring1/Natural8.h"

#include "Arcadia/Ring1/Real32.h"
#include "Arcadia/Ring1/Real64.h"

#include "Arcadia/Ring1/Size.h"

#include "Arcadia/Ring1/Process.h"

#include "Arcadia/Ring1/Void.h"

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_hash*
Arcadia_SizeValue
Arcadia_hashAtomValue
  (
    Arcadia_Thread* thread,
    Arcadia_AtomValue x
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_hash*
Arcadia_SizeValue
Arcadia_hashTypeValue
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue x
  );

#endif // ARCADIA_RING1_NUMERICS_HASH_H_INCLUDED
