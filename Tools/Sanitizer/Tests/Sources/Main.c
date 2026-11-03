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

#include "Arcadia/Testing/Include.h"
#include "Arcadia/Sanitizer/Sanitizer.h"
#include <stdlib.h>
#include <string.h>

static void
assertTransform
  (
    Arcadia_Thread* thread,
    char const* input,
    Arcadia_SizeValue inputLength,
    char const* expected,
    Arcadia_SizeValue expectedLength,
    Arcadia_SizeValue expectedLine
  )
{
  Arcadia_ByteArrayBuilder* source = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, source, input, inputLength);
  Arcadia_ByteArrayBuilder* result = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_SizeValue line = 0;
  Arcadia_BooleanValue changed = Arcadia_Sanitizer_normalize(thread, source, result, &line);
  Arcadia_Tests_assertTrue(thread, changed == (expectedLine ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False));
  Arcadia_Tests_assertTrue(thread, line == expectedLine);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArrayBuilder_isEqualTo_pn(thread, result, expected, expectedLength));
  Arcadia_ByteArrayBuilder* second = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_SizeValue secondLine = 99;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Sanitizer_normalize(thread, result, second, &secondLine));
  Arcadia_Tests_assertTrue(thread, secondLine == 0);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArrayBuilder_isEqualTo(thread, result, second));
}

static void
normalizeTest
  (
    Arcadia_Thread* thread
  )
{
  assertTransform(thread, "", 0, "", 0, 0);
  assertTransform(thread, "one\ntwo\n", 8, "one\ntwo\n", 8, 0);
  assertTransform(thread, "one \r\ntwo\t\rthree\t ", 18, "one\ntwo\nthree", 13, 1);
  assertTransform(thread, "a\tb \tc", 6, "a\tb \tc", 6, 0);
  assertTransform(thread, "A\rB", 3, "A\nB", 3, 1);
  assertTransform(thread, "x\nq\t", 4, "x\nq", 3, 2);
  assertTransform(thread, "\xEF\xBB\xBFhello\r\n", 10, "\xEF\xBB\xBFhello\n", 9, 1);
}

static void
fileRoundTripTest
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  Arcadia_FilePath* path = Arcadia_FileSystem_createTemporaryFile(thread, fileSystem);
  Arcadia_Tests_assertTrue(thread, !Arcadia_FileSystem_isSymbolicLink(thread, fileSystem, path));
  Arcadia_ByteArrayBuilder* data = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, data, "a \r\nb\t", 6);
  Arcadia_FileSystem_setFileContents(thread, fileSystem, path, data);
  Arcadia_FileHandle* output = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  Arcadia_FileHandle_openStandardOutput(thread, output);
  Arcadia_Sanitizer_Statistics check = { 0 };
  Arcadia_Sanitizer_scan(thread, fileSystem, path, output, Arcadia_BooleanValue_False, &check);
  Arcadia_Tests_assertTrue(thread, check.inspected == 1 && check.changed == 1);
  Arcadia_ByteArrayBuilder* unchanged = Arcadia_FileSystem_getFileContents(thread, fileSystem, path);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArrayBuilder_isEqualTo_pn(thread, unchanged, "a \r\nb\t", 6));
  Arcadia_Sanitizer_Statistics fix = { 0 };
  Arcadia_Sanitizer_scan(thread, fileSystem, path, output, Arcadia_BooleanValue_True, &fix);
  Arcadia_Tests_assertTrue(thread, fix.inspected == 1 && fix.changed == 1);
  Arcadia_ByteArrayBuilder* updated = Arcadia_FileSystem_getFileContents(thread, fileSystem, path);
  Arcadia_Tests_assertTrue(thread, Arcadia_ByteArrayBuilder_isEqualTo_pn(thread, updated, "a\nb", 3));
  Arcadia_Sanitizer_Statistics clean = { 0 };
  Arcadia_Sanitizer_scan(thread, fileSystem, path, output, Arcadia_BooleanValue_False, &clean);
  Arcadia_Tests_assertTrue(thread, clean.inspected == 1 && clean.changed == 0);
  Arcadia_FileSystem_deleteRegularFile(thread, fileSystem, path);
}

static int
runNormalize
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&normalizeTest) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runFileRoundTrip
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&fileRoundTripTest) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "NormalizeBytes", &runNormalize },
    { "CheckAndFixFile", &runFileRoundTrip },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
