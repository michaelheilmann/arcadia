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

#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/EnterPassResource.h"

#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/FrameBufferResource.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/TextureResource.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/BackendContext.h"

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResourceDispatch* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* enterPassResource
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_setTargetFrameBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* frameBuffer
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.EnterPassResource", Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource,
                         u8"Arcadia.Visuals.EnterPassResource", Arcadia_Engine_Visuals_EnterPassResource,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource);
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
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResourceDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->load = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_loadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unload = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_unloadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unlink = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_unlinkImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*, Arcadia_Engine_Visuals_EnterPassResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_renderImpl;
  ((Arcadia_Engine_Visuals_EnterPassResourceDispatch*)self)->setTargetFrameBuffer = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_EnterPassResource*, Arcadia_Engine_Visuals_FrameBufferResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_setTargetFrameBufferImpl;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self
  )
{
  static const Arcadia_Natural8Value matrixMask = Arcadia_Engine_Visuals_EnterPassResource_ViewToProjectionMatrixDirty
                                                | Arcadia_Engine_Visuals_EnterPassResource_WorldToViewMatrixDirty
                                                ;
  if (((Arcadia_Engine_Visuals_EnterPassResource*)self)->dirty & matrixMask) {
    Arcadia_Engine_Visuals_ConstantBufferResource_clear(thread, (Arcadia_Engine_Visuals_ConstantBufferResource*)((Arcadia_Engine_Visuals_EnterPassResource*)self)->viewerConstantBuffer);
    Arcadia_Engine_Visuals_ConstantBufferResource_writeMatrix4x4Real32(thread, (Arcadia_Engine_Visuals_ConstantBufferResource*)((Arcadia_Engine_Visuals_EnterPassResource*)self)->viewerConstantBuffer, Arcadia_BooleanValue_True, ((Arcadia_Engine_Visuals_EnterPassResource*)self)->viewToProjectionMatrix);
    Arcadia_Engine_Visuals_ConstantBufferResource_writeMatrix4x4Real32(thread, (Arcadia_Engine_Visuals_ConstantBufferResource*)((Arcadia_Engine_Visuals_EnterPassResource*)self)->viewerConstantBuffer, Arcadia_BooleanValue_True, ((Arcadia_Engine_Visuals_EnterPassResource*)self)->worldToViewMatrix);
    ((Arcadia_Engine_Visuals_EnterPassResource*)self)->dirty &= ~matrixMask;
  }
  Arcadia_Engine_Visuals_Resource_load(thread, (Arcadia_Engine_Visuals_Resource*)((Arcadia_Engine_Visuals_EnterPassResource*)self)->viewerConstantBuffer);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self
  )
{
  if (((Arcadia_Engine_Visuals_EnterPassResource*)self)->viewerConstantBuffer) {
    Arcadia_Engine_Visuals_Resource_unref(thread, (Arcadia_Engine_Visuals_Resource*)((Arcadia_Engine_Visuals_EnterPassResource*)self)->viewerConstantBuffer);
    ((Arcadia_Engine_Visuals_EnterPassResource*)self)->viewerConstantBuffer = NULL;
  }
  ((Arcadia_Engine_Visuals_Resource*)self)->context = NULL;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* enterPassResource
  )
{
  static const Arcadia_Natural8Value matrixMask = Arcadia_Engine_Visuals_EnterPassResource_ViewToProjectionMatrixDirty
                                                | Arcadia_Engine_Visuals_EnterPassResource_WorldToViewMatrixDirty
                                                ;

  if (((Arcadia_Engine_Visuals_EnterPassResource*)self)->dirty & matrixMask) {
    Arcadia_Engine_Visuals_ConstantBufferResource_clear(thread, (Arcadia_Engine_Visuals_ConstantBufferResource*)((Arcadia_Engine_Visuals_EnterPassResource*)self)->viewerConstantBuffer);
    Arcadia_Engine_Visuals_ConstantBufferResource_writeMatrix4x4Real32(thread, (Arcadia_Engine_Visuals_ConstantBufferResource*)((Arcadia_Engine_Visuals_EnterPassResource*)self)->viewerConstantBuffer, Arcadia_BooleanValue_True, ((Arcadia_Engine_Visuals_EnterPassResource*)self)->viewToProjectionMatrix);
    Arcadia_Engine_Visuals_ConstantBufferResource_writeMatrix4x4Real32(thread, (Arcadia_Engine_Visuals_ConstantBufferResource*)((Arcadia_Engine_Visuals_EnterPassResource*)self)->viewerConstantBuffer, Arcadia_BooleanValue_True, ((Arcadia_Engine_Visuals_EnterPassResource*)self)->worldToViewMatrix);
    ((Arcadia_Engine_Visuals_EnterPassResource*)self)->dirty &= ~matrixMask;
  }
  Arcadia_Engine_Visuals_Resource_load(thread, (Arcadia_Engine_Visuals_Resource*)((Arcadia_Engine_Visuals_EnterPassResource*)self)->viewerConstantBuffer);

  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
  Arcadia_Real32Value l = ((Arcadia_Engine_Visuals_EnterPassResource*)self)->canvasSize.width * ((Arcadia_Engine_Visuals_EnterPassResource*)self)->relativeViewportRectangle.left,
                      b = ((Arcadia_Engine_Visuals_EnterPassResource*)self)->canvasSize.height * ((Arcadia_Engine_Visuals_EnterPassResource*)self)->relativeViewportRectangle.bottom,
                      w = ((Arcadia_Engine_Visuals_EnterPassResource*)self)->canvasSize.width * ((Arcadia_Engine_Visuals_EnterPassResource*)self)->relativeViewportRectangle.right
                        - ((Arcadia_Engine_Visuals_EnterPassResource*)self)->canvasSize.width * ((Arcadia_Engine_Visuals_EnterPassResource*)self)->relativeViewportRectangle.left,
                      h = ((Arcadia_Engine_Visuals_EnterPassResource*)self)->canvasSize.height * ((Arcadia_Engine_Visuals_EnterPassResource*)self)->relativeViewportRectangle.top
                        - ((Arcadia_Engine_Visuals_EnterPassResource*)self)->canvasSize.height * ((Arcadia_Engine_Visuals_EnterPassResource*)self)->relativeViewportRectangle.bottom;
  gl->glViewport(l, b, w, h);
  gl->glEnable(GL_SCISSOR_TEST);
  gl->glScissor(l, b, w, h);

  if (((Arcadia_Engine_Visuals_EnterPassResource*)self)->clearDepthBuffer) {
    gl->glClearDepth(((Arcadia_Engine_Visuals_EnterPassResource*)self)->clearDepth);
    gl->glClear(GL_DEPTH_BUFFER_BIT);
  }
  if (((Arcadia_Engine_Visuals_EnterPassResource*)self)->clearColorBuffer) {
    gl->glClearColor(((Arcadia_Engine_Visuals_EnterPassResource*)self)->clearColor->components[0],
                     ((Arcadia_Engine_Visuals_EnterPassResource*)self)->clearColor->components[1],
                     ((Arcadia_Engine_Visuals_EnterPassResource*)self)->clearColor->components[2],
                     ((Arcadia_Engine_Visuals_EnterPassResource*)self)->clearColor->components[3]);
    gl->glClear(GL_COLOR_BUFFER_BIT);
  }

}

Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource*
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* backendContext
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource_setTargetFrameBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_EnterPassResource* self,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_FrameBufferResource* frameBuffer
  )
{
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
  if (frameBuffer) {
    gl->glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->frameBufferID);
    gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBuffer->texture->id, 0);
    if (gl->glGetError()) {
      return;
    }
  } else {
    gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}
