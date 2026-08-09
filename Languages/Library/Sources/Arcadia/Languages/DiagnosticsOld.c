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

#define ARCADIA_LANGUAGES_MODULE (1)
#include "Arcadia/Languages/DiagnosticsOld.h"

#include "Arcadia/FileSystem/Include.h"

void
Arcadia_Languages_DiagnosticsOld_emit
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* message
  )
{
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, Arcadia_FileSystem_getOrCreate(thread));
  Arcadia_FileHandle_openStandardOutput(thread, fileHandle);
  Arcadia_FileHandle_writeStringBuffer(thread, fileHandle, message);
}
