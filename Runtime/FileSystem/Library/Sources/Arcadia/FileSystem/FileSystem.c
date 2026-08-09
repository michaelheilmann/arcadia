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
#include "Arcadia/FileSystem/FileSystem.h"

#include "Arcadia/FileSystem/Implementation/DefaultFileSystem.h"
#include "Arcadia/FileSystem/Include.h"

static void
Arcadia_FileSystem_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  );

static void
Arcadia_FileSystem_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystemDispatch* self
  );

static void
Arcadia_FileSystem_visit
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  );

static void
Arcadia_FileSystem_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_FileSystem_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_FileSystem_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_FileSystem_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_FileSystem_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.FileSystem", Arcadia_FileSystem,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_FileSystem_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  )
{
  Arcadia_EnterConstructor(Arcadia_FileSystem);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_FileSystem);
}

static void
Arcadia_FileSystem_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystemDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_FileSystem_visit
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_FileSystem_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  )
{/*Intentionally empty.*/}

void
Arcadia_FileSystem_createDirectoryFile
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  )
{ Arcadia_VirtualCall(Arcadia_FileSystem, createDirectoryFile, self, path); }

void
Arcadia_FileSystem_createRegularFile
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  )
{ Arcadia_VirtualCall(Arcadia_FileSystem, createRegularFile, self, path); }

Arcadia_DirectoryIterator*
Arcadia_FileSystem_createDirectoryIterator
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileSystem, createDirectoryIterator, self, path); }

Arcadia_FileHandle*
Arcadia_FileSystem_createFileHandle
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileSystem, createFileHandle, self); }

void
Arcadia_FileSystem_deleteDirectoryFile
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  )
{ Arcadia_VirtualCall(Arcadia_FileSystem, deleteDirectoryFile, self, path); }

void
Arcadia_FileSystem_deleteFile
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  )
{ Arcadia_VirtualCall(Arcadia_FileSystem, deleteFile, self, path); }

void
Arcadia_FileSystem_deleteRegularFile
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  )
{ Arcadia_VirtualCall(Arcadia_FileSystem, deleteRegularFile, self, path); }

Arcadia_BooleanValue
Arcadia_FileSystem_directoryFileExists
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileSystem, directoryFileExists, self, path); }

Arcadia_FilePath*
Arcadia_FileSystem_getWorkingDirectory
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileSystem, getWorkingDirectory, self); }

Arcadia_FilePath*
Arcadia_FileSystem_getConfigurationDirectory
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileSystem, getConfigurationDirectory, self); }

Arcadia_FilePath*
Arcadia_FileSystem_getExecutable
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileSystem, getExecutable, self); }

Arcadia_ByteArrayBuilder*
Arcadia_FileSystem_getFileContents
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileSystem, getFileContents, self, path); }

Arcadia_FileType
Arcadia_FileSystem_getFileType
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileSystem, getFileType, self, path); }

Arcadia_Natural64Value
Arcadia_FileSystem_getLastWriteTime
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileSystem, getLastWriteTime, self, path); }

Arcadia_FilePath*
Arcadia_FileSystem_getSaveDirectory
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileSystem, getSaveDirectory, self); }

Arcadia_BooleanValue
Arcadia_FileSystem_regularFileExists
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileSystem, regularFileExists, self, path); }

void
Arcadia_FileSystem_setFileContents
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path,
    Arcadia_ByteArrayBuilder* contents
  )
{ Arcadia_VirtualCall(Arcadia_FileSystem, setFileContents, self, path, contents); }

Arcadia_FileSystem*
Arcadia_FileSystem_getOrCreate
  (
    Arcadia_Thread* thread
  )
{ return (Arcadia_FileSystem*)Arcadia_DefaultFileSystem_getOrCreate(thread); }

void
Arcadia_FileSystem_createDirectoryFiles
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self,
    Arcadia_FilePath* path
  )
{
  // If the path is relative, then make it absolute.
  if (Arcadia_FilePath_isRelative(thread, path)) {
    Arcadia_FilePath* temporary = Arcadia_FileSystem_getWorkingDirectory(thread, (Arcadia_FileSystem*)self);
    Arcadia_FilePath_append(thread, temporary, path);
    path = temporary;
  }
  // Ensure each path component exists.
  Arcadia_SizeValue n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)path->fileNames);
  if (!n) { 
    // As the path is absolute and has no filenames, it refers to a file system root.
    // We do not create file system roots.
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  // Begin at the topmost component.
  Arcadia_FilePath* temporary = Arcadia_FilePath_clone(thread, path);
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)temporary->fileNames);
  for (Arcadia_SizeValue i = 0; i < n; ++i) {
    Arcadia_String* suffixString = (Arcadia_String*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, (Arcadia_List*)path->fileNames, i,
                                                                                                  _Arcadia_String_getType(thread));
    Arcadia_FilePath* suffix = Arcadia_FilePath_parseGeneric(thread, suffixString);
    Arcadia_FilePath_append(thread, temporary, suffix);
    // Raises Arcadia_Status_OperationFailed if
    // - a component exists but is not a directory file
    // - a component could not be created
    Arcadia_FileSystem_createDirectoryFile(thread, Arcadia_FileSystem_getOrCreate(thread), temporary);
  }
}

Arcadia_FilePath*
Arcadia_FileSystem_createTemporaryFile
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_FileSystem, createTemporaryFile, self); }
