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
#include "Arcadia/VPL/Tree/Node.h"

static void
Arcadia_VPL_Tree_Node_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* self
  );

static void
Arcadia_VPL_Tree_Node_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_NodeDispatch* self
  );

static void
Arcadia_VPL_Tree_Node_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_VPL_Tree_Node_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_VPL_Tree_Node_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_VPL_Tree_Node_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.Tree.Node", Arcadia_VPL_Tree_Node,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_VPL_Tree_Node_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Tree_Node);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->flags = Arcadia_ValueStack_getInteger32Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_VPL_Tree_Node);
}

static void
Arcadia_VPL_Tree_Node_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_NodeDispatch* self
  )
{ }

static void
Arcadia_VPL_Tree_Node_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* self
  )
{/*Intentionally empty.*/}

Arcadia_VPL_Tree_Node*
Arcadia_VPL_Tree_Node_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Tree_Node);
  Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_VPL_Tree_NodeFlags_Empty);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_VPL_Tree_Node);
}

void
Arcadia_VPL_TreeNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* self,
    Arcadia_VPL_Tree_NodeVisitor* visitor
  )
{ Arcadia_VirtualCall(Arcadia_VPL_Tree_Node, visit, self, visitor); }
