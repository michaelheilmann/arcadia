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
#include "Arcadia/DDL/Nodes/NameNode.h"

static void
Arcadia_DDL_NameNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NameNode* self
  );

static void
Arcadia_DDL_NameNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NameNodeDispatch* self
  );

static void
Arcadia_DDL_NameNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NameNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDL_NameNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDL_NameNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDL_NameNode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDL_NameNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDL_NameNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDL_NameNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDL.NameNode", Arcadia_DDL_NameNode,
                         u8"Arcadia.DDL.Node", Arcadia_DDL_Node,
                         &_Arcadia_DDL_NameNode_typeOperations);

static inline Arcadia_BooleanValue
isFirst
  (
    Arcadia_Natural8Value byte
  )
{
  return '_' == byte
      || ('a' <= byte && byte <= 'z')
      || ('A' <= byte && byte <= 'Z');
}

static inline Arcadia_BooleanValue
isRest
  (
    Arcadia_Natural8Value byte
  )
{
  return '_' == byte
    || ('a' <= byte && byte <= 'z')
    || ('A' <= byte && byte <= 'Z')
    || ('0' <= byte && byte <= '9');
}

static void
Arcadia_DDL_NameNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NameNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDL_NameNode);
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_DDL_NodeType_Name);
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
      Arcadia_Natural8Value const* start = Arcadia_String_getBytes(thread, self->value);
      Arcadia_Natural8Value const* end = start + Arcadia_String_getNumberOfBytes(thread, self->value);
      Arcadia_Natural8Value const* current = start;
      if (current == end || !isFirst(*current)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
      do {
        current++;
      } while (current != end && isRest(*current));
      if (current != end) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
  Arcadia_LeaveConstructor(Arcadia_DDL_NameNode);
}

static void
Arcadia_DDL_NameNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NameNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DDL_NameNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NameNode* self
  )
{
  if (self->value) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->value);
  }
}

Arcadia_DDL_NameNode*
Arcadia_DDL_NameNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_NameNode);
  if (string) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, string);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_NameNode);
}
