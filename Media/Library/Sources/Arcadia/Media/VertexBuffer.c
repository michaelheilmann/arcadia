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

#define ARCADIA_MEDIA_PRIVATE (1)
#include "Arcadia/Media/VertexBuffer.h"

#include "Arcadia/Media/Include.h"
#include <stdint.h>

static void
Arcadia_Media_VertexBuffer_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* self
  );

static void
Arcadia_Media_VertexBuffer_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* self
  );

static void
Arcadia_Media_VertexBuffer_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBufferDispatch* self
  );

static void
Arcadia_Media_VertexBuffer_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_VertexBuffer_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Media_VertexBuffer_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_VertexBuffer_initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_VertexBuffer_visitImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.VertexBuffer", Arcadia_Media_VertexBuffer,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Media_VertexBuffer_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_VertexBuffer);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  self->vertexBytes = NULL;
  self->vertexDescriptor = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_Media_VertexDescriptor_getType(thread));
  self->numberOfVertices = Arcadia_ValueStack_getSizeValue(thread, 1);
  Arcadia_SizeValue stride = Arcadia_Media_VertexDescriptor_getStride(thread, self->vertexDescriptor);
  if (self->numberOfVertices && stride > SIZE_MAX / self->numberOfVertices) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  // The factory allocates fixed-size mutable storage. It is not resized after construction.
  self->vertexBytes = Arcadia_ByteArrayFactory_createMutable(thread, stride * self->numberOfVertices);
  //
  Arcadia_LeaveConstructor(Arcadia_Media_VertexBuffer);
}

static void
Arcadia_Media_VertexBuffer_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* self
  )
{/* The ByteArray owns the allocation and releases it through its destructor. */}

static void
Arcadia_Media_VertexBuffer_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBufferDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Media_VertexBuffer_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* self
  )
{
  if (self->vertexDescriptor) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->vertexDescriptor);
  }
  if (self->vertexBytes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->vertexBytes);
  }
}

Arcadia_Media_VertexBuffer*
Arcadia_Media_VertexBuffer_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptor* vertexDescriptor,
    Arcadia_SizeValue numberOfVertices
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_VertexBuffer);
  if (vertexDescriptor) Arcadia_ValueStack_pushObjectReferenceValue(thread, vertexDescriptor); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushSizeValue(thread, numberOfVertices);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Media_VertexBuffer);
}

Arcadia_SizeValue
Arcadia_Media_VertexBuffer_getNumberOfVertices
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* self
  )
{ return self->numberOfVertices; }

Arcadia_Media_VertexDescriptor*
Arcadia_Media_VertexBuffer_getVertexDescriptor
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* self
  )
{ return self->vertexDescriptor; }

Arcadia_ByteArray*
Arcadia_Media_VertexBuffer_getVertexBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* self
  )
{ return self->vertexBytes; }

Arcadia_SizeValue
Arcadia_Media_VertexBuffer_getNumberOfBytes
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* self
  )
{ return Arcadia_ByteArray_getSize(thread, self->vertexBytes); }

void
Arcadia_Media_VertexBuffer_copyInto
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* target,
    Arcadia_Media_VertexBuffer* source
  )
{
  if (!target || !source || target->vertexDescriptor != source->vertexDescriptor ||
      target->numberOfVertices != source->numberOfVertices) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (target == source) {
    return;
  }
  Arcadia_SizeValue size = Arcadia_Media_VertexBuffer_getNumberOfBytes(thread, source);
  if (size != Arcadia_Media_VertexBuffer_getNumberOfBytes(thread, target)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (size) {
    Arcadia_Memory_copy(thread, Arcadia_ByteArray_getMutableBytes(thread, target->vertexBytes),
                        Arcadia_ByteArray_getBytes(thread, source->vertexBytes), size);
  }
}

Arcadia_Media_VertexBuffer*
Arcadia_Media_VertexBuffer_copy
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexBuffer* source
  )
{
  if (!source) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // Protect the source across the destination's collecting allocation.
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source);
  Arcadia_Media_VertexBuffer* target = Arcadia_Media_VertexBuffer_create
    (
      thread,
      Arcadia_Media_VertexBuffer_getVertexDescriptor(thread, source),
      Arcadia_Media_VertexBuffer_getNumberOfVertices(thread, source)
    );
  Arcadia_Object_lock(thread, (Arcadia_Object*)target);
  Arcadia_Media_VertexBuffer_copyInto(thread, target, source);
  Arcadia_Object_unlock(thread, (Arcadia_Object*)target);
  Arcadia_ValueStack_popValues(thread, 1);
  return target;
}
