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
#include "Arcadia/ADL/Definitions/Visuals/MeshGenerator.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Reference.h"
#include "Arcadia/ADL/Definitions/Visuals/Include.h"

static void
Arcadia_ADL_MeshGenerator_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshGenerator* self
  );

static void
Arcadia_ADL_MeshGenerator_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshGenerator* self
  );

static void
Arcadia_ADL_MeshGenerator_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshGeneratorDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_MeshGenerator_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_MeshGenerator_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_MeshGenerator_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.MeshGenerator", Arcadia_ADL_MeshGenerator,
                          u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                          &_typeOperations);

static void
Arcadia_ADL_MeshGenerator_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshGenerator* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_MeshGenerator);
  {
    Arcadia_Value definitions, name;
    definitions = Arcadia_ValueStack_getValue(thread, 3);
    name = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->ambientColor = Arcadia_ADL_Reference_create
    (
      thread,
      ((Arcadia_ADL_Definition*)self)->definitions,
      (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked
        (
          thread,
          1,
          _Arcadia_String_getType(thread)
        )
    );
  Arcadia_LeaveConstructor(Arcadia_ADL_MeshGenerator);
}

static void
Arcadia_ADL_MeshGenerator_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshGenerator* self
  )
{
  if (self->ambientColor) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ambientColor);
  }
}

static void
Arcadia_ADL_MeshGenerator_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshGenerator* self
  )
{
  Arcadia_ADL_Reference_resolve(thread, self->ambientColor);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self->ambientColor->definition, _Arcadia_ADL_ColorDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_ADL_MeshGenerator_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshGeneratorDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = (void (*)(Arcadia_Thread*, Arcadia_ADL_Definition*))&Arcadia_ADL_MeshGenerator_linkImpl;
}

Arcadia_ADL_ColorDefinition*
Arcadia_ADL_MeshGenerator_getAmbientColor
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshGenerator* self
  )
{ return (Arcadia_ADL_ColorDefinition*)self->ambientColor->definition; }
