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
#include "Arcadia/Media/PixelBufferOperations/CheckerboardFill.h"

static void
Arcadia_Media_PixelBufferOperations_CheckerboardFill_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_CheckerboardFill* self
  );

static void
Arcadia_Media_PixelBufferOperations_CheckerboardFill_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_CheckerboardFillDispatch* self
  );

static void
Arcadia_Media_PixelBufferOperations_CheckerboardFill_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_CheckerboardFill* self
  );

static void
Arcadia_Media_PixelBufferOperations_CheckerboardFill_apply
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_CheckerboardFill* self,
    Arcadia_Media_PixelBuffer* target
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_PixelBufferOperations_CheckerboardFill_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_PixelBufferOperations_CheckerboardFill_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_PixelBufferOperations_CheckerboardFill_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.PixelBufferOperations.CheckerboardFill", Arcadia_Media_PixelBufferOperations_CheckerboardFill,
                         u8"Arcadia.Media.PixelBufferOperation", Arcadia_Media_PixelBufferOperation,
                         &_typeOperations);

static void
Arcadia_Media_PixelBufferOperations_CheckerboardFill_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_CheckerboardFill* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_PixelBufferOperations_CheckerboardFill);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->definition = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_getType(thread));
  Arcadia_LeaveConstructor(Arcadia_Media_PixelBufferOperations_CheckerboardFill);
}

static void
Arcadia_Media_PixelBufferOperations_CheckerboardFill_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_CheckerboardFillDispatch* self
  )
{
  ((Arcadia_Media_PixelBufferOperationDispatch*)self)->apply = (void (*)(Arcadia_Thread*, Arcadia_Media_PixelBufferOperation*, Arcadia_Media_PixelBuffer*)) & Arcadia_Media_PixelBufferOperations_CheckerboardFill_apply;
}

static void
Arcadia_Media_PixelBufferOperations_CheckerboardFill_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_CheckerboardFill* self
  )
{
  if (self->definition) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->definition);
  }
}

static void
Arcadia_Media_PixelBufferOperations_CheckerboardFill_apply
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperations_CheckerboardFill* self,
    Arcadia_Media_PixelBuffer* target
  )
{
  if (0 == self->definition->checkerWidth || 0 == self->definition->checkerHeight) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (NULL == self->definition->firstCheckerColor->definition || NULL == self->definition->secondCheckerColor->definition) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ADL_ColorDefinition* firstCheckerColor = (Arcadia_ADL_ColorDefinition*)self->definition->firstCheckerColor->definition,
                             * secondCheckerColor = (Arcadia_ADL_ColorDefinition*)self->definition->secondCheckerColor->definition;
  Arcadia_Integer32Value checkerWidth = self->definition->checkerWidth,
                         checkerHeight = self->definition->checkerHeight;
  for (Arcadia_Integer32Value y = 0; y < Arcadia_Media_PixelBuffer_getHeight(thread, target) / checkerHeight + 1; ++y) {
    for (Arcadia_Integer32Value x = 0; x < Arcadia_Media_PixelBuffer_getWidth(thread, target) / checkerWidth + 1; ++x) {
      if ((x + y) % 2 == 0) {
        Arcadia_Natural8Value red = firstCheckerColor->red;
        Arcadia_Natural8Value green = firstCheckerColor->green;
        Arcadia_Natural8Value blue = firstCheckerColor->blue;
        Arcadia_Media_PixelBuffer_fillRectangle(thread, target, x * checkerWidth, y * checkerHeight, checkerWidth, checkerHeight,
                                                  red, green, blue, 255);
      } else {
        Arcadia_Natural8Value red = secondCheckerColor->red;
        Arcadia_Natural8Value green = secondCheckerColor->green;
        Arcadia_Natural8Value blue = secondCheckerColor->blue;
        Arcadia_Media_PixelBuffer_fillRectangle(thread, target, x * checkerWidth, y * checkerHeight, checkerWidth, checkerHeight,
                                                  red, green, blue, 255);
      }

    }
  }
}

Arcadia_Media_PixelBufferOperations_CheckerboardFill*
Arcadia_Media_PixelBufferOperations_CheckerboardFill_create
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* definition
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_PixelBufferOperations_CheckerboardFill);
  if (definition) {
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definition);
  } else {
    Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Media_PixelBufferOperations_CheckerboardFill);
}
