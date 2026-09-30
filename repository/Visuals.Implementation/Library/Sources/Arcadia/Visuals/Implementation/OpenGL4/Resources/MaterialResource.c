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

#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/MaterialResource.h"

#include "Arcadia/Visuals/Implementation/OpenGL4/BackendContext.h"

#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/ConstantBufferResource.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/ProgramResource.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/TextureResource.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/VertexBufferResource.h"

#include <assert.h>

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResourceDispatch* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.MaterialResource", Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource,
                         u8"Arcadia.Visuals.MaterialResource", Arcadia_Engine_Visuals_MaterialResource,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource);
  if (7 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value t;
    t = Arcadia_ValueStack_getValue(thread, 7);
    Arcadia_ValueStack_pushValue(thread, &t);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }

  self->ambientColorTexture = NULL;
  self->program = NULL;

  self->blendEnabled = Arcadia_ValueStack_getBooleanValue(thread, 6);
  Arcadia_EnumerationValue blendSourceFunction = Arcadia_ValueStack_getEnumerationValue(thread, 5);
  Arcadia_EnumerationValue blendDestinationFunction = Arcadia_ValueStack_getEnumerationValue(thread, 4);
  if (blendSourceFunction.type != _Arcadia_Engine_Visuals_BlendFunction_getType(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (blendDestinationFunction.type != _Arcadia_Engine_Visuals_BlendFunction_getType(thread)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->blendSourceFunction = (Arcadia_Engine_Visuals_BlendFunction)blendSourceFunction.value;
  self->blendDestinationFunction = (Arcadia_Engine_Visuals_BlendFunction)blendDestinationFunction.value;

  self->ambientColorSource = Arcadia_ValueStack_getInteger32Value(thread, 3);

  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_Object* o;

    o = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getType(thread));
    Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)o);
    self->ambientColorTexture = (Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource*)o;

    o = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource_getType(thread));
    Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)o);
    self->program = (Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource*)o;

   Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (self->program) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->program);
      self->program = NULL;
    }
    if (self->ambientColorTexture) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->ambientColorTexture);
      self->ambientColorTexture = NULL;
    }
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResourceDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->load = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_loadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unload = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_unloadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unlink = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_unlinkImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*, Arcadia_Engine_Visuals_EnterPassResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_renderImpl;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self
  )
{
  assert(NULL == self->ambientColorTexture);
  assert(NULL == self->program);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self
  )
{
  Arcadia_Engine_Visuals_Resource_load(thread, (Arcadia_Engine_Visuals_Resource*)self->ambientColorTexture);
  Arcadia_Engine_Visuals_Resource_load(thread, (Arcadia_Engine_Visuals_Resource*)self->program);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self
  )
{
  if (self->ambientColorTexture) {
    Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->ambientColorTexture);
    self->ambientColorTexture = NULL;
  }
  if (self->program) {
    Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->program);
    self->program = NULL;
  }
  ((Arcadia_Engine_Visuals_Resource*)self)->context = NULL;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  )
{
  Arcadia_Engine_Visuals_Resource_load(thread, (Arcadia_Engine_Visuals_Resource*)self->ambientColorTexture);
  Arcadia_Engine_Visuals_Resource_load(thread, (Arcadia_Engine_Visuals_Resource*)self->program);
}

Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource*
Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* backendContext,
    Arcadia_BooleanValue blendEnabled,
    Arcadia_Engine_Visuals_BlendFunction blendSourceFunction,
    Arcadia_Engine_Visuals_BlendFunction blendDestinationFunction,
    Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource ambientColorSource,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* ambientColorTexture,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ProgramResource* program
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushBooleanValue(thread, blendEnabled);
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_BlendFunction_getType(thread), blendSourceFunction));
  Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_BlendFunction_getType(thread), blendDestinationFunction));
  Arcadia_ValueStack_pushInteger32Value(thread, ambientColorSource);
  if (ambientColorTexture) Arcadia_ValueStack_pushObjectReferenceValue(thread, ambientColorTexture); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (program) Arcadia_ValueStack_pushObjectReferenceValue(thread, program); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 7);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_MaterialResource);
}
