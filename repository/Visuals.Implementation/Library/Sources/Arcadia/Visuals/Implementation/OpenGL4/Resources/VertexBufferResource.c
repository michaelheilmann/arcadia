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

#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/VertexBufferResource.h"

#include "Arcadia/Visuals/Implementation/OpenGL4/BackendContext.h"
#include <assert.h>

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResourceDispatch* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.VertexBufferResource", Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource,
                         u8"Arcadia.Visuals.VertexBufferResource", Arcadia_Engine_Visuals_VertexBufferResource,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource);
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

  self->vertexArrayID = 0;
  self->vertexBufferID = 0;

  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResourceDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->load = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_loadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unload = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_unloadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unlink = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_unlinkImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*, Arcadia_Engine_Visuals_EnterPassResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_renderImpl;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self
  )
{
  assert(0 == self->vertexArrayID);
  assert(0 == self->vertexBufferID);
  assert(NULL == ((Arcadia_Engine_Visuals_Resource*)self)->context);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self
  )
{
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);

  if (0 == self->vertexBufferID) {
    // (1) clear errors
    while (gl->glGetError()) {
    }
    // (2) create buffer
    gl->glGenBuffers(1, &self->vertexBufferID);
    if (GL_NO_ERROR != gl->glGetError()) {
      return;
    }
  }

  if (((Arcadia_Engine_Visuals_VertexBufferResource*)self)->dirty & Arcadia_Engine_Visuals_VertexBufferResource_VertexDataDirty) {
    // (1) bind the buffer
    gl->glBindBuffer(GL_ARRAY_BUFFER, self->vertexBufferID);
    if (GL_NO_ERROR != gl->glGetError()) {
      return;
    }
    // (2) upload the data
    gl->glBufferData(GL_ARRAY_BUFFER, ((Arcadia_Engine_Visuals_VertexBufferResource*)self)->numberOfBytes,
                                      ((Arcadia_Engine_Visuals_VertexBufferResource*)self)->bytes, GL_STATIC_DRAW);
    if (GL_NO_ERROR != gl->glGetError()) {
      return;
    }
    ((Arcadia_Engine_Visuals_VertexBufferResource*)self)->dirty &= ~Arcadia_Engine_Visuals_VertexBufferResource_VertexDataDirty;
  }

  if (0 == self->vertexArrayID) {
    // (1) clear errors
    while (gl->glGetError()) {
    }
    // (2) create vertex array object
    gl->glGenVertexArrays(1, &self->vertexArrayID);
    if (GL_NO_ERROR != gl->glGetError()) {
      return;
    }
  }
  if (((Arcadia_Engine_Visuals_VertexBufferResource*)self)->dirty & Arcadia_Engine_Visuals_VertexBufferResource_VertexDescriptorDirty) {
    Arcadia_SizeValue stride = ((Arcadia_Engine_Visuals_VertexBufferResource*)self)->vertexDescriptor->stride;
    // (1) bind the vertex array
    gl->glBindVertexArray(self->vertexArrayID);
    if (GL_NO_ERROR != gl->glGetError()) {
      return;
    }
    // (2) specify the vertex element elements and the buffer they are originated from.
    for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)((Arcadia_Engine_Visuals_VertexBufferResource*)self)->vertexDescriptor->vertexElementDescriptors); i < n; ++i) {
      Arcadia_Media_VertexElementDescriptor* vertexElementDescriptor = (Arcadia_Media_VertexElementDescriptor*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)((Arcadia_Engine_Visuals_VertexBufferResource*)self)->vertexDescriptor->vertexElementDescriptors, i, _Arcadia_Media_VertexElementDescriptor_getType(thread));
      GLint glType;
      GLsizei glSize;
      switch (vertexElementDescriptor->syntactics) {
        case Arcadia_Media_VertexElementSyntactics_Real32: {
          glType = GL_FLOAT;
          glSize = 1;
        } break;
        case Arcadia_Media_VertexElementSyntactics_Real32Real32: {
          glType = GL_FLOAT;
          glSize = 2;
        } break;
        case Arcadia_Media_VertexElementSyntactics_Real32Real32Real32: {
          glType = GL_FLOAT;
          glSize = 3;
        } break;
        case Arcadia_Media_VertexElementSyntactics_Real32Real32Real32Real32: {
          glType = GL_FLOAT;
          glSize = 4;
        } break;
        default: {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
          Arcadia_Thread_jump(thread);
        } break;
      };
      // (2.1) enable the vertex attribute array.
      gl->glEnableVertexAttribArray(i);
      if (GL_NO_ERROR != gl->glGetError()) {
        return;
      }
      // (2.2) specify the vertex attribute format of that vertex attribute array.
      gl->glVertexAttribFormat(i, glSize, glType, GL_FALSE, vertexElementDescriptor->offset);
      if (GL_NO_ERROR != gl->glGetError()) {
        return;
      }
      // (2.3) specify the vertex buffer for that vertex attribute array.
      gl->glBindVertexBuffer(i, self->vertexBufferID, 0, stride);
      if (GL_NO_ERROR != gl->glGetError()) {
        return;
      }
    }
    ((Arcadia_Engine_Visuals_VertexBufferResource*)self)->dirty &= ~Arcadia_Engine_Visuals_VertexBufferResource_VertexDescriptorDirty;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self
  )
{
  if (self->vertexArrayID) {
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
    gl->glDeleteVertexArrays(1, &self->vertexArrayID);
    self->vertexArrayID = 0;
  }
  if (self->vertexBufferID) {
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
    _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
    gl->glDeleteBuffers(1, &self->vertexBufferID);
    self->vertexBufferID = 0;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self
  )
{
  assert(0 == self->vertexArrayID);
  assert(0 == self->vertexBufferID);
  ((Arcadia_Engine_Visuals_Resource*)self)->context = NULL;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  )
{
  Arcadia_Engine_Visuals_Resource_load(thread, (Arcadia_Engine_Visuals_Resource*)self);
}

Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource*
Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* backendContext
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_VertexBufferResource);
}
