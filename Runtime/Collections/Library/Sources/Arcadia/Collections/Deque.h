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

#if !defined(ARCADIA_COLLECTIONS_DEQUE_H_INCLUDED)
#define ARCADIA_COLLECTIONS_DEQUE_H_INCLUDED

#if !defined(ARCADIA_COLLECTIONS_MODULE)
  #error("do not include directly, include `Arcadia/Collections/Include.h` instead")
#endif

#include "Arcadia/Collections/Collection.h"

Arcadia_declareObjectType(u8"Arcadia.Deque", Arcadia_Deque,
                          u8"Arcadia.Collection");

struct Arcadia_DequeDispatch {
  Arcadia_CollectionDispatch _parent;

  Arcadia_Value(*getAt)(Arcadia_Thread*, Arcadia_Deque*, Arcadia_SizeValue);
  Arcadia_Value(*getBack)(Arcadia_Thread*, Arcadia_Deque*);
  Arcadia_Value(*getFront)(Arcadia_Thread*, Arcadia_Deque*);
  void (*insertAt)(Arcadia_Thread*, Arcadia_Deque*, Arcadia_SizeValue, Arcadia_Value);
  void (*insertBack)(Arcadia_Thread*, Arcadia_Deque*, Arcadia_Value);
  void (*insertFront)(Arcadia_Thread*, Arcadia_Deque*, Arcadia_Value);
  void (*removeBack)(Arcadia_Thread*, Arcadia_Deque*);
  void (*removeFront)(Arcadia_Thread*, Arcadia_Deque*);
  void (*removeAt)(Arcadia_Thread*, Arcadia_Deque*, Arcadia_SizeValue);
};

struct Arcadia_Deque {
  Arcadia_Collection parent;
};

void
Arcadia_Deque_insertFront
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self,
    Arcadia_Value value
  );

void
Arcadia_Deque_insertBack
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self,
    Arcadia_Value value
  );

void
Arcadia_Deque_insertAt
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self,
    Arcadia_SizeValue index,
    Arcadia_Value value
  );

Arcadia_Value
Arcadia_Deque_getFront
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self
  );

Arcadia_Value
Arcadia_Deque_getBack
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self
  );

Arcadia_Value
Arcadia_Deque_getAt
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self,
    Arcadia_SizeValue index
  );

void
Arcadia_Deque_removeFront
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self
  );

void
Arcadia_Deque_removeBack
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self
  );

void
Arcadia_Deque_removeAt
  (
    Arcadia_Thread* thread,
    Arcadia_Deque* self,
    Arcadia_SizeValue index
  );

#define Define(Type, Suffix, Variable) \
  void \
  Arcadia_Deque_insertBack##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_Deque* self, \
      Type##Value Variable##Value \
    ); \
\
  void \
  Arcadia_Deque_insertFront##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_Deque* self, \
      Type##Value Variable##Value \
    ); \
\
  void \
  Arcadia_Deque_insert##Suffix##ValueAt \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_Deque* self, \
      Arcadia_SizeValue index, \
      Type##Value Variable##Value \
    ); \
\
  Arcadia_BooleanValue \
  Arcadia_Deque_is##Suffix##ValueAt \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_Deque* self, \
      Arcadia_SizeValue index \
    ); \
\
  Type##Value \
  Arcadia_Deque_get##Suffix##ValueAt \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_Deque* self, \
      Arcadia_SizeValue index \
    );

Define(Arcadia_Boolean, Boolean, boolean)
Define(Arcadia_BigInteger, BigInteger, bigInteger)
Define(Arcadia_ForeignProcedure, ForeignProcedure, foreignProcedure)
Define(Arcadia_Integer8, Integer8, integer8)
Define(Arcadia_Integer16, Integer16, integer16)
Define(Arcadia_Integer32, Integer32, integer32)
Define(Arcadia_Integer64, Integer64, integer64)
Define(Arcadia_Natural8, Natural8, natural8)
Define(Arcadia_Natural16, Natural16, natural16)
Define(Arcadia_Natural32, Natural32, natural32)
Define(Arcadia_Natural64, Natural64, natural64)
Define(Arcadia_ObjectReference, ObjectReference, objectReference)
Define(Arcadia_Size, Size, size)
Define(Arcadia_Void, Void, void)

#undef Define

#endif // ARCADIA_COLLECTIONS_DEQUE_H_INCLUDED
