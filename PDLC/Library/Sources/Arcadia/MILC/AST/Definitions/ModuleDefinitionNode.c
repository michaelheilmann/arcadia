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

#include "Arcadia/MILC/AST/Definitions/ModuleDefinitionNode.h"

#include "Arcadia/MILC/AST/Include.h"

/// @code
/// constructor(nativeName:Arcadia.String, procedureName:Arcadia.String, procedureParameters:Arcadia.List, procedureBody:Arcadia.List)
/// @endcode
static void
Arcadia_MILC_AST_ModuleDefinitionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleDefinitionNode* self
  );

static void
Arcadia_MILC_AST_ModuleDefinitionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleDefinitionNodeDispatch* self
  );

static void
Arcadia_MILC_AST_ModuleDefinitionNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleDefinitionNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_ModuleDefinitionNode_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_ModuleDefinitionNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_AST_ModuleDefinitionNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.ModuleDefinitionNode", Arcadia_MILC_AST_ModuleDefinitionNode,
                         u8"Arcadia.MILC.AST.DefinitionNode", Arcadia_MILC_AST_DefinitionNode,
                         &_typeOperations);

static void
Arcadia_MILC_AST_ModuleDefinitionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleDefinitionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_ModuleDefinitionNode);
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
  self->moduleName = (Arcadia_MILC_AST_IdentifierNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_MILC_AST_IdentifierNode_getType(thread));
  if (Arcadia_ValueStack_isVoidValue(thread, 1)) {
    self->moduleBody = NULL;
  } else {
    self->moduleBody = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_List_getType(thread));
  }
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_ModuleDefinitionNode);
}

static void
Arcadia_MILC_AST_ModuleDefinitionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleDefinitionNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_AST_ModuleDefinitionNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleDefinitionNode* self
  )
{
  Arcadia_Object_visit(thread, (Arcadia_Object*)self->moduleName);
  if (self->moduleName) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->moduleName);
  }
  if (self->moduleBody) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->moduleBody);
  }
}

Arcadia_MILC_AST_ModuleDefinitionNode*
Arcadia_MILC_AST_ModuleDefinitionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_MILC_AST_IdentifierNode* moduleName,
    Arcadia_List* moduleBody
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_AST_ModuleDefinitionNode);
  Arcadia_ValueStack_pushSizeValue(thread, startOffset);
  if (moduleName) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, moduleName);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (moduleBody) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, moduleBody);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_MILC_AST_ModuleDefinitionNode);
}
