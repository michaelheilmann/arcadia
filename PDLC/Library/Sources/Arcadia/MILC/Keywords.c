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

#define ARCADIA_MILC_PRIVATE (1)
#include "Arcadia/MILC/Keywords.h"

#include "Arcadia/Collections/Include.h"

struct Arcadia_MILC_KeywordsDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_MILC_Keywords {
  Arcadia_Object _parent;
  /// Map from strings to word types.
  Arcadia_Map* map;
};

static void
Arcadia_MILC_Keywords_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Keywords* self
  );

static void
Arcadia_MILC_Keywords_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_KeywordsDispatch* self
  );

static void
Arcadia_MILC_Keywords_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Keywords* self
  );

static void
Arcadia_MILC_Keywords_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Keywords* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_Keywords_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_MILC_Keywords_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_Keywords_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_Keywords_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.Keywords", Arcadia_MILC_Keywords,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_MILC_Keywords_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Keywords* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_Keywords);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->map = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_LeaveConstructor(Arcadia_MILC_Keywords);
}

static void
Arcadia_MILC_Keywords_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_KeywordsDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_Keywords_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Keywords* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_Keywords_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Keywords* self
  )
{
  if (self->map) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->map);
  }
}

Arcadia_MILC_Keywords*
Arcadia_MILC_Keywords_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_Keywords);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MILC_Keywords);
}

void
Arcadia_MILC_Keywords_add
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Keywords* self,
    Arcadia_String* string,
    Arcadia_Value wordType
  )
{
  if (Arcadia_Value_isVoidValue(&wordType)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value word = Arcadia_Map_get(thread, self->map, Arcadia_Value_makeObjectReferenceValue(string));
  if (!Arcadia_Value_isVoidValue(&word)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Exists);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Map_set(thread, self->map, Arcadia_Value_makeObjectReferenceValue(string), wordType, NULL, NULL);
}

Arcadia_Value
Arcadia_MILC_Keywords_scan
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Keywords* self,
    Arcadia_String* string
  )
{
  return Arcadia_Map_get(thread, self->map, Arcadia_Value_makeObjectReferenceValue(string));
}
