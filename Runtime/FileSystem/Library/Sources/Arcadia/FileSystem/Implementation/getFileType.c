// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#define ARCADIA_FILESYSTEM_MODULE (1)
#include "Arcadia/FileSystem/Implementation/getFileType.h"

#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem

  #include <errno.h>
  #include <sys/stat.h>

#else

  #error("environment not (yet) supported")

#endif

static Arcadia_FileType
getFileType1
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_String* path
  )
{
#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem

  DWORD dwFileAttributes = GetFileAttributes(Arcadia_String_getBytes(thread, path));
  if (dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
    DWORD dwLastError = GetLastError();
    SetLastError(0);
    if (dwLastError == ERROR_FILE_NOT_FOUND || dwLastError == ERROR_PATH_NOT_FOUND) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
      Arcadia_Thread_jump(thread);
    } else if (dwLastError == ERROR_INVALID_NAME ||
               dwLastError == ERROR_INVALID_DRIVE ||
               dwLastError == ERROR_NOT_READY ||
               dwLastError == ERROR_INVALID_PARAMETER ||
               dwLastError == ERROR_BAD_PATHNAME ||
               dwLastError == ERROR_BAD_NETPATH) {
      // This should not happen.
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    } else {
      // This should not happen.
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  } else {
    if (FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & dwFileAttributes)) {
      return Arcadia_FileType_Directory;
    } else {
      return Arcadia_FileType_Regular;
    }
  }

#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem

  struct stat t;
  int result = stat(Arcadia_String_getBytes(thread, path), &t);
  if (0 == result) {
    if (0 != S_ISDIR(t.st_mode))
      return Arcadia_FileType_Directory;
    else if (0 != S_ISREG(t.st_mode))
      return Arcadia_FileType_Regular;
    else
      return Arcadia_FileType_Unknown;
  } else {
    if (errno == ENOTDIR || errno == ENOENT || errno == ENAMETOOLONG || errno == EACCES) {
      // ENOTDIR, ENOENT, ENAMETOOLONG, and EACCES all imply that the file does not exist or is not accessible due to permission constraints.
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
      Arcadia_Thread_jump(thread);
    } else if (errno == EOVERFLOW) {
      // EOVERFLOW indicates that some file exists but is not accessible due to technical constraints.
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
      Arcadia_Thread_jump(thread);
    } else /*if (errno == ELOOP || errno == ENOMEN || errno == EFAULT || errno == EBADF)*/ {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  }

#else

  #error("environment not (yet) supported")

#endif
}

Arcadia_FileType
Arcadia_DefaultFileSystem_getFileTypeHelper
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  )
{ return getFileType1(thread, self, Arcadia_FilePath_toNative(thread, path, Arcadia_BooleanValue_True)); }
