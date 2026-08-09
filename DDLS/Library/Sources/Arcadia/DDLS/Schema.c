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

#include "Arcadia/DDLS/Schema.h"

static void
Arcadia_DDLS_Type_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Type* self
  );

static void
Arcadia_DDLS_Type_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Type* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_Type_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_Type_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_Type_visitImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_Type_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_Type_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.Type", Arcadia_DDLS_Type,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_DDLS_Type_typeOperations);

static void
Arcadia_DDLS_Type_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Type* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_Type);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  Arcadia_LeaveConstructor(Arcadia_DDLS_Type);
}

static void
Arcadia_DDLS_Type_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Type* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DDLS_ListType_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ListType* self
  );

static void
Arcadia_DDLS_ListType_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ListType* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_ListType_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_ListType_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_ListType_visitImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_ListType_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_ListType_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.ListType", Arcadia_DDLS_ListType,
                         u8"Arcadia.DDLS.Type", Arcadia_DDLS_Type,
                         &_Arcadia_DDLS_ListType_typeOperations);

static void
Arcadia_DDLS_ListType_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ListType* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_ListType);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->element = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_DDLS_Type_getType(thread));
  //
  Arcadia_LeaveConstructor(Arcadia_DDLS_ListType);
}

static void
Arcadia_DDLS_ListType_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ListType* self
  )
{
  if (self->element) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->element);
  }
}

Arcadia_DDLS_ListType*
Arcadia_DDLS_ListType_create
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Type* type
  )
{
  Arcadia_SizeValue oldValueStackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)type);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  ARCADIA_CREATEOBJECT(Arcadia_DDLS_ListType);
}

static void
Arcadia_DDLS_MapType_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_MapType* self
  );

static void
Arcadia_DDLS_MapType_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_MapType* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_MapType_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_MapType_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_MapType_visitImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_MapType_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_MapType_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.MapType", Arcadia_DDLS_MapType,
                         u8"Arcadia.DDLS.Type", Arcadia_DDLS_Type,
                         &_Arcadia_DDLS_MapType_typeOperations);

static void
Arcadia_DDLS_MapType_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_MapType* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_MapType);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->entries = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  //
  Arcadia_LeaveConstructor(Arcadia_DDLS_MapType);
}

static void
Arcadia_DDLS_MapType_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_MapType* self
  )
{
  if (self->entries) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->entries);
  }
}

Arcadia_DDLS_MapType*
Arcadia_DDLS_MapType_create
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue oldValueStackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  ARCADIA_CREATEOBJECT(Arcadia_DDLS_MapType);
}

static void
Arcadia_DDLS_ScalarType_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ScalarType* self
  );

static void
Arcadia_DDLS_ScalarType_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ScalarType* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_ScalarType_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_ScalarType_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_ScalarType_visitImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_ScalarType_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_ScalarType_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.ScalarType", Arcadia_DDLS_ScalarType,
                         u8"Arcadia.DDLS.Type", Arcadia_DDLS_Type,
                         &_Arcadia_DDLS_ScalarType_typeOperations);

static void
Arcadia_DDLS_ScalarType_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ScalarType* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_ScalarType);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->name = NULL;
  //
  Arcadia_LeaveConstructor(Arcadia_DDLS_ScalarType);
}

static void
Arcadia_DDLS_ScalarType_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_ScalarType* self
  )
{
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
}

Arcadia_DDLS_ScalarType*
Arcadia_DDLS_ScalarType_create
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue oldValueStackSize = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  ARCADIA_CREATEOBJECT(Arcadia_DDLS_ScalarType);
}
