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

#include "Arcadia/MILC/AST/Instructions/InvokeInstructionNode.h"

#include "Arcadia/MILC/AST/Include.h"

/// @code
/// constructor(callee:Arcadia.MIL.VariableOperandAst,operands:Arcadia.List)
/// @endcode
static void
Arcadia_MILC_AST_InvokeInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_InvokeInstructionNode* self
  );

static void
Arcadia_MILC_AST_InvokeInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_InvokeInstructionNodeDispatch* self
  );

static void
Arcadia_MILC_AST_InvokeInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_InvokeInstructionNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_InvokeInstructionNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_AST_InvokeInstructionNode_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_InvokeInstructionNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.InvokeInstructionNode", Arcadia_MILC_AST_InvokeInstructionNode,
                         u8"Arcadia.MILC.AST.InstructionNode", Arcadia_MILC_AST_InstructionNode,
                         &_typeOperations);

void
Arcadia_MILC_AST_InvokeInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_InvokeInstructionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_InvokeInstructionNode);
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  self->callee = (Arcadia_MILC_AST_VariableOperandNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_MILC_AST_VariableOperandNode_getType(thread));
  self->operands = (Arcadia_List*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_List_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_InvokeInstructionNode);
}

static void
Arcadia_MILC_AST_InvokeInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_InvokeInstructionNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_AST_InvokeInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_InvokeInstructionNode* self
  )
{
  Arcadia_Object_visit(thread, (Arcadia_Object*)self->callee);
  Arcadia_Object_visit(thread, (Arcadia_Object*)self->operands);
}

Arcadia_MILC_AST_InvokeInstructionNode*
Arcadia_MILC_AST_InvokeInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_MILC_AST_VariableOperandNode* callee,
    Arcadia_List* operands
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_AST_InvokeInstructionNode);
  Arcadia_ValueStack_pushSizeValue(thread, startOffset);
  if (callee) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, callee);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (operands) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, operands);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_MILC_AST_InvokeInstructionNode);
}
