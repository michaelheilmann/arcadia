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

#include "Arcadia/PDLC/AST/InstructionNode.h"

#include "Arcadia/PDLC/AST/Include.h"

/// @code
/// constructor()
/// @endcode
static void
Arcadia_PDLC_AST_InstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_InstructionNode* self
  );

static void
Arcadia_PDLC_AST_InstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_InstructionNodeDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_AST_InstructionNode_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_AST_InstructionNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.AST.InstructionNode", Arcadia_PDLC_AST_InstructionNode,
                         u8"Arcadia.PDLC.AST.InstructionStatementNode", Arcadia_PDLC_AST_InstructionStatementNode,
                         &_typeOperations);

void
Arcadia_PDLC_AST_InstructionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_InstructionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_AST_InstructionNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_PDLC_AST_InstructionNode);
}

static void
Arcadia_PDLC_AST_InstructionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_InstructionNodeDispatch* self
  )
{ }
