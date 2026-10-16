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
#include "Arcadia/VPL/IR/IndexReferenceValue.h"
#include "Arcadia/VPL/IR/ObjectArguments.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_IndexReferenceValue* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_IndexReferenceValue* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType
  (
    u8"Arcadia.VPL.IR.IndexReferenceValue",
    Arcadia_VPL_IR_IndexReferenceValue,
    u8"Arcadia.VPL.IR.ReferenceValue",
    Arcadia_VPL_IR_ReferenceValue,
    &_typeOperations
  );

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_IndexReferenceValue* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_IndexReferenceValue);
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Object* type = Arcadia_VPL_IR_getRequiredObjectArgument(thread, 1, _Arcadia_VPL_IR_Type_getType(thread));
    Arcadia_ValueStack_pushNatural8Value(thread, Arcadia_VPL_IR_StorageKind_Index);
    Arcadia_ValueStack_pushSizeValue(thread, 0);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, type);
    Arcadia_ValueStack_pushNatural8Value(thread, 3);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  self->base = (Arcadia_VPL_IR_ReferenceValue*)Arcadia_VPL_IR_getRequiredObjectArgument(thread, 3, _Arcadia_VPL_IR_ReferenceValue_getType(thread));
  self->index = (Arcadia_VPL_IR_Value*)Arcadia_VPL_IR_getRequiredObjectArgument(thread, 2, _Arcadia_VPL_IR_Value_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_IndexReferenceValue);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_IndexReferenceValue* self
  )
{
  if (self->base) Arcadia_Object_visit(thread, (Arcadia_Object*)self->base);
  if (self->index) Arcadia_Object_visit(thread, (Arcadia_Object*)self->index);
}

Arcadia_VPL_IR_IndexReferenceValue*
Arcadia_VPL_IR_IndexReferenceValue_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReferenceValue* base,
    Arcadia_VPL_IR_Value* index,
    Arcadia_VPL_IR_Type* type
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_IndexReferenceValue);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)base);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)index);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)type);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_VPL_IR_IndexReferenceValue);
}
