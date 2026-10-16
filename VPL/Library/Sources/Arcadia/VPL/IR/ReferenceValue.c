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
#include "Arcadia/VPL/IR/ReferenceValue.h"
#include "Arcadia/VPL/IR/ObjectArguments.h"

static void
Arcadia_VPL_IR_ReferenceValue_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReferenceValue* self
  );

static void
Arcadia_VPL_IR_ReferenceValue_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReferenceValueDispatch* self
  );

static void
Arcadia_VPL_IR_ReferenceValue_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReferenceValue* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_IR_ReferenceValue_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_IR_ReferenceValue_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_IR_ReferenceValue_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.ReferenceValue", Arcadia_VPL_IR_ReferenceValue,
                         u8"Arcadia.VPL.IR.Value", Arcadia_VPL_IR_Value,
                         &_typeOperations);

static void
Arcadia_VPL_IR_ReferenceValue_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReferenceValue* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_ReferenceValue);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, Arcadia_VPL_IR_ValueKind_Reference);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->storageKind = Arcadia_ValueStack_getNatural8Value(thread, 3);
  self->slot = Arcadia_ValueStack_getSizeValue(thread, 2);
  self->type = (Arcadia_VPL_IR_Type*)Arcadia_VPL_IR_getRequiredObjectArgument(thread, 1, _Arcadia_VPL_IR_Type_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_ReferenceValue);
}

static void
Arcadia_VPL_IR_ReferenceValue_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReferenceValueDispatch* self
  )
{ }

static void
Arcadia_VPL_IR_ReferenceValue_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReferenceValue* self
  )
{
  if (self->type) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->type);
  }
}

Arcadia_VPL_IR_ReferenceValue*
Arcadia_VPL_IR_ReferenceValue_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue slot,
    Arcadia_VPL_IR_Type* type
  )
{
  return Arcadia_VPL_IR_ReferenceValue_createWithStorage(thread, Arcadia_VPL_IR_StorageKind_Local, slot, type);
}

Arcadia_VPL_IR_ReferenceValue*
Arcadia_VPL_IR_ReferenceValue_createWithStorage
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value storageKind,
    Arcadia_SizeValue slot,
    Arcadia_VPL_IR_Type* type
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_ReferenceValue);
  Arcadia_ValueStack_pushNatural8Value(thread, storageKind);
  Arcadia_ValueStack_pushSizeValue(thread, slot);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)type);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_VPL_IR_ReferenceValue);
}
