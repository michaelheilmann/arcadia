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
#include "Arcadia/DDL/Nodes/MapEntryNode.h"

#include "Arcadia/DDL/Nodes/Include.h"

static void
Arcadia_DDL_MapEntryNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapEntryNode* self
  );

static void
Arcadia_DDL_MapEntryNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapEntryNodeDispatch* self
  );

static void
Arcadia_DDL_MapEntryNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapEntryNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDL_MapEntryNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDL_MapEntryNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDL_MapEntryNode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDL_MapEntryNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDL_MapEntryNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDL_MapEntryNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDL.MapEntryNode", Arcadia_DDL_MapEntryNode,
                         u8"Arcadia.DDL.Node", Arcadia_DDL_Node,
                         &_Arcadia_DDL_MapEntryNode_typeOperations);

static void
Arcadia_DDL_MapEntryNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapEntryNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDL_MapEntryNode);
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_DDL_NodeType_MapEntry);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->key = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_DDL_NameNode_getType(thread));
  self->value = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_DDL_Node_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_DDL_MapEntryNode);
}

static void
Arcadia_DDL_MapEntryNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapEntryNodeDispatch* self
  )
{ }

static void
Arcadia_DDL_MapEntryNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_MapEntryNode* self
  )
{
  if (self->key) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->key);
  }
  if (self->value) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->value);
  }
}


Arcadia_DDL_MapEntryNode*
Arcadia_DDL_MapEntryNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NameNode* key,
    Arcadia_DDL_Node* value
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_MapEntryNode);
  if (key) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, key);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (value) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, value);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_DDL_MapEntryNode);
}
