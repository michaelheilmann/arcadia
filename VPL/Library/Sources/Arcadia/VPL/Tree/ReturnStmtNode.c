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

#define ARCADIA_VPL_PRIVATE (1)
#include "Arcadia/VPL/Tree/ReturnStmtNode.h"
#include "Arcadia/VPL/Tree/NodeArguments.h"

static void
Arcadia_VPL_Tree_ReturnStmtNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_ReturnStmtNode* self
  );

static void
Arcadia_VPL_Tree_ReturnStmtNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_ReturnStmtNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_Tree_ReturnStmtNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_Tree_ReturnStmtNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.Tree.ReturnStmtNode", Arcadia_VPL_Tree_ReturnStmtNode,
                         u8"Arcadia.VPL.Tree.Node", Arcadia_VPL_Tree_Node,
                         &_typeOperations);

static void
Arcadia_VPL_Tree_ReturnStmtNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_ReturnStmtNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Tree_ReturnStmtNode);
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_VPL_Tree_NodeFlags_ReturnStmt);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->value = (Arcadia_VPL_Tree_Node*)Arcadia_VPL_Tree_getObjectArgument(thread, 1, _Arcadia_VPL_Tree_Node_getType(thread), Arcadia_BooleanValue_True);
  Arcadia_LeaveConstructor(Arcadia_VPL_Tree_ReturnStmtNode);
}

static void
Arcadia_VPL_Tree_ReturnStmtNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_ReturnStmtNode* self
  )
{
  if (self->value) Arcadia_Object_visit(thread, (Arcadia_Object*)self->value);
}

Arcadia_VPL_Tree_ReturnStmtNode*
Arcadia_VPL_Tree_ReturnStmtNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* value
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Tree_ReturnStmtNode);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_VPL_Tree_ReturnStmtNode);
}
