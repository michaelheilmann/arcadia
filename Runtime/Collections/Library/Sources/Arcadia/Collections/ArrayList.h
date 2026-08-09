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

#if !defined(ARCADIA_COLLECTIONS_ARRAYLIST_H_INCLUDED)
#define ARCADIA_COLLECTIONS_ARRAYLIST_H_INCLUDED

#if !defined(ARCADIA_COLLECTIONS_MODULE)
  #error("do not include directly, include `Arcadia/Collections/Include.h` instead")
#endif

#include "Arcadia/Collections/List.h"

Arcadia_declareObjectType(u8"Arcadia.ArrayList", Arcadia_ArrayList,
                          u8"Arcadia.List");

struct Arcadia_ArrayListDispatch {
  Arcadia_ListDispatch _parent;
};

struct Arcadia_ArrayList {
  Arcadia_List _parent;
  Arcadia_Value* elements;
  Arcadia_SizeValue size;
  Arcadia_SizeValue capacity;
};

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_ArrayList_create
Arcadia_ArrayList*
Arcadia_ArrayList_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_COLLECTIONS_ARRAYLIST_H_INCLUDED
