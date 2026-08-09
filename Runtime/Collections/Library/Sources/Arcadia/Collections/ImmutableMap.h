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

#if !defined(ARCADIA_COLLECTIONS_IMMUTABLEMAP_H_INCLUDED)
#define ARCADIA_COLLECTIONS_IMMUTABLEMAP_H_INCLUDED

#if !defined(ARCADIA_COLLECTIONS_PRIVATE)
  #error("do not include directly, include `Arcadia/Collections/Include.h` instead")
#endif

#include "Arcadia/Collections/Map.h"

Arcadia_declareObjectType(u8"Arcadia.ImmutableMap", Arcadia_ImmutableMap,
                          u8"Arcadia.Map");

struct Arcadia_ImmutableMap {
  Arcadia_Map parent;
};

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_ImmutableMap_create
// construct(Arcadia.Map other)
Arcadia_ImmutableMap*
Arcadia_ImmutableMap_create
  (
    Arcadia_Thread* thread,
    Arcadia_Value value
  );

#endif // ARCADIA_COLLECTIONS_IMMUTABLEMAP_H_INCLUDED
