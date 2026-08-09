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

#define ARCADIA_MEDIA_MODULE (1)
#include "Arcadia/Media/VertexDescriptorBuilder.h"

#include "Arcadia/Collections/Include.h"
#include "Arcadia/Media/VertexDescriptor.module.h"

static void
Arcadia_Media_VertexDescriptorBuilder_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilder* self
  );

static void
Arcadia_Media_VertexDescriptorBuilder_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilderDispatch* self
  );

static void
Arcadia_Media_VertexDescriptorBuilder_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilder* self
  );

static const Arcadia_ObjectType_Operations _Arcadia_Media_VertexDescriptorBuilder_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_VertexDescriptorBuilder_construct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_VertexDescriptorBuilder_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_VertexDescriptorBuilder_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_Media_VertexDescriptorBuilder_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_Media_VertexDescriptorBuilder_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.VertexDescriptorBuilder", Arcadia_Media_VertexDescriptorBuilder,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_Arcadia_Media_VertexDescriptorBuilder_typeOperations);

static void
Arcadia_Media_VertexDescriptorBuilder_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilder* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_VertexDescriptorBuilder);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->stride = Arcadia_SizeValue_Literal(0);
  self->vertexElementDescriptors = (Arcadia_List*)Arcadia_ArrayList_create(thread);

  Arcadia_LeaveConstructor(Arcadia_Media_VertexDescriptorBuilder);
}

static void
Arcadia_Media_VertexDescriptorBuilder_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilderDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Media_VertexDescriptorBuilder_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilder* self
  )
{
  if (self->vertexElementDescriptors) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->vertexElementDescriptors);
  }
}

Arcadia_Media_VertexDescriptorBuilder*
Arcadia_Media_VertexDescriptorBuilder_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_VertexDescriptorBuilder);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Media_VertexDescriptorBuilder);
}

Arcadia_Media_VertexDescriptor*
Arcadia_Media_VertexDescriptorBuilder_build
  (
    Arcadia_Thread* thread,
    Arcadia_Media_VertexDescriptorBuilder* self
  )
{
  Arcadia_SizeValue stride = 0;
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->vertexElementDescriptors); i < n; ++i) {
    Arcadia_Media_VertexElementDescriptor* vertexElementDescriptor = (Arcadia_Media_VertexElementDescriptor*)Arcadia_List_getObjectReferenceValueAt(thread, self->vertexElementDescriptors, i);
    Arcadia_SizeValue newStride = vertexElementDescriptor->offset;
    switch (vertexElementDescriptor->syntactics) {
      case Arcadia_Media_VertexElementSyntactics_Real32: {
        newStride += 1 * sizeof(Arcadia_Real32Value);
      } break;
      case Arcadia_Media_VertexElementSyntactics_Real32Real32: {
        newStride += 2 * sizeof(Arcadia_Real32Value);
      } break;
      case Arcadia_Media_VertexElementSyntactics_Real32Real32Real32: {
        newStride += 3 * sizeof(Arcadia_Real32Value);
      } break;
      case Arcadia_Media_VertexElementSyntactics_Real32Real32Real32Real32: {
        newStride += 4 * sizeof(Arcadia_Real32Value);
      } break;
      default: {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      } break;
    };
    if (newStride > stride) {
      stride = newStride;
    }
  };
  Arcadia_ImmutableList* vertexElementDescriptors = (Arcadia_ImmutableList*)Arcadia_ImmutableList_create(thread, Arcadia_Value_makeObjectReferenceValue(self->vertexElementDescriptors));
  Arcadia_Media_VertexDescriptor* vertexDescriptor = Arcadia_Media_VertexDescriptor_create(thread, stride, vertexElementDescriptors);
  return vertexDescriptor;
}
