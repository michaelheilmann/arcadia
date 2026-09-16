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
#include "Arcadia/Media/VertexDescriptor.h"

static void
Arcadia_Media_VertexDescriptor_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptor* self
  );

static void
Arcadia_Media_VertexDescriptor_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorDispatch* self
  );

static void
Arcadia_Media_VertexDescriptor_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptor* self
  );

static Arcadia_Natural32Value
computeVertexSize
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* vertexElementDescriptors
  );

static const Arcadia_ObjectType_Operations _Arcadia_Media_VertexDescriptor_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_VertexDescriptor_construct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_VertexDescriptor_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_VertexDescriptor_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_Media_VertexDescriptor_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Media_VertexDescriptor_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.VertexDescriptor", Arcadia_Media_VertexDescriptor,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_Media_VertexDescriptor_typeOperations);

static void
Arcadia_Media_VertexDescriptor_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptor* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_VertexDescriptor);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (Arcadia_ValueStack_getSize(thread) < 1 || 2 != Arcadia_ValueStack_getNatural8Value(thread, 0)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->stride = Arcadia_ValueStack_getSizeValue(thread, 2);
  self->vertexElementDescriptors = Arcadia_ValueStack_getObjectReferenceValue(thread, 1);

  self->vertexSize = computeVertexSize(thread, self->vertexElementDescriptors);

  Arcadia_LeaveConstructor(Arcadia_Media_VertexDescriptor);
}

static void
Arcadia_Media_VertexDescriptor_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Media_VertexDescriptor_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptor* self
  )
{
  if (self->vertexElementDescriptors) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->vertexElementDescriptors);
  }
}

static Arcadia_Natural32Value
computeVertexSize
  (
    Arcadia_Thread* thread,
    Arcadia_ImmutableList* vertexElementDescriptors
  )
{ 
  Arcadia_Natural32Value vertexSize = 0;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)vertexElementDescriptors); i < n; ++i) {
    Arcadia_Media_VertexElementDescriptor* vertexElementDescriptor = (Arcadia_Media_VertexElementDescriptor*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)vertexElementDescriptors, i, _Arcadia_Media_VertexElementDescriptor_getType(thread));
    Arcadia_Natural32Value temporary = Arcadia_Media_VertexElementDescriptor_getOffset(thread, vertexElementDescriptor)
      + Arcadia_Media_VertexElementDescriptor_getSize(thread, vertexElementDescriptor);
    if (temporary > vertexSize) {
      vertexSize = temporary;
    }
  }
  return vertexSize;
}

Arcadia_Media_VertexDescriptor*
Arcadia_Media_VertexDescriptor_create
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue stride,
    Arcadia_ImmutableList* vertexElementDescriptors
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_VertexDescriptor);
  Arcadia_ValueStack_pushSizeValue(thread, stride);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)vertexElementDescriptors);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Media_VertexDescriptor);
}

Arcadia_Natural32Value
Arcadia_Media_VertexDescriptor_getVertexSize
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptor* self
  )
{ return self->vertexSize; }
