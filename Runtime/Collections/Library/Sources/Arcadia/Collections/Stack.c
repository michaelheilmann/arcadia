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
#include "Arcadia/Collections/Stack.h"

static void
Arcadia_Stack_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Stack* self
  );

static void
Arcadia_Stack_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_StackDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Stack_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Stack_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Stack", Arcadia_Stack,
                         u8"Arcadia.Collection", Arcadia_Collection,
                         &_typeOperations);

static void
Arcadia_Stack_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Stack* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Stack);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Stack);
}

static void
Arcadia_Stack_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_StackDispatch* self
  )
{/*Intentionally empty.*/}

void
Arcadia_Stack_push
  (
    Arcadia_Thread* thread,
    Arcadia_Stack* self,
    Arcadia_Value value
  )
{ Arcadia_VirtualCall(Arcadia_Stack, push, self, value); }

void
Arcadia_Stack_pop
  (
    Arcadia_Thread* thread,
    Arcadia_Stack* self
  )
{ Arcadia_VirtualCall(Arcadia_Stack, pop, self); }

Arcadia_Value
Arcadia_Stack_peek
  (
    Arcadia_Thread* thread,
    Arcadia_Stack* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Stack, peek, self); }

Arcadia_Value
Arcadia_Stack_peekAt
  (
    Arcadia_Thread* thread,
    Arcadia_Stack* self,
    Arcadia_SizeValue index
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Stack, peekAt, self, index); }

#define Define(Type, Suffix, Variable) \
  void \
  Arcadia_Stack_push##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_Stack* self, \
      Type##Value Variable##Value \
    ) \
  { \
    Arcadia_Value value; \
    Arcadia_Value_set##Suffix##Value(&value, Variable##Value); \
    Arcadia_Stack_push(thread, self, value); \
  } \
\
  Arcadia_BooleanValue \
  Arcadia_Stack_is##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_Stack* self, \
      Arcadia_SizeValue index \
    ) \
  { \
    Arcadia_Value v = Arcadia_Stack_peekAt(thread, self, index); \
    return Arcadia_Value_is##Suffix##Value(&v); \
  } \
\
  Type##Value \
  Arcadia_Stack_get##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_Stack* self, \
      Arcadia_SizeValue index \
    ) \
  { \
    Arcadia_Value v = Arcadia_Stack_peekAt(thread, self, index); \
    if (!Arcadia_Value_is##Suffix##Value(&v)) { \
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); \
      Arcadia_Thread_jump(thread); \
    } \
    return Arcadia_Value_get##Suffix##Value(&v); \
  } \
\
  Type##Value \
  Arcadia_Stack_peek##Suffix##Value \
    (\
      Arcadia_Thread* thread, \
      Arcadia_Stack* self \
    ) \
  { \
    Arcadia_Value v = Arcadia_Stack_peek(thread, self); \
    if (!Arcadia_Value_is##Suffix##Value(&v)) { \
      \
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); \
        Arcadia_Thread_jump(thread); \
    } \
    return Arcadia_Value_get##Suffix##Value(&v); \
  }

Define(Arcadia_Boolean, Boolean, boolean)
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
