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
#include "Arcadia/FileSystem/Windows/getRoamingFolder.h"

#include "Arcadia/FileSystem/FilePath.h"

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #include <Shlobj.h>

#endif

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

Arcadia_FilePath*
Arcadia_DefaultFileSystem_getRoamingFolderHelper
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  )
{
  PWSTR p = NULL;
  HRESULT hResult = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &p);
  if (FAILED(hResult)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  int iResult;
  iResult = WideCharToMultiByte(CP_UTF8, 0, p, -1, NULL, 0, NULL, NULL);
  if (!iResult) {
    CoTaskMemFree(p);
    p = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  char* q = malloc(sizeof(char) * iResult);
  if (!q) {
    CoTaskMemFree(p);
    p = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  iResult = WideCharToMultiByte(CP_UTF8, 0, p, -1, q, iResult, NULL, NULL);
  CoTaskMemFree(p);
  p = NULL;
  if (!iResult) {
    free(q);
    q = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }

  Arcadia_FilePath* filePath = NULL;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    filePath = Arcadia_FilePath_parseWindows(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, q, iResult - 1))));
    Arcadia_Thread_popJumpTarget(thread);
    free(q);
    q = NULL;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    free(q);
    q = NULL;
    Arcadia_Thread_jump(thread);
  }
  return filePath;
}

#endif
