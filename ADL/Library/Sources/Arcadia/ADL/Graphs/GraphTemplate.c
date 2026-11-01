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

#define ARCADIA_ADL_PRIVATE (1)
#include "Arcadia/ADL/Graphs/GraphTemplate.h"

static void
Arcadia_ADL_GraphTemplate_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphTemplate* self
  );

static void
Arcadia_ADL_GraphTemplate_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphTemplateDispatch* self
  );

static void
Arcadia_ADL_GraphTemplate_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphTemplate* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_GraphTemplate_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_GraphTemplate_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_GraphTemplate_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.GraphTemplate", Arcadia_ADL_GraphTemplate,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_ADL_GraphTemplate_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphTemplate* self
  )
{
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
  if (self->nodes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->nodes);
  }
  if (self->symbols) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->symbols);
  }
  if (self->symbolTable) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->symbolTable);
  }
  if (self->parameterByName) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->parameterByName);
  }
  if (self->outputSymbol) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->outputSymbol);
  }
}

static void
Arcadia_ADL_GraphTemplate_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphTemplate* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_GraphTemplate);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->name = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread));
  self->nodes = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Object_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_ADL_GraphTemplate);
}

static void
Arcadia_ADL_GraphTemplate_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphTemplateDispatch* self
  )
{ }

Arcadia_ADL_GraphTemplate*
Arcadia_ADL_GraphTemplate_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_List* nodes
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_GraphTemplate);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)nodes);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_ADL_GraphTemplate);
}
