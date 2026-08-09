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
#include "Arcadia/DDL/Nodes/BooleanNode.h"

static void
Arcadia_DDL_BooleanNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_BooleanNode* self
  );

static void
Arcadia_DDL_BooleanNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_BooleanNodeDispatch* self
  );

static void
Arcadia_DDL_BooleanNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_BooleanNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDL_BooleanNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDL_BooleanNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDL_BooleanNode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDL_BooleanNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDL_BooleanNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDL_BooleanNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDL.BooleanNode", Arcadia_DDL_BooleanNode,
                         u8"Arcadia.DDL.Node", Arcadia_DDL_Node,
                         &_Arcadia_DDL_BooleanNode_typeOperations);

static void
Arcadia_DDL_BooleanNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_BooleanNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDL_BooleanNode);
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_DDL_NodeType_Boolean);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value x = Arcadia_ValueStack_getValue(thread, 1);
  switch (Arcadia_Value_getTag(&x)) {
    case Arcadia_ValueTag_Boolean: {
      self->value = Arcadia_String_createFromBoolean(thread, Arcadia_Value_getBooleanValue(&x));
    } break;
    case Arcadia_ValueTag_ObjectReference: {
      self->value = Arcadia_ArgumentsValidation_getObjectReferenceValue(thread, &x, _Arcadia_String_getType(thread));
      static const Arcadia_Natural8Value trueString[] = u8"true";
      static const Arcadia_Natural8Value falseString[] = u8"false";
      if (sizeof(trueString) - 1 != Arcadia_String_getNumberOfBytes(thread, self->value) ||
          0 != Arcadia_Memory_compare(thread, trueString, Arcadia_String_getBytes(thread, self->value), sizeof(trueString) - 1)) {
        if (sizeof(falseString) - 1 != Arcadia_String_getNumberOfBytes(thread, self->value) &&
          0 !=Arcadia_Memory_compare(thread, trueString, Arcadia_String_getBytes(thread, self->value), sizeof(falseString) - 1)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
          Arcadia_Thread_jump(thread);
        }
      }
    } break;
    default: {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    } break;
  };
  Arcadia_LeaveConstructor(Arcadia_DDL_BooleanNode);
}

static void
Arcadia_DDL_BooleanNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_BooleanNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DDL_BooleanNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_BooleanNode* self
  )
{
  if (self->value) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->value);
  }
}

Arcadia_DDL_BooleanNode*
Arcadia_DDL_BooleanNode_createBoolean
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue booleanValue
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_BooleanNode);
  Arcadia_ValueStack_pushBooleanValue(thread, booleanValue);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_BooleanNode);
}

Arcadia_DDL_BooleanNode*
Arcadia_DDL_BooleanNode_createString
  (
    Arcadia_Thread* thread,
    Arcadia_String* stringValue
  )
{
  _Arcadia_BeginCreate();
  if (stringValue) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, stringValue);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_BooleanNode);
}
