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

#define ARCADIA_ADL_MODULE (1)
#include "Arcadia/ADL/Reference.h"

#include "Arcadia/Languages/Include.h"
#include "Arcadia/ADL/Definitions.h"

static void
Arcadia_ADL_Reference_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* self
  );

static void
Arcadia_ADL_Reference_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ReferenceDispatch* self
  );

static void
Arcadia_ADL_Reference_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_Reference_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_Reference_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_Reference_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.Reference", Arcadia_ADL_Reference,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_ADL_Reference_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* self
  )
{
  if (self->definition) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->definition);
  }
  if (self->definitionName) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->definitionName);
  }
  if (self->definitions) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->definitions);
  }
}

static void
Arcadia_ADL_Reference_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_Reference);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  self->definition = NULL;
  self->definitionName = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  self->definitions = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_ADL_Definitions_getType(thread));
  //
  Arcadia_LeaveConstructor(Arcadia_ADL_Reference);
}

static void
Arcadia_ADL_Reference_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ReferenceDispatch* self
  )
{ }

Arcadia_ADL_Reference*
Arcadia_ADL_Reference_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* definitionName
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_Reference);
  if (definitions) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  if (definitionName) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitionName);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_ADL_Reference);
}

Arcadia_ADL_Definitions*
Arcadia_ADL_References_getDefinitions
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* self
  )
{ return self->definitions; }

Arcadia_String*
Arcadia_ADL_Reference_getDefinitionName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* self
  )
{ return self->definitionName; }

void
Arcadia_ADL_Reference_resolve
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Reference* self
  )
{
  if (!self->definition) {
    self->definition = Arcadia_ADL_Definitions_getDefinitionOrNull(thread, self->definitions, self->definitionName);
    if (!self->definition) {
      Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
      Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"unable to resolve reference `");
      Arcadia_StringBuilder_insertBackString(thread, message, self->definitionName);
      Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"`");
      Arcadia_Languages_DiagnosticsOld_emit(thread, message);

      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
      Arcadia_Thread_jump(thread);
    }
  }
}
