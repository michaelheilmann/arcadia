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

#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/FrameBufferResource.h"

#include "Arcadia/Visuals/Implementation/OpenGL4/BackendContext.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/TextureResource.h"
#include <assert.h>

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResourceDispatch* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_activateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_deactivateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_setSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.FrameBufferResource", Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource,
                         u8"Arcadia.Visuals.FrameBufferResource", Arcadia_Engine_Visuals_FrameBufferResource,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource);
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value t;
    t = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ValueStack_pushValue(thread, &t);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }

  self->texture = NULL;
  self->dirty = Arcadia_BooleanValue_True;
  self->width = 320;
  self->height = 240;
  self->frameBufferID = 0;
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResourceDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_FrameBufferResourceDispatch*)self)->activate = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_FrameBufferResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_activateImpl;
  ((Arcadia_Engine_Visuals_FrameBufferResourceDispatch*)self)->deactivate = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_FrameBufferResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_deactivateImpl;
  ((Arcadia_Engine_Visuals_FrameBufferResourceDispatch*)self)->setSize = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_FrameBufferResource*, Arcadia_Integer32Value, Arcadia_Integer32Value)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_setSizeImpl;
  ((Arcadia_Engine_Visuals_FrameBufferResourceDispatch*)self)->getSize = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_FrameBufferResource*, Arcadia_Integer32Value*, Arcadia_Integer32Value*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_setSizeImpl;


  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->load = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_loadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unload = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_unloadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unlink = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_unlinkImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*, Arcadia_Engine_Visuals_EnterPassResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_renderImpl;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  )
{
  assert(NULL == self->texture);
  assert(0 == self->frameBufferID);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  )
{
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
  if (!self->texture) {
    self->texture = (Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource*)Arcadia_Engine_Visuals_BackendContext_createTextureResource(thread, (Arcadia_Engine_Visuals_BackendContext*)context);
    Arcadia_Engine_Visuals_Resource_ref(thread, (Arcadia_Engine_Visuals_Resource*)self->texture);
  }
  if (self->dirty) {
    Arcadia_Engine_Visuals_TextureResource_setWidth(thread, (Arcadia_Engine_Visuals_TextureResource*)self->texture, self->width);
    Arcadia_Engine_Visuals_TextureResource_setHeight(thread, (Arcadia_Engine_Visuals_TextureResource*)self->texture, self->height);
    self->dirty = Arcadia_BooleanValue_False;
  }
  if (!self->frameBufferID) {
    gl->glGenFramebuffers(1, &self->frameBufferID);
    if (gl->glGetError()) {
      return;
    }
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  )
{
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
  if (self->frameBufferID) {
    gl->glDeleteFramebuffers(1, &self->frameBufferID);
    self->frameBufferID = 0;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  )
{
  if (self->texture) {
    ((Arcadia_Engine_Visuals_Resource*)self->texture)->referenceCount--;
    self->texture = NULL;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_activateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  )
{
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
  Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_loadImpl(thread, self);
  gl->glBindFramebuffer(GL_FRAMEBUFFER, self->frameBufferID);
  gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->texture->id, 0);
  if (gl->glGetError()) {
    return;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_deactivateImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self
  )
{
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
  if (self->frameBufferID) {
    gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_setSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  if (self->width != width || self->height != height) {
    self->width = width;
    self->height = height;
    self->dirty = Arcadia_BooleanValue_True;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_getSizeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  *width = self->width;
  *height = self->height;
}

Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource*
Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* backendContext
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource);
}
