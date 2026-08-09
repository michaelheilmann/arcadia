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

#include "Arcadia.TemplateEngine/DependenciesContext.h"

#include "Arcadia/DDL/Include.h"

static void
DependenciesContext_constructImpl
  (
    Arcadia_Thread* thread,
    DependenciesContext* self
  );

static void
DependenciesContext_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    DependenciesContextDispatch* self
  );

static void
DependenciesContext_destruct
  (
    Arcadia_Thread* thread,
    DependenciesContext* self
  );

static void
DependenciesContext_visit
  (
    Arcadia_Thread* thread,
    DependenciesContext* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&DependenciesContext_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&DependenciesContext_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&DependenciesContext_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&DependenciesContext_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.TemplateEngine.DependenciesContext", DependenciesContext,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
DependenciesContext_constructImpl
  (
    Arcadia_Thread* thread,
    DependenciesContext* self
  )
{
  Arcadia_EnterConstructor(DependenciesContext);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }

  self->dependenciesFilePath = NULL;
  self->dependencies = (Arcadia_Map*)Arcadia_HashMap_create(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));

  Arcadia_LeaveConstructor(DependenciesContext);
}

static void
DependenciesContext_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    DependenciesContextDispatch* self
  )
{/*Intentionally empty.*/}

static void
DependenciesContext_destruct
  (
    Arcadia_Thread* thread,
    DependenciesContext* self
  )
{/*Intentionally empty.*/}

static void
DependenciesContext_visit
  (
    Arcadia_Thread* thread,
    DependenciesContext* self
  )
{
  if (self->dependencies) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->dependencies);
  }
  if (self->dependenciesFilePath) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->dependenciesFilePath);
  }
  if (self->dependencies) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->dependencies);
  }
}

DependenciesContext*
DependenciesContext_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(DependenciesContext);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(DependenciesContext);
}

static void
writePath
  (
    Arcadia_Thread* thread,
    DependenciesContext* self,
    Arcadia_String* pathString,
    Arcadia_Unicode_Encoder* targetEncoder,
    Arcadia_ByteArrayBuilder* targetByteBuffer
  )
{
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, pathString));
  while (Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    if (codePoint == ' ') {
      Arcadia_Natural32Value t[] = { '\\', codePoint };
      Arcadia_Unicode_Encoder_encodeCodePoints(thread, targetEncoder, t, 2, targetByteBuffer);
    } else {
      Arcadia_Unicode_Encoder_encodeCodePoints(thread, targetEncoder, &codePoint, 1, targetByteBuffer);
    }
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
  }
}

void
DependenciesContext_write
  (
    Arcadia_Thread* thread,
    DependenciesContext* self
  )
{
  if (!self->dependenciesFilePath) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_List* tosList = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  // (1) Create a buffer to write to.
  Arcadia_ByteArrayBuilder* byteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Unicode_Encoder* writer = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  // (2) Iterate over the keys.
  Arcadia_List* froms = Arcadia_Map_getKeys(thread, self->dependencies);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)froms); i < n; ++i) {
    // (2.1) Get the "from" string.
    Arcadia_String* from = (Arcadia_String*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, froms, i, _Arcadia_String_getType(thread));
    // (2.2) Get the "tos" list.
    Arcadia_Set* tosSet = (Arcadia_Set*)Arcadia_Map_getObjectReferenceValueChecked(thread, self->dependencies, Arcadia_Value_makeObjectReferenceValue(from), _Arcadia_Set_getType(thread));
    Arcadia_Collection_clear(thread, (Arcadia_Collection*)tosList);
    Arcadia_Set_getAll(thread, tosSet, tosList);
    // (2.3) Write the "from" string.
    writePath(thread, self, from, writer, byteBuffer);
    Arcadia_Unicode_Encoder_encodeString(thread, writer, Arcadia_String_createFromCxxString(thread, u8" :"), byteBuffer);
    for (Arcadia_SizeValue j = 0, m = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)tosList); j < m; ++j) {
      // (2.4) Get the "to" string.
      Arcadia_String* to = (Arcadia_String*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, tosList, j, _Arcadia_String_getType(thread));
      // (2.5) Write the "to" string.
      Arcadia_Unicode_Encoder_encodeString(thread, writer, Arcadia_String_createFromCxxString(thread, u8" "), byteBuffer);
      writePath(thread, self, to, writer, byteBuffer);
    }
    Arcadia_Unicode_Encoder_encodeString(thread, writer, Arcadia_String_createFromCxxString(thread, u8"\r\n"), byteBuffer);
  }
  {
    // (1) Ensure the path is absolute.
    if (!Arcadia_FilePath_isAbsolute(thread, self->dependenciesFilePath)) {
      Arcadia_FilePath* workingDirectory = Arcadia_FileSystem_getWorkingDirectory(thread, Arcadia_FileSystem_getOrCreate(thread));
      Arcadia_FilePath_append(thread, workingDirectory, self->dependenciesFilePath);
      self->dependenciesFilePath = workingDirectory;
    }
    // (2) Remove the filename component. If there is no file name, this fails.
    Arcadia_FilePath* directories = Arcadia_FilePath_clone(thread, self->dependenciesFilePath);
    Arcadia_List_removeBack(thread, (Arcadia_List*)directories->fileNames, 1);
    // (3) Ensure all directories exist.
    Arcadia_FileSystem_createDirectoryFiles(thread, Arcadia_FileSystem_getOrCreate(thread), directories);
  }
  // (3) Write the file contents.
  Arcadia_FileSystem_setFileContents(thread, Arcadia_FileSystem_getOrCreate(thread), self->dependenciesFilePath, byteBuffer);
}

void
DependencyContext_onFile
  (
    Arcadia_Thread* thread,
    DependenciesContext* self,
    Arcadia_FilePath* path
  )
{
  Arcadia_String* fromString = Arcadia_FilePath_toNative(thread, Arcadia_FilePath_getFullPath(thread, path), Arcadia_BooleanValue_False);
  Arcadia_Value temporary = Arcadia_Map_get(thread, self->dependencies, Arcadia_Value_makeObjectReferenceValue(fromString));
  if (Arcadia_Value_isVoidValue(&temporary)) {
    Arcadia_Set* toStringSet = (Arcadia_Set*)Arcadia_HashSet_create(thread);
    Arcadia_Map_set(thread, self->dependencies, Arcadia_Value_makeObjectReferenceValue(fromString),
                                                Arcadia_Value_makeObjectReferenceValue(toStringSet), NULL, NULL);
  }
}

void
DependencyContext_onDependency
  (
    Arcadia_Thread* thread,
    DependenciesContext* self,
    Arcadia_FilePath* from,
    Arcadia_FilePath* to
  )
{
  Arcadia_String* fromString = Arcadia_FilePath_toNative(thread, Arcadia_FilePath_getFullPath(thread, from), Arcadia_BooleanValue_False);
  Arcadia_String* toString = Arcadia_FilePath_toNative(thread, Arcadia_FilePath_getFullPath(thread, to), Arcadia_BooleanValue_False);
  Arcadia_Value temporary = Arcadia_Map_get(thread, self->dependencies, Arcadia_Value_makeObjectReferenceValue(fromString));
  Arcadia_Set* toStringSet = NULL;
  if (Arcadia_Value_isVoidValue(&temporary)) {
    toStringSet = (Arcadia_Set*)Arcadia_HashSet_create(thread);
    Arcadia_Map_set(thread, self->dependencies, Arcadia_Value_makeObjectReferenceValue(fromString),
                                                Arcadia_Value_makeObjectReferenceValue(toStringSet), NULL, NULL);
  } else {
    toStringSet = Arcadia_Value_getObjectReferenceValueChecked(thread, temporary, _Arcadia_Set_getType(thread));
  }
  Arcadia_Set_add(thread, toStringSet, Arcadia_Value_makeObjectReferenceValue(toString), NULL);
}
