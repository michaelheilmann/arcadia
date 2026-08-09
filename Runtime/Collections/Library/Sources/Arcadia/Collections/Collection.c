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
#include "Arcadia/Collections/Collection.h"

#include "Arcadia/Collections/Include.h"

static void
Arcadia_Collection_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  );

static void
Arcadia_Collection_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_CollectionDispatch* self
  );

static Arcadia_BooleanValue
Arcadia_Collection_isEmptyImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Collection_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Collection_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Collection", Arcadia_Collection,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Collection_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Collection);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Collection);
}

static void
Arcadia_Collection_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_CollectionDispatch* self
  )
{
  self->isEmpty = &Arcadia_Collection_isEmptyImpl;
}

static Arcadia_BooleanValue
Arcadia_Collection_isEmptyImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  )
{ return Arcadia_SizeValue_Literal(0) == Arcadia_Collection_getSize(thread, self); }

void
Arcadia_Collection_clear
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  )
{ Arcadia_VirtualCall(Arcadia_Collection, clear, self); }

Arcadia_SizeValue
Arcadia_Collection_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Collection, getSize, self); }

Arcadia_BooleanValue
Arcadia_Collection_isEmpty
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Collection, isEmpty, self); }

Arcadia_BooleanValue
Arcadia_Collection_isImmutable
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Collection, isImmutable, self); }
