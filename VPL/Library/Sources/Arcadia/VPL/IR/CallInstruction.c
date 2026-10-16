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
#include "Arcadia/VPL/IR/CallInstruction.h"

static void
Arcadia_VPL_IR_CallInstruction_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_CallInstruction* self
  );

static void
Arcadia_VPL_IR_CallInstruction_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_CallInstructionDispatch* self
  );

static void
Arcadia_VPL_IR_CallInstruction_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_CallInstruction* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_IR_CallInstruction_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_IR_CallInstruction_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_IR_CallInstruction_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.CallInstruction", Arcadia_VPL_IR_CallInstruction,
                         u8"Arcadia.VPL.IR.Instruction", Arcadia_VPL_IR_Instruction,
                         &_typeOperations);

static void
Arcadia_VPL_IR_CallInstruction_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_CallInstruction* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_CallInstruction);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, Arcadia_VPL_IR_InstructionKind_Call);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->result = NULL;
  if (!Arcadia_ValueStack_isObjectReferenceValue(thread, 4)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_ValueStack_getObjectReferenceValue(thread, 4)) {
    self->result = (Arcadia_VPL_IR_RegisterValue*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 4, _Arcadia_VPL_IR_RegisterValue_getType(thread));
  }
  if (!Arcadia_ValueStack_isObjectReferenceValue(thread, 3)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (NULL == Arcadia_ValueStack_getObjectReferenceValue(thread, 3)) {
    self->calleeFunction = NULL;
  } else {
    self->calleeFunction = (Arcadia_VPL_IR_Function*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_VPL_IR_Function_getType(thread));
  }
  self->builtin = Arcadia_ValueStack_getNatural8Value(thread, 2);
  self->arguments = (Arcadia_List*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_List_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_CallInstruction);
}

static void
Arcadia_VPL_IR_CallInstruction_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_CallInstructionDispatch* self
  )
{ }

static void
Arcadia_VPL_IR_CallInstruction_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_CallInstruction* self
  )
{
  if (self->result) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->result);
  }
  if (self->calleeFunction) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->calleeFunction);
  }
  if (self->arguments) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->arguments);
  }
}

Arcadia_VPL_IR_CallInstruction*
Arcadia_VPL_IR_CallInstruction_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_RegisterValue* result,
    Arcadia_VPL_IR_Function* calleeFunction,
    Arcadia_Natural8Value builtin,
    Arcadia_List* arguments
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_CallInstruction);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)result);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)calleeFunction);
  Arcadia_ValueStack_pushNatural8Value(thread, builtin);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)arguments);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_VPL_IR_CallInstruction);
}
