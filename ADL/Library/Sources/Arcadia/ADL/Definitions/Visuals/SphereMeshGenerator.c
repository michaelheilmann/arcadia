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
#include "Arcadia/ADL/Definitions/Visuals/SphereMeshGenerator.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Reference.h"
#include "Arcadia/ADL/Definitions/Visuals/Include.h"

static void
Arcadia_ADL_SphereMeshGenerator_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGenerator* self
  );

static void
Arcadia_ADL_SphereMeshGenerator_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGenerator* self
  );

static void
Arcadia_ADL_SphereMeshGenerator_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGeneratorDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_SphereMeshGenerator_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_SphereMeshGenerator_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_SphereMeshGenerator_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.SphereMeshGenerator", Arcadia_ADL_SphereMeshGenerator,
                          u8"Arcadia.ADL.MeshGenerator", Arcadia_ADL_MeshGenerator,
                          &_typeOperations);

static void
Arcadia_ADL_SphereMeshGenerator_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGenerator* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_ADL_SphereMeshGenerator_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGenerator* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_SphereMeshGenerator);
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
  self->radius = Arcadia_ValueStack_getReal32Value(thread, 3);
  self->segments = Arcadia_ValueStack_getNatural32Value(thread, 2);
  Arcadia_LeaveConstructor(Arcadia_ADL_SphereMeshGenerator);
}

static void
Arcadia_ADL_SphereMeshGenerator_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGeneratorDispatch* self
  )
{ }

Arcadia_ADL_SphereMeshGenerator*
Arcadia_ADL_SphereMeshGenerator_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Real32Value radius,
    Arcadia_Natural32Value segments,
    Arcadia_String* ambientColorName
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_SphereMeshGenerator);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushReal32Value(thread, radius);
  Arcadia_ValueStack_pushNatural32Value(thread, segments);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)ambientColorName);
  Arcadia_ValueStack_pushNatural8Value(thread, 5);
  _Arcadia_EndCreate(Arcadia_ADL_SphereMeshGenerator);
}

Arcadia_Real32Value
Arcadia_ADL_SphereMeshGenerator_getRadius
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGenerator* self
  )
{ return self->radius; }

Arcadia_Natural32Value
Arcadia_ADL_SphereMeshGenerator_getSegments
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_SphereMeshGenerator* self
  )
{ return self->segments; }
