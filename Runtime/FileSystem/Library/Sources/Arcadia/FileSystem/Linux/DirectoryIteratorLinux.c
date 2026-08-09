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
#include "Arcadia/FileSystem/Linux/DirectoryIteratorLinux.h"

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/FileSystem/FilePath.h"

#include <string.h>
#include <errno.h>
#if (Arcadia_Configuration_OperatingSystem == Arcadia_Configuration_OperatingSystem_Macos)
	#include <sys/types.h>
	#include <sys/dir.h>
#else
	#include <dirent.h>
#endif

static void
Arcadia_DirectoryIteratorLinux_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorLinux* self
  );

static void
Arcadia_DirectoryIteratorLinux_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorLinuxDispatch* self
  );

static void
Arcadia_DirectoryIteratorLinux_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorLinux* self
  );

static Arcadia_FilePath*
Arcadia_DirectoryIteratorLinux_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorLinux* self
  );

static Arcadia_BooleanValue
Arcadia_DirectoryIteratorLinux_hasValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorLinux* self
  );

static void
Arcadia_DirectoryIteratorLinux_nextValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorLinux* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DirectoryIteratorLinux_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_DirectoryIteratorLinux_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DirectoryIteratorLinux_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DirectoryIteratorLinux", Arcadia_DirectoryIteratorLinux,
                         u8"Arcadia.DirectoryIterator", Arcadia_DirectoryIterator,
                         &_typeOperations);

static void
Arcadia_DirectoryIteratorLinux_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorLinux* self
  )
{
  Arcadia_EnterConstructor(Arcadia_DirectoryIteratorLinux);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  // Clear errno.
  errno = 0;
  // Open directory.
  Arcadia_FilePath* path = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_FilePath_getType(thread));
  Arcadia_String* pathString = Arcadia_FilePath_toNative(thread, path, Arcadia_BooleanValue_True);
  self->dir = opendir(Arcadia_String_getBytes(thread, pathString));
  if (!self->dir) {
    switch (errno) {
      case ENOENT: {
        errno = 0;
        Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
        Arcadia_Thread_jump(thread);
      } break;
      case EMFILE:
      case ENOMEM: {
        errno = 0;
        Arcadia_Thread_setStatus(thread, Arcadia_Status_AllocationFailed);
        Arcadia_Thread_jump(thread);
      } break;
      case EACCES:
      case EBADF:
      default: {
        errno = 0;
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      } break;
    };
  }
  // Read first directory entry.
  self->dirent = readdir(self->dir);
  if (!self->dirent) {
    closedir(self->dir);
    self->dir = NULL;
    if (errno) {
      errno = 0;
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  }
  Arcadia_LeaveConstructor(Arcadia_DirectoryIteratorLinux);
}

static void
Arcadia_DirectoryIteratorLinux_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorLinuxDispatch* self
  )
{
  ((Arcadia_DirectoryIteratorDispatch*)self)->getValue = (Arcadia_FilePath* (*)(Arcadia_Thread*, Arcadia_DirectoryIterator*)) & Arcadia_DirectoryIteratorLinux_getValue;
  ((Arcadia_DirectoryIteratorDispatch*)self)->hasValue = (Arcadia_BooleanValue (*)(Arcadia_Thread*, Arcadia_DirectoryIterator*)) &Arcadia_DirectoryIteratorLinux_hasValue;
  ((Arcadia_DirectoryIteratorDispatch*)self)->nextValue = (void (*)(Arcadia_Thread*, Arcadia_DirectoryIterator*)) &Arcadia_DirectoryIteratorLinux_nextValue;
}

Arcadia_DirectoryIteratorLinux*
Arcadia_DirectoryIteratorLinux_create
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* path
  )
{
  _Arcadia_BeginCreate(Arcadia_DirectoryIteratorLinux);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)path);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DirectoryIteratorLinux);
}

static void
Arcadia_DirectoryIteratorLinux_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorLinux* self
  )
{
  if (self->dir) {
    if (self->dirent) {
      self->dirent = NULL;
    }
    closedir(self->dir);
    self->dir = NULL;
  }
}

static Arcadia_FilePath*
Arcadia_DirectoryIteratorLinux_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorLinux* self
  )
{
  if (self->dirent) {
    Arcadia_String* fileNameString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, self->dirent->d_name, strlen(self->dirent->d_name))));
    return Arcadia_FilePath_parseNative(thread, fileNameString);
  } else {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
}

static Arcadia_BooleanValue
Arcadia_DirectoryIteratorLinux_hasValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorLinux* self
  )
{
  return NULL != self->dirent;
}

static void
Arcadia_DirectoryIteratorLinux_nextValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorLinux* self
  )
{
  errno = 0;
  self->dirent = readdir(self->dir);
  if (!self->dirent) {
    closedir(self->dir);
    self->dir = NULL;
    if (errno) {
      errno = 0;
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  }
}
