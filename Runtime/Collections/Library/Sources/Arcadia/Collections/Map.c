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
#include "Arcadia/Collections/Map.h"

static void
Arcadia_Map_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self
  );

static void
Arcadia_Map_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MapDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*) & Arcadia_Map_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Map_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Map", Arcadia_Map,
                         u8"Arcadia.Collection", Arcadia_Collection,
                         &_typeOperations);

static void
Arcadia_Map_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Map);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Map);
}

static void
Arcadia_Map_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MapDispatch* self
  )
{/*Intentionally empty.*/}

Arcadia_Value
Arcadia_Map_get
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self,
    Arcadia_Value key
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Map, get, self, key); }

void
Arcadia_Map_remove
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self,
    Arcadia_Value key,
    Arcadia_Value* oldKey,
    Arcadia_Value* oldValue
  )
{ Arcadia_VirtualCall(Arcadia_Map, remove, self, key, oldKey, oldValue); }

void
Arcadia_Map_set
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self,
    Arcadia_Value key,
    Arcadia_Value value,
    Arcadia_Value* oldKey,
    Arcadia_Value* oldValue
  )
{ Arcadia_VirtualCall(Arcadia_Map, set, self, key, value, oldKey, oldValue); }

Arcadia_List*
Arcadia_Map_getKeys
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Map, getKeys, self); }

Arcadia_List*
Arcadia_Map_getValues
  (
    Arcadia_Thread* thread,
    Arcadia_Map* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Map, getValues, self); }
