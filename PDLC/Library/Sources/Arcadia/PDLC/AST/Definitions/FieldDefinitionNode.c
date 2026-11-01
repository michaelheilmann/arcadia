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

#include "Arcadia/PDLC/AST/Definitions/FieldDefinitionNode.h"

#include "Arcadia/PDLC/AST/Include.h"

/// @code
/// constructor(variableName:Arcadia.String)
/// @endcode
static void
Arcadia_PDLC_AST_FieldDefinitionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_FieldDefinitionNode* self
  );

static void
Arcadia_PDLC_AST_FieldDefinitionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_FieldDefinitionNodeDispatch* self
  );

static void
Arcadia_PDLC_AST_FieldDefinitionNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_FieldDefinitionNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_AST_FieldDefinitionNode_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_AST_FieldDefinitionNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_AST_FieldDefinitionNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.AST.FieldDefinitionNode", Arcadia_PDLC_AST_FieldDefinitionNode,
                         u8"Arcadia.PDLC.AST.DefinitionNode", Arcadia_PDLC_AST_DefinitionNode,
                         &_typeOperations);

static void
Arcadia_PDLC_AST_FieldDefinitionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_FieldDefinitionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_AST_FieldDefinitionNode);
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
  self->type = Arcadia_ValueStack_isVoidValue(thread, 1) ? NULL : (Arcadia_PDLC_AST_IdentifierNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_PDLC_AST_IdentifierNode_getType(thread));
  self->isReference = Arcadia_BooleanValue_False;
  self->isReadOnly = Arcadia_BooleanValue_False;
  self->isOutput = Arcadia_BooleanValue_False;
  self->ownership = Arcadia_PDLC_Ownership_Managed;
  self->recordFields = NULL;
  self->initializer = NULL;
  self->name = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_PDLC_AST_FieldDefinitionNode);
}

static void
Arcadia_PDLC_AST_FieldDefinitionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_FieldDefinitionNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_AST_FieldDefinitionNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_FieldDefinitionNode* self
  )
{
  if (self->recordFields) Arcadia_Object_visit(thread, (Arcadia_Object*)self->recordFields);
  if (self->initializer) Arcadia_Object_visit(thread, (Arcadia_Object*)self->initializer);
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
  if (self->type) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->type);
  }
}

Arcadia_PDLC_AST_FieldDefinitionNode*
Arcadia_PDLC_AST_FieldDefinitionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* name,
    Arcadia_PDLC_AST_IdentifierNode* type
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_AST_FieldDefinitionNode);
  Arcadia_ValueStack_pushSizeValue(thread, startOffset);
  if (name) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (type) {
  if (type) Arcadia_ValueStack_pushObjectReferenceValue(thread, type);
  else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_PDLC_AST_FieldDefinitionNode);
}
