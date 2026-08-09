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

#include "Arcadia/MILC/AST/Instructions/BinaryInstructionNode.h"

#include "Arcadia/MILC/AST/Include.h"

/// @code
/// constructor(kind : Arcadia.MILC.AST.BinaryInstructionKind, target, operand1, operand2 : Arcadia.MILC.AST.OperandNode)
/// @endcode
static void
Arcadia_MILC_AST_BinaryInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_BinaryInstructionNode* self
  );

static void
Arcadia_MILC_AST_BinaryInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_BinaryInstructionNodeDispatch* self
  );

static void
Arcadia_MILC_AST_BinaryInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_BinaryInstructionNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_MILC_AST_BinaryInstructionNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_BinaryInstructionNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_AST_BinaryInstructionNode_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_BinaryInstructionNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_MILC_AST_BinaryInstructionNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_MILC_AST_BinaryInstructionNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.BinaryInstructionNode", Arcadia_MILC_AST_BinaryInstructionNode,
                         u8"Arcadia.MILC.AST.InstructionNode", Arcadia_MILC_AST_InstructionNode,
                         &_Arcadia_MILC_AST_BinaryInstructionNode_typeOperations);

static void
Arcadia_MILC_AST_BinaryInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_BinaryInstructionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_BinaryInstructionNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->kind = Arcadia_ValueStack_getInteger32Value(thread, 4);
  self->target = (Arcadia_MILC_AST_OperandNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_MILC_AST_OperandNode_getType(thread));
  self->operand1 = (Arcadia_MILC_AST_OperandNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_MILC_AST_OperandNode_getType(thread));
  self->operand2 = (Arcadia_MILC_AST_OperandNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_MILC_AST_OperandNode_getType(thread));

  Arcadia_LeaveConstructor(Arcadia_MILC_AST_BinaryInstructionNode);
}

static void
Arcadia_MILC_AST_BinaryInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_BinaryInstructionNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_AST_BinaryInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_BinaryInstructionNode* self
  )
{
  if (self->target) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->target);
  }
  if (self->operand1) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->operand1);
  }
  if (self->operand2) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->operand2);
  }
}

Arcadia_MILC_AST_BinaryInstructionNode*
Arcadia_MILC_AST_BinaryInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_BinaryInstructionKind kind,
    Arcadia_MILC_AST_OperandNode* target,
    Arcadia_MILC_AST_OperandNode* operand1,
    Arcadia_MILC_AST_OperandNode* operand2
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_AST_BinaryInstructionNode);
  Arcadia_ValueStack_pushInteger32Value(thread, kind);
  if (target) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, target);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (operand1) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, operand1);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (operand2) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, operand2);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_MILC_AST_BinaryInstructionNode);
}
