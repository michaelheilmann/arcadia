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

#include "Arcadia/MILC/AST/Instructions/ReturnInstructionNode.h"

#include "Arcadia/MILC/AST/Include.h"

/// @code
/// constructor(operand:Arcadia.MILC.AST.OperandNode)
/// @endcode
static void
Arcadia_MILC_AST_ReturnInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ReturnInstructionNode* self
  );

static void
Arcadia_MILC_AST_ReturnInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ReturnInstructionNodeDispatch* self
  );

static void
Arcadia_MILC_AST_ReturnInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ReturnInstructionNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_MILC_AST_ReturnInstructionNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_ReturnInstructionNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_AST_ReturnInstructionNode_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_ReturnInstructionNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_MILC_AST_ReturnInstructionNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_MILC_AST_ReturnInstructionNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.ReturnInstructionNode", Arcadia_MILC_AST_ReturnInstructionNode,
                         u8"Arcadia.MILC.AST.InstructionNode", Arcadia_MILC_AST_InstructionNode,
                         &_Arcadia_MILC_AST_ReturnInstructionNode_typeOperations);

static void
Arcadia_MILC_AST_ReturnInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ReturnInstructionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_ReturnInstructionNode);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  self->operand = (Arcadia_MILC_AST_OperandNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_MILC_AST_OperandNode_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_ReturnInstructionNode);
}

static void
Arcadia_MILC_AST_ReturnInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ReturnInstructionNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_AST_ReturnInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ReturnInstructionNode* self
  )
{ Arcadia_Object_visit(thread, (Arcadia_Object*)self->operand); }

Arcadia_MILC_AST_ReturnInstructionNode*
Arcadia_MILC_AST_ReturnInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_MILC_AST_OperandNode* operand
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_AST_ReturnInstructionNode);
  Arcadia_ValueStack_pushSizeValue(thread, startOffset);
  if (operand) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, operand);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_MILC_AST_ReturnInstructionNode);
}
