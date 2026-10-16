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
#include "Arcadia/VPL/IR/ConstructInstruction.h"

static void
Arcadia_VPL_IR_ConstructInstruction_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ConstructInstruction* self
  );

static void
Arcadia_VPL_IR_ConstructInstruction_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ConstructInstructionDispatch* self
  );

static void
Arcadia_VPL_IR_ConstructInstruction_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ConstructInstruction* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_IR_ConstructInstruction_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_IR_ConstructInstruction_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_IR_ConstructInstruction_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.IR.ConstructInstruction", Arcadia_VPL_IR_ConstructInstruction,
                         u8"Arcadia.VPL.IR.Instruction", Arcadia_VPL_IR_Instruction,
                         &_typeOperations);

static void
Arcadia_VPL_IR_ConstructInstruction_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ConstructInstruction* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_IR_ConstructInstruction);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, Arcadia_VPL_IR_InstructionKind_Construct);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->result = (Arcadia_VPL_IR_RegisterValue*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_VPL_IR_RegisterValue_getType(thread));
  self->type = (Arcadia_VPL_IR_Type*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_VPL_IR_Type_getType(thread));
  self->arguments = (Arcadia_List*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_List_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_VPL_IR_ConstructInstruction);
}

static void
Arcadia_VPL_IR_ConstructInstruction_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ConstructInstructionDispatch* self
  )
{ }

static void
Arcadia_VPL_IR_ConstructInstruction_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_ConstructInstruction* self
  )
{
  if (self->result) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->result);
  }
  if (self->type) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->type);
  }
  if (self->arguments) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->arguments);
  }
}

Arcadia_VPL_IR_ConstructInstruction*
Arcadia_VPL_IR_ConstructInstruction_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_RegisterValue* result,
    Arcadia_VPL_IR_Type* type,
    Arcadia_List* arguments
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_IR_ConstructInstruction);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)result);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)type);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)arguments);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_VPL_IR_ConstructInstruction);
}