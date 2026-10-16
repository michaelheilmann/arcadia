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

#define ARCADIA_VPL_PRIVATE (1)
#include "Arcadia/VPL/IR/RecordType.h"

static void
Arcadia_VPL_IR_RecordType_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_RecordType* self
  );

static void
Arcadia_VPL_IR_RecordType_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_RecordTypeDispatch* self
  );

static void
Arcadia_VPL_IR_RecordType_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_RecordType* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_IR_RecordType_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_IR_RecordType_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_IR_RecordType_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.RecordType", Arcadia_VPL_IR_RecordType,
                         u8"Arcadia.VPL.IR.Type", Arcadia_VPL_IR_Type,
                         &_typeOperations);

static void
Arcadia_VPL_IR_RecordType_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_RecordType* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_RecordType);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, Arcadia_VPL_IR_TypeKind_Record);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->name = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread));
  self->fields = (Arcadia_List*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_List_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_RecordType);
}

static void
Arcadia_VPL_IR_RecordType_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_RecordTypeDispatch* self
  )
{ }

static void
Arcadia_VPL_IR_RecordType_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_RecordType* self
  )
{
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
  if (self->fields) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->fields);
  }
}

Arcadia_VPL_IR_RecordType*
Arcadia_VPL_IR_RecordType_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_List* fields
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_RecordType);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)fields);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_VPL_IR_RecordType);
}