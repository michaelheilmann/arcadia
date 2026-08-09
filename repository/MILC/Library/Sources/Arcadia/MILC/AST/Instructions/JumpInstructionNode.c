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

#include "Arcadia/MILC/AST/Instructions/JumpInstructionNode.h"

#include "Arcadia/MILC/AST/Include.h"

/// @code
/// constructor(operand:Arcadia.MILC.AST.OperandNode)
/// @endcode
static void
Arcadia_MILC_AST_JumpInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_JumpInstructionNode* self
  );

static void
Arcadia_MILC_AST_JumpInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_JumpInstructionNodeDispatch* self
  );

static void
Arcadia_MILC_AST_JumpInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_JumpInstructionNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_MILC_AST_JumpInstructionNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_JumpInstructionNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_AST_JumpInstructionNode_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_JumpInstructionNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_MILC_AST_JumpInstructionNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_MILC_AST_JumpInstructionNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.JumpInstructionNode", Arcadia_MILC_AST_JumpInstructionNode,
                         u8"Arcadia.MILC.AST.InstructionNode", Arcadia_MILC_AST_InstructionNode,
                         &_Arcadia_MILC_AST_JumpInstructionNode_typeOperations);

static void
Arcadia_MILC_AST_JumpInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_JumpInstructionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_JumpInstructionNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->labelName = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_JumpInstructionNode);
}

static void
Arcadia_MILC_AST_JumpInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_JumpInstructionNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_AST_JumpInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_JumpInstructionNode* self
  )
{
  if (self->labelName) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->labelName);
  }
}

Arcadia_MILC_AST_JumpInstructionNode*
Arcadia_MILC_AST_JumpInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* labelName
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_AST_JumpInstructionNode);
  if (labelName) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, labelName);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_MILC_AST_JumpInstructionNode);
}
