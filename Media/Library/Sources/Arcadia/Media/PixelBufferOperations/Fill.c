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
#include "Arcadia/Media/PixelBufferOperations/Fill.h"

static void
Arcadia_Media_PixelBufferOperations_Fill_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_Fill* self
  );

static void
Arcadia_Media_PixelBufferOperations_Fill_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_FillDispatch* self
  );

static void
Arcadia_Media_PixelBufferOperations_Fill_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_Fill* self
  );

static void
Arcadia_Media_PixelBufferOperations_Fill_apply
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_Fill* self,
    Arcadia_Media_PixelBuffer* target
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_PixelBufferOperations_Fill_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_PixelBufferOperations_Fill_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_PixelBufferOperations_Fill_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.PixelBufferOperations.Fill", Arcadia_Media_PixelBufferOperations_Fill,
                         u8"Arcadia.Media.PixelBufferOperation", Arcadia_Media_PixelBufferOperation,
                         &_typeOperations);

static void
Arcadia_Media_PixelBufferOperations_Fill_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_Fill* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_PixelBufferOperations_Fill);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->definition = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_Media_PixelBufferOperations_Fill);
}

static void
Arcadia_Media_PixelBufferOperations_Fill_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_FillDispatch* self
  )
{
  ((Arcadia_Media_PixelBufferOperationDispatch*)self)->apply = (void (*)(Arcadia_Thread*, Arcadia_Media_PixelBufferOperation*, Arcadia_Media_PixelBuffer*)) & Arcadia_Media_PixelBufferOperations_Fill_apply;
}

static void
Arcadia_Media_PixelBufferOperations_Fill_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_Fill* self
  )
{
  if (self->definition) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->definition);
  }
}

static void
Arcadia_Media_PixelBufferOperations_Fill_apply
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_Fill* self,
    Arcadia_Media_PixelBuffer* target
  )
{
  if (NULL == self->definition->color->definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_ColorDefinition* color = (Arcadia_ADL_ColorDefinition*)self->definition->color->definition;
  Arcadia_Natural8Value red = color->red;
  Arcadia_Natural8Value green = color->green;
  Arcadia_Natural8Value blue = color->blue;
  Arcadia_Media_PixelBuffer_fillRectangle(thread, target, 0, 0, target->width, target->height, red, green, blue, 255);
}

Arcadia_Media_PixelBufferOperations_Fill*
Arcadia_Media_PixelBufferOperations_Fill_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* definition
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_PixelBufferOperations_Fill);
  if (definition) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definition);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Media_PixelBufferOperations_Fill);
}
