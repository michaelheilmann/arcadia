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
#include "Arcadia/VPL/IR/BinaryInstruction.h"

static void
Arcadia_VPL_IR_BinaryInstruction_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_BinaryInstruction* self
  );

static void
Arcadia_VPL_IR_BinaryInstruction_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_BinaryInstructionDispatch* self
  );

static void
Arcadia_VPL_IR_BinaryInstruction_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_BinaryInstruction* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_IR_BinaryInstruction_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_IR_BinaryInstruction_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_IR_BinaryInstruction_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.BinaryInstruction", Arcadia_VPL_IR_BinaryInstruction,
                         u8"Arcadia.VPL.IR.Instruction", Arcadia_VPL_IR_Instruction,
                         &_typeOperations);

static void
Arcadia_VPL_IR_BinaryInstruction_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_BinaryInstruction* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_BinaryInstruction);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, Arcadia_VPL_IR_InstructionKind_Binary);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->result = (Arcadia_VPL_IR_RegisterValue*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 4, _Arcadia_VPL_IR_RegisterValue_getType(thread));
  self->operatorKind = Arcadia_ValueStack_getNatural8Value(thread, 3);
  self->lhs = (Arcadia_VPL_IR_Value*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_VPL_IR_Value_getType(thread));
  self->rhs = (Arcadia_VPL_IR_Value*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_VPL_IR_Value_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_BinaryInstruction);
}

static void
Arcadia_VPL_IR_BinaryInstruction_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_BinaryInstructionDispatch* self
  )
{ }

static void
Arcadia_VPL_IR_BinaryInstruction_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_BinaryInstruction* self
  )
{
  if (self->result) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->result);
  }
  if (self->lhs) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->lhs);
  }
  if (self->rhs) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->rhs);
  }
}

Arcadia_VPL_IR_BinaryInstruction*
Arcadia_VPL_IR_BinaryInstruction_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_RegisterValue* result,
    Arcadia_Natural8Value operatorKind,
    Arcadia_VPL_IR_Value* lhs,
    Arcadia_VPL_IR_Value* rhs
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_BinaryInstruction);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)result);
  Arcadia_ValueStack_pushNatural8Value(thread, operatorKind);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)lhs);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)rhs);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_VPL_IR_BinaryInstruction);
}