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
#include "Arcadia/ADL/Definitions/Visuals/ModelDefinition.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Reference.h"
#include "Arcadia/ADL/Definitions/Visuals/Include.h"

static void
Arcadia_ADL_ModelDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ModelDefinition* self
  );

static void
Arcadia_ADL_ModelDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ModelDefinition* self
  );

static void
Arcadia_ADL_ModelDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ModelDefinition* self
  );

static void
Arcadia_ADL_ModelDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ModelDefinitionDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_ModelDefinition_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_ModelDefinition_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_ModelDefinition_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.ModelDefinition", Arcadia_ADL_ModelDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &_typeOperations);

static void
Arcadia_ADL_ModelDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ModelDefinition* self
  )
{
  Arcadia_ADL_Reference_resolve(thread, self->mesh);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self->mesh->definition, _Arcadia_ADL_MeshDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_ADL_Reference_resolve(thread, self->material);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self->material->definition, _Arcadia_ADL_MaterialDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_ADL_ModelDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ModelDefinition* self
  )
{
  if (self->mesh) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->mesh);
  }
  if (self->material) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->material);
  }
}

static void
Arcadia_ADL_ModelDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ModelDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_ModelDefinition);
  {
    Arcadia_Value definitions, name;
    definitions = Arcadia_ValueStack_getValue(thread, 4);
    name = Arcadia_ValueStack_getValue(thread, 3);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->mesh = Arcadia_ADL_Reference_create(thread, ((Arcadia_ADL_Definition*)self)->definitions,
                                            (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread)));
  self->material = Arcadia_ADL_Reference_create(thread, ((Arcadia_ADL_Definition*)self)->definitions,
                                                (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread)));
  Arcadia_LeaveConstructor(Arcadia_ADL_ModelDefinition);
}

static void
Arcadia_ADL_ModelDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_ModelDefinitionDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = (void (*)(Arcadia_Thread*, Arcadia_ADL_Definition*)) & Arcadia_ADL_ModelDefinition_linkImpl;
}

Arcadia_ADL_ModelDefinition*
Arcadia_ADL_ModelDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_String* meshName,
    Arcadia_String* materialName
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_ModelDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)meshName);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)materialName);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  _Arcadia_EndCreate(Arcadia_ADL_ModelDefinition);
}
