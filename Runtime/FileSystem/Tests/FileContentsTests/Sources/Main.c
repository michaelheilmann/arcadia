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

#include "Arcadia/FileSystem/Include.h"
#include "Arcadia/Ring2/Include.h"

#include <stdlib.h>
#include <string.h>

static void
assertFileContents
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* fileSystem,
    Arcadia_FilePath* filePath,
    char const* expected
  )
{
  Arcadia_ByteArrayBuilder* received = Arcadia_FileSystem_getFileContents(thread, fileSystem, filePath);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArrayBuilder_isEqualTo_pn(thread, received, expected, strlen(expected)));
}

static void
setAndAssertFileContents
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* fileSystem,
    Arcadia_FilePath* filePath,
    char const* expected
  )
{
  Arcadia_ByteArrayBuilder* contents = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, contents, expected, strlen(expected));
  Arcadia_FileSystem_setFileContents(thread, fileSystem, filePath, contents);
  assertFileContents(thread, fileSystem, filePath, expected);
}

static void
fileContentsTests
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FilePath* filePath = Arcadia_FileSystem_createTemporaryFile(thread, fileSystem);

  setAndAssertFileContents(thread, fileSystem, filePath, "");
  setAndAssertFileContents(thread, fileSystem, filePath, "Hello, World!\n");

  Arcadia_FileSystem_deleteRegularFile(thread, fileSystem, filePath);
}

int
main
  (
    int argc,
    char** argv
  )
{
  if (!Arcadia_Tests_safeExecute(&fileContentsTests)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
