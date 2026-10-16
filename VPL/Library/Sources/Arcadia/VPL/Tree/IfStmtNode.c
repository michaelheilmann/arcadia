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
#include "Arcadia/VPL/Tree/IfStmtNode.h"
#include "Arcadia/VPL/Tree/NodeArguments.h"

static void
Arcadia_VPL_Tree_IfStmtNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_IfStmtNode* self
  );

static void
Arcadia_VPL_Tree_IfStmtNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_IfStmtNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_Tree_IfStmtNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_Tree_IfStmtNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.Tree.IfStmtNode", Arcadia_VPL_Tree_IfStmtNode,
                         u8"Arcadia.VPL.Tree.Node", Arcadia_VPL_Tree_Node,
                         &_typeOperations);

static void
Arcadia_VPL_Tree_IfStmtNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_IfStmtNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Tree_IfStmtNode);
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_VPL_Tree_NodeFlags_IfStmt);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->condition = (Arcadia_VPL_Tree_Node*)Arcadia_VPL_Tree_getObjectArgument(thread, 3, _Arcadia_VPL_Tree_Node_getType(thread), Arcadia_BooleanValue_False);
  self->thenBody = (Arcadia_List*)Arcadia_VPL_Tree_getObjectArgument(thread, 2, _Arcadia_List_getType(thread), Arcadia_BooleanValue_False);
  self->elseBody = (Arcadia_List*)Arcadia_VPL_Tree_getObjectArgument(thread, 1, _Arcadia_List_getType(thread), Arcadia_BooleanValue_True);
  Arcadia_LeaveConstructor(Arcadia_VPL_Tree_IfStmtNode);
}

static void
Arcadia_VPL_Tree_IfStmtNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_IfStmtNode* self
  )
{
  if (self->condition) Arcadia_Object_visit(thread, (Arcadia_Object*)self->condition);
  if (self->thenBody) Arcadia_Object_visit(thread, (Arcadia_Object*)self->thenBody);
  if (self->elseBody) Arcadia_Object_visit(thread, (Arcadia_Object*)self->elseBody);
}

Arcadia_VPL_Tree_IfStmtNode*
Arcadia_VPL_Tree_IfStmtNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* condition,
    Arcadia_List* thenBody,
    Arcadia_List* elseBody
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Tree_IfStmtNode);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)condition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)thenBody);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)elseBody);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_VPL_Tree_IfStmtNode);
}
