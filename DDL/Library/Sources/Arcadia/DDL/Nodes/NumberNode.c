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
#include "Arcadia/DDL/Nodes/NumberNode.h"

static void
Arcadia_DDL_NumberNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NumberNode* self
  );

static void
Arcadia_DDL_NumberNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NumberNodeDispatch* self
  );

static void
Arcadia_DDL_NumberNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NumberNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_DDL_NumberNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DDL_NumberNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DDL_NumberNode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DDL_NumberNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_DDL_NumberNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_DDL_NumberNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DDL.NumberNode", Arcadia_DDL_NumberNode,
                         u8"Arcadia.DDL.Node", Arcadia_DDL_Node,
                         &_Arcadia_DDL_NumberNode_typeOperations);

static inline bool isDigit(Arcadia_Natural8Value byte) {
  return ('0' <= byte && byte <= '9');
}

static void
Arcadia_DDL_NumberNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NumberNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DDL_NumberNode);
  {
    Arcadia_ValueStack_pushInteger32Value(thread, Arcadia_DDL_NodeType_Number);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value x = Arcadia_ValueStack_getValue(thread, 1);
  switch (Arcadia_Value_getTag(&x)) {
    case Arcadia_ValueTag_Integer16: {
      self->value = Arcadia_String_createFromInteger16(thread, Arcadia_Value_getInteger16Value(&x));
    } break;
    case Arcadia_ValueTag_Integer32: {
      self->value = Arcadia_String_createFromInteger32(thread, Arcadia_Value_getInteger32Value(&x));
    } break;
    case Arcadia_ValueTag_Integer64: {
      self->value = Arcadia_String_createFromInteger64(thread, Arcadia_Value_getInteger64Value(&x));
    } break;
    case Arcadia_ValueTag_Integer8: {
      self->value = Arcadia_String_createFromInteger8(thread, Arcadia_Value_getInteger8Value(&x));
    } break;
    case Arcadia_ValueTag_Natural16: {
      self->value = Arcadia_String_createFromNatural16(thread, Arcadia_Value_getNatural16Value(&x));
    } break;
    case Arcadia_ValueTag_Natural32: {
      self->value = Arcadia_String_createFromNatural32(thread, Arcadia_Value_getNatural32Value(&x));
    } break;
    case Arcadia_ValueTag_Natural64: {
      self->value = Arcadia_String_createFromNatural64(thread, Arcadia_Value_getNatural64Value(&x));
    } break;
    case Arcadia_ValueTag_Natural8: {
      self->value = Arcadia_String_createFromNatural8(thread, Arcadia_Value_getNatural8Value(&x));
    } break;
    case Arcadia_ValueTag_Real32: {
      self->value = Arcadia_String_createFromReal32(thread, Arcadia_Value_getReal32Value(&x));
    } break;
    case Arcadia_ValueTag_Real64: {
      self->value = Arcadia_String_createFromReal64(thread, Arcadia_Value_getReal64Value(&x));
    } break;
    case Arcadia_ValueTag_ObjectReference: {
      self->value = Arcadia_ArgumentsValidation_getObjectReferenceValue(thread, &x, _Arcadia_String_getType(thread));
      Arcadia_Natural8Value const* start = Arcadia_String_getBytes(thread, self->value);
      Arcadia_Natural8Value const* end = start + Arcadia_String_getNumberOfBytes(thread, self->value);
      Arcadia_Natural8Value const* current = start;
      if (current != end && *current == '+' || *current == '-') {
        current++;
      }
      if (current == end) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
      if (isDigit(*current)) {
        do {
          current++;
        } while (current != end && isDigit(*current));
        if (current != end && *current == '.') {
          current++;
          while (current != end && isDigit(*current)) {
            current++;
          }
        }
      } else if ('.' == *current) {
          current++;
          if (current == end || !isDigit(*current)) {
            Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
            Arcadia_Thread_jump(thread);
          }
          do {
            current++;
          } while (current != end && isDigit(*current));
      } else {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      }
      if (current != end && ('e' == *current || 'E' == *current)) {
        current++;
        if (current != end && *current == '+' || *current == '-') {
          current++;
        }
        if (current == end || !isDigit(*current)) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
          Arcadia_Thread_jump(thread);
        }
        do {
          current++;
        } while (current != end && isDigit(*current));
      }
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
  Arcadia_LeaveConstructor(Arcadia_DDL_NumberNode);
}

static void
Arcadia_DDL_NumberNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NumberNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DDL_NumberNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_NumberNode* self
  )
{
  if (self->value) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->value);
  }
}

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createInteger16
  (
    Arcadia_Thread* thread,
    Arcadia_Integer16Value integer16Value
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_NumberNode);
  Arcadia_ValueStack_pushInteger16Value(thread, integer16Value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_NumberNode);
}

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value integer32Value
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_NumberNode);
  Arcadia_ValueStack_pushInteger32Value(thread, integer32Value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_NumberNode);
}

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createInteger64
  (
    Arcadia_Thread* thread,
    Arcadia_Integer64Value integer64Value
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_NumberNode);
  Arcadia_ValueStack_pushInteger64Value(thread, integer64Value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_NumberNode);
}

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createInteger8
  (
    Arcadia_Thread* thread,
    Arcadia_Integer8Value integer8Value
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_NumberNode);
  Arcadia_ValueStack_pushInteger8Value(thread, integer8Value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_NumberNode);
}

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createNatural16
  (
    Arcadia_Thread* thread,
    Arcadia_Natural16Value natural16Value
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_NumberNode);
  Arcadia_ValueStack_pushNatural16Value(thread, natural16Value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_NumberNode);
}

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createNatural32
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value natural32Value
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_NumberNode);
  Arcadia_ValueStack_pushNatural32Value(thread, natural32Value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_NumberNode);
}

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createNatural64
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value natural64Value
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_NumberNode);
  Arcadia_ValueStack_pushNatural64Value(thread, natural64Value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_NumberNode);
}

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createNatural8
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value natural8Value
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_NumberNode);
  Arcadia_ValueStack_pushNatural8Value(thread, natural8Value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_NumberNode);
}

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createReal32
  (
    Arcadia_Thread* thread,
    Arcadia_Real32Value real32Value
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_NumberNode);
  Arcadia_ValueStack_pushReal32Value(thread, real32Value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_NumberNode);
}

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createReal64
  (
    Arcadia_Thread* thread,
    Arcadia_Real64Value real64Value
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_NumberNode);
  Arcadia_ValueStack_pushReal64Value(thread, real64Value);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_NumberNode);
}

Arcadia_DDL_NumberNode*
Arcadia_DDL_NumberNode_createString
  (
    Arcadia_Thread* thread,
    Arcadia_String* stringValue
  )
{
  _Arcadia_BeginCreate(Arcadia_DDL_NumberNode);
  if (stringValue) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, stringValue);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DDL_NumberNode);
}
