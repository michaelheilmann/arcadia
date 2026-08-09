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
#include "Arcadia/VPL/Tree/BinaryExprNode.h"

static void
Arcadia_VPL_Tree_BinaryExprNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_BinaryExprNode* self
  );

static void
Arcadia_VPL_Tree_BinaryExprNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_BinaryExprNodeDispatch* self
  );

static void
Arcadia_VPL_Tree_BinaryExprNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_BinaryExprNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_Tree_BinaryExprNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_Tree_BinaryExprNode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_Tree_BinaryExprNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};


Arcadia_defineObjectType(u8"Arcadia.VPL.Tree.BinaryExprNode", Arcadia_VPL_Tree_BinaryExprNode,
                         u8"Arcadia.VPL.Tree.Node", Arcadia_VPL_Tree_Node,
                         &_typeOperations);

static void
Arcadia_VPL_Tree_BinaryExprNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_BinaryExprNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Tree_BinaryExprNode);
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_VPL_Tree_NodeFlags_BinaryExpr);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->kind = Arcadia_ValueStack_getInteger32Value(thread, 3);
  self->lhs = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_VPL_Tree_Node_getType(thread));
  self->rhs = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_VPL_Tree_Node_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_VPL_Tree_BinaryExprNode);
}

static void
Arcadia_VPL_Tree_BinaryExprNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_BinaryExprNodeDispatch* self
  )
{ }

static void
Arcadia_VPL_Tree_BinaryExprNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_BinaryExprNode* self
  )
{
  if (self->lhs) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->lhs);
  }
  if (self->rhs) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->rhs);
  }
}

Arcadia_VPL_Tree_BinaryExprNode*
Arcadia_VPL_Tree_BinaryExprNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value kind,
    Arcadia_VPL_Tree_Node* lhs,
    Arcadia_VPL_Tree_Node* rhs
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Tree_BinaryExprNode);
  Arcadia_ValueStack_pushInteger32Value(thread, kind);
  if (lhs) Arcadia_ValueStack_pushObjectReferenceValue(thread, lhs); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (rhs) Arcadia_ValueStack_pushObjectReferenceValue(thread, rhs); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_VPL_Tree_BinaryExprNode);
}
