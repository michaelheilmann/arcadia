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

#include "Arcadia/PDLC/AST/Instructions/JumpInstructionNode.h"

#include "Arcadia/PDLC/AST/Include.h"

/// @code
/// constructor(kind,condition,labelName)
/// @endcode
static void
Arcadia_PDLC_AST_JumpInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_JumpInstructionNode* self
  );

static void
Arcadia_PDLC_AST_JumpInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_JumpInstructionNodeDispatch* self
  );

static void
Arcadia_PDLC_AST_JumpInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_JumpInstructionNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_PDLC_AST_JumpInstructionNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_AST_JumpInstructionNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_AST_JumpInstructionNode_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_AST_JumpInstructionNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_PDLC_AST_JumpInstructionNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_PDLC_AST_JumpInstructionNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.AST.JumpInstructionNode", Arcadia_PDLC_AST_JumpInstructionNode,
                         u8"Arcadia.PDLC.AST.InstructionNode", Arcadia_PDLC_AST_InstructionNode,
                         &_Arcadia_PDLC_AST_JumpInstructionNode_typeOperations);

static void
Arcadia_PDLC_AST_JumpInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_JumpInstructionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_AST_JumpInstructionNode);
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  self->kind = (Arcadia_PDLC_AST_JumpInstructionKind)Arcadia_ValueStack_getNatural32Value(thread, 3);
  Arcadia_Value condition = Arcadia_ValueStack_getValue(thread, 2);
  if (condition.tag == Arcadia_ValueTag_Void) {
    self->condition = NULL;
  } else {
    self->condition = (Arcadia_PDLC_AST_OperandNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_PDLC_AST_OperandNode_getType(thread));
  }
  self->labelName = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_PDLC_AST_JumpInstructionNode);
}

static void
Arcadia_PDLC_AST_JumpInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_JumpInstructionNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_AST_JumpInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_JumpInstructionNode* self
  )
{
  if (self->labelName) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->labelName);
  }
  if (self->condition) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->condition);
  }
}

Arcadia_PDLC_AST_JumpInstructionNode*
Arcadia_PDLC_AST_JumpInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_PDLC_AST_JumpInstructionKind kind,
    Arcadia_PDLC_AST_OperandNode* condition,
    Arcadia_String* labelName
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_AST_JumpInstructionNode);
  Arcadia_ValueStack_pushSizeValue(thread, startOffset);
  Arcadia_ValueStack_pushNatural32Value(thread, kind);
  if (condition) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, condition);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (labelName) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, labelName);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_PDLC_AST_JumpInstructionNode);
}
