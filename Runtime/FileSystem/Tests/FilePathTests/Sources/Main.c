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
#include <string.h>
#include "Arcadia/FileSystem/Include.h"

static void
checkNormalized
  (
    Arcadia_Thread* thread,
    char const* p,
    char const* q
  )
{
  Arcadia_String* filePathStringSource = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, p, strlen(p))));
  Arcadia_FilePath* filePath = Arcadia_FilePath_parseNative(thread, filePathStringSource);
  Arcadia_String* filePathStringTarget = Arcadia_FilePath_toNative(thread, filePath, Arcadia_BooleanValue_False);

  if (Arcadia_String_getNumberOfBytes(thread, filePathStringTarget) != strlen(q)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
  if (memcmp(Arcadia_String_getBytes(thread, filePathStringTarget), q, strlen(q))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
normalizationTests
  (
    Arcadia_Thread* thread
  )
{
  checkNormalized(thread, u8"a/..", ".");
  checkNormalized(thread, u8"a/b/../..", ".");
  checkNormalized(thread, u8".", ".");
  checkNormalized(thread, u8"./", ".");
}

static void
regressionTest1
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_String* u = Arcadia_String_createFromCxxString(thread, u8"./../../x/y");
  Arcadia_FilePath* v = Arcadia_FilePath_parseGeneric(thread, u);
  Arcadia_String* w = Arcadia_FilePath_toGeneric(thread, v);

  Arcadia_Value t = Arcadia_Value_makeObjectReferenceValue(w);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)u, &t));
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&normalizationTests)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&regressionTest1)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
