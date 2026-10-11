// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#define ARCADIA_FILESYSTEM_MODULE (1)
#include "Arcadia/FileSystem/Implementation/deleteRegularFile.h"

#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem

  #include <errno.h>
  #include <unistd.h>

#else

  #error("environment not (yet) supported")

#endif

void
Arcadia_DefaultFileSystem_deleteRegularFileHelper
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  )
{
  Arcadia_String* pathString = Arcadia_FilePath_toNative(thread, path, Arcadia_BooleanValue_True);
#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  BOOL result = DeleteFile(Arcadia_String_getBytes(thread, pathString));
  if (!result) {
    DWORD error = GetLastError();
    if (ERROR_ACCESS_DENIED == error) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_AccessDenied);
      Arcadia_Thread_jump(thread);
    } else if (ERROR_FILE_NOT_FOUND == error) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
      Arcadia_Thread_jump(thread);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
      Arcadia_Thread_jump(thread);
    }
  }
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  int result = unlink(Arcadia_String_getBytes(thread, pathString));
  if (-1 == result) {
    switch (errno) {
      case EACCES:
      case EBUSY:
      case EPERM:
      case EROFS:
      case EISDIR:
      case ELOOP:
      case ENAMETOOLONG:
      case ENOENT:
      case ENOTDIR: {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_AccessDenied);
        Arcadia_Thread_jump(thread);
      } break;
      case EIO: {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
        Arcadia_Thread_jump(thread);
      } break;
      case ENOMEM: {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
        Arcadia_Thread_jump(thread);
      } break;
      case EFAULT:
      default: {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      } break;
    };
  }
#else
  #error("environment not (yet) supported")
#endif
}
