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
#include "Arcadia/ADL/Definition.h"

#include "Arcadia/ADL/Definitions.h"

static void
Arcadia_ADL_Definition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self
  );

static void
Arcadia_ADL_Definition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self
  );

static void
Arcadia_ADL_Definition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DefinitionDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_Definition_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_Definition_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_Definition_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_ADL_Definition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self
  )
{
  if (self->definitions) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->definitions);
  }
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
}

static void
Arcadia_ADL_Definition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_Definition);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->definitions = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_ADL_Definitions_getType(thread));
  self->name = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_ADL_Definition);
}

static void
Arcadia_ADL_Definition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_DefinitionDispatch* self
  )
{
}

Arcadia_ADL_Definitions*
Arcadia_ADL_Definitions_getDefinitions
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self
  )
{ return self->definitions; }

Arcadia_String*
Arcadia_ADL_Definition_getName
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self
  )
{ return self->name; }

void
Arcadia_ADL_Definition_link
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self
  )
{ Arcadia_VirtualCall(Arcadia_ADL_Definition, link, self); }
