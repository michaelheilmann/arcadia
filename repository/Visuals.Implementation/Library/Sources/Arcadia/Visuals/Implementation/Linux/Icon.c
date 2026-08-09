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

#include "Arcadia/Visuals/Implementation/Linux/Icon.h"

static void
Arcadia_Visuals_Linux_Icon_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Linux_Icon* self
  );

static void
Arcadia_Visuals_Linux_Icon_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Linux_IconDispatch* self
  );

static void
Arcadia_Visuals_Linux_Icon_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Linux_Icon* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Visuals_Linux_Icon_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Visuals_Linux_Icon_destruct,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Visuals_Linux_Icon_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Linux.Icon", Arcadia_Visuals_Linux_Icon,
                         u8"Arcadia.Engine.Visuals.Icon", Arcadia_Engine_Visuals_Icon,
                         &_typeOperations);

static void
Arcadia_Visuals_Linux_Icon_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Linux_Icon* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Visuals_Linux_Icon);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_Media_PixelBuffer* pixelBuffer = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Media_PixelBuffer_getType(thread));
  pixelBuffer = Arcadia_Media_PixelBuffer_createClone(thread, pixelBuffer);
  Arcadia_Media_PixelBuffer_setPixelFormat(thread, pixelBuffer, Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8);

  Arcadia_Natural32Value dummy, numberOfPixels, numberOfBytes;
  if (!Arcadia_safeMultiplyNatural32Value(thread, (Arcadia_Natural32Value)Arcadia_Media_PixelBuffer_getWidth(thread, pixelBuffer),
                                                  (Arcadia_Natural32Value)Arcadia_Media_PixelBuffer_getHeight(thread, pixelBuffer), &dummy, &numberOfPixels)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_Natural32Value_Maximum - 2 < numberOfPixels) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_safeMultiplyNatural32Value(thread, numberOfPixels + 2, sizeof(unsigned long), &dummy, &numberOfBytes)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->numberOfBytes = numberOfBytes;
  self->width = Arcadia_Media_PixelBuffer_getWidth(thread, pixelBuffer);
  self->height = Arcadia_Media_PixelBuffer_getHeight(thread, pixelBuffer);
  self->bytes = Arcadia_Memory_allocateUnmanaged(thread, numberOfBytes);
  self->bytes[0] = (Arcadia_Natural32Value)self->width;
  self->bytes[1] = (Arcadia_Natural32Value)self->height;
  unsigned long* p = &self->bytes[2];
  for (Arcadia_SizeValue y = 0; y < self->height; ++y) {
    for (Arcadia_SizeValue x = 0; x < self->width; ++x) {
      Arcadia_Natural8Value red, green, blue, alpha;
      Arcadia_Media_PixelBuffer_getPixelRGBA(thread, pixelBuffer, x, y, &red, &green, &blue, &alpha);
      *p++ = alpha << 24
           | blue << 16
           | green << 8
           | red << 0;
    }
  }
  Arcadia_LeaveConstructor(Arcadia_Visuals_Linux_Icon);
}

static void
Arcadia_Visuals_Linux_Icon_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Linux_IconDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Visuals_Linux_Icon_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Visuals_Linux_Icon* self
  )
{
  if (self->bytes) {
    Arcadia_Memory_deallocateUnmanaged(thread, self->bytes);
    self->bytes = NULL;
  }
}

Arcadia_Visuals_Linux_Icon*
Arcadia_Visuals_Linux_Icon_create
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* pixelBuffer
  )
{
  _Arcadia_BeginCreate(Arcadia_Visuals_Linux_Icon);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, pixelBuffer);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Visuals_Linux_Icon);
}
