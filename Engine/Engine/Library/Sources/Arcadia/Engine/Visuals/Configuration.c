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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Engine/Visuals/Configuration.h"

static void
Arcadia_Visuals_Configuration_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Configuration* self
  );

static void
Arcadia_Visuals_Configuration_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Configuration* self
  );

static void
Arcadia_Visuals_Configuration_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_ConfigurationDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Visuals_Configuration_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Visuals_Configuration_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Visuals_Configuration_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.Configuration", Arcadia_Visuals_Configuration,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Visuals_Configuration_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Configuration* self
  )
{
  Arcadia_Object_visit(thread, (Arcadia_Object*)self->opengl.version.major);
  Arcadia_Object_visit(thread, (Arcadia_Object*)self->opengl.version.minor);

  Arcadia_Object_visit(thread, (Arcadia_Object*)self->depthBuffer.depthBits);

  Arcadia_Object_visit(thread, (Arcadia_Object*)self->colorBuffer.redBits);
  Arcadia_Object_visit(thread, (Arcadia_Object*)self->colorBuffer.greenBits);
  Arcadia_Object_visit(thread, (Arcadia_Object*)self->colorBuffer.blueBits);
  Arcadia_Object_visit(thread, (Arcadia_Object*)self->colorBuffer.alphaBits);
}

static void
Arcadia_Visuals_Configuration_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Configuration* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Visuals_Configuration);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->opengl.version.major = Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"4", sizeof(u8"4") - 1));
  self->opengl.version.minor = Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"3", sizeof(u8"3") - 1));

  self->depthBuffer.depthBits = Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"24", sizeof(u8"24") - 1));

  self->colorBuffer.redBits = Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"8", sizeof(u8"8") - 1));
  self->colorBuffer.greenBits = Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"8", sizeof(u8"8") - 1));
  self->colorBuffer.blueBits = Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"8", sizeof(u8"8") - 1));
  self->colorBuffer.alphaBits = Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"8", sizeof(u8"8") - 1));

  Arcadia_LeaveConstructor(Arcadia_Visuals_Configuration);
}

static void
Arcadia_Visuals_Configuration_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_ConfigurationDispatch* self
  )
{/*Intentionally empty.*/}

Arcadia_Visuals_Configuration*
Arcadia_Visuals_Configuration_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Visuals_Configuration);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Visuals_Configuration);
}
