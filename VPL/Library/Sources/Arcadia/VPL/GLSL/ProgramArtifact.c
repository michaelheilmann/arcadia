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

#define ARCADIA_VPL_PRIVATE (1)
#include "Arcadia/VPL/GLSL/BackendInternal.h"

static void
bindingConstruct
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Binding* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Backends_GLSL_Binding);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  if (_numberOfArguments) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->name = NULL;
  self->glName = NULL;
  self->type = NULL;
  self->kind = 0;
  self->slot = 0;
  self->location = -1;
  self->blockBinding = -1;
  self->direction = 0;
  self->semantics = 0;
  Arcadia_LeaveConstructor(Arcadia_VPL_Backends_GLSL_Binding);
}

static void
bindingVisit
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_Binding* self
  )
{
  if (self->name) Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  if (self->glName) Arcadia_Object_visit(thread, (Arcadia_Object*)self->glName);
  if (self->type) Arcadia_Object_visit(thread, (Arcadia_Object*)self->type);
}

static const Arcadia_ObjectType_Operations bindingObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&bindingConstruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&bindingVisit,
};

static const Arcadia_Type_Operations bindingOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &bindingObjectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.Backends.GLSL.Binding", Arcadia_VPL_Backends_GLSL_Binding,
                         u8"Arcadia.Object", Arcadia_Object, &bindingOperations);

Arcadia_VPL_Backends_GLSL_Binding*
_Arcadia_VPL_GLSL_newBinding
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Backends_GLSL_Binding);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_VPL_Backends_GLSL_Binding);
}

static void
artifactConstruct
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_ProgramArtifact* self
  )
{
  Arcadia_EnterConstructor(Arcadia_VPL_Backends_GLSL_ProgramArtifact);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  if (_numberOfArguments) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  self->vertexCode = NULL;
  self->fragmentCode = NULL;
  self->uniforms = NULL;
  self->vertexInterface = NULL;
  self->fragmentInterface = NULL;
  self->fragmentColorOutput = NULL;
  self->fragmentColorLocation = -1;
  self->version = 420;
  Arcadia_LeaveConstructor(Arcadia_VPL_Backends_GLSL_ProgramArtifact);
}

static void
artifactVisit
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Backends_GLSL_ProgramArtifact* self
  )
{
  if (self->vertexCode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->vertexCode);
  if (self->fragmentCode) Arcadia_Object_visit(thread, (Arcadia_Object*)self->fragmentCode);
  if (self->uniforms) Arcadia_Object_visit(thread, (Arcadia_Object*)self->uniforms);
  if (self->vertexInterface) Arcadia_Object_visit(thread, (Arcadia_Object*)self->vertexInterface);
  if (self->fragmentInterface) Arcadia_Object_visit(thread, (Arcadia_Object*)self->fragmentInterface);
  if (self->fragmentColorOutput) Arcadia_Object_visit(thread, (Arcadia_Object*)self->fragmentColorOutput);
}

static const Arcadia_ObjectType_Operations artifactObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&artifactConstruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&artifactVisit,
};

static const Arcadia_Type_Operations artifactOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &artifactObjectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.VPL.Backends.GLSL.ProgramArtifact", Arcadia_VPL_Backends_GLSL_ProgramArtifact,
                         u8"Arcadia.Object", Arcadia_Object, &artifactOperations);

Arcadia_VPL_Backends_GLSL_ProgramArtifact*
_Arcadia_VPL_GLSL_newArtifact
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_VPL_Backends_GLSL_ProgramArtifact);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_VPL_Backends_GLSL_ProgramArtifact);
}
