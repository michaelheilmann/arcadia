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
#include "Arcadia/DDL/Nodes/VoidNode.h"

static void
Arcadia_DDL_VoidNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_VoidNode* self
  );

static void
Arcadia_DDL_VoidNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_VoidNodeDispatch* self
  );

static void
Arcadia_DDL_VoidNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_VoidNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDL_VoidNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDL_VoidNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDL_VoidNode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDL_VoidNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDL_VoidNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDL_VoidNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDL.VoidNode", Arcadia_DDL_VoidNode,
                         u8"Arcadia.DDL.Node", Arcadia_DDL_Node,
                         &_Arcadia_DDL_VoidNode_typeOperations);

static void
Arcadia_DDL_VoidNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_VoidNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDL_VoidNode);
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_DDL_NodeType_Void);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value x = Arcadia_ValueStack_getValue(thread, 1);
  switch (Arcadia_Value_getTag(&x)) {
    case Arcadia_ValueTag_Void: {
      self->value = Arcadia_String_createFromVoid(thread, Arcadia_Value_getVoidValue(&x));
    } break;
    case Arcadia_ValueTag_ObjectReference: {
      self->value = Arcadia_ArgumentsValidation_getObjectReferenceValue(thread, &x, _Arcadia_String_getType(thread));
      static const Arcadia_Natural8Value voidString[] = u8"void";
      if (sizeof(voidString) - 1 != Arcadia_String_getNumberOfBytes(thread, self->value) ||
          0 != Arcadia_Memory_compare(thread, voidString, Arcadia_String_getBytes(thread, self->value), sizeof(voidString) - 1)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
          Arcadia_Thread_jump(thread);
      }
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
  Arcadia_LeaveConstructor(Arcadia_DDL_VoidNode);
}

static void
Arcadia_DDL_VoidNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_VoidNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DDL_VoidNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_VoidNode* self
  )
{
  if (self->value) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->value);
  }
}

Arcadia_DDL_VoidNode*
Arcadia_DDL_VoidNode_createVoid
  (
    Arcadia_Thread* thread,
    Arcadia_VoidValue voidValue
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_VoidNode);
  Arcadia_ValueStack_pushVoidValue(thread, voidValue);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_VoidNode);
}

Arcadia_DDL_VoidNode*
Arcadia_DDL_VoidNode_createString
  (
    Arcadia_Thread* thread,
    Arcadia_String* stringValue
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_VoidNode);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, stringValue);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_VoidNode);
}
