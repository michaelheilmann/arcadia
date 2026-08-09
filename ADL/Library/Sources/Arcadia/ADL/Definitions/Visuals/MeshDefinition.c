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
#include "Arcadia/ADL/Definitions/Visuals/MeshDefinition.h"

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Reader.module.h"
#include "Arcadia/ADL/Reference.h"
#include "Arcadia/ADL/Definitions/Visuals/Include.h"

static void
Arcadia_ADL_MeshDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  );

static void
Arcadia_ADL_MeshDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  );

static void
Arcadia_ADL_MeshDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  );

static void
Arcadia_ADL_MeshDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinitionDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ADL_MeshDefinition_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ADL_MeshDefinition_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ADL_MeshDefinition_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ADL.MeshDefinition", Arcadia_ADL_MeshDefinition,
                         u8"Arcadia.ADL.Definition", Arcadia_ADL_Definition,
                         &_typeOperations);

static void
Arcadia_ADL_MeshDefinition_linkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  )
{
  Arcadia_ADL_Reference_resolve(thread, self->ambientColor);
  if (!Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self->ambientColor->definition, _Arcadia_ADL_ColorDefinition_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static void
Arcadia_ADL_MeshDefinition_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  )
{
  if (self->vertexPositions) {
    Arcadia_RuntimeByteArray_visit(thread, self->vertexPositions);
  }
  if (self->vertexAmbientColors) {
    Arcadia_RuntimeByteArray_visit(thread, self->vertexAmbientColors);
  }
  if (self->ambientColor) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->ambientColor);
  }
}

static void
Arcadia_ADL_MeshDefinition_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ADL_MeshDefinition);
  {
    Arcadia_Value definitions, name;
    definitions = Arcadia_ValueStack_getValue(thread, 7);
    name = Arcadia_ValueStack_getValue(thread, 6);
    Arcadia_ValueStack_pushValue(thread, &definitions);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (7 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->numberOfVertices = Arcadia_ValueStack_getSizeValue(thread, 5);
  self->vertexPositions = Arcadia_ValueStack_getRuntimeByteArrayValue(thread, 4);
  self->vertexAmbientColors = Arcadia_ValueStack_getRuntimeByteArrayValue(thread, 3);
  self->vertexAmbientTextureCoordinates = Arcadia_ValueStack_getRuntimeByteArrayValue(thread, 2);
  self->ambientColor = Arcadia_ADL_Reference_create(thread, ((Arcadia_ADL_Definition*)self)->definitions,
                                                            (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread)));
  Arcadia_LeaveConstructor(Arcadia_ADL_MeshDefinition);
}

static void
Arcadia_ADL_MeshDefinition_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinitionDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = (void (*)(Arcadia_Thread*, Arcadia_ADL_Definition*)) & Arcadia_ADL_MeshDefinition_linkImpl;
}

Arcadia_ADL_MeshDefinition*
Arcadia_ADL_MeshDefinition_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_SizeValue numberOfVertices,
    Arcadia_RuntimeByteArray* vertexPositions,
    Arcadia_RuntimeByteArray* vertexAmbientColors,
    Arcadia_RuntimeByteArray* vertexAmbientTextureCoordinates,
    Arcadia_String* ambientColorName
  )
{
  _Arcadia_BeginCreate(Arcadia_ADL_MeshDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_ValueStack_pushSizeValue(thread, numberOfVertices);
  Arcadia_ValueStack_pushRuntimeByteArrayValue(thread, vertexPositions);
  Arcadia_ValueStack_pushRuntimeByteArrayValue(thread, vertexAmbientColors);
  Arcadia_ValueStack_pushRuntimeByteArrayValue(thread, vertexAmbientTextureCoordinates);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)ambientColorName);
  Arcadia_ValueStack_pushNatural8Value(thread, 7);
  _Arcadia_EndCreate(Arcadia_ADL_MeshDefinition);
}

Arcadia_SizeValue
Arcadia_ADL_MeshDefinition_getNumberOfVertices
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  )
{ return self->numberOfVertices; }

Arcadia_RuntimeByteArray*
Arcadia_ADL_MeshDefinition_getVertexPositions
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  )
{ return self->vertexPositions; }

Arcadia_RuntimeByteArray*
Arcadia_ADL_MeshDefinition_getVertexAmbientColors
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  )
{ return self->vertexAmbientColors; }

Arcadia_RuntimeByteArray*
Arcadia_ADL_MeshDefinition_getVertexAmbientTextureCoordinates
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_MeshDefinition* self
  )
{ return self->vertexAmbientTextureCoordinates; }
