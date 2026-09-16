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

#include "Arcadia/MILC/AST/StatementNode.h"

#include "Arcadia/MILC/AST/Include.h"

static void
Arcadia_MILC_AST_StatementNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_StatementNode* self
  );

static void
Arcadia_MILC_AST_StatementNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_StatementNodeDispatch* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_MILC_AST_StatementNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_StatementNode_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_StatementNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_MILC_AST_StatementNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_MILC_AST_StatementNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.StatementNode", Arcadia_MILC_AST_StatementNode,
                         u8"Arcadia.MILC.AST.Node", Arcadia_MILC_AST_Node,
                         &_Arcadia_MILC_AST_StatementNode_typeOperations);

static void
Arcadia_MILC_AST_StatementNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_StatementNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_StatementNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_StatementNode);
}

static void
Arcadia_MILC_AST_StatementNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_StatementNodeDispatch* self
  )
{/*Intentionally empty.*/}
