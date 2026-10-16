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
#include "Arcadia/VPL/IR/StoreInstruction.h"

static void
Arcadia_VPL_IR_StoreInstruction_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StoreInstruction* self
  );

static void
Arcadia_VPL_IR_StoreInstruction_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StoreInstructionDispatch* self
  );

static void
Arcadia_VPL_IR_StoreInstruction_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StoreInstruction* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_IR_StoreInstruction_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_IR_StoreInstruction_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_IR_StoreInstruction_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.StoreInstruction", Arcadia_VPL_IR_StoreInstruction,
                         u8"Arcadia.VPL.IR.Instruction", Arcadia_VPL_IR_Instruction,
                         &_typeOperations);

static void
Arcadia_VPL_IR_StoreInstruction_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StoreInstruction* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_StoreInstruction);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, Arcadia_VPL_IR_InstructionKind_Store);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->target = (Arcadia_VPL_IR_ReferenceValue*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_VPL_IR_ReferenceValue_getType(thread));
  self->value = (Arcadia_VPL_IR_Value*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_VPL_IR_Value_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_StoreInstruction);
}

static void
Arcadia_VPL_IR_StoreInstruction_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StoreInstructionDispatch* self
  )
{ }

static void
Arcadia_VPL_IR_StoreInstruction_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_StoreInstruction* self
  )
{
  if (self->target) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->target);
  }
  if (self->value) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->value);
  }
}

Arcadia_VPL_IR_StoreInstruction*
Arcadia_VPL_IR_StoreInstruction_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ReferenceValue* target,
    Arcadia_VPL_IR_Value* value
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_StoreInstruction);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)target);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)value);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_VPL_IR_StoreInstruction);
}