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

#include "Arcadia/PDLC/AST/Instructions/ConstructInstructionNode.h"

#include "Arcadia/PDLC/AST/Include.h"

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_ConstructInstructionNode* self
  );

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_ConstructInstructionNodeDispatch* self
  );

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_ConstructInstructionNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.AST.ConstructInstructionNode", Arcadia_PDLC_AST_ConstructInstructionNode,
                         u8"Arcadia.PDLC.AST.InstructionNode", Arcadia_PDLC_AST_InstructionNode,
                         &_typeOperations);

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_ConstructInstructionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_AST_ConstructInstructionNode);
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  self->target = (Arcadia_PDLC_AST_OperandNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_PDLC_AST_OperandNode_getType(thread));
  self->callee = (Arcadia_PDLC_AST_VariableOperandNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_PDLC_AST_VariableOperandNode_getType(thread));
  self->operands = (Arcadia_List*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_List_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_PDLC_AST_ConstructInstructionNode);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_ConstructInstructionNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_ConstructInstructionNode* self
  )
{
  if (self->target) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->target);
  }
  if (self->callee) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->callee);
  }
  if (self->operands) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->operands);
  }
}

Arcadia_PDLC_AST_ConstructInstructionNode*
Arcadia_PDLC_AST_ConstructInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_PDLC_AST_OperandNode* target,
    Arcadia_PDLC_AST_VariableOperandNode* callee,
    Arcadia_List* operands
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_AST_ConstructInstructionNode);
  Arcadia_ValueStack_pushSizeValue(thread, startOffset);
  if (target) Arcadia_ValueStack_pushObjectReferenceValue(thread, target); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (callee) Arcadia_ValueStack_pushObjectReferenceValue(thread, callee); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (operands) Arcadia_ValueStack_pushObjectReferenceValue(thread, operands); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_PDLC_AST_ConstructInstructionNode);
}
