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
#include "Arcadia/ADL/Graphs/GraphSymbol.module.h"

static void
Arcadia_ADL_GraphSymbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphSymbol* self
  );

static void
Arcadia_ADL_GraphSymbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphSymbolDispatch* self
  );

static void
Arcadia_ADL_GraphSymbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphSymbol* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_GraphSymbol_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_GraphSymbol_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_GraphSymbol_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.GraphSymbol", Arcadia_ADL_GraphSymbol,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_ADL_GraphSymbol_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphSymbol* self
  )
{
  if (self->typeName) Arcadia_Object_visit(thread, (Arcadia_Object*)self->typeName);
  if (self->name) Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  if (self->referenceType) Arcadia_Object_visit(thread, (Arcadia_Object*)self->referenceType);
  if (self->valueType) Arcadia_Object_visit(thread, (Arcadia_Object*)self->valueType);
  if (self->node) Arcadia_Object_visit(thread, (Arcadia_Object*)self->node);
  if (self->referencedNames) Arcadia_Object_visit(thread, (Arcadia_Object*)self->referencedNames);
}

static void
Arcadia_ADL_GraphSymbol_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphSymbol* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_GraphSymbol);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->typeName = NULL;
  self->name = NULL;
  self->referenceType = NULL;
  self->valueType = NULL;
  self->node = NULL;
  self->referencedNames = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_LeaveConstructor(Arcadia_ADL_GraphSymbol);
}

static void
Arcadia_ADL_GraphSymbol_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_GraphSymbolDispatch* self
  )
{ }

Arcadia_ADL_GraphSymbol*
Arcadia_ADL_GraphSymbol_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_GraphSymbol);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ADL_GraphSymbol);
}
