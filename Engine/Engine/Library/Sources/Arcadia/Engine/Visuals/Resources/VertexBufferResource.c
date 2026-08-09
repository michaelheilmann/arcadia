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
#include "Arcadia/Engine/Visuals/Resources/VertexBufferResource.h"

#include "Arcadia/Engine/Visuals/BackendContext.h"
#include <assert.h>

static void
Arcadia_Engine_Visuals_VertexBufferResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  );

static void
Arcadia_Engine_Visuals_VertexBufferResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResourceDispatch* self
  );

static void
Arcadia_Engine_Visuals_VertexBufferResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  );

static void
Arcadia_Engine_Visuals_VertexBufferResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  );

static Arcadia_SizeValue
Arcadia_Engine_Visuals_VertexBufferResource_getNumberOfVerticesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  );

static Arcadia_Media_VertexDescriptor*
Arcadia_Engine_Visuals_VertexBufferResource_getVertexDescriptorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  );

static void
Arcadia_Engine_Visuals_VertexBufferResource_setDataImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self,
    Arcadia_Media_VertexDescriptor* vertexDescriptor,
    Arcadia_SizeValue numberOfVertices,
    const void* bytes,
    Arcadia_SizeValue numberOfBytes
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_VertexBufferResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_VertexBufferResource_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_VertexBufferResource_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_VertexBufferResource_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.VertexBufferResource", Arcadia_Engine_Visuals_VertexBufferResource,
                         u8"Arcadia.Visuals.Resource", Arcadia_Engine_Visuals_Resource,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_VertexBufferResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_VertexBufferResource);
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
  //
  Arcadia_Media_VertexDescriptorBuilder* vertexDescriptorBuilder = Arcadia_Media_VertexDescriptorBuilder_create(thread);
  self->vertexDescriptor = Arcadia_Media_VertexDescriptorBuilder_build(thread, vertexDescriptorBuilder);
  //
  self->numberOfVertices = 0;
  self->numberOfBytes = 0;
  self->bytes = Arcadia_Memory_allocateUnmanaged(thread, 0);
  //
  self->dirty = Arcadia_Engine_Visuals_VertexBufferResource_VertexDataDirty
              | Arcadia_Engine_Visuals_VertexBufferResource_VertexDescriptorDirty;
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_VertexBufferResource);
}

static void
Arcadia_Engine_Visuals_VertexBufferResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResourceDispatch* self
  )
{
  self->getNumberOfVertices = &Arcadia_Engine_Visuals_VertexBufferResource_getNumberOfVerticesImpl;
  self->getVertexDescriptor = &Arcadia_Engine_Visuals_VertexBufferResource_getVertexDescriptorImpl;
  self->setData = &Arcadia_Engine_Visuals_VertexBufferResource_setDataImpl;
}

static void
Arcadia_Engine_Visuals_VertexBufferResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  )
{
  if (self->bytes) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->bytes);
    self->bytes = NULL;
  }
}

static void
Arcadia_Engine_Visuals_VertexBufferResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  )
{
  if (self->vertexDescriptor) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->vertexDescriptor);
  }
}

static Arcadia_SizeValue
Arcadia_Engine_Visuals_VertexBufferResource_getNumberOfVerticesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  )
{ return self->numberOfVertices; }

static Arcadia_Media_VertexDescriptor*
Arcadia_Engine_Visuals_VertexBufferResource_getVertexDescriptorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  )
{ return self->vertexDescriptor; }

static void
Arcadia_Engine_Visuals_VertexBufferResource_setDataImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self,
    Arcadia_Media_VertexDescriptor* vertexDescriptor,
    Arcadia_SizeValue numberOfVertices,
    const void* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  Arcadia_Memory_reallocateUnmanaged(thread, (void**)&self->bytes, numberOfBytes);
  Arcadia_Memory_copy(thread, self->bytes, bytes, numberOfBytes);
  self->vertexDescriptor = vertexDescriptor;
  self->numberOfBytes = numberOfBytes;
  self->numberOfVertices = numberOfVertices;
  self->dirty = Arcadia_Engine_Visuals_VertexBufferResource_VertexDataDirty
              | Arcadia_Engine_Visuals_VertexBufferResource_VertexDescriptorDirty;
}

Arcadia_SizeValue
Arcadia_Engine_Visuals_VertexBufferResource_getNumberOfVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_VertexBufferResource, getNumberOfVertices, self); }

Arcadia_Media_VertexDescriptor*
Arcadia_Engine_Visuals_VertexBufferResource_getVertexDescriptor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_VertexBufferResource, getVertexDescriptor, self); }

void
Arcadia_Engine_Visuals_VertexBufferResource_setData
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_VertexBufferResource* self,
    Arcadia_Media_VertexDescriptor* vertexDescriptor,
    Arcadia_SizeValue numberOfVertices,
    const void* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_VertexBufferResource, setData, self, vertexDescriptor, numberOfVertices, bytes, numberOfBytes); }
