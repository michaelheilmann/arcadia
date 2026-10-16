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

#include "Arcadia/PDLC/AST/DefinitionStatements/LabelDefinitionStatementNode.h"

#include "Arcadia/PDLC/AST/Include.h"

/// @code
/// constructor(labelName:Arcadia.String)
/// @endcode
static void
Arcadia_PDLC_AST_LabelDefinitionStatementNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_LabelDefinitionStatementNode* self
  );

static void
Arcadia_PDLC_AST_LabelDefinitionStatementNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_LabelDefinitionStatementNodeDispatch* self
  );

static void
Arcadia_PDLC_AST_LabelDefinitionStatementNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_LabelDefinitionStatementNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_PDLC_AST_LabelDefinitionStatementNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_PDLC_AST_LabelDefinitionStatementNode_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_PDLC_AST_LabelDefinitionStatementNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.PDLC.AST.LabelDefinitionStatementNode", Arcadia_PDLC_AST_LabelDefinitionStatementNode,
                         u8"Arcadia.PDLC.AST.DefinitionStatementNode", Arcadia_PDLC_AST_DefinitionStatementNode,
                         &_typeOperations);

static void
Arcadia_PDLC_AST_LabelDefinitionStatementNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_LabelDefinitionStatementNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_PDLC_AST_LabelDefinitionStatementNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->labelName = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_PDLC_AST_LabelDefinitionStatementNode);
}

static void
Arcadia_PDLC_AST_LabelDefinitionStatementNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_LabelDefinitionStatementNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_PDLC_AST_LabelDefinitionStatementNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_AST_LabelDefinitionStatementNode* self
  )
{ Arcadia_Object_visit(thread, (Arcadia_Object*)self->labelName); }

Arcadia_PDLC_AST_LabelDefinitionStatementNode*
Arcadia_PDLC_AST_LabelDefinitionStatementNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_String* labelName
  )
{
  _Arcadia_BeginCreate(Arcadia_PDLC_AST_LabelDefinitionStatementNode);
  if (labelName) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, labelName);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_PDLC_AST_LabelDefinitionStatementNode);
}
