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
#include "Arcadia/FileSystem/Windows/DirectoryIteratorWindows.h"

#include "Arcadia/FileSystem/Include.h"

static void
Arcadia_DirectoryIteratorWindows_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorWindows* self
  );

static void
Arcadia_DirectoryIteratorWindows_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorWindowsDispatch* self
  );

static void
Arcadia_DirectoryIteratorWindows_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorWindows* self
  );

static Arcadia_FilePath*
Arcadia_DirectoryIteratorWindows_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorWindows* self
  );

static Arcadia_BooleanValue
Arcadia_DirectoryIteratorWindows_hasValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorWindows* self
  );

static void
Arcadia_DirectoryIteratorWindows_nextValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorWindows* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_DirectoryIteratorWindows_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_DirectoryIteratorWindows_destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_DirectoryIteratorWindows_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.DirectoryIteratorWindows", Arcadia_DirectoryIteratorWindows,
                         u8"Arcadia.DirectoryIterator", Arcadia_DirectoryIterator,
                         &_typeOperations);

static void
Arcadia_DirectoryIteratorWindows_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorWindows* self
  )
{
  static uint32_t zeroTerminator = '\0';
  Arcadia_EnterConstructor(Arcadia_DirectoryIteratorWindows);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->path = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_FilePath_getType(thread));
  self->handle = INVALID_HANDLE_VALUE;
  Arcadia_StringBuilder* queryStringBuilder = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBack(thread, queryStringBuilder, Arcadia_Value_makeObjectReferenceValue(Arcadia_FilePath_toNative(thread, self->path, Arcadia_BooleanValue_False)));
  // (1) Append '/' if the path does not end with '/'.
  if (!Arcadia_StringBuilder_endsWith_pn(thread, queryStringBuilder, u8"\\", sizeof(u8"\\") - 1)) {
    Arcadia_StringBuilder_insertBackCxxString(thread, queryStringBuilder, u8"\\");
  }
  // (2) Append '*\0'.
  Arcadia_StringBuilder_insertBackCxxString(thread, queryStringBuilder, u8"*");
  Arcadia_StringBuilder_insertBackCodePoints(thread, queryStringBuilder, &zeroTerminator, 1);
  Arcadia_String* queryString = Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(queryStringBuilder));
  SetLastError(0);
  self->handle = FindFirstFileA(Arcadia_String_getBytes(thread, queryString), &self->data);
  if (INVALID_HANDLE_VALUE == self->handle) {
    DWORD dwLastError = GetLastError();
    SetLastError(0);
    if (dwLastError == ERROR_FILE_NOT_FOUND) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_NotFound);
      Arcadia_Thread_jump(thread);
    } else {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
  }
  Arcadia_LeaveConstructor(Arcadia_DirectoryIteratorWindows);
}

static void
Arcadia_DirectoryIteratorWindows_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorWindowsDispatch* self
  )
{
  ((Arcadia_DirectoryIteratorDispatch*)self)->getValue = (Arcadia_FilePath * (*)(Arcadia_Thread*, Arcadia_DirectoryIterator*)) & Arcadia_DirectoryIteratorWindows_getValue;
  ((Arcadia_DirectoryIteratorDispatch*)self)->hasValue = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_DirectoryIterator*)) & Arcadia_DirectoryIteratorWindows_hasValue;
  ((Arcadia_DirectoryIteratorDispatch*)self)->nextValue = (void (*)(Arcadia_Thread*, Arcadia_DirectoryIterator*)) & Arcadia_DirectoryIteratorWindows_nextValue;
}

static void
Arcadia_DirectoryIteratorWindows_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorWindows* self
  )
{
  if (self->handle) {
    FindClose(self->handle);
    self->handle = INVALID_HANDLE_VALUE;
  }
}

static Arcadia_FilePath*
Arcadia_DirectoryIteratorWindows_getValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorWindows* self
  )
{
  if (INVALID_HANDLE_VALUE == self->handle) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  } else {
    Arcadia_String* fileNameString = Arcadia_String_create(thread, Arcadia_Value_makeRuntimeUTF8StringValue(Arcadia_RuntimeUTF8String_create(thread, self->data.cFileName, strlen(self->data.cFileName))));
    return Arcadia_FilePath_parseNative(thread, fileNameString);
  }
}

static Arcadia_BooleanValue
Arcadia_DirectoryIteratorWindows_hasValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorWindows* self
  )
{ return INVALID_HANDLE_VALUE != self->handle; }

static void
Arcadia_DirectoryIteratorWindows_nextValue
  (
    Arcadia_Thread* thread,
    Arcadia_DirectoryIteratorWindows* self
  )
{
  if (self->handle != INVALID_HANDLE_VALUE) {
    SetLastError(0);
    BOOL result = FindNextFileA(self->handle, &self->data);
    if (!result) {
      FindClose(self->handle);
      self->handle = INVALID_HANDLE_VALUE;
      if (GetLastError() == ERROR_NO_MORE_FILES) {
        SetLastError(0);
      } else {
        SetLastError(0);
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
    }
  }
}

Arcadia_DirectoryIteratorWindows*
Arcadia_DirectoryIteratorWindows_create
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* path
  )
{
  _Arcadia_BeginCreate(Arcadia_DirectoryIteratorWindows);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)path);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_DirectoryIteratorWindows);
}
