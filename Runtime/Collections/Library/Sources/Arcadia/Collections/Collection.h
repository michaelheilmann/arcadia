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

#if !defined(ARCADIA_COLLECTIONS_COLLECTION_H_INCLUDED)
#define ARCADIA_COLLECTIONS_COLLECTION_H_INCLUDED

#if !defined(ARCADIA_COLLECTIONS_MODULE)
  #error("do not include directly, include `Arcadia/Collections/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"

Arcadia_declareObjectType(u8"Arcadia.Collection", Arcadia_Collection,
                          u8"Arcadia.Object");

struct Arcadia_CollectionDispatch {
  Arcadia_ObjectDispatch _parent;

  void (*clear)(Arcadia_Thread*, Arcadia_Collection*);
  Arcadia_SizeValue(*getSize)(Arcadia_Thread*, Arcadia_Collection*);
  Arcadia_BooleanValue(*isEmpty)(Arcadia_Thread*, Arcadia_Collection*);
  Arcadia_BooleanValue(*isImmutable)(Arcadia_Thread*, Arcadia_Collection*);
};

struct Arcadia_Collection {
  Arcadia_Object _parent;
};

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Collection_clear
void
Arcadia_Collection_clear
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Colleciton_getSize
Arcadia_SizeValue
Arcadia_Collection_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Collection_isEmpty
Arcadia_BooleanValue
Arcadia_Collection_isEmpty
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Collection_isImmmutable
Arcadia_BooleanValue
Arcadia_Collection_isImmutable
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  );

#endif // ARCADIA_COLLECTIONS_COLLECTION_H_INCLUDED
