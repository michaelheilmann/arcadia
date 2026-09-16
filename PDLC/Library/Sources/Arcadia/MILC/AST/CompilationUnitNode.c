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

#include "Arcadia/MILC/AST/CompilationUnitNode.h"

#include "Arcadia/MILC/AST/Include.h"

/// @code
/// constructor()
/// @endcode
static void
Arcadia_MILC_AST_CompilationUnitNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_CompilationUnitNode* self
  );

static void
Arcadia_MILC_AST_CompilationUnitNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_CompilationUnitNodeDispatch* self
  );

static void
Arcadia_MILC_AST_CompilationUnitNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_CompilationUnitNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_MILC_AST_CompilationUnitNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_CompilationUnitNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_AST_CompilationUnitNode_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_CompilationUnitNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_MILC_AST_CompilationUnitNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_MILC_AST_CompilationUnitNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.CompilationUnitNode", Arcadia_MILC_AST_CompilationUnitNode,
                         u8"Arcadia.MILC.AST.Node", Arcadia_MILC_AST_Node,
                         &_Arcadia_MILC_AST_CompilationUnitNode_typeOperations);

static void
Arcadia_MILC_AST_CompilationUnitNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_CompilationUnitNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_CompilationUnitNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->moduleNode = NULL;
  self->filePath = NULL;
  self->definitions = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_CompilationUnitNode);
}

static void
Arcadia_MILC_AST_CompilationUnitNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_CompilationUnitNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_AST_CompilationUnitNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_CompilationUnitNode* self
  )
{
  if (self->moduleNode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->moduleNode);
  }
  if (self->filePath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->filePath);
  }
  if (self->definitions) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->definitions); 
  }
}

Arcadia_MILC_AST_CompilationUnitNode*
Arcadia_MILC_AST_CompilationUnitNode_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_AST_CompilationUnitNode);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MILC_AST_CompilationUnitNode);
}

void
Arcadia_MILC_AST_CompilationUnitNode_appendDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_CompilationUnitNode* self,
    Arcadia_MILC_AST_DefinitionNode* definition
  )
{
  Arcadia_List_insertBackObjectReferenceValue(thread, self->definitions, (Arcadia_ObjectReferenceValue)definition);
}

Arcadia_SizeValue
Arcadia_MILC_AST_CompilationUnitNode_getNumberOfDefinitions
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_CompilationUnitNode* self
  )
{ return Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->definitions); }

Arcadia_MILC_AST_DefinitionNode*
Arcadia_MILC_AST_CompilationUnitNode_getDefinitionAt
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_CompilationUnitNode* self,
    Arcadia_SizeValue index
  )
{
  Arcadia_Value definitionAstValue = Arcadia_List_getAt(thread, self->definitions, index);
  Arcadia_MILC_AST_DefinitionNode* definitionAst = Arcadia_Value_getObjectReferenceValue(&definitionAstValue);
  return definitionAst;
}
