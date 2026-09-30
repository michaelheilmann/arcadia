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

#include "Arcadia/MILC/AST/Literals/IntegerLiteralNode.h"

#include "Arcadia/MILC/AST/Include.h"

static void
Arcadia_MILC_AST_IntegerLiteralNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_IntegerLiteralNode* self
  );

static void
Arcadia_MILC_AST_IntegerLiteralNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_IntegerLiteralNodeDispatch* self
  );

static void
Arcadia_MILC_AST_IntegerLiteralNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_IntegerLiteralNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_MILC_AST_IntegerLiteralNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_IntegerLiteralNode_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_IntegerLiteralNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_AST_IntegerLiteralNode_visitImpl,
};

static const Arcadia_Type_Operations _Arcadia_MILC_AST_IntegerLiteralNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_MILC_AST_IntegerLiteralNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.IntegerLiteralNode", Arcadia_MILC_AST_IntegerLiteralNode,
                         u8"Arcadia.MILC.AST.Node", Arcadia_MILC_AST_Node,
                         &_Arcadia_MILC_AST_IntegerLiteralNode_typeOperations);

static void
Arcadia_MILC_AST_IntegerLiteralNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_IntegerLiteralNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_IntegerLiteralNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->type = NULL;
  self->value = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_IntegerLiteralNode);
}

static void
Arcadia_MILC_AST_IntegerLiteralNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_IntegerLiteralNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_AST_IntegerLiteralNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_IntegerLiteralNode* self
  )
{
  if (self->type) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->type);
  }
  if (self->value) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->value);
  }
}

Arcadia_MILC_AST_IntegerLiteralNode*
Arcadia_MILC_AST_IntegerLiteralNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* value
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_AST_IntegerLiteralNode);
  if (value) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, value);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_MILC_AST_IntegerLiteralNode);
}
