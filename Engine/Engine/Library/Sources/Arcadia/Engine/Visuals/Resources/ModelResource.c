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
#include "Arcadia/Engine/Visuals/Resources/ModelResource.h"

#include "Arcadia/Engine/Visuals/Resources/ConstantBufferResource.h"
#include "Arcadia/Engine/Visuals/Resources/VertexBufferResource.h"
#include "Arcadia/Engine/Visuals/BackendContext.h"
#include <assert.h>

static void
Arcadia_Engine_Visuals_ModelResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self
  );

static void
Arcadia_Engine_Visuals_ModelResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self
  );

static void
Arcadia_Engine_Visuals_ModelResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResourceDispatch* self
  );

static void
Arcadia_Engine_Visuals_ModelResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self
  );

static void
Arcadia_Engine_Visuals_ModelResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self
  );

static void
Arcadia_Engine_Visuals_ModelResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self
  );

static void
Arcadia_Engine_Visuals_ModelResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self
  );

static void
Arcadia_Engine_Visuals_ModelResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_ModelResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_ModelResource_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_ModelResource_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_ModelResource_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.ModelResource", Arcadia_Engine_Visuals_ModelResource,
                         u8"Arcadia.Visuals.Resource", Arcadia_Engine_Visuals_Resource,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_ModelResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_ModelResource);
  if (4 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value t;
    t = Arcadia_ValueStack_getValue(thread, 4);
    Arcadia_ValueStack_pushValue(thread, &t);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  self->constantBuffer = NULL;
  self->meshConstantBuffer = NULL;
  self->meshVertexBuffer = NULL;
  self->material = NULL;
  //
  self->dirty = Arcadia_Engine_Visuals_ModelResource_LocalToWorldMatrixDirty;
  //
  self->localToWorldMatrix = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Math_Matrix4Real32_setIdentity(thread, self->localToWorldMatrix);
  //
  Arcadia_Engine_Visuals_BackendContext* backendContext =
    Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 4, _Arcadia_Engine_Visuals_BackendContext_getType(thread));


  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_Object* o;

    o = (Arcadia_Object*)Arcadia_Engine_Visuals_BackendContext_createConstantBufferResource(thread, (Arcadia_Engine_Visuals_BackendContext*)backendContext);
    Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)o);
    self->constantBuffer = (Arcadia_Engine_Visuals_ConstantBufferResource*)o;

    o = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_Engine_Visuals_ConstantBufferResource_getType(thread));
    Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)o);
    self->meshConstantBuffer = (Arcadia_Engine_Visuals_ConstantBufferResource*)o;

    o = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Engine_Visuals_VertexBufferResource_getType(thread));
    Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)o);
    self->meshVertexBuffer = (Arcadia_Engine_Visuals_VertexBufferResource*)o;

    o = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Engine_Visuals_MaterialResource_getType(thread));
    Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)o);
    self->material = (Arcadia_Engine_Visuals_MaterialResource*)o;

    Arcadia_Thread_popJumpTarget(thread);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (self->material) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->material);
      self->material = NULL;
    }
    if (self->meshVertexBuffer) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->meshVertexBuffer);
      self->meshVertexBuffer = NULL;
    }
    if (self->meshConstantBuffer) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->meshConstantBuffer);
      self->meshConstantBuffer = NULL;
    }
    if (self->constantBuffer) {
      Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->constantBuffer);
      self->constantBuffer = NULL;
    }
    Arcadia_Thread_jump(thread);
  }
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_ModelResource);
}

static void
Arcadia_Engine_Visuals_ModelResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self
  )
{
  assert(NULL == self->constantBuffer);
  assert(NULL == self->meshConstantBuffer);
  assert(NULL == self->meshVertexBuffer);
  assert(NULL == self->material);
}

static void
Arcadia_Engine_Visuals_ModelResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResourceDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->load = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_ModelResource_loadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unload = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_ModelResource_unloadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unlink = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_ModelResource_unlinkImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*, Arcadia_Engine_Visuals_EnterPassResource*)) & Arcadia_Engine_Visuals_ModelResource_renderImpl;
}

static void
Arcadia_Engine_Visuals_ModelResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self
  )
{
  if (self->localToWorldMatrix) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->localToWorldMatrix);
  }
}

static void
Arcadia_Engine_Visuals_ModelResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self
  )
{
  Arcadia_Engine_Visuals_Resource_load(thread, (Arcadia_Engine_Visuals_Resource*)self->meshConstantBuffer);
  Arcadia_Engine_Visuals_Resource_load(thread, (Arcadia_Engine_Visuals_Resource*)self->meshVertexBuffer);
  Arcadia_Engine_Visuals_Resource_load(thread, (Arcadia_Engine_Visuals_Resource*)self->material);
}

static void
Arcadia_Engine_Visuals_ModelResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_ModelResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self
  )
{
  if (self->constantBuffer) {
    Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->constantBuffer);
    self->constantBuffer = NULL;
  }
  if (self->meshConstantBuffer) {
    Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->meshConstantBuffer);
    self->meshConstantBuffer = NULL;
  }
  if (self->meshVertexBuffer) {
    Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->meshVertexBuffer);
    self->meshVertexBuffer = NULL;
  }
  if (self->material) {
    Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)self->material);
    self->material = NULL;
  }
  ((Arcadia_Engine_Visuals_Resource*)self)->context = NULL;
}

static void
Arcadia_Engine_Visuals_ModelResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  )
{ }

void
Arcadia_Engine_Visuals_ModelResource_setLocalToWorldMatrix
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ModelResource* self,
    Arcadia_Math_Matrix4Real32* localToWorldMatrix
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_ModelResource, setLocalToWorldMatrix, self, localToWorldMatrix); }
