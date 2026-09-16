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

#include "Arcadia/MILC/AST/Instructions/RaiseInstructionNode.h"

#include "Arcadia/MILC/AST/Include.h"

/// @code
/// constructor()
/// @endcode
static void
Arcadia_MILC_AST_RaiseInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_RaiseInstructionNode* self
  );

static void
Arcadia_MILC_AST_RaiseInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_RaiseInstructionNodeDispatch* self
  );

static void
Arcadia_MILC_AST_RaiseInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_RaiseInstructionNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_MILC_AST_RaiseInstructionNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_RaiseInstructionNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_AST_RaiseInstructionNode_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_RaiseInstructionNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_MILC_AST_RaiseInstructionNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_MILC_AST_RaiseInstructionNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.RaiseInstructionNode", Arcadia_MILC_AST_RaiseInstructionNode,
                         u8"Arcadia.MILC.AST.InstructionNode", Arcadia_MILC_AST_InstructionNode,
                         &_Arcadia_MILC_AST_RaiseInstructionNode_typeOperations);

static void
Arcadia_MILC_AST_RaiseInstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_RaiseInstructionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_RaiseInstructionNode);
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_RaiseInstructionNode);
}

static void
Arcadia_MILC_AST_RaiseInstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_RaiseInstructionNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_AST_RaiseInstructionNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_RaiseInstructionNode* self
  )
{/*Intentionally empty.*/}

Arcadia_MILC_AST_RaiseInstructionNode*
Arcadia_MILC_AST_RaiseInstructionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_AST_RaiseInstructionNode);
  Arcadia_ValueStack_pushSizeValue(thread, startOffset);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_MILC_AST_RaiseInstructionNode);
}
