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

#define ARCADIA_LANGUAGES_MODULE (1)
#include "Arcadia/Languages/InputFileManager.h"

#include "Arcadia/Languages/InputFileManager/PhysicalInputFile.h"

static void
Arcadia_Languages_InputFileManager_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManager* self
  );

static void
Arcadia_Languages_InputFileManager_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManagerDispatch* self
  );

static void
Arcadia_Languages_InputFileManager_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManager* self
  );

static void
Arcadia_Languages_InputFileManager_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManager* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Languages_InputFileManager_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Languages_InputFileManager_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Languages_InputFileManager_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Languages_InputFileManager_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Languages.InputFileManager", Arcadia_Languages_InputFileManager,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Languages_InputFileManager_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManager* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Languages_InputFileManager);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->diagnostics = (Arcadia_Languages_Diagnostics*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_Languages_Diagnostics_getType(thread));
  self->files = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_LeaveConstructor(Arcadia_Languages_InputFileManager);
}

static void
Arcadia_Languages_InputFileManager_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManagerDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Languages_InputFileManager_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManager* self
  )
{
  if (self->diagnostics) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->diagnostics);
  }
  if (self->files) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->files);
  }
}

static void
Arcadia_Languages_InputFileManager_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManager* self
  )
{/*Intentionally empty.*/}

Arcadia_Languages_InputFileManager*
Arcadia_Languages_InputFileManager_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics
  )
{
  _Arcadia_BeginCreate(Arcadia_Languages_InputFileManager);
  if (diagnostics) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnostics); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Languages_InputFileManager);
}

Arcadia_Languages_InputFile*
Arcadia_Languages_InputFileManager_createPhysicalInputFile
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManager* self,
    Arcadia_String* diagnosticName,
    Arcadia_FilePath* path
  )
{ return (Arcadia_Languages_InputFile*)Arcadia_Languages_PhysicalInputFile_create(thread, diagnosticName, path); }

Arcadia_UnicodeCodePointReader*
Arcadia_Languages_InputFileManager_getFileReader
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManager* self,
    Arcadia_FilePath* path
  )
{
  Arcadia_Languages_InputFile* inputFile = Arcadia_Languages_InputFile_create(thread, Arcadia_FilePath_toNative(thread, path, Arcadia_BooleanValue_False), path);
  Arcadia_ByteArray* inputFileContents = Arcadia_Languages_InputFile_getContents(thread, inputFile);
  return (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, inputFileContents));
}
