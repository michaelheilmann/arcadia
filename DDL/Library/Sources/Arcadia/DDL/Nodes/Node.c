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
#include "Arcadia/DDL/Nodes/Node.h"

static void
Arcadia_DDL_Node_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* self
  );

static void
Arcadia_DDL_Node_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NodeDispatch* self
  );

static void
Arcadia_DDL_Node_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDL_Node_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDL_Node_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDL_Node_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDL_Node_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDL_Node_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDL_Node_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDL.Node", Arcadia_DDL_Node,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_DDL_Node_typeOperations);

static void
Arcadia_DDL_Node_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDL_Node);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->type = Arcadia_ValueStack_getInteger32Value(thread, 1);
  switch (self->type) {
    case Arcadia_DDL_NodeType_Boolean:
    case Arcadia_DDL_NodeType_List:
    case Arcadia_DDL_NodeType_Map:
    case Arcadia_DDL_NodeType_MapEntry:
    case Arcadia_DDL_NodeType_Name:
    case Arcadia_DDL_NodeType_Number:
    case Arcadia_DDL_NodeType_String:
    case Arcadia_DDL_NodeType_Void: {
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
  Arcadia_LeaveConstructor(Arcadia_DDL_Node);
}

static void
Arcadia_DDL_Node_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DDL_Node_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Node* self
  )
{/*Initially empty.*/}
