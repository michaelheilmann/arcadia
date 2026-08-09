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
#include "Arcadia/ADL/Definitions.h"

#include "Arcadia/ADL/Definition.h"

static void
Arcadia_ADL_Definitions_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* self
  );

static void
Arcadia_ADL_Definitions_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* self
  );

static void
Arcadia_ADL_Definitions_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DefinitionsDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_Definitions_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_Definitions_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_Definitions_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.Definitions", Arcadia_ADL_Definitions,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_ADL_Definitions_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* self
  )
{
  if (self->definitions) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->definitions);
  }
}

static void
Arcadia_ADL_Definitions_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_Definitions);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->definitions = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_LeaveConstructor(Arcadia_ADL_Definitions);
}

static void
Arcadia_ADL_Definitions_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DefinitionsDispatch* self
  )
{ }

Arcadia_ADL_Definitions*
Arcadia_ADL_Definitions_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate();
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_ADL_Definitions);
}

Arcadia_ADL_Definition*
Arcadia_ADL_Definitions_getDefinitionOrNull
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* self,
    Arcadia_String* name
  )
{
  Arcadia_Value v = Arcadia_Map_get(thread, self->definitions, Arcadia_Value_makeObjectReferenceValue(name));
  if (Arcadia_Value_isVoidValue(&v)) {
    return NULL;
  }
  return (Arcadia_ADL_Definition*)Arcadia_Value_getObjectReferenceValueChecked(thread, v, _Arcadia_ADL_Definition_getType(thread));
}

void
Arcadia_ADL_Definitions_link
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* self
  )
{
  Arcadia_List* definitions = Arcadia_Map_getValues(thread, self->definitions);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)definitions); i < n; ++i) {
    Arcadia_ADL_Definition* definition =
      (Arcadia_ADL_Definition*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, definitions, i, _Arcadia_ADL_Definition_getType(thread));
    Arcadia_ADL_Definition_link(thread, definition);
  }
}
