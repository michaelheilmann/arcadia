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
#include "Arcadia/FileSystem/FileHandleExtensions.h"

#include "Arcadia/FileSystem/FileHandle.h"

void
Arcadia_FileHandle_writeByteBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    Arcadia_ByteArrayBuilder* source
  )
{
  Arcadia_SizeValue bytesToWrite = Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, source), bytesWritten = 0;
  Arcadia_Natural8Value const* bytes = Arcadia_ByteArrayBuilder_getBytes(thread, source);
  while (bytesToWrite > bytesWritten) {
    Arcadia_SizeValue bytesWritteNow = 0;
    Arcadia_FileHandle_write(thread, self, bytes + bytesWritten, bytesToWrite - bytesWritten, &bytesWritteNow);
    bytesWritten += bytesWritteNow;
  }
}

void
Arcadia_FileHandle_writeString
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    Arcadia_String* source
  )
{
  Arcadia_SizeValue bytesToWrite = Arcadia_String_getNumberOfBytes(thread, source), bytesWritten = 0;
  Arcadia_Natural8Value const* bytes = Arcadia_String_getBytes(thread, source);
  while (bytesToWrite > bytesWritten) {
    Arcadia_SizeValue bytesWritteNow = 0;
    Arcadia_FileHandle_write(thread, self, bytes + bytesWritten, bytesToWrite - bytesWritten, &bytesWritteNow);
    bytesWritten += bytesWritteNow;
  }
}

void
Arcadia_FileHandle_writeStringBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* self,
    Arcadia_StringBuilder* source
  )
{
  Arcadia_SizeValue bytesToWrite = Arcadia_StringBuilder_getNumberOfBytes(thread, source), bytesWritten = 0;
  Arcadia_Natural8Value const* bytes = Arcadia_StringBuilder_getBytes(thread, source);
  while (bytesToWrite > bytesWritten) {
    Arcadia_SizeValue bytesWritteNow = 0;
    Arcadia_FileHandle_write(thread, self, bytes + bytesWritten, bytesToWrite - bytesWritten, &bytesWritteNow);
    bytesWritten += bytesWritteNow;
  }
}
