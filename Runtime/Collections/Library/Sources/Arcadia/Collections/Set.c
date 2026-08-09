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
#include "Arcadia/Collections/Set.h"

static void
Arcadia_Set_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Set* self
  );

static void
Arcadia_Set_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_SetDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Set_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Set_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Set", Arcadia_Set,
                         u8"Arcadia.Collection", Arcadia_Collection,
                         &_typeOperations);

static void
Arcadia_Set_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Set* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Set);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Set);
}

static void
Arcadia_Set_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_SetDispatch* self
  )
{/*Intentionally empty.*/}

void
Arcadia_Set_add
  (
    Arcadia_Thread* thread,
    Arcadia_Set* self,
    Arcadia_Value value,
    Arcadia_Value* oldValue
  )
{ Arcadia_VirtualCall(Arcadia_Set, add, self, value, oldValue); }

Arcadia_BooleanValue
Arcadia_Set_contains
  (
    Arcadia_Thread* thread,
    Arcadia_Set* self,
    Arcadia_Value value
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Set, contains, self, value); }

Arcadia_Value
Arcadia_Set_get
  (
    Arcadia_Thread* thread,
    Arcadia_Set* self,
    Arcadia_Value value
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Set, get, self, value); }

void
Arcadia_Set_remove
  (
    Arcadia_Thread* thread,
    Arcadia_Set* self,
    Arcadia_Value value,
    Arcadia_Value* oldValue
  )
{ Arcadia_VirtualCall(Arcadia_Set, remove, self, value, oldValue); }

void
Arcadia_Set_getAll
  (
    Arcadia_Thread* thread,
    Arcadia_Set* self,
    Arcadia_List* target
  )
{ Arcadia_VirtualCall(Arcadia_Set, getAll, self, target); }
