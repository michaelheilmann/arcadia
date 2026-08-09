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

#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/ConstantBufferResource.h"

#include "Arcadia/Visuals/Implementation/OpenGL4/BackendContext.h"
#include "Arcadia/Visuals/Implementation/OpenGL4/BackendIncludes.h"
#include <assert.h>

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResourceDispatch* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_setDataImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self,
    const void* bytes,
    Arcadia_SizeValue numberOfBytes
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_writeColor4Real32Impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self,
    Arcadia_Math_Color4Real32 const* source
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_writeMatrix4x4Real32Impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self,
    Arcadia_BooleanValue transpose,
    Arcadia_Math_Matrix4Real32 const* source
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.ConstantBufferResource", Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource,
                         u8"Arcadia.Visuals.ConstantBufferResource", Arcadia_Engine_Visuals_ConstantBufferResource,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource);
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
  self->dirty = Arcadia_BooleanValue_True;
  self->byteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  self->bufferID = 0;
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResourceDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->load = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_loadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unload = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_unloadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unlink = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_unlinkImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*, Arcadia_Engine_Visuals_EnterPassResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_renderImpl;

  ((Arcadia_Engine_Visuals_ConstantBufferResourceDispatch*)self)->clear = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ConstantBufferResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_clearImpl;
  ((Arcadia_Engine_Visuals_ConstantBufferResourceDispatch*)self)->writeColor4Real32 = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ConstantBufferResource*, Arcadia_Math_Color4Real32 const*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_writeColor4Real32Impl;
  ((Arcadia_Engine_Visuals_ConstantBufferResourceDispatch*)self)->writeMatrix4x4Real32 = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ConstantBufferResource*, Arcadia_BooleanValue, Arcadia_Math_Matrix4Real32 const*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_writeMatrix4x4Real32Impl;
  ((Arcadia_Engine_Visuals_ConstantBufferResourceDispatch*)self)->setData = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_ConstantBufferResource*, const void*, Arcadia_SizeValue)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_setDataImpl;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  )
{
  assert(0 == self->bufferID);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  )
{
  if (self->byteBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->byteBuffer);
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  )
{
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);

  if (0 == self->bufferID) {
    gl->glGenBuffers(1, &self->bufferID);
  }
  if (self->dirty) {
    gl->glBindBuffer(GL_UNIFORM_BUFFER, self->bufferID);
    gl->glBufferData(GL_UNIFORM_BUFFER, Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, self->byteBuffer), Arcadia_ByteArrayBuilder_getBytes(thread, self->byteBuffer), GL_STATIC_DRAW);
    if (gl->glGetError()) {
      return;
    }
    self->dirty = Arcadia_BooleanValue_False;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  )
{
  if (self->bufferID) {
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
    gl->glDeleteBuffers(1, &self->bufferID);
    self->bufferID = 0;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  )
{
  assert(0 == self->bufferID);
  ((Arcadia_Engine_Visuals_Resource*)self)->context = NULL;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  )
{
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);

  if (0 == self->bufferID) {
    gl->glGenBuffers(1, &self->bufferID);
  }
  if (self->dirty) {
    gl->glBindBuffer(GL_UNIFORM_BUFFER, self->bufferID);
    gl->glBufferData(GL_UNIFORM_BUFFER, Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, self->byteBuffer), Arcadia_ByteArrayBuilder_getBytes(thread, self->byteBuffer), GL_STATIC_DRAW);
    if (gl->glGetError()) {
      return;
    }
    self->dirty = Arcadia_BooleanValue_False;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_setDataImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self,
    const void* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  Arcadia_ByteArrayBuilder_clear(thread, self->byteBuffer);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, self->byteBuffer, bytes, numberOfBytes);
  self->dirty = Arcadia_BooleanValue_True;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_clearImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self
  )
{ Arcadia_ByteArrayBuilder_clear(thread, self->byteBuffer); self->dirty = Arcadia_BooleanValue_True; }

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_writeColor4Real32Impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self,
    Arcadia_Math_Color4Real32 const* source
  )
{
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, self->byteBuffer, &(source->components[0]), sizeof(float) * 4);
}

static inline void
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_writeMatrix4x4Real32Impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource* self,
    Arcadia_BooleanValue transpose,
    Arcadia_Math_Matrix4Real32 const* source
  )
{
  if (transpose) {
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        Arcadia_ByteArrayBuilder_insertBackBytes(thread, self->byteBuffer, &(source->elements[j][i]), sizeof(float));
      }
    }
  } else {
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        Arcadia_ByteArrayBuilder_insertBackBytes(thread, self->byteBuffer, &(source->elements[i][j]), sizeof(float));
      }
    }
  }
  self->dirty = Arcadia_BooleanValue_True;
}

Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource*
Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* backendContext
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_ConstantBufferResource);
}
