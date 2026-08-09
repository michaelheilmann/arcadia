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

#if !defined(ARCADIA_FILESYSTEM_FILEHANDLE_H_INCLUDED)
#define ARCADIA_FILESYSTEM_FILEHANDLE_H_INCLUDED

#if !defined(ARCADIA_FILESYSTEM_MODULE)
  #error("do not include directly, include `Arcadia/FileSystem/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"
typedef struct Arcadia_FilePath Arcadia_FilePath;
typedef struct Arcadia_FileSystem Arcadia_FileSystem;

Arcadia_declareObjectType(u8"Arcadia.FileHandle", Arcadia_FileHandle,
                          u8"Arcadia.Object");

struct Arcadia_FileHandleDispatch {
  Arcadia_ObjectDispatch _parent;

  Arcadia_BooleanValue
  (*isClosed)
    (
      Arcadia_Thread* thread,
      Arcadia_FileHandle const* self
    );

  Arcadia_BooleanValue
  (*isOpened)
    (
      Arcadia_Thread* thread,
      Arcadia_FileHandle const* self
    );

  Arcadia_BooleanValue
  (*isOpenedForReading)
    (
      Arcadia_Thread* thread,
      Arcadia_FileHandle const* self
    );

  Arcadia_BooleanValue
  (*isOpenedForWriting)
    (
      Arcadia_Thread* thread,
      Arcadia_FileHandle const* self
    );

  void
  (*close)
    (
      Arcadia_Thread* thread,
      Arcadia_FileHandle* self
    );

  void
  (*openForReading)
    (
      Arcadia_Thread* thread,
      Arcadia_FileHandle* self,
      Arcadia_FilePath* path
    );

  void
  (*openForWriting)
    (
      Arcadia_Thread* thread,
      Arcadia_FileHandle* self,
      Arcadia_FilePath* path
    );

  void
  (*read)
    (
      Arcadia_Thread* thread,
      Arcadia_FileHandle* self,
      void* byes,
      Arcadia_SizeValue bytesToRead,
      Arcadia_SizeValue* bytesRead
    );

  void
  (*write)
    (
      Arcadia_Thread* thread,
      Arcadia_FileHandle* self,
      void const* bytes,
      Arcadia_SizeValue bytesToWrite,
      Arcadia_SizeValue* bytesWritten
    );

  void
  (*openStandardInput)
    (
      Arcadia_Thread* thread,
      Arcadia_FileHandle* self
    );

  void
  (*openStandardOutput)
    (
      Arcadia_Thread* thread,
      Arcadia_FileHandle* self
    );

  void
  (*openStandardError)
    (
      Arcadia_Thread* thread,
      Arcadia_FileHandle* self
    );

};

struct Arcadia_FileHandle {
  Arcadia_Object _parent;
};

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileHandle_close
void
Arcadia_FileHandle_close
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileHandle_isClosed
Arcadia_BooleanValue
Arcadia_FileHandle_isClosed
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileHandle_isOpened
Arcadia_BooleanValue
Arcadia_FileHandle_isOpened
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileHandle_isOpenedForReading
Arcadia_BooleanValue
Arcadia_FileHandle_isOpenedForReading
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileHandle_isOpenedForWriting
Arcadia_BooleanValue
Arcadia_FileHandle_isOpenedForWriting
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle const* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileHandle_openForReading
void
Arcadia_FileHandle_openForReading
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    Arcadia_FilePath* path
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileHandle_openForWriting
void
Arcadia_FileHandle_openForWriting
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    Arcadia_FilePath* path
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileHandle_openStandardError
void
Arcadia_FileHandle_openStandardError
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileHandle_openStandardInput
void
Arcadia_FileHandle_openStandardInput
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileHandle_openStandardOutput
void
Arcadia_FileHandle_openStandardOutput
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileHandle_read
void
Arcadia_FileHandle_read
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    void* bytes,
    Arcadia_SizeValue bytesToRead,
    Arcadia_SizeValue* bytesRead
  );

// https://michaelheilmann.com/Arcadia/Ring2/#Arcadia_FileHandle_write
void
Arcadia_FileHandle_write
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    void const* bytes,
    Arcadia_SizeValue bytesToWrite,
    Arcadia_SizeValue* bytesWritten
  );

#endif // ARCADIA_FILESYSTEM_FILEHANDLE_H_INCLUDED
