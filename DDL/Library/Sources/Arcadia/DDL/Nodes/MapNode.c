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
#include "Arcadia/DDL/Nodes/MapNode.h"

static void
Arcadia_DDL_MapNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* self
  );

static void
Arcadia_DDL_MapNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNodeDispatch* self
  );

static void
Arcadia_DDL_MapNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDL_MapNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDL_MapNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDL_MapNode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDL_MapNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDL_MapNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDL_MapNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDL.MapNode", Arcadia_DDL_MapNode,
                         u8"Arcadia.DDL.Node", Arcadia_DDL_Node,
                         &_Arcadia_DDL_MapNode_typeOperations);

static void
Arcadia_DDL_MapNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDL_MapNode);
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_DDL_NodeType_Map);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->entries = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_LeaveConstructor(Arcadia_DDL_MapNode);
}

static void
Arcadia_DDL_MapNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DDL_MapNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* self
  )
{
  if (self->entries) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->entries);
  }
}

Arcadia_DDL_MapNode*
Arcadia_DDL_MapNode_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_MapNode);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_DDL_MapNode);
}

void
Arcadia_DDL_MapNode_insertBack
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* self,
    Arcadia_DDL_MapEntryNode* entry
  )
{ Arcadia_List_insertBackObjectReferenceValue(thread, self->entries, entry); }

void
Arcadia_DDL_MapNode_inserFront
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* self,
    Arcadia_DDL_MapEntryNode* entry
  )
{ Arcadia_List_insertFrontObjectReferenceValue(thread, self->entries, entry); }

void
Arcadia_DDL_MapNode_insertAt
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapNode* self,
    Arcadia_SizeValue index,
    Arcadia_DDL_MapEntryNode* entry
  )
{ Arcadia_List_insertObjectReferenceValueAt(thread, self->entries, index, entry); }
