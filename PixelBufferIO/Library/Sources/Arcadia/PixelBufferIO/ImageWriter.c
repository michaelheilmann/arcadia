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

#include "Arcadia/PixelBufferIO/ImageWriter.h"

Arcadia_ImmutableList*
Arcadia_Imaging_ImageWriter_getSupportedTypes
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriter* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Imaging_ImageWriter, getSupportedTypes, self); }

void
Arcadia_Imaging_ImageWriter_write
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriter* self,
    Arcadia_List* source,
    Arcadia_Imaging_ImageWriterParameters* target
  )
{ Arcadia_VirtualCall(Arcadia_Imaging_ImageWriter, write, self, source, target); }

static void
Arcadia_Imaging_ImageWriter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriter* self
  );

static void
Arcadia_Imaging_ImageWriter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Imaging_ImageWriter_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Imaging_ImageWriter_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Imaging.ImageWriter", Arcadia_Imaging_ImageWriter,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Imaging_ImageWriter_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriter* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Imaging_ImageWriter);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Imaging_ImageWriter);
}

static void
Arcadia_Imaging_ImageWriter_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Imaging_ImageWriterDispatch* self
  )
{/*Intentionally empty.*/}
