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

#define ARCADIA_LOGGING_MODULE (1)
#include "Arcadia/Logging/FileLog.h"

#include "Arcadia/FileSystem/Include.h"

static void
Arcadia_FileLog_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileLog* self
  );

static void
Arcadia_FileLog_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileLogDispatch* self
  );

static void
Arcadia_FileLog_visit
  (
    Arcadia_Thread* thread,
    Arcadia_FileLog* self
  );

static void
Arcadia_FileLog_informationImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileLog* self,
    Arcadia_String* message
  );

static void
Arcadia_FileLog_warningImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileLog* self,
    Arcadia_String* message
  );

static void
Arcadia_FileLog_errorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileLog* self,
    Arcadia_String* message
  );

static void
writeBytes
  (
    Arcadia_Thread* thread,
    Arcadia_FileLog* self,
    const Arcadia_Natural8Value* bytes,
    Arcadia_SizeValue numberOfBytes
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_FileLog_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_FileLog_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_FileLog_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.FileLog", Arcadia_FileLog,
                         u8"Arcadia.Log", Arcadia_Log,
                         &_typeOperations);

static void
Arcadia_FileLog_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileLog* self
  )
{
  Arcadia_EnterConstructor(Arcadia_FileLog);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  self->fileHandle = Arcadia_FileSystem_createFileHandle(thread, fileSystem);
  Arcadia_FileHandle_openStandardOutput(thread, self->fileHandle);
  Arcadia_LeaveConstructor(Arcadia_FileLog);
}

static void
Arcadia_FileLog_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileLogDispatch* self
  )
{
  ((Arcadia_LogDispatch*)self)->information = (void (*)(Arcadia_Thread*, Arcadia_Log*, Arcadia_String*)) & Arcadia_FileLog_informationImpl;
  ((Arcadia_LogDispatch*)self)->warning = (void (*)(Arcadia_Thread*, Arcadia_Log*, Arcadia_String*)) & Arcadia_FileLog_warningImpl;
  ((Arcadia_LogDispatch*)self)->error = (void (*)(Arcadia_Thread*, Arcadia_Log*, Arcadia_String*)) & Arcadia_FileLog_errorImpl;
}

static void
Arcadia_FileLog_visit
  (
    Arcadia_Thread* thread,
    Arcadia_FileLog* self
  )
{
  if (self->fileHandle) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->fileHandle);
  }
}

static void
Arcadia_FileLog_informationImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileLog* self,
    Arcadia_String* message
  )
{
  const char* p = Arcadia_String_getBytes(thread, message);
  Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(thread, message);
  writeBytes(thread, self, p, n);
}

static void
Arcadia_FileLog_warningImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileLog* self,
    Arcadia_String* message
  )
{
  const char* p = Arcadia_String_getBytes(thread, message);
  Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(thread, message);
  writeBytes(thread, self, p, n);
}

static void
Arcadia_FileLog_errorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileLog* self,
    Arcadia_String* message
  )
{
  const char* p = Arcadia_String_getBytes(thread, message);
  Arcadia_SizeValue n = Arcadia_String_getNumberOfBytes(thread, message);
  writeBytes(thread, self, p, n);
}

static void
writeBytes
  (
    Arcadia_Thread* thread,
    Arcadia_FileLog* self,
    const Arcadia_Natural8Value* bytes,
    Arcadia_SizeValue numberOfBytes
  )
{
  Arcadia_SizeValue written = 0, toWrite = numberOfBytes;
  Arcadia_SizeValue numberOfAttempts = 0, maximumNumberOfAttempts = 3;
  while (toWrite) {
    Arcadia_SizeValue writtenNow = 0;
    Arcadia_FileHandle_write(thread, self->fileHandle, bytes + written, toWrite, &writtenNow);
    if (writtenNow == 0) {
      if (numberOfAttempts == maximumNumberOfAttempts) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
        Arcadia_Thread_jump(thread);
      }
      numberOfAttempts++;
    } else {
      numberOfAttempts = 0;
      toWrite -= writtenNow;
      written += writtenNow;
    }
  }
}

Arcadia_FileLog*
Arcadia_FileLog_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_FileLog);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_FileLog);
}
