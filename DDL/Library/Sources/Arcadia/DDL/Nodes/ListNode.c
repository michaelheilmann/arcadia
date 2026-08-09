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

#define ARCADIA_DDL_NODES_PRIVATE (1)
#include "Arcadia/DDL/Nodes/ListNode.h"

static void
Arcadia_DDL_ListNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_ListNode* self
  );

static void
Arcadia_DDL_ListNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_ListNodeDispatch* self
  );

static void
Arcadia_DDL_ListNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_ListNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDL_ListNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDL_ListNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDL_ListNode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDL_ListNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDL_ListNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDL_ListNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDL.ListNode", Arcadia_DDL_ListNode,
                         u8"Arcadia.DDL.Node", Arcadia_DDL_Node,
                         &_Arcadia_DDL_ListNode_typeOperations);

static void
Arcadia_DDL_ListNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_ListNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDL_ListNode);
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_DDL_NodeType_List);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->elements = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_LeaveConstructor(Arcadia_DDL_ListNode);
}

static void
Arcadia_DDL_ListNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_ListNodeDispatch* self
  )
{ }

static void
Arcadia_DDL_ListNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_ListNode* self
  )
{
  if (self->elements) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->elements);
  }
}

Arcadia_DDL_ListNode*
Arcadia_DDL_ListNode_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_ListNode);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_DDL_ListNode);
}

Arcadia_SizeValue
Arcadia_DDL_ListNode_getNumberOfElements
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_ListNode* self
  )
{ return Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->elements); }

Arcadia_DDL_Node*
Arcadia_DDL_ListNode_getElementAt
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_ListNode* self,
    Arcadia_SizeValue index
  )
{ return (Arcadia_DDL_Node*)Arcadia_List_getObjectReferenceValueAt(thread, self->elements, index); }
