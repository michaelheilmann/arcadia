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

#include "Arcadia/Sanitizer/Sanitizer.h"
#include <string.h>

static void
writeBytes
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* file,
    void const* bytes,
    Arcadia_SizeValue length
  )
{
  Arcadia_Natural8Value const* p = (Arcadia_Natural8Value const*)bytes;
  while (length) {
    Arcadia_SizeValue written = 0;
    Arcadia_FileHandle_write(thread, file, p, length, &written);
    if (!written) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
      Arcadia_Thread_jump(thread);
    }
    p += written;
    length -= written;
  }
}

static void
writeText
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* file,
    char const* text
  )
{
  writeBytes(thread, file, text, strlen(text));
}

static void
writeNumber
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* file,
    Arcadia_SizeValue number
  )
{
  char buffer[3 * sizeof(Arcadia_SizeValue) + 1];
  Arcadia_SizeValue index = sizeof(buffer);
  do {
    buffer[--index] = (char)('0' + (number % 10));
    number /= 10;
  } while (number);
  writeBytes(thread, file, buffer + index, sizeof(buffer) - index);
}

static void
writePath
  (
    Arcadia_Thread* thread,
    Arcadia_FileHandle* output,
    Arcadia_FilePath* path
  )
{
  Arcadia_String* string = Arcadia_FilePath_toGeneric(thread, path);
  writeBytes(thread, output, Arcadia_String_getBytes(thread, string), Arcadia_String_getNumberOfBytes(thread, string));
}

static void
recordProblem
  (
    Arcadia_SizeValue* firstAffectedLine,
    Arcadia_SizeValue line
  )
{
  if (!*firstAffectedLine) {
    *firstAffectedLine = line;
  }
}

Arcadia_BooleanValue
Arcadia_Sanitizer_normalize
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* input,
    Arcadia_ByteArrayBuilder* output,
    Arcadia_SizeValue* firstAffectedLine
  )
{
  Arcadia_SizeValue length = Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, input);
  Arcadia_Natural8Value const* bytes = Arcadia_ByteArrayBuilder_getBytes(thread, input);
  Arcadia_SizeValue line = 1;
  Arcadia_SizeValue pendingStart = 0;
  Arcadia_SizeValue pendingCount = 0;
  *firstAffectedLine = 0;
  Arcadia_ByteArrayBuilder_clear(thread, output);
  for (Arcadia_SizeValue i = 0; i < length; ++i) {
    Arcadia_Natural8Value byte = bytes[i];
    if (byte == ' ' || byte == '\t') {
      if (!pendingCount) {
        pendingStart = i;
      }
      ++pendingCount;
    } else if (byte == '\r' || byte == '\n') {
      if (pendingCount) {
        recordProblem(firstAffectedLine, line);
        pendingCount = 0;
      }
      if (byte == '\r') {
        recordProblem(firstAffectedLine, line);
        if (i + 1 < length && bytes[i + 1] == '\n') {
          ++i;
        }
      }
      Arcadia_ByteArrayBuilder_insertBackBytes(thread, output, "\n", 1);
      ++line;
    } else {
      if (pendingCount) {
        Arcadia_ByteArrayBuilder_insertBackBytes(thread, output, bytes + pendingStart, pendingCount);
        pendingCount = 0;
      }
      Arcadia_ByteArrayBuilder_insertBackBytes(thread, output, bytes + i, 1);
    }
  }
  if (pendingCount) {
    recordProblem(firstAffectedLine, line);
  }
  return *firstAffectedLine ? Arcadia_BooleanValue_True : Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
nameEquals
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* path,
    char const* literal
  )
{
  Arcadia_String* name = Arcadia_FilePath_toGeneric(thread, path);
  return Arcadia_String_isEqualTo_pn(thread, name, literal, strlen(literal));
}

static Arcadia_BooleanValue
skipDirectory
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* childName
  )
{
  char const* excluded[] = {
    ".", "..", ".git", ".vs", ".venv", ".Dependencies",
    "build", ".build", "CMakeFiles", "node_modules", "__pycache__"
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(excluded) / sizeof(excluded[0]); ++i) {
    if (nameEquals(thread, childName, excluded[i])) {
      return Arcadia_BooleanValue_True;
    }
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
isSourceFile
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* path
  )
{
  // The suffix is checked using bytes so multi-part extensions are supported.
  Arcadia_String* name = Arcadia_FilePath_toGeneric(thread, path);
  Arcadia_Natural8Value const* bytes = Arcadia_String_getBytes(thread, name);
  Arcadia_SizeValue length = Arcadia_String_getNumberOfBytes(thread, name);
  char const* suffixes[] = {
    ".txt", ".c", ".h", ".i", ".tl", ".te", ".pdl", ".adl",
    ".ddl", ".md", ".cmake", ".c.g", ".h.g"
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(suffixes) / sizeof(suffixes[0]); ++i) {
    Arcadia_SizeValue n = strlen(suffixes[i]);
    if (length >= n && !memcmp(bytes + length - n, suffixes[i], n)) {
      return Arcadia_BooleanValue_True;
    }
  }
  return Arcadia_BooleanValue_False;
}

static Arcadia_BooleanValue
isBinary
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* bytes
  )
{
  Arcadia_Natural8Value const* p = Arcadia_ByteArrayBuilder_getBytes(thread, bytes);
  Arcadia_SizeValue size = Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, bytes);
  for (Arcadia_SizeValue i = 0; i < size; ++i) {
    if (p[i] == 0) {
      return Arcadia_BooleanValue_True;
    }
  }
  return Arcadia_BooleanValue_False;
}

static void
scanRegularFile
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* fileSystem,
    Arcadia_FilePath* path,
    Arcadia_FileHandle* output,
    Arcadia_BooleanValue fix,
    Arcadia_Sanitizer_Statistics* statistics
  )
{
  Arcadia_ByteArrayBuilder* original = Arcadia_FileSystem_getFileContents(thread, fileSystem, path);
  ++statistics->inspected;
  if (isBinary(thread, original)) {
    ++statistics->skippedBinary;
    writeText(thread, output, "skipped binary: ");
    writePath(thread, output, path);
    writeText(thread, output, "\n");
    return;
  }
  Arcadia_ByteArrayBuilder* normalized = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_SizeValue firstAffectedLine = 0;
  if (Arcadia_Sanitizer_normalize(thread, original, normalized, &firstAffectedLine)) {
    ++statistics->changed;
    writePath(thread, output, path);
    writeText(thread, output, ":");
    writeNumber(thread, output, firstAffectedLine);
    writeText(thread, output, fix ? ": fixed whitespace\n" : ": whitespace differs\n");
    if (fix) {
      Arcadia_FileSystem_setFileContents(thread, fileSystem, path, normalized);
    }
  }
}

static void
scanPath
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* fileSystem,
    Arcadia_FilePath* path,
    Arcadia_FileHandle* output,
    Arcadia_BooleanValue fix,
    Arcadia_Sanitizer_Statistics* statistics,
    Arcadia_BooleanValue explicitFile
  )
{
  if (Arcadia_FileSystem_isSymbolicLink(thread, fileSystem, path)) {
    ++statistics->skippedLinks;
    writeText(thread, output, "skipped link: ");
    writePath(thread, output, path);
    writeText(thread, output, "\n");
    return;
  }
  Arcadia_FileType type = Arcadia_FileSystem_getFileType(thread, fileSystem, path);
  if (type == Arcadia_FileType_Regular) {
    if (explicitFile || isSourceFile(thread, path)) {
      scanRegularFile(thread, fileSystem, path, output, fix, statistics);
    }
  } else if (type == Arcadia_FileType_Directory) {
    Arcadia_DirectoryIterator* it = Arcadia_FileSystem_createDirectoryIterator(thread, fileSystem, path);
    while (Arcadia_DirectoryIterator_hasValue(thread, it)) {
      Arcadia_FilePath* name = Arcadia_DirectoryIterator_getValue(thread, it);
      if (!skipDirectory(thread, name)) {
        Arcadia_FilePath* child = Arcadia_FilePath_clone(thread, path);
        Arcadia_FilePath_append(thread, child, name);
        scanPath(thread, fileSystem, child, output, fix, statistics, Arcadia_BooleanValue_False);
      }
      Arcadia_DirectoryIterator_nextValue(thread, it);
    }
  }
}

void
Arcadia_Sanitizer_scan
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* fileSystem,
    Arcadia_FilePath* path,
    Arcadia_FileHandle* output,
    Arcadia_BooleanValue fix,
    Arcadia_Sanitizer_Statistics* statistics
  )
{
  scanPath(thread, fileSystem, path, output, fix, statistics, Arcadia_BooleanValue_True);
}
