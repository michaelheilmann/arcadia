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

#if !defined(ARCADIA_ADL_DIAGNOSTICS_H_INCLUDED)
#define ARCADIA_ADL_DIAGNOSTICS_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/FileSystem/Include.h"
#include "Arcadia/ADL/Definition.h"

Arcadia_NoReturn() static inline void
Arcadia_ADL_Diagnostics_raiseNotAPixelBufferDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_String* key
  )
{
  Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"ADL definition `");
  Arcadia_StringBuilder_insertBackString(thread, message, key);
  Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"` is not a pixel buffer definition\n");

  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  Arcadia_FileHandle_openStandardOutput(thread, fileHandle);

  Arcadia_FileHandle_writeStringBuffer(thread, fileHandle, message);

  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

Arcadia_NoReturn() static inline void
Arcadia_ADL_Diagnostics_raisePixelBufferOperationNotYetSupported
  (
    Arcadia_Thread* thread,
    Arcadia_String* key
  )
{
  Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"ADL pixel buffer operation `");
  Arcadia_StringBuilder_insertBackString(thread, message, key);
  Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"` is not (yet) supported\n");

  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  Arcadia_FileHandle_openStandardOutput(thread, fileHandle);

  Arcadia_FileHandle_writeStringBuffer(thread, fileHandle, message);

  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

Arcadia_NoReturn() static inline void
Arcadia_ADL_Diagnostics_raiseDefinitionNotFound
  (
    Arcadia_Thread* thread,
    Arcadia_String* key
  )
{
  Arcadia_StringBuilder* message = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"starting ADL definition `");
  Arcadia_StringBuilder_insertBackString(thread, message, key);
  Arcadia_StringBuilder_insertBackCxxString(thread, message, u8"` not found\n");

  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  Arcadia_FileHandle_openStandardOutput(thread, fileHandle);

  Arcadia_FileHandle_writeStringBuffer(thread, fileHandle, message);

  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

#endif  // ARCADIA_ADL_DIAGNOSTICS_H_INCLUDED
