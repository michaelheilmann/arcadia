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
#include "Arcadia/FileSystem/Implementation/DefaultFileSystem.h"

#include "Arcadia/FileSystem/Include.h"
#include "Arcadia/FileSystem/Implementation/DefaultFileHandle.h"
#include "Arcadia/ARMS/Include.h"

#include "Arcadia/FileSystem/Implementation/deleteDirectoryFile.h"
#include "Arcadia/FileSystem/Implementation/deleteRegularFile.h"
#include "Arcadia/FileSystem/Implementation/getFileType.h"

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  #include "Arcadia/FileSystem/Windows/DirectoryIteratorWindows.h"
  #include "Arcadia/FileSystem/Windows/createTemporaryFile.h"
  #include "Arcadia/FileSystem/Windows/getLocalFolder.h"
  #include "Arcadia/FileSystem/Windows/getRoamingFolder.h"

#endif

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
    Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

  #include "Arcadia/FileSystem/Linux/DirectoryIteratorLinux.h"
  #include "Arcadia/FileSystem/Linux/createTemporaryFile.h"
  #include "Arcadia/FileSystem/Linux/getHomeFolder.h"

#endif

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h> // GetFileAttributes

#elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
      Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

  #include <string.h> // strlen
  #include <sys/stat.h> // stat
  #include <sys/types.h>
  #include <errno.h> // errno
  #include <unistd.h> // sysconf, getcwd, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO

  #include <linux/limits.h> // PATH_MAX
  #include <limits.h> // PATH_MAX

  #include <pwd.h> // struct passwd
  #include <fcntl.h>

#else

  #error("operating system not (yet) supported")

#endif

static void
Arcadia_DefaultFileSystem_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  );

static void
Arcadia_DefaultFileSystem_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystemDispatch* self
  );

static void
Arcadia_DefaultFileSystem_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  );

static void
Arcadia_DefaultFileSystem_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  );

static void
Arcadia_DefaultFileSystem_createDirectoryFileImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  );

static Arcadia_DirectoryIterator*
Arcadia_DefaultFileSystem_createDirectoryIteratorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  );

static Arcadia_FileHandle*
Arcadia_DefaultFileSystem_createFileHandleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  );

static void
Arcadia_DefaultFileSystem_createRegularFileImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  );

static void
Arcadia_DefaultFileSystem_deleteDirectoryFileImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  );

static void
Arcadia_DefaultFileSystem_deleteFileImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  );

static void
Arcadia_DefaultFileSystem_deleteRegularFileImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  );

static Arcadia_BooleanValue
Arcadia_DefaultFileSystem_directoryFileExistsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  );

static Arcadia_FilePath*
Arcadia_DefaultFileSystem_getConfigurationDirectoryImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  );

static Arcadia_FilePath*
Arcadia_DefaultFileSystem_getExecutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  );

static Arcadia_ByteArrayBuilder*
Arcadia_DefaultFileSystem_getFileContentsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  );

static Arcadia_FileType
Arcadia_DefaultFileSystem_getFileTypeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  );

static Arcadia_Natural64Value
Arcadia_DefaultFileSystem_getLastWriteTimeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  );

static Arcadia_FilePath*
Arcadia_DefaultFileSystem_getSaveFolderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  );

static Arcadia_FilePath*
Arcadia_DefaultFileSystem_getWorkingDirectoryImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  );

static Arcadia_BooleanValue
Arcadia_DefaultFileSystem_regularFileExistsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  );

static void
Arcadia_DefaultFileSystem_setFileContentsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path,
    Arcadia_ByteArrayBuilder * contents
  );

static Arcadia_DefaultFileSystem*
Arcadia_DefaultFileSystem_create
  (
    Arcadia_Thread* thread
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DefaultFileSystem_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_DefaultFileSystem_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DefaultFileSystem_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DefaultFileSystem_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DefaultFileSystem", Arcadia_DefaultFileSystem,
                         u8"Arcadia.FileSystme", Arcadia_FileSystem,
                         &_typeOperations);

static void
Arcadia_DefaultFileSystem_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DefaultFileSystem);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem

  self->stdin = INVALID_HANDLE_VALUE;
  self->stdout = INVALID_HANDLE_VALUE;
  self->stderr = INVALID_HANDLE_VALUE;
  self->stdin = GetStdHandle(STD_INPUT_HANDLE);
  if (self->stdin == INVALID_HANDLE_VALUE) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  self->stdout = GetStdHandle(STD_OUTPUT_HANDLE);
  if (self->stdout == INVALID_HANDLE_VALUE) {
    self->stdin = INVALID_HANDLE_VALUE;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  self->stderr = GetStdHandle(STD_ERROR_HANDLE);
  if (self->stderr == INVALID_HANDLE_VALUE) {
    self->stdout = INVALID_HANDLE_VALUE;
    self->stdin = INVALID_HANDLE_VALUE;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }

#endif

  Arcadia_LeaveConstructor(Arcadia_DefaultFileSystem);
}

static void
Arcadia_DefaultFileSystem_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystemDispatch* self
  )
{
  ((Arcadia_FileSystemDispatch*)self)->createDirectoryFile = (void (*)(Arcadia_Thread*, Arcadia_FileSystem*, Arcadia_FilePath*)) & Arcadia_DefaultFileSystem_createDirectoryFileImpl;
  ((Arcadia_FileSystemDispatch*)self)->createFileHandle = (Arcadia_FileHandle * (*)(Arcadia_Thread*, Arcadia_FileSystem*)) & Arcadia_DefaultFileSystem_createFileHandleImpl;
  ((Arcadia_FileSystemDispatch*)self)->createDirectoryIterator = (Arcadia_DirectoryIterator * (*)(Arcadia_Thread*, Arcadia_FileSystem*, Arcadia_FilePath*)) & Arcadia_DefaultFileSystem_createDirectoryIteratorImpl;
  ((Arcadia_FileSystemDispatch*)self)->createRegularFile = (void (*)(Arcadia_Thread*, Arcadia_FileSystem*, Arcadia_FilePath*)) & Arcadia_DefaultFileSystem_createRegularFileImpl;
  ((Arcadia_FileSystemDispatch*)self)->createTemporaryFile = (Arcadia_FilePath * (*)(Arcadia_Thread*, Arcadia_FileSystem*)) & Arcadia_DefaultFileSystem_createTemporaryFile;

  ((Arcadia_FileSystemDispatch*)self)->deleteDirectoryFile = (void (*)(Arcadia_Thread*, Arcadia_FileSystem*, Arcadia_FilePath*)) & Arcadia_DefaultFileSystem_deleteDirectoryFileImpl;
  ((Arcadia_FileSystemDispatch*)self)->deleteFile = (void (*)(Arcadia_Thread*, Arcadia_FileSystem*, Arcadia_FilePath*)) & Arcadia_DefaultFileSystem_deleteFileImpl;
  ((Arcadia_FileSystemDispatch*)self)->deleteRegularFile = (void (*)(Arcadia_Thread*, Arcadia_FileSystem*, Arcadia_FilePath*)) & Arcadia_DefaultFileSystem_deleteRegularFileImpl;

  ((Arcadia_FileSystemDispatch*)self)->directoryFileExists = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_FileSystem*, Arcadia_FilePath*)) & Arcadia_DefaultFileSystem_directoryFileExistsImpl;

  ((Arcadia_FileSystemDispatch*)self)->getConfigurationDirectory = (Arcadia_FilePath * (*)(Arcadia_Thread*, Arcadia_FileSystem*)) & Arcadia_DefaultFileSystem_getConfigurationDirectoryImpl;
  ((Arcadia_FileSystemDispatch*)self)->getExecutable = (Arcadia_FilePath * (*)(Arcadia_Thread*, Arcadia_FileSystem*)) & Arcadia_DefaultFileSystem_getExecutableImpl;
  ((Arcadia_FileSystemDispatch*)self)->getFileContents = (Arcadia_ByteArrayBuilder * (*)(Arcadia_Thread*, Arcadia_FileSystem*, Arcadia_FilePath*)) & Arcadia_DefaultFileSystem_getFileContentsImpl;
  ((Arcadia_FileSystemDispatch*)self)->getFileType = (Arcadia_FileType(*)(Arcadia_Thread*, Arcadia_FileSystem*, Arcadia_FilePath*)) & Arcadia_DefaultFileSystem_getFileTypeImpl;
  ((Arcadia_FileSystemDispatch*)self)->getLastWriteTime = (Arcadia_Natural64Value(*)(Arcadia_Thread*, Arcadia_FileSystem*, Arcadia_FilePath*)) & Arcadia_DefaultFileSystem_getLastWriteTimeImpl;
  ((Arcadia_FileSystemDispatch*)self)->getSaveDirectory = (Arcadia_FilePath * (*)(Arcadia_Thread*, Arcadia_FileSystem*)) & Arcadia_DefaultFileSystem_getSaveFolderImpl;
  ((Arcadia_FileSystemDispatch*)self)->getWorkingDirectory = (Arcadia_FilePath * (*)(Arcadia_Thread*, Arcadia_FileSystem*)) & Arcadia_DefaultFileSystem_getWorkingDirectoryImpl;

  ((Arcadia_FileSystemDispatch*)self)->regularFileExists = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_FileSystem*, Arcadia_FilePath*)) & Arcadia_DefaultFileSystem_regularFileExistsImpl;

  ((Arcadia_FileSystemDispatch*)self)->setFileContents = (void (*)(Arcadia_Thread*, Arcadia_FileSystem*, Arcadia_FilePath*, Arcadia_ByteArrayBuilder*)) & Arcadia_DefaultFileSystem_setFileContentsImpl;
}

static void
Arcadia_DefaultFileSystem_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_DefaultFileSystem_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  )
{
#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  self->stderr = INVALID_HANDLE_VALUE;
  self->stdout = INVALID_HANDLE_VALUE;
  self->stdin = INVALID_HANDLE_VALUE;
#endif
}

static void
Arcadia_DefaultFileSystem_createDirectoryFileImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  )
{
  Arcadia_String* nativePath = Arcadia_FilePath_toNative(thread, path, Arcadia_BooleanValue_True);

#if defined(_DEBUG) && 0
  Arcadia_logf(Arcadia_LogFlags_Info, "%s:%d: creating directory `", __FILE__, __LINE__);
  Arcadia_logf(Arcadia_LogFlags_Info, nativePath->immutableUTF8String->bytes);
  Arcadia_logf(Arcadia_LogFlags_Info, "`\n");
#endif

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
  if (FALSE == CreateDirectory(Arcadia_String_getBytes(thread, nativePath), NULL)) {
    if (ERROR_ALREADY_EXISTS != GetLastError()) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
      Arcadia_Thread_jump(thread);
    }
  }
#elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux
  if (-1 == mkdir(Arcadia_String_getBytes(thread, nativePath), 0777)) {
    if (errno == EEXIST) {
      errno = 0;
      struct stat sb;
      if (-1 == stat(Arcadia_String_getBytes(thread, nativePath), &sb)) {
        errno = 0;
        Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
        Arcadia_Thread_jump(thread);
      }
      if (!S_ISDIR(sb.st_mode)) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
        Arcadia_Thread_jump(thread);
      }
    } else {
      errno = 0;
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
      Arcadia_Thread_jump(thread);
    }
  }
#else
  #error("operating system not (yet) supported")
#endif
}

static Arcadia_DirectoryIterator*
Arcadia_DefaultFileSystem_createDirectoryIteratorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  )
{
#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
  return (Arcadia_DirectoryIterator*)Arcadia_DirectoryIteratorWindows_create(thread, path);
#elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux
  return (Arcadia_DirectoryIterator*)Arcadia_DirectoryIteratorLinux_create(thread, path);
#else
  #error("operating system not (yet) supported")
#endif
}

static Arcadia_FileHandle*
Arcadia_DefaultFileSystem_createFileHandleImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  )
{
  return (Arcadia_FileHandle*)Arcadia_DefaultFileHandle_create(thread, (Arcadia_FileSystem*)self);
}

static void
Arcadia_DefaultFileSystem_createRegularFileImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  )
{/*Intentionally empty.*/}

static void
Arcadia_DefaultFileSystem_deleteDirectoryFileImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  )
{ Arcadia_DefaultFileSystem_deleteDirectoryFileHelper(thread, self, path); }

static void
Arcadia_DefaultFileSystem_deleteFileImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  )
{
  if (Arcadia_FileType_Directory == Arcadia_FileSystem_getFileType(thread, (Arcadia_FileSystem*)self, path)) {
    Arcadia_FileSystem_deleteDirectoryFile(thread, (Arcadia_FileSystem*)self, path);
  } else {
    Arcadia_FileSystem_deleteRegularFile(thread, (Arcadia_FileSystem*)self, path);
  }
}

static void
Arcadia_DefaultFileSystem_deleteRegularFileImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  )
{ Arcadia_DefaultFileSystem_deleteRegularFileHelper(thread, self, path); }

static Arcadia_BooleanValue
Arcadia_DefaultFileSystem_directoryFileExistsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  )
{
  Arcadia_String* nativePathString = Arcadia_FilePath_toNative(thread, path, Arcadia_BooleanValue_True);
#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  DWORD dwFileAttributes = GetFileAttributes(Arcadia_String_getBytes(thread, nativePathString));
  return ((dwFileAttributes != INVALID_FILE_ATTRIBUTES) &&
          (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  struct stat stat;
  if(-1 == lstat(Arcadia_String_getBytes(thread, nativePathString), &stat)) {
    switch (errno) {
      case EOVERFLOW:
      case ENOMEM: {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      } break;
    };
    return Arcadia_BooleanValue_False;
  }
  return S_IFDIR == (stat.st_mode & S_IFMT);
#else
  #error("environment not (yet) supported")
#endif
}

static Arcadia_FilePath*
Arcadia_DefaultFileSystem_getConfigurationDirectoryImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  )
{
#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
  Arcadia_FilePath* filePath = Arcadia_DefaultFileSystem_getLocalFolderHelper(thread, self);
  Arcadia_FilePath_append(thread, filePath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Michael Heilmann's Arcadia")));
  return filePath;
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  Arcadia_FilePath* filePath = Arcadia_DefaultFileSystem_getHomeFolderHelper(thread, self);
  Arcadia_FilePath_append(thread, filePath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Michael Heilmann's Arcadia/Configurations")));
  return filePath;
#else
  #error("environment not (yet) supported")
#endif
}

static Arcadia_FilePath*
Arcadia_DefaultFileSystem_getExecutableImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  )
{
#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
  HMODULE module = GetModuleHandleA(NULL);
  if (!module) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue n = MAX_PATH;
  void* p = Arcadia_Memory_allocateUnmanaged(thread, n);
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    do {
      DWORD m = GetModuleFileNameA(module, p, n);
      // There are three cases:
      // m = 0: The function failed.
      // m = n: The buffer is too small.
      //        As MAX_PATH is most likely to small for modern file systems,
      //        we resort to dynamic memory allocation.
      // m < n: m denotes the number of characters copied without the zero terminator.
      if (m == 0) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      } else if (m == n) {
        Arcadia_SizeValue hi, lo;
        Arcadia_safeAddFullSizeValue(thread, 64, MAX_PATH, &hi, &lo);
        if (hi) {
          lo = Arcadia_SizeValue_Maximum;
        }
        if (lo == n) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        }
        n = lo;
        Arcadia_Memory_reallocateUnmanaged(thread, (void**)&p, n);
      } else {
        Arcadia_FilePath* path = Arcadia_FilePath_parseWindows(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, p, n))));
        Arcadia_Memory_deallocateUnmanaged(thread, p);
        p = NULL;
        Arcadia_Thread_popJumpTarget(thread);
        return path;
      }
    } while (true);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    if (p) {
      Arcadia_Memory_deallocateUnmanaged(thread, p);
      p = NULL;
    }
    Arcadia_Thread_jump(thread);
  }
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  // space for `PATH_MAX` plus 1 character
  char p[PATH_MAX + 1];
  // `readlink` will write up to `PATH_MAX` plus 1 chars and will not append a zero terminator.
  // The path will not be truncated it must be smaller than or equal to `PATH_MAX`.
  ssize_t n = readlink("/proc/self/exe", p, PATH_MAX);
  if (-1 == n) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  // `readlink` returns a string in some encoding.
  // We assume that this encoding is UTF-8 at the moment.
  return Arcadia_FilePath_parseUnix(thread, Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, p, n))));
#else
  #error("environment not (yet) supported")
#endif
}

static Arcadia_ByteArrayBuilder*
Arcadia_DefaultFileSystem_getFileContentsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  )
{
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, (Arcadia_FileSystem*)self);
  Arcadia_FileHandle_openForReading(thread, fileHandle, path);
  Arcadia_ByteArrayBuilder* byteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  char p[5012]; size_t n;
  do {
    Arcadia_FileHandle_read(thread, fileHandle, p, 5012, &n);
    if (n > 0) {
      Arcadia_ByteArrayBuilder_insertBackBytes(thread, byteBuffer, p, n);
    }
  } while (n > 0);
  Arcadia_FileHandle_close(thread, fileHandle);
  fileHandle = NULL;
  return byteBuffer;
}

static Arcadia_FileType
Arcadia_DefaultFileSystem_getFileTypeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  )
{ return Arcadia_DefaultFileSystem_getFileTypeHelper(thread, self, path); }

static Arcadia_Natural64Value
Arcadia_DefaultFileSystem_getLastWriteTimeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  )
{
  Arcadia_String* pathString = Arcadia_FilePath_toNative(thread, path, Arcadia_BooleanValue_True);
#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
  HANDLE hFile = CreateFile(Arcadia_String_getBytes(thread, pathString), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
  if (INVALID_HANDLE_VALUE == hFile) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  FILETIME ftCreate, ftAccess, ftWrite;
  if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)) {
    CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  CloseHandle(hFile);
  hFile = INVALID_HANDLE_VALUE;
  SYSTEMTIME sTime;
  if (!FileTimeToSystemTime(&ftWrite, &sTime)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  return ((Arcadia_Natural64Value)ftWrite.dwHighDateTime) << 32
       | ((Arcadia_Natural64Value)ftWrite.dwLowDateTime);
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  struct stat sb;
  if (-1 == lstat(Arcadia_String_getBytes(thread, pathString), &sb)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  return (Arcadia_Natural64Value)sb.st_mtime;
#else
  #error("environment not (yet) supported")
#endif
}

static Arcadia_FilePath*
Arcadia_DefaultFileSystem_getSaveFolderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  )
{
#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
  Arcadia_FilePath* filePath = Arcadia_DefaultFileSystem_getRoamingFolderHelper(thread, self);
  Arcadia_FilePath_append(thread, filePath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Michael Heilmann's Arcadia")));
  return filePath;
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  Arcadia_FilePath* filePath  = Arcadia_DefaultFileSystem_getHomeFolderHelper(thread, self);
  Arcadia_FilePath_append(thread, filePath, Arcadia_FilePath_parseGeneric(thread, Arcadia_String_createFromCxxString(thread, u8"Michael Heilmann's Arcadia/Saves")));
  return filePath;
#else
  #error("environment not (yet) supported")
#endif
}

static Arcadia_FilePath*
Arcadia_DefaultFileSystem_getWorkingDirectoryImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self
  )
{
#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
  DWORD dwBufferSize = GetCurrentDirectory(0, NULL);
  if (0 == dwBufferSize) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  char* pBuffer = NULL;
  if (Arcadia_ARMS_MemoryManager_allocate(Arcadia_ARMS_getSlabMemoryManager(), (void**)&pBuffer, dwBufferSize)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
    Arcadia_Thread_jump(thread);
  }
  if (!GetCurrentDirectory(dwBufferSize, pBuffer)) {
    Arcadia_ARMS_MemoryManager_deallocate(Arcadia_ARMS_getSlabMemoryManager(), pBuffer);
    pBuffer = NULL;
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_String* filePathString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, pBuffer, dwBufferSize - 1)));
    Arcadia_FilePath* filePath = Arcadia_FilePath_parseWindows(thread, filePathString);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ARMS_MemoryManager_deallocate(Arcadia_ARMS_getSlabMemoryManager(), pBuffer);
    pBuffer = NULL;
    return filePath;
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ARMS_MemoryManager_deallocate(Arcadia_ARMS_getSlabMemoryManager(), pBuffer);
    pBuffer = NULL;
    Arcadia_Thread_jump(thread);
  }
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  Arcadia_StaticAssert(PATH_MAX < Arcadia_SizeValue_Maximum, "environment not (yet) supported");
  #define CWD_CAPACITY (PATH_MAX+1)
  char cwd[CWD_CAPACITY];
  if (!getcwd(cwd, CWD_CAPACITY)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_String* filePathString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, cwd, strlen(cwd))));
  Arcadia_FilePath* filePath = Arcadia_FilePath_parseUnix(thread, filePathString);
  return filePath;
#else
  #error("environment not (yet) supported")
#endif
}

static Arcadia_BooleanValue
Arcadia_DefaultFileSystem_regularFileExistsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path
  )
{
  Arcadia_String* nativePathString = Arcadia_FilePath_toNative(thread, path, Arcadia_BooleanValue_True);
#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  DWORD dwFileAttributes = GetFileAttributes(Arcadia_String_getBytes(thread, nativePathString));
  return ((dwFileAttributes != INVALID_FILE_ATTRIBUTES) &&
         !(dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  struct stat stat;
  if(-1 == lstat(Arcadia_String_getBytes(thread, nativePathString), &stat)) {
    switch (errno) {
      case EOVERFLOW:
      case ENOMEM: {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      } break;
    };
    return Arcadia_BooleanValue_False;
  }
  return S_IFREG == (stat.st_mode & S_IFMT) || S_IFLNK == (stat.st_mode & S_IFMT);
#else
  #error("environment not (yet) supported")
#endif
}

static void
Arcadia_DefaultFileSystem_setFileContentsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileSystem* self,
    Arcadia_FilePath* path,
    Arcadia_ByteArrayBuilder * contents
  )
{
  Arcadia_FileHandle* fileHandle = Arcadia_FileSystem_createFileHandle(thread, (Arcadia_FileSystem*)self);
  Arcadia_FileHandle_openForWriting(thread, fileHandle, path);
  Arcadia_SizeValue bytesToWrite = contents->sz,
                    bytesWritten = 0;
  while (bytesToWrite > 0) {
    Arcadia_SizeValue bytesWrittenNow;
    Arcadia_FileHandle_write(thread, fileHandle, contents->p + bytesWritten, bytesToWrite, &bytesWrittenNow);
    bytesWritten += bytesWrittenNow;
    bytesToWrite -= bytesWrittenNow;
  }
  Arcadia_FileHandle_close(thread, fileHandle);
  fileHandle = NULL;
}

static Arcadia_DefaultFileSystem*
Arcadia_DefaultFileSystem_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_DefaultFileSystem);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_DefaultFileSystem);
}

static Arcadia_DefaultFileSystem* g_instance = NULL;

static void
Arcadia_DefaultFileSystem_destroyCallback
  (
    void* observer,
    void* observed
  )
{ g_instance = NULL; }

Arcadia_DefaultFileSystem*
Arcadia_DefaultFileSystem_getOrCreate
  (
    Arcadia_Thread* thread
  )
{
  if (!g_instance) {
    Arcadia_DefaultFileSystem* instance = Arcadia_DefaultFileSystem_create(thread);
    Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)instance, NULL, (void (*)(void*, Arcadia_Object*)) &Arcadia_DefaultFileSystem_destroyCallback);
    g_instance = instance;
  }
  return g_instance;
}
