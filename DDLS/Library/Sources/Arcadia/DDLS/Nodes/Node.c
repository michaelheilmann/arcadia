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
#include "Arcadia/DDLS/Nodes/Node.h"

static void
Arcadia_DDLS_Node_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Node* self
  );

static void
Arcadia_DDLS_Node_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_NodeDispatch* self
  );

static void
Arcadia_DDLS_Node_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Node* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDLS_Node_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDLS_Node_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDLS_Node_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDLS_Node_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDLS_Node_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDLS_Node_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDLS.Node", Arcadia_DDLS_Node,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_DDLS_Node_typeOperations);

static void
Arcadia_DDLS_Node_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Node* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDLS_Node);
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
  Arcadia_LeaveConstructor(Arcadia_DDLS_Node);
}

static void
Arcadia_DDLS_Node_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_NodeDispatch* self
  )
{ }

static void
Arcadia_DDLS_Node_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDLS_Node* self
  )
{/*Intentionally empty.*/}
