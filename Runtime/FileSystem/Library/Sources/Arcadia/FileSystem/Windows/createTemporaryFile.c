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
#include "Arcadia/FileSystem/Windows/createTemporaryFile.h"

#include "Arcadia/FileSystem/FilePath.h"

#include <stdlib.h> // malloc, free

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #include <Shlobj.h>

#endif

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

Arcadia_FilePath*
Arcadia_DefaultFileSystem_createTemporaryFile
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  )
{
  DWORD required = GetTempPathW(0, NULL);
  if (!required) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  WCHAR* temporaryDirectory = malloc(sizeof(WCHAR) * required);
  if (!temporaryDirectory) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  DWORD length = GetTempPathW(required, temporaryDirectory);
  if (!length || length >= required) {
    free(temporaryDirectory);
    temporaryDirectory = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }

  WCHAR temporaryPath[MAX_PATH + 1];
  if (!GetTempFileNameW(temporaryDirectory, L"Arc", 0, temporaryPath)) {
    free(temporaryDirectory);
    temporaryDirectory = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  free(temporaryDirectory);
  temporaryDirectory = NULL;

  int numberOfBytes = WideCharToMultiByte(CP_UTF8, 0, temporaryPath, -1, NULL, 0, NULL, NULL);
  if (!numberOfBytes) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  char* utf8Path = malloc(sizeof(char) * numberOfBytes);
  if (!utf8Path) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  if (!WideCharToMultiByte(CP_UTF8, 0, temporaryPath, -1, utf8Path, numberOfBytes, NULL, NULL)) {
    free(utf8Path);
    utf8Path = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_FilePath* filePath = NULL;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    filePath = Arcadia_FilePath_parseWindows(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, utf8Path, numberOfBytes - 1))));
    Arcadia_Thread_popJumpTarget(thread);
    free(utf8Path);
    utf8Path = NULL;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    free(utf8Path);
    utf8Path = NULL;
    Arcadia_Thread_jump(thread);
  }
  return filePath;
}

#endif
