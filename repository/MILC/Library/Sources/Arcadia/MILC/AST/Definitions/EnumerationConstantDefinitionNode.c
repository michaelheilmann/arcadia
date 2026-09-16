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

#include "Arcadia/MILC/AST/Definitions/EnumerationConstantDefinitionNode.h"

#include "Arcadia/MILC/AST/Include.h"

/// @code
/// constructor(variableName:Arcadia.String)
/// @endcode
static void
Arcadia_MILC_AST_EnumerationConstantDefinitionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_EnumerationConstantDefinitionNode* self
  );

static void
Arcadia_MILC_AST_EnumerationConstantDefinitionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_EnumerationConstantDefinitionNodeDispatch* self
  );

static void
Arcadia_MILC_AST_EnumerationConstantDefinitionNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_EnumerationConstantDefinitionNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_EnumerationConstantDefinitionNode_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_EnumerationConstantDefinitionNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_AST_EnumerationConstantDefinitionNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.EnumerationConstantDefinitionNode", Arcadia_MILC_AST_EnumerationConstantDefinitionNode,
                         u8"Arcadia.MILC.AST.DefinitionNode", Arcadia_MILC_AST_DefinitionNode,
                         &_typeOperations);

static void
Arcadia_MILC_AST_EnumerationConstantDefinitionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_EnumerationConstantDefinitionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_EnumerationConstantDefinitionNode);
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value startOffset = Arcadia_ValueStack_getValue(thread, 3);
    Arcadia_ValueStack_pushValue(thread, &startOffset);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  self->name = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread));
  if (Arcadia_ValueStack_isVoidValue(thread, 1)) {
    self->initializer = NULL;
  } else {
    self->initializer = (Arcadia_MILC_AST_IntegerLiteralNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_MILC_AST_IntegerLiteralNode_getType(thread));
  }
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_EnumerationConstantDefinitionNode);
}

static void
Arcadia_MILC_AST_EnumerationConstantDefinitionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_EnumerationConstantDefinitionNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_AST_EnumerationConstantDefinitionNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_EnumerationConstantDefinitionNode* self
  )
{
  if (self->initializer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->initializer);
  }
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name); 
  }
}

Arcadia_MILC_AST_EnumerationConstantDefinitionNode*
Arcadia_MILC_AST_EnumerationConstantDefinitionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* name,
    Arcadia_MILC_AST_IntegerLiteralNode* initializer
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_AST_EnumerationConstantDefinitionNode);
  Arcadia_ValueStack_pushSizeValue(thread, startOffset);
  if (name) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (initializer) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, initializer);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_MILC_AST_EnumerationConstantDefinitionNode);
}
