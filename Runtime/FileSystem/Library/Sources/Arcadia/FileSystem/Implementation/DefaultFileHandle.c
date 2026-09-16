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
#include "Arcadia/FileSystem/Implementation/DefaultFileHandle.h"

#include "Arcadia/FileSystem/Implementation/DefaultFileSystem.h"
#include "Arcadia/FileSystem/Include.h"

#define Flags_OpenRead (1)

#define Flags_OpenWrite (2)

static void
Arcadia_DefaultFileHandle_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  );

static void
Arcadia_DefaultFileHandle_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandleDispatch* self
  );

static void
Arcadia_DefaultFileHandle_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  );

static void
Arcadia_DefaultFileHandle_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  );

static Arcadia_BooleanValue
Arcadia_DefaultFileHandle_isClosedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle const* self
  );

static Arcadia_BooleanValue
Arcadia_DefaultFileHandle_isOpenedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle const* self
  );

static Arcadia_BooleanValue
Arcadia_DefaultFileHandle_isOpenedForReadingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle const* self
  );

static Arcadia_BooleanValue
Arcadia_DefaultFileHandle_isOpenedForWritingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle const* self
  );

static void
Arcadia_DefaultFileHandle_closeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  );

static void
Arcadia_DefaultFileHandle_openImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self,
    Arcadia_FilePath* path,
    Arcadia_FileAccessMode fileAccessMode,
    Arcadia_ExistingFilePolicy existingFilePolicy,
    Arcadia_NonExistingFilePolicy nonExistingFilePolicy
  );

static void
Arcadia_DefaultFileHandle_openForReadingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self,
    Arcadia_FilePath* path
  );

static void
Arcadia_DefaultFileHandle_openForWritingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self,
    Arcadia_FilePath* path
  );

static void
Arcadia_DefaultFileHandle_readImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self,
    void* bytes,
    Arcadia_SizeValue bytesToRead,
    Arcadia_SizeValue* bytesRead
  );

static void
Arcadia_DefaultFileHandle_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self,
    void const* bytes,
    Arcadia_SizeValue bytesToWrite,
    Arcadia_SizeValue* bytesWritten
  );

static void
Arcadia_DefaultFileHandle_openStandardErrorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  );

static void
Arcadia_DefaultFileHandle_openStandardInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  );

static void
Arcadia_DefaultFileHandle_openStandardOutputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DefaultFileHandle_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_DefaultFileHandle_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_DefaultFileHandle_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DefaultFileHandle_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DefaultFileHandle", Arcadia_DefaultFileHandle,
                         u8"Arcadia.FileHandle", Arcadia_FileHandle,
                         &_typeOperations);

static void
Arcadia_DefaultFileHandle_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DefaultFileHandle);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->fileSystem = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_FileSystem_getType(thread));
  Arcadia_Object_lock(thread, (Arcadia_Object*)self->fileSystem);
#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows
  self->fd = INVALID_HANDLE_VALUE;
#elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
      Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin
  self->fd = 0;
#else
  #error("environment not (yet) supported")
#endif
  self->flags = 0;
  //
  Arcadia_LeaveConstructor(Arcadia_DefaultFileHandle);
}


static void
Arcadia_DefaultFileHandle_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandleDispatch* self
  )
{
  ((Arcadia_FileHandleDispatch*)self)->close = (void (*)(Arcadia_Thread*, Arcadia_FileHandle*)) & Arcadia_DefaultFileHandle_closeImpl;
  ((Arcadia_FileHandleDispatch*)self)->isClosed = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_FileHandle const*)) & Arcadia_DefaultFileHandle_isClosedImpl;
  ((Arcadia_FileHandleDispatch*)self)->isOpened = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_FileHandle const*)) & Arcadia_DefaultFileHandle_isOpenedImpl;
  ((Arcadia_FileHandleDispatch*)self)->isOpenedForReading = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_FileHandle const*)) & Arcadia_DefaultFileHandle_isOpenedForReadingImpl;
  ((Arcadia_FileHandleDispatch*)self)->isOpenedForWriting = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_FileHandle const*)) & Arcadia_DefaultFileHandle_isOpenedForWritingImpl;
  ((Arcadia_FileHandleDispatch*)self)->openForReading = (void (*)(Arcadia_Thread*, Arcadia_FileHandle*, Arcadia_FilePath*)) & Arcadia_DefaultFileHandle_openForReadingImpl;
  ((Arcadia_FileHandleDispatch*)self)->openForWriting = (void (*)(Arcadia_Thread*, Arcadia_FileHandle*, Arcadia_FilePath*)) & Arcadia_DefaultFileHandle_openForWritingImpl;
  ((Arcadia_FileHandleDispatch*)self)->openStandardError = (void (*)(Arcadia_Thread*, Arcadia_FileHandle*)) & Arcadia_DefaultFileHandle_openStandardErrorImpl;
  ((Arcadia_FileHandleDispatch*)self)->openStandardInput = (void (*)(Arcadia_Thread*, Arcadia_FileHandle*)) & Arcadia_DefaultFileHandle_openStandardInputImpl;
  ((Arcadia_FileHandleDispatch*)self)->openStandardOutput = (void (*)(Arcadia_Thread*, Arcadia_FileHandle*)) & Arcadia_DefaultFileHandle_openStandardOutputImpl;
  ((Arcadia_FileHandleDispatch*)self)->read = (void (*)(Arcadia_Thread*, Arcadia_FileHandle*, void*, Arcadia_SizeValue, Arcadia_SizeValue*)) & Arcadia_DefaultFileHandle_readImpl;
  ((Arcadia_FileHandleDispatch*)self)->write = (void (*)(Arcadia_Thread*, Arcadia_FileHandle*, void const*, Arcadia_SizeValue, Arcadia_SizeValue*)) & Arcadia_DefaultFileHandle_writeImpl;
}

static void
Arcadia_DefaultFileHandle_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  )
{
#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  if (self->fd != INVALID_HANDLE_VALUE) {
    if (self->fd != ((Arcadia_DefaultFileSystem*)self->fileSystem)->stdin &&
        self->fd != ((Arcadia_DefaultFileSystem*)self->fileSystem)->stderr &&
        self->fd != ((Arcadia_DefaultFileSystem*)self->fileSystem)->stdout) {
      CloseHandle(self->fd);
    }
    self->fd = INVALID_HANDLE_VALUE;
  }

#elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
      Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

  if (self->fd) {
    if (self->fd != STDIN_FILENO && self->fd != STDERR_FILENO && self->fd != STDOUT_FILENO) {
      close(self->fd);
    }
    self->fd = 0;
  }

#else

  #error("environment not (yet) supported")

#endif
  self->flags = 0;
  Arcadia_Object_unlock(thread, (Arcadia_Object*)self->fileSystem);
  self->fileSystem = NULL;
}

static void
Arcadia_DefaultFileHandle_visit
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  )
{
  Arcadia_Object_visit(thread, (Arcadia_Object*)self->fileSystem);
}

static void
Arcadia_DefaultFileHandle_closeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  )
{
#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  if (self->fd != INVALID_HANDLE_VALUE) {
    if (self->fd != ((Arcadia_DefaultFileSystem*)self->fileSystem)->stdin &&
      self->fd != ((Arcadia_DefaultFileSystem*)self->fileSystem)->stderr &&
      self->fd != ((Arcadia_DefaultFileSystem*)self->fileSystem)->stdout) {
      CloseHandle(self->fd);
    }
    self->fd = INVALID_HANDLE_VALUE;
  }

#elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
      Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

  if (self->fd) {
    if (self->fd != STDIN_FILENO && self->fd != STDERR_FILENO && self->fd != STDOUT_FILENO) {
      close(self->fd);
    }
    self->fd = 0;
  }

#else

  #error("environment not (yet) supported")

#endif

  self->flags = 0;
}

static Arcadia_BooleanValue
Arcadia_DefaultFileHandle_isClosedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle const* self
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return 0 == ((Flags_OpenRead | Flags_OpenWrite) & self->flags);
}

static Arcadia_BooleanValue
Arcadia_DefaultFileHandle_isOpenedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle const* self
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return 0 != (Flags_OpenWrite & self->flags);
}

static Arcadia_BooleanValue
Arcadia_DefaultFileHandle_isOpenedForReadingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle const* self
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return 0 != (Flags_OpenRead & self->flags);
}

static Arcadia_BooleanValue
Arcadia_DefaultFileHandle_isOpenedForWritingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle const* self
  )
{
  if (!self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Flags_OpenWrite == (Flags_OpenWrite & self->flags);
}

static void
Arcadia_DefaultFileHandle_openImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self,
    Arcadia_FilePath* path,
    Arcadia_FileAccessMode fileAccessMode,
    Arcadia_ExistingFilePolicy existingFilePolicy,
    Arcadia_NonExistingFilePolicy nonExistingFilePolicy
  )
{
  // (1) Close existing backend.
  Arcadia_FileHandle_close(thread, (Arcadia_FileHandle*)self);
  // (2) Obtain native path.
  Arcadia_String* nativePath = Arcadia_FilePath_toNative(thread, path, Arcadia_BooleanValue_True);

#if defined(_DEBUG)
  Arcadia_logf(Arcadia_LogFlags_Info, "opening file `");
  Arcadia_logf(Arcadia_LogFlags_Info, nativePath->immutableUTF8String->bytes);
  Arcadia_logf(Arcadia_LogFlags_Info, "`\n");
#endif

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  DWORD dwDesiredAccess = 0;
  if (!fileAccessMode) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (Arcadia_FileAccessMode_Read == (fileAccessMode & Arcadia_FileAccessMode_Read)) {
    dwDesiredAccess |= GENERIC_READ;
  }
  if (Arcadia_FileAccessMode_Write == (fileAccessMode & Arcadia_FileAccessMode_Write)) {
    dwDesiredAccess |= GENERIC_WRITE;
  }
  DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
  DWORD dwCreationDisposition = 0;
  typedef struct Policy {
    Arcadia_ExistingFilePolicy existingFilePolicy;
    Arcadia_NonExistingFilePolicy nonExistingFilePolicy;
    DWORD dwCreationDispositionModifier;
    DWORD dwDesiredAccessModifier;
  } Policy;
  static const Policy policies[] = {
    // CREATE_NEW is a conditional flag which fails
    // if the file exists and succeeds if the file
    // does not exist and is successfully created.
    // It is not used.
    { Arcadia_ExistingFilePolicy_Truncate,
      Arcadia_NonExistingFilePolicy_Create,
      CREATE_ALWAYS,
      0 },
    { Arcadia_ExistingFilePolicy_Retain,
      Arcadia_NonExistingFilePolicy_Create,
      OPEN_ALWAYS,
      0 },
    { Arcadia_ExistingFilePolicy_Truncate,
      Arcadia_NonExistingFilePolicy_Fail,
      TRUNCATE_EXISTING,
      GENERIC_WRITE },
    { Arcadia_ExistingFilePolicy_Retain,
      Arcadia_NonExistingFilePolicy_Fail,
      OPEN_EXISTING,
      0 },
  };
  Policy const* policy = NULL;
  for (size_t i = 0, n = sizeof(policies) / sizeof(Policy); i < n; ++i) {
    Policy const* e = &(policies[i]);
    if (e->existingFilePolicy == existingFilePolicy &&
        e->nonExistingFilePolicy == nonExistingFilePolicy) {
      policy = e;
      break;
    }
  }
  if (policy == NULL) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  dwCreationDisposition |= policy->dwCreationDispositionModifier;
  dwDesiredAccess |= policy->dwDesiredAccessModifier;

  self->fd = CreateFile(Arcadia_String_getBytes(thread, nativePath), dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
  if (INVALID_HANDLE_VALUE == self->fd) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }

#elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
      Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

  int flags = 0;
  if (Arcadia_FileAccessMode_ReadWrite == (fileAccessMode & Arcadia_FileAccessMode_ReadWrite)) {
    flags |= O_RDWR;
  } else if (Arcadia_FileAccessMode_Read == (fileAccessMode & Arcadia_FileAccessMode_Read)) {
    flags |= O_RDONLY;
  } else if (Arcadia_FileAccessMode_Write == (fileAccessMode & Arcadia_FileAccessMode_Write)) {
    flags |= O_WRONLY;
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  typedef struct Policy {
    Arcadia_ExistingFilePolicy existingFilePolicy;
    Arcadia_NonExistingFilePolicy nonExistingFilePolicy;
    int flagsModifier;
  } Policy;
  static const Policy policies[] = {
    { Arcadia_ExistingFilePolicy_Truncate,
      Arcadia_NonExistingFilePolicy_Create,
      O_CREAT | O_TRUNC },
    { Arcadia_ExistingFilePolicy_Retain,
      Arcadia_NonExistingFilePolicy_Create,
      O_CREAT },
    { Arcadia_ExistingFilePolicy_Truncate,
      Arcadia_NonExistingFilePolicy_Fail,
      O_TRUNC },
    { Arcadia_ExistingFilePolicy_Retain,
      Arcadia_NonExistingFilePolicy_Fail,
      0 },
  };
  Policy const* policy = NULL;
  for (size_t i = 0, n = sizeof(policies) / sizeof(Policy); i < n; ++i) {
    Policy const* e = &(policies[i]);
    if (e->existingFilePolicy == existingFilePolicy &&
        e->nonExistingFilePolicy == nonExistingFilePolicy) {
      policy = e;
      break;
    }
  }
  if (NULL == policy) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  flags |= policy->flagsModifier;
  if (nonExistingFilePolicy == Arcadia_NonExistingFilePolicy_Create) {
    self->fd = open(Arcadia_String_getBytes(thread, nativePath), flags, S_IRWXU);
  } else {
    self->fd = open(Arcadia_String_getBytes(thread, nativePath), flags);
  }
  if (0 == self->fd) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }

#else

  #error("environment not (yet) supported")

#endif

  if (Arcadia_FileAccessMode_Read == (fileAccessMode & Arcadia_FileAccessMode_Read)) {
    self->flags |= Flags_OpenRead;
  }
  if (Arcadia_FileAccessMode_Write == (fileAccessMode & Arcadia_FileAccessMode_Write)) {
    self->flags |= Flags_OpenWrite;
  }

}

static void
Arcadia_DefaultFileHandle_openForReadingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self,
    Arcadia_FilePath* path
  )
{
  Arcadia_DefaultFileHandle_openImpl(thread, self, path, Arcadia_FileAccessMode_Read, Arcadia_ExistingFilePolicy_Retain, Arcadia_NonExistingFilePolicy_Fail);
}

static void
Arcadia_DefaultFileHandle_openForWritingImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self,
    Arcadia_FilePath* path
  )
{
  Arcadia_DefaultFileHandle_openImpl(thread, self, path, Arcadia_FileAccessMode_Write, Arcadia_ExistingFilePolicy_Truncate, Arcadia_NonExistingFilePolicy_Create);
}

static void
Arcadia_DefaultFileHandle_openStandardErrorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  )
{
  Arcadia_FileHandle_close(thread, (Arcadia_FileHandle*)self);

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  self->fd = ((Arcadia_DefaultFileSystem*)self->fileSystem)->stderr;

#elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
      Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

  self->fd = STDERR_FILENO;

#else

  #error("environment not (yet) supported")

#endif

  self->flags |= Flags_OpenWrite;
}

static void
Arcadia_DefaultFileHandle_openStandardInputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  )
{
  Arcadia_FileHandle_close(thread, (Arcadia_FileHandle*)self);

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  self->fd = ((Arcadia_DefaultFileSystem*)self->fileSystem)->stdin;

#elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
      Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

  self->fd = STDIN_FILENO;

#else

  #error("environment not (yet) supported")

#endif

  self->flags |= Flags_OpenRead;
}

static void
Arcadia_DefaultFileHandle_openStandardOutputImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self
  )
{
  Arcadia_FileHandle_close(thread, (Arcadia_FileHandle*)self);

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  self->fd = ((Arcadia_DefaultFileSystem*)self->fileSystem)->stdout;

#elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
      Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

  self->fd = STDOUT_FILENO;

#else

  #error("environment not (yet) supported")

#endif

  self->flags |= Flags_OpenWrite;
}

static void
Arcadia_DefaultFileHandle_readImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self,
    void* bytes,
    Arcadia_SizeValue bytesToRead,
    Arcadia_SizeValue* bytesRead
  )
{
  if (!bytes || !bytesRead) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_FileHandle_isOpenedForReading(thread, (Arcadia_FileHandle*)self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!bytesToRead) {
    *bytesRead = 0;
    return;
  }

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  DWORD bytesReadNow;
  if (!ReadFile(self->fd, bytes, bytesToRead, &bytesReadNow, NULL)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  *bytesRead = bytesReadNow;

#elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
      Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

  ssize_t bytesReadNow = read(self->fd, bytes, bytesToRead > SSIZE_MAX ? SSIZE_MAX : bytesToRead);
  if (bytesReadNow == -1) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  *bytesRead = bytesReadNow;

#else

  #error("environment not (yet) supported")

#endif
}

static void
Arcadia_DefaultFileHandle_writeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DefaultFileHandle* self,
    void const* bytes,
    Arcadia_SizeValue bytesToWrite,
    Arcadia_SizeValue* bytesWritten
  )
{
  if (!bytes || !bytesWritten) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (!Arcadia_FileHandle_isOpenedForWriting(thread, (Arcadia_FileHandle*)self)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }

#if Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Windows

  DWORD bytesWrittenNow;
  if (!WriteFile(self->fd, bytes, bytesToWrite, &bytesWrittenNow, NULL)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  *bytesWritten = bytesWrittenNow;

#elif Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Linux || \
      Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Cygwin

  ssize_t bytesWrittenNow;
  bytesWrittenNow = write(self->fd, bytes, bytesToWrite > SSIZE_MAX ? SSIZE_MAX : bytesToWrite);
  if (-1 == bytesWrittenNow) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  *bytesWritten = bytesWrittenNow;

#else

  #error("environment not (yet) supported")

#endif

}

Arcadia_DefaultFileHandle*
Arcadia_DefaultFileHandle_create
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* fileSystem
  )
{
  if (!fileSystem) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  _Arcadia_BeginCreate(Arcadia_DefaultFileHandle);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)fileSystem);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DefaultFileHandle);
}
