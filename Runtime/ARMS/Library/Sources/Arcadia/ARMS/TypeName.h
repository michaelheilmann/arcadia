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

#if !defined(ARCADIA_ARMS_TYPENAME_H_INCLUDED)
#define ARCADIA_ARMS_TYPENAME_H_INCLUDED

#include "Arcadia/ARMS/Configure.h"
#include "Arcadia/ARMS/SizeType.h"
#include "Arcadia/ARMS/Natural8Type.h"
#include "Arcadia/ARMS/StatusType.h"

typedef struct Arcadia_ARMS_TypeName Arcadia_ARMS_TypeName;

/** TOOD: The set of types and the set of type names are monotonically non-decreasing until they are destroyed when the process ends. */
Arcadia_ARMS_Status
Arcadia_ARMS_TypeName_getOrCreate
  (
    Arcadia_ARMS_TypeName** result,
    const Arcadia_ARMS_Natural8* bytes,
    Arcadia_ARMS_Size numberOfBytes
  );

Arcadia_ARMS_Status
Arcadia_ARMS_TypeName_getData
  (
    Arcadia_ARMS_TypeName* typeName,
    const Arcadia_ARMS_Natural8** bytes,
    Arcadia_ARMS_Size* numberOfBytes
  );

/** TOOD: Add reference counting for type names. */
void
Arcadia_ARMS_TypeName_ref
  (
    Arcadia_ARMS_TypeName* typeName
  );

/** TOOD: Add reference counting for type names. */
void
Arcadia_ARMS_TypeName_unref
  (
    Arcadia_ARMS_TypeName* typeName
  );

#endif // ARCADIA_ARMS_TYPENAME_H_INCLUDED
