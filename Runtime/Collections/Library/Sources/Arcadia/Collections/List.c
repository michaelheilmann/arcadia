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

#define ARCADIA_COLLECTIONS_MODULE (1)
#include "Arcadia/Collections/List.h"

#include "Arcadia/Collections/Include.h"

static void
Arcadia_List_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_List* self
  );

static void
Arcadia_List_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ListDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_List_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_List_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.List", Arcadia_List,
                         u8"Arcadia.Collection", Arcadia_Collection,
                         &_typeOperations);

static void
Arcadia_List_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_List* self
  )
{
  Arcadia_EnterConstructor(Arcadia_List);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_List);
}

static void
Arcadia_List_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ListDispatch* self
  )
{/*Intentionally empty.*/}

Arcadia_Value
Arcadia_List_getAt
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_SizeValue index
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_List, getAt, self, index); }

void
Arcadia_List_insertBack
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_Value value
  )
{ Arcadia_VirtualCall(Arcadia_List, insertBack, self, value); }

void
Arcadia_List_insertFront
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_Value value
  )
{ Arcadia_VirtualCall(Arcadia_List, insertFront, self, value); }

void
Arcadia_List_insertAt
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_SizeValue index,
    Arcadia_Value value
  )
{ Arcadia_VirtualCall(Arcadia_List, insertAt, self, index, value); }

void
Arcadia_List_removeAt
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_SizeValue index,
    Arcadia_SizeValue count
  )
{ Arcadia_VirtualCall(Arcadia_List, removeAt, self, index, count); }

#define Define(Type, Suffix, Variable) \
  void \
  Arcadia_List_insertBack##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_List* self, \
      Type##Value Variable##Value \
    ) \
  { \
    Arcadia_Value value; \
    Arcadia_Value_set##Suffix##Value(&value, Variable##Value); \
    Arcadia_List_insertBack(thread, self, value); \
  } \
\
  void \
  Arcadia_List_insertFront##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_List* self, \
      Type##Value Variable##Value \
    ) \
  { \
    Arcadia_Value value; \
    Arcadia_Value_set##Suffix##Value(&value, Variable##Value); \
    Arcadia_List_insertFront(thread, self, value); \
  } \
\
\
  void \
  Arcadia_List_insert##Suffix##ValueAt \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_List* self, \
      Arcadia_SizeValue index, \
      Type##Value Variable##Value \
    ) \
  { \
    Arcadia_Value value; \
    Arcadia_Value_set##Suffix##Value(&value, Variable##Value); \
    Arcadia_List_insertAt(thread, self, index, value); \
  } \
\
  Arcadia_BooleanValue \
  Arcadia_List_is##Suffix##ValueAt \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_List* self, \
      Arcadia_SizeValue index \
    ) \
  { \
    Arcadia_Value v = Arcadia_List_getAt(thread, self, index); \
    return Arcadia_Value_is##Suffix##Value(&v); \
  } \
\
  Type##Value \
  Arcadia_List_get##Suffix##ValueAt \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_List* self, \
      Arcadia_SizeValue index \
    ) \
  { \
    Arcadia_Value v = Arcadia_List_getAt(thread, self, index); \
    if (!Arcadia_Value_is##Suffix##Value(&v)) { \
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); \
      Arcadia_Thread_jump(thread); \
    } \
    return Arcadia_Value_get##Suffix##Value(&v); \
  }

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

Arcadia_List*
Arcadia_List_filter
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_Value context,
    Arcadia_BooleanValue(*predicate)(Arcadia_Thread* thread, Arcadia_Value context, Arcadia_Value value)
  )
{
  Arcadia_List* result = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self); i < n; ++i) {
    Arcadia_Value v = Arcadia_List_getAt(thread, self, i);
    if ((*predicate)(thread, context, v)) {
      Arcadia_List_insertBack(thread, result, v);
    }
  }
  return result;
}

void
Arcadia_List_forEach
  (
    Arcadia_Thread* thread,
    Arcadia_List* self,
    Arcadia_Value context,
    Arcadia_BooleanValue(*predicate)(Arcadia_Thread* thread, Arcadia_Value context, Arcadia_Value value)
  )
{ 
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self); i < n; ++i) {
    Arcadia_Value v = Arcadia_List_getAt(thread, self, i);
    if (!(*predicate)(thread, context, v)) {
      break;
    }
  }
}
