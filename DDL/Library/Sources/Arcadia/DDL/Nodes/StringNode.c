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
#include "Arcadia/DDL/Nodes/StringNode.h"

static void
Arcadia_DDL_StringNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_StringNode* self
  );

static void
Arcadia_DDL_StringNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_StringNodeDispatch* self
  );

static void
Arcadia_DDL_StringNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_StringNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDL_StringNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDL_StringNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDL_StringNode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDL_StringNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDL_StringNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDL_StringNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDL.StringNode", Arcadia_DDL_StringNode,
                         u8"Arcadia.DDL.Node", Arcadia_DDL_Node,
                         &_Arcadia_DDL_StringNode_typeOperations);

static void
Arcadia_DDL_StringNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_StringNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDL_StringNode);
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_DDL_NodeType_String);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value x = Arcadia_ValueStack_getValue(thread, 1);
  switch (Arcadia_Value_getTag(&x)) {
    case Arcadia_ValueTag_ObjectReference: {
      self->value = Arcadia_ArgumentsValidation_getObjectReferenceValue(thread, &x, _Arcadia_String_getType(thread));
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
  Arcadia_LeaveConstructor(Arcadia_DDL_StringNode);
}

static void
Arcadia_DDL_StringNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_StringNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DDL_StringNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_StringNode* self
  )
{
  if (self->value) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->value);
  }
}

Arcadia_DDL_StringNode*
Arcadia_DDL_StringNode_createString
  (
    Arcadia_Thread* thread,
    Arcadia_String* stringValue
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_StringNode);
  if (stringValue) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, stringValue);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_StringNode);
}
