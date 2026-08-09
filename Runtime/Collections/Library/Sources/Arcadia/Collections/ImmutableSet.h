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

#if !defined(ARCADIA_COLLECTIONS_IMMUTABLESET_H_INCLUDED)
#define ARCADIA_COLLECTIONS_IMMUTABLESET_H_INCLUDED

#if !defined(ARCADIA_COLLECTIONS_MODULE)
  #error("do not include directly, include `Arcadia/Collections/Include.h` instead")
#endif

#include "Arcadia/Collections/Set.h"

Arcadia_declareObjectType(u8"Arcadia.ImmutableSet", Arcadia_ImmutableSet,
                          u8"Arcadia.Set");

struct Arcadia_ImmutableSetDispatch {
  Arcadia_SetDispatch _parent;
};

struct Arcadia_ImmutableSet {
  Arcadia_Set parent;
  Arcadia_Value* elements;
  Arcadia_SizeValue size;
};

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_ImmutableSet_create
// construct(Arcadia.Set other)
Arcadia_ImmutableSet*
Arcadia_ImmutableSet_create
  (
    Arcadia_Thread* thread,
    Arcadia_Value value
  );

#endif // ARCADIA_COLLECTIONS_IMMUTABLESET_H_INCLUDED
