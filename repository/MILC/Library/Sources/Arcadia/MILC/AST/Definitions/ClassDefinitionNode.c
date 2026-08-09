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

#include "Arcadia/MILC/AST/Definitions/ClassDefinitionNode.h"

#include "Arcadia/MILC/AST/Include.h"

static void
Arcadia_MILC_AST_ClassDefinitionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ClassDefinitionNode* self
  );

static void
Arcadia_MILC_AST_ClassDefinitionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ClassDefinitionNodeDispatch* self
  );

static void
Arcadia_MILC_AST_ClassDefinitionNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ClassDefinitionNode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MILC_AST_ClassDefinitionNode_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MILC_AST_ClassDefinitionNode_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_MILC_AST_ClassDefinitionNode_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.MILC.AST.ClassDefinitionNode", Arcadia_MILC_AST_ClassDefinitionNode,
                         u8"Arcadia.MILC.AST.DefinitionNode", Arcadia_MILC_AST_DefinitionNode,
                         &_typeOperations);

static void
Arcadia_MILC_AST_ClassDefinitionNode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ClassDefinitionNode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MILC_AST_ClassDefinitionNode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->className = (Arcadia_MILC_AST_IdentifierNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_MILC_AST_IdentifierNode_getType(thread));
  if (Arcadia_ValueStack_isObjectReferenceValue(thread, 2)) {
    self->extendedClassName = (Arcadia_MILC_AST_IdentifierNode*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_MILC_AST_IdentifierNode_getType(thread));
  } else if (Arcadia_ValueStack_isVoidValue(thread, 2)) {
    self->extendedClassName = NULL;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->classBody = (Arcadia_List*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_List_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_MILC_AST_ClassDefinitionNode);
}

static void
Arcadia_MILC_AST_ClassDefinitionNode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ClassDefinitionNodeDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_MILC_AST_ClassDefinitionNode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_AST_ClassDefinitionNode* self
  )
{
  if (self->className) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->className);
  }
  if (self->extendedClassName) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->extendedClassName);
  }
  if (self->classBody) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->classBody);
  }
}

Arcadia_MILC_AST_ClassDefinitionNode*
Arcadia_MILC_AST_ClassDefinitionNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue startOffset,
    Arcadia_MILC_AST_IdentifierNode* className,
    Arcadia_MILC_AST_IdentifierNode* extendedClassName,
    Arcadia_List* classBody
  )
{
  _Arcadia_BeginCreate(Arcadia_MILC_AST_ClassDefinitionNode);
  if (className) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, className);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (extendedClassName) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, extendedClassName);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (classBody) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, classBody);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_MILC_AST_ClassDefinitionNode);
}
