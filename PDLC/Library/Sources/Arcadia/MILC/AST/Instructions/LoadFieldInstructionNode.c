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

#include "Arcadia/MILC/AST/Instructions/LoadFieldInstructionNode.h"

#include "Arcadia/MILC/AST/Include.h"

/// @code
/// constructor()
/// @endcode
static void
Arcadia_MILC_AST_LoadFieldInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_LoadFieldInstructionNode* self
  );

static void
Arcadia_MILC_AST_LoadFieldInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_LoadFieldInstructionNodeDispatch* self
  );

static void
Arcadia_MILC_AST_LoadFieldInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_LoadFieldInstructionNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_MILC_AST_LoadFieldInstructionNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_LoadFieldInstructionNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_AST_LoadFieldInstructionNode_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_LoadFieldInstructionNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_MILC_AST_LoadFieldInstructionNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_MILC_AST_LoadFieldInstructionNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.LoadFieldInstructionNode", Arcadia_MILC_AST_LoadFieldInstructionNode,
                         u8"Arcadia.MILC.AST.InstructionNode", Arcadia_MILC_AST_InstructionNode,
                         &_Arcadia_MILC_AST_LoadFieldInstructionNode_typeOperations);

static void
Arcadia_MILC_AST_LoadFieldInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_LoadFieldInstructionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_LoadFieldInstructionNode);
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  self->target = (Arcadia_MILC_AST_OperandNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_MILC_AST_OperandNode_getType(thread));
  self->source = (Arcadia_MILC_AST_OperandNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_MILC_AST_OperandNode_getType(thread));
  self->name = (Arcadia_MILC_AST_OperandNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_MILC_AST_OperandNode_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_LoadFieldInstructionNode);
}

static void
Arcadia_MILC_AST_LoadFieldInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_LoadFieldInstructionNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_AST_LoadFieldInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_LoadFieldInstructionNode* self
  )
{
  if (self->target) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->target);
  }
  if (self->source) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->source);
  }
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
}

Arcadia_MILC_AST_LoadFieldInstructionNode*
Arcadia_MILC_AST_LoadFieldInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_MILC_AST_OperandNode* target,
    Arcadia_MILC_AST_OperandNode* source,
    Arcadia_MILC_AST_OperandNode* name
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_AST_LoadFieldInstructionNode);
  Arcadia_ValueStack_pushSizeValue(thread, startOffset);
  if (target) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)target); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (source) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (name) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_MILC_AST_LoadFieldInstructionNode);
}
