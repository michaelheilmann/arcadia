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

#if !defined(ARCADIA_COLLECTIONS_LIST_H_INCLUDED)
#define ARCADIA_COLLECTIONS_LIST_H_INCLUDED

#if !defined(ARCADIA_COLLECTIONS_MODULE)
  #error("do not include directly, include `Arcadia/Collections/Include.h` instead")
#endif

#include "Arcadia/Collections/Collection.h"

Arcadia_declareObjectType(u8"Arcadia.List", Arcadia_List,
                          u8"Arcadia.Collection");

struct Arcadia_ListDispatch {
  Arcadia_CollectionDispatch _parent;

  Arcadia_Value(*getAt)(Arcadia_Thread* thread, Arcadia_List*, Arcadia_SizeValue index);
  void (*insertAt)(Arcadia_Thread*, Arcadia_List*, Arcadia_SizeValue index, Arcadia_Value);
  void (*insertBack)(Arcadia_Thread*, Arcadia_List*, Arcadia_Value);
  void (*insertFront)(Arcadia_Thread*, Arcadia_List*, Arcadia_Value);
  void (*removeAt)(Arcadia_Thread*, Arcadia_List*, Arcadia_SizeValue, Arcadia_SizeValue);
};

struct Arcadia_List {
  Arcadia_Collection parent;
};

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_List_insertFront
void
Arcadia_List_insertBack
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_Value value
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_List_insertBack
void
Arcadia_List_insertFront
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_Value value
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_List_insertAt
void
Arcadia_List_insertAt
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_SizeValue index,
    Arcadia_Value value
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_List_getAt
Arcadia_Value
Arcadia_List_getAt
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_SizeValue index
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_List_removeAt
void
Arcadia_List_removeAt
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_SizeValue index,
    Arcadia_SizeValue count
  );

static inline void
Arcadia_List_removeBack
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_SizeValue count
  )
{
  Arcadia_SizeValue size = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self);
  if (!size) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Empty);
    Arcadia_Thread_jump(thread);
  }
  if (!count) {
    return;
  }
  Arcadia_List_removeAt(thread, self, size - count, count);
}

static inline void
Arcadia_List_removeFront
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_SizeValue count
  )
{
  Arcadia_List_removeAt(thread, self, 0, count);
}

Arcadia_List*
Arcadia_List_filter
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_Value context,
    Arcadia_BooleanValue(*predicate)(Arcadia_Thread* thread, Arcadia_Value context, Arcadia_Value value)
  );

void
Arcadia_List_forEach
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_Value context,
    Arcadia_BooleanValue(*predicate)(Arcadia_Thread* thread, Arcadia_Value context, Arcadia_Value value)
  );

static inline Arcadia_BooleanValue
Arcadia_List_contains
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_Value value
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self); i < n; ++i) {
    Arcadia_Value e = Arcadia_List_getAt(thread, self, i);
    if (Arcadia_Value_isEqualTo(thread, &value, &e)) {
      return Arcadia_BooleanValue_True;
    }
  }
  return Arcadia_BooleanValue_False;
}

#define Define(Type, Suffix, Variable) \
  void \
  Arcadia_List_insertBack##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_List* self, \
      Type##Value Variable##Value \
    ); \
\
  void \
  Arcadia_List_insertFront##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_List* self, \
      Type##Value Variable##Value \
    ); \
\
  void \
  Arcadia_List_insert##Suffix##ValueAt \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_List* self, \
      Arcadia_SizeValue index, \
      Type##Value Variable##Value \
    ); \
\
  Arcadia_BooleanValue \
  Arcadia_List_is##Suffix##ValueAt \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_List* self, \
      Arcadia_SizeValue index \
    ); \
\
  Type##Value \
  Arcadia_List_get##Suffix##ValueAt \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_List* self, \
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

static inline Arcadia_Object*
Arcadia_List_getObjectReferenceValueCheckedAt
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_SizeValue index,
    Arcadia_Type* type
  )
{
  Arcadia_Object* element = Arcadia_List_getObjectReferenceValueAt(thread, self, index);
  if (type) {
    if (!Arcadia_Object_isInstanceOf(thread, element, type)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
  }
  return element;
}

#endif // ARCADIA_COLLECTIONS_LIST_H_INCLUDED
