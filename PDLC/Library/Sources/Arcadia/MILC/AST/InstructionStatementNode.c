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

#include "Arcadia/MILC/AST/InstructionStatementNode.h"

#include "Arcadia/MILC/AST/Include.h"

static void
Arcadia_MILC_AST_InstructionStatementNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_InstructionStatementNode* self
  );

static void
Arcadia_MILC_AST_InstructionStatementNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_InstructionStatementNodeDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_InstructionStatementNode_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_InstructionStatementNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.InstructionStatementNode", Arcadia_MILC_AST_InstructionStatementNode,
                         u8"Arcadia.MILC.AST.StatementNode", Arcadia_MILC_AST_StatementNode,
                         &_typeOperations);

static void
Arcadia_MILC_AST_InstructionStatementNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_InstructionStatementNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_InstructionStatementNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_InstructionStatementNode);
}

static void
Arcadia_MILC_AST_InstructionStatementNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_InstructionStatementNodeDispatch* self
  )
{/*Intentionally empty.*/}
