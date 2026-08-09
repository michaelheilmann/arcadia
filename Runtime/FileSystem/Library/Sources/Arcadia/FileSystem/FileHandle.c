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

#define ARCADIA_FILESYSTEM_MODULE (1)
#include "Arcadia/FileSystem/FileHandle.h"

#include "Arcadia/FileSystem/Include.h"

#define Flags_OpenRead (1)

#define Flags_OpenWrite (2)

static void
Arcadia_FileHandle_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  );

static void
Arcadia_FileHandle_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandleDispatch* self
  );

static void
Arcadia_FileHandle_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  );

static void
Arcadia_FileHandle_visit
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_FileHandle_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_FileHandle_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_FileHandle_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_FileHandle_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.FileHandle", Arcadia_FileHandle,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_FileHandle_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  )
{
  Arcadia_EnterConstructor(Arcadia_FileHandle);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  //
  Arcadia_LeaveConstructor(Arcadia_FileHandle);
}

static void
Arcadia_FileHandle_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandleDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_FileHandle_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_FileHandle_visit
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  )
{/*Intentionally empty.*/}

void
Arcadia_FileHandle_close
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  )
{ Arcadia_VirtualCall(Arcadia_FileHandle, close, self); }

Arcadia_BooleanValue
Arcadia_FileHandle_isClosed
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle const* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileHandle, isClosed, self); }

Arcadia_BooleanValue
Arcadia_FileHandle_isOpened
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle const* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileHandle, isOpened, self); }

Arcadia_BooleanValue
Arcadia_FileHandle_isOpenedForReading
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle const* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileHandle, isOpenedForReading, self); }

Arcadia_BooleanValue
Arcadia_FileHandle_isOpenedForWriting
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle const* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileHandle, isOpenedForWriting, self); }

void
Arcadia_FileHandle_openForReading
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    Arcadia_FilePath* path
  )
{ Arcadia_VirtualCall(Arcadia_FileHandle, openForReading, self, path); }

void
Arcadia_FileHandle_openForWriting
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    Arcadia_FilePath* path
  )
{ Arcadia_VirtualCall(Arcadia_FileHandle, openForWriting, self, path); }

void
Arcadia_FileHandle_read
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    void* bytes,
    Arcadia_SizeValue bytesToRead,
    Arcadia_SizeValue* bytesRead
  )
{ Arcadia_VirtualCall(Arcadia_FileHandle, read, self, bytes, bytesToRead, bytesRead); }

void
Arcadia_FileHandle_write
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    void const* bytes,
    Arcadia_SizeValue bytesToWrite,
    Arcadia_SizeValue* bytesWritten
  )
{ Arcadia_VirtualCall(Arcadia_FileHandle, write, self, bytes, bytesToWrite, bytesWritten); }

void
Arcadia_FileHandle_openStandardError
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  )
{ Arcadia_VirtualCall(Arcadia_FileHandle, openStandardError, self); }

void
Arcadia_FileHandle_openStandardInput
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  )
{ Arcadia_VirtualCall(Arcadia_FileHandle, openStandardInput, self); }

void
Arcadia_FileHandle_openStandardOutput
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  )
{ Arcadia_VirtualCall(Arcadia_FileHandle, openStandardOutput, self); }
