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

#define ARCADIA_DDLS_NODES_PRIVATE (1)
#include "Arcadia/DDLS/Nodes/AnyNode.h"

static void
Arcadia_DDLS_AnyNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_AnyNode* self
  );

static void
Arcadia_DDLS_AnyNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_AnyNodeDispatch* self
  );

static void
Arcadia_DDLS_AnyNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_AnyNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_AnyNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_AnyNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_AnyNode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDLS_AnyNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_AnyNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_AnyNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.AnyNode", Arcadia_DDLS_AnyNode,
                         u8"Arcadia.DDLS.Node", Arcadia_DDLS_Node,
                         &_Arcadia_DDLS_AnyNode_typeOperations);

static void
Arcadia_DDLS_AnyNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_AnyNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_AnyNode);
  //
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  Arcadia_LeaveConstructor(Arcadia_DDLS_AnyNode);
}

static void
Arcadia_DDLS_AnyNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_AnyNodeDispatch* self
  )
{ }

static void
Arcadia_DDLS_AnyNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_AnyNode* self
  )
{/*Intentionally empty.*/}

Arcadia_DDLS_AnyNode*
Arcadia_DDLS_AnyNode_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_DDLS_AnyNode);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_DDLS_AnyNode);
}
