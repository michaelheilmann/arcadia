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

#include "Arcadia/PixelBufferIO/ImageReader.h"

Arcadia_ImmutableList*
Arcadia_Imaging_ImageReader_getSupportedTypes
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReader* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Imaging_ImageReader, getSupportedTypes, self); }

Arcadia_List*
Arcadia_Imaging_ImageReader_read
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReader* self,
    Arcadia_Imaging_ImageReaderParameters* source
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Imaging_ImageReader, read, self, source); }

static void
Arcadia_Imaging_ImageReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Imaging_ImageReader);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Imaging_ImageReader);
}

static void
Arcadia_Imaging_ImageReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageReaderDispatch* self
  )
{/*Intentionally empty.*/}

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Imaging_ImageReader_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Imaging_ImageReader_initializeDispatchImpl,
};

static const Arcadia_TypeDescriptor_Operations _typeOperations = {
  Arcadia_TypeDescriptor_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Imaging.ImageReader", Arcadia_Imaging_ImageReader,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);
