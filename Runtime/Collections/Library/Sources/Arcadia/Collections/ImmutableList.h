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

#if !defined(ARCADIA_COLLECTIONS_IMMUTABLELIST_H_INCLUDED)
#define ARCADIA_COLLECTIONS_IMMUTABLELIST_H_INCLUDED

#if !defined(ARCADIA_COLLECTIONS_MODULE)
  #error("do not include directly, include `Arcadia/Collections/Include.h` instead")
#endif

#include "Arcadia/Collections/List.h"

Arcadia_declareObjectType(u8"Arcadia.ImmutableList", Arcadia_ImmutableList,
                          u8"Arcadia.List");

struct Arcadia_ImmutableListDispatch {
  Arcadia_ListDispatch _parent;
};

struct Arcadia_ImmutableList {
  Arcadia_List parent;
  Arcadia_Value* elements;
  Arcadia_SizeValue size;
};

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_ImmutableList_create
// construct(Arcadia.List other)
Arcadia_ImmutableList*
Arcadia_ImmutableList_create
  (
    Arcadia_Thread* thread,
    Arcadia_Value argument
  );

#endif // ARCADIA_COLLECTIONS_IMMUTABLELIST_H_INCLUDED
