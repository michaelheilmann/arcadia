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

#include "Arcadia/MILC/AST/ModuleNode.h"

#include "Arcadia/MILC/AST/Include.h"

/// @code
/// constructor()
/// @endcode
static void
Arcadia_MILC_AST_ModuleNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleNode* self
  );

static void
Arcadia_MILC_AST_ModuleNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleNodeDispatch* self
  );

static void
Arcadia_MILC_AST_ModuleNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleNode* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_MILC_AST_ModuleNode_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_ModuleNode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_AST_ModuleNode_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_ModuleNode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_MILC_AST_ModuleNode_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_MILC_AST_ModuleNode_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.ModuleNode", Arcadia_MILC_AST_ModuleNode,
                         u8"Arcadia.MILC.AST.Node", Arcadia_MILC_AST_Node,
                         &_Arcadia_MILC_AST_ModuleNode_typeOperations);

static void
Arcadia_MILC_AST_ModuleNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_ModuleNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->moduleSymbol = NULL;
  self->moduleDirectoryPath = NULL;
  self->compilationUnits = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_ModuleNode);
}

static void
Arcadia_MILC_AST_ModuleNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_AST_ModuleNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleNode* self
  )
{
  if (self->moduleSymbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->moduleSymbol);
  }
  if (self->moduleDirectoryPath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->moduleDirectoryPath);
  }
  if (self->compilationUnits) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->compilationUnits);
  }
}

Arcadia_MILC_AST_ModuleNode*
Arcadia_MILC_AST_ModuleNode_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_AST_ModuleNode);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MILC_AST_ModuleNode);
}

void
Arcadia_MILC_AST_ModuleNode_appendCompilationUnit
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleNode* self,
    Arcadia_MILC_AST_CompilationUnitNode* compilationUnit
  )
{
  Arcadia_List_insertBackObjectReferenceValue(thread, self->compilationUnits, (Arcadia_ObjectReferenceValue)compilationUnit);
}

Arcadia_SizeValue
Arcadia_MILC_AST_ModuleNode_getNumberOfCompilationUnits
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleNode* self
  )
{ return Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->compilationUnits); }

Arcadia_MILC_AST_CompilationUnitNode*
Arcadia_MILC_AST_ModuleNode_getCompilationUnitAt
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ModuleNode* self,
    Arcadia_SizeValue index
  )
{
  return (Arcadia_MILC_AST_CompilationUnitNode*)Arcadia_List_getObjectReferenceValueAt(thread, self->compilationUnits, index);
}
