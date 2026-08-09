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

#include "Arcadia/Ring2/Include.h"


#include <stdlib.h>
#include "Arcadia/FileSystem/Include.h"

#if 0
#include <string.h>
#include <stdio.h> // @todo Remove references to `stdio.h`.

// @todo Remove references to `stdio.h`.
#endif
static void
directoryIteratorTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_logf(Arcadia_LogFlags_Info, "directory iterator test 1\n");
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FilePath* filePath = Arcadia_FileSystem_getWorkingDirectory(thread, fileSystem);
  Arcadia_logf(Arcadia_LogFlags_Info, "  working directory: %s\n", Arcadia_String_getBytes(thread, Arcadia_FilePath_toNative(thread, filePath, Arcadia_BooleanValue_True)));
  Arcadia_FilePath_append(thread, filePath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Directory1")));
  Arcadia_logf(Arcadia_LogFlags_Info, "  iterated directory: %s\n", Arcadia_String_getBytes(thread, Arcadia_FilePath_toNative(thread, filePath, Arcadia_BooleanValue_True)));
  if (Arcadia_FileSystem_directoryFileExists(thread, fileSystem, filePath)) {
    Arcadia_logf(Arcadia_LogFlags_Info, "  directory %s exists\n", Arcadia_String_getBytes(thread, Arcadia_FilePath_toNative(thread, filePath, Arcadia_BooleanValue_True)));
  }
  Arcadia_DirectoryIterator* directoryIterator = (Arcadia_DirectoryIterator*)Arcadia_FileSystem_createDirectoryIterator(thread, fileSystem, filePath);
  Arcadia_Integer8Value count = 0;
  while (Arcadia_DirectoryIterator_hasValue(thread, directoryIterator)) {
    Arcadia_FilePath* filePath = Arcadia_DirectoryIterator_getValue(thread, directoryIterator);
    Arcadia_Tests_assertTrue(thread, NULL != filePath);
    Arcadia_String* filePathString = Arcadia_FilePath_toGeneric(thread, filePath);
    Arcadia_Tests_assertTrue(thread, NULL != filePathString);
    Arcadia_DirectoryIterator_nextValue(thread, directoryIterator);
    count++;
  }
  Arcadia_Tests_assertTrue(thread, 3 == count);
}

#if 0
// @todo Remove references to `stdio.h`.
#endif
static void
directoryIteratorTest2
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_logf(Arcadia_LogFlags_Info, "directory iterator test 2\n");
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FilePath* filePath = Arcadia_FileSystem_getWorkingDirectory(thread, fileSystem);
  Arcadia_logf(Arcadia_LogFlags_Info, "  working directory: %s\n", Arcadia_String_getBytes(thread, Arcadia_FilePath_toNative(thread, filePath, Arcadia_BooleanValue_True)));
  Arcadia_FilePath_append(thread, filePath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Assets/Directory2")));
  Arcadia_logf(Arcadia_LogFlags_Info, "  iterated directory: %s\n", Arcadia_String_getBytes(thread, Arcadia_FilePath_toNative(thread, filePath, Arcadia_BooleanValue_True)));
  if (Arcadia_FileSystem_directoryFileExists(thread, fileSystem, filePath)) {
    Arcadia_logf(Arcadia_LogFlags_Info, "  directory %s exists\n", Arcadia_String_getBytes(thread, Arcadia_FilePath_toNative(thread, filePath, Arcadia_BooleanValue_True)));
  }
  Arcadia_DirectoryIterator* directoryIterator = (Arcadia_DirectoryIterator*)Arcadia_FileSystem_createDirectoryIterator(thread, fileSystem, filePath);
  Arcadia_Integer8Value count = 0;
  while (Arcadia_DirectoryIterator_hasValue(thread, directoryIterator)) {
    Arcadia_FilePath* filePath = Arcadia_DirectoryIterator_getValue(thread, directoryIterator);
    Arcadia_Tests_assertTrue(thread, NULL != filePath);
    Arcadia_String* filePathString = Arcadia_FilePath_toGeneric(thread, filePath);
    Arcadia_Tests_assertTrue(thread, NULL != filePathString);
    Arcadia_DirectoryIterator_nextValue(thread, directoryIterator);
    count++;
  }
  Arcadia_Tests_assertTrue(thread, 4 == count);
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&directoryIteratorTest1)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&directoryIteratorTest2)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
