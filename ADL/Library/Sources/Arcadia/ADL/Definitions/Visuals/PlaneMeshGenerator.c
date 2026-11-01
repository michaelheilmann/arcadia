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
#include "Arcadia/ADL/Definitions/Visuals/PlaneMeshGenerator.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Reference.h"
#include "Arcadia/ADL/Definitions/Visuals/Include.h"

static void
Arcadia_ADL_PlaneMeshGenerator_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PlaneMeshGenerator* self
  );

static void
Arcadia_ADL_PlaneMeshGenerator_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PlaneMeshGenerator* self
  );

static void
Arcadia_ADL_PlaneMeshGenerator_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PlaneMeshGeneratorDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_PlaneMeshGenerator_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_PlaneMeshGenerator_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_PlaneMeshGenerator_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.PlaneMeshGenerator", Arcadia_ADL_PlaneMeshGenerator,
                          u8"Arcadia.ADL.MeshGenerator", Arcadia_ADL_MeshGenerator,
                          &_typeOperations);

static void
Arcadia_ADL_PlaneMeshGenerator_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PlaneMeshGenerator* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_ADL_PlaneMeshGenerator_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PlaneMeshGenerator* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_PlaneMeshGenerator);
  {
    Arcadia_Value definitions, name, ambientColorName;
    definitions = Arcadia_ValueStack_getValue(thread, 5);
    name = Arcadia_ValueStack_getValue(thread, 4);
    ambientColorName = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushValue(thread, &ambientColorName);
    Arcadia_ValueStack_pushNatural8Value(thread, 3);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (5 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->width = Arcadia_ValueStack_getReal32Value(thread, 3);
  self->height = Arcadia_ValueStack_getReal32Value(thread, 2);
  Arcadia_LeaveConstructor(Arcadia_ADL_PlaneMeshGenerator);
}

static void
Arcadia_ADL_PlaneMeshGenerator_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PlaneMeshGeneratorDispatch* self
  )
{ }

Arcadia_ADL_PlaneMeshGenerator*
Arcadia_ADL_PlaneMeshGenerator_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Real32Value width,
    Arcadia_Real32Value height,
    Arcadia_String* ambientColorName
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_PlaneMeshGenerator);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushReal32Value(thread, width);
  Arcadia_ValueStack_pushReal32Value(thread, height);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)ambientColorName);
  Arcadia_ValueStack_pushNatural8Value(thread, 5);
  _Arcadia_EndCreate(Arcadia_ADL_PlaneMeshGenerator);
}

Arcadia_Real32Value
Arcadia_ADL_PlaneMeshGenerator_getWidth
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PlaneMeshGenerator* self
  )
{ return self->width; }

Arcadia_Real32Value
Arcadia_ADL_PlaneMeshGenerator_getHeight
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PlaneMeshGenerator* self
  )
{ return self->height; }
