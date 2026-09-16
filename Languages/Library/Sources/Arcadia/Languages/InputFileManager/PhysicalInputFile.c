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
#include "Arcadia/Languages/InputFileManager/PhysicalInputFile.h"

#include <assert.h>

static Arcadia_String*
Arcadia_Languages_PhysicalInputFile_getNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  );

static Arcadia_FilePath*
Arcadia_Languages_PhysicalInputFile_getPathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  );

static Arcadia_ByteArray*
Arcadia_Languages_PhysicalInputFile_getContentsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  );

static Arcadia_SizeValue
Arcadia_Languages_PhysicalInputFile_getLineImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self,
    Arcadia_SizeValue offset
  );

static void
doUpdateLineMap
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  );

static void
doUpdateContents
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  );

static void
Arcadia_Languages_PhysicalInputFile_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  );

static void
Arcadia_Languages_PhysicalInputFile_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFileDispatch* self
  );

static void
Arcadia_Languages_PhysicalInputFile_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  );

static void
Arcadia_Languages_PhysicalInputFile_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  );

static void
isEqualToImpl
  (
    Arcadia_Thread* thread
  );

static void
getHashImpl
  (
    Arcadia_Thread* thread
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Languages_PhysicalInputFile_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Languages_PhysicalInputFile_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Languages_PhysicalInputFile_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Languages_PhysicalInputFile_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Languages.PhysicalInputFile", Arcadia_Languages_PhysicalInputFile,
                         u8"Arcadia.Languages.InputFile", Arcadia_Languages_InputFile,
                         &_typeOperations);

static Arcadia_String*
Arcadia_Languages_PhysicalInputFile_getNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  )
{ return self->name; }

static Arcadia_FilePath*
Arcadia_Languages_PhysicalInputFile_getPathImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  )
{ return Arcadia_FilePath_clone(thread, self->path); }

static Arcadia_ByteArray*
Arcadia_Languages_PhysicalInputFile_getContentsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  )
{
  if (NULL == self->contents) {
    doUpdateContents(thread, self);
  }
  return self->contents;
}

static Arcadia_SizeValue
Arcadia_Languages_PhysicalInputFile_getLineImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self,
    Arcadia_SizeValue offset
  )
{
  if (NULL == self->lineMap) {
    doUpdateLineMap(thread, self);
  }
  return Arcadia_Languages_LineMap_getLine(thread, self->lineMap, offset);
}


static void
doUpdateLineMap
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  )
{
  if (!self->contents) {
    doUpdateContents(thread, self);
  }
  if (!self->lineMap) {
    self->lineMap = Arcadia_Languages_LineMap_create(thread, self->contents);
  }
}

static void
doUpdateContents
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  )
{ 
  self->contents = Arcadia_ByteArrayBuilder_toByteArray(thread, Arcadia_FileSystem_getFileContents(thread, Arcadia_FileSystem_getOrCreate(thread), self->path));
}

static void
Arcadia_Languages_PhysicalInputFile_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Languages_PhysicalInputFile);
  if (2 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value name = Arcadia_ValueStack_getValue(thread, 2);
    Arcadia_Value path = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ValueStack_pushValue(thread, &name);
    Arcadia_ValueStack_pushValue(thread, &path);
    Arcadia_ValueStack_pushNatural8Value(thread, 2);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  self->name = (Arcadia_String*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread));
  self->path = (Arcadia_FilePath*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_FilePath_getType(thread));
  self->path = Arcadia_FilePath_clone(thread, self->path);
  Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
  if (Arcadia_FilePath_isRelative(thread, self->path)) {
    Arcadia_FilePath* temporary = Arcadia_FileSystem_getWorkingDirectory(thread, fileSystem);
    Arcadia_FilePath_append(thread, temporary, self->path);
    self->path = temporary;
  }
  self->hashValue = Arcadia_Object_getHash(thread, (Arcadia_Object*)Arcadia_FilePath_toGeneric(thread, self->path));
  self->lineMap = NULL;
  self->contents = NULL;
  Arcadia_LeaveConstructor(Arcadia_Languages_PhysicalInputFile);
}

static void
Arcadia_Languages_PhysicalInputFile_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFileDispatch* self
  )
{
  ((Arcadia_ObjectDispatch*)self)->isEqualTo = &isEqualToImpl;
  ((Arcadia_ObjectDispatch*)self)->getHash = &getHashImpl;

  ((Arcadia_Languages_InputFileDispatch*)self)->getName = (Arcadia_String* (*)(Arcadia_Thread*, Arcadia_Languages_InputFile*)) & Arcadia_Languages_PhysicalInputFile_getNameImpl;
  ((Arcadia_Languages_InputFileDispatch*)self)->getPath = (Arcadia_FilePath* (*)(Arcadia_Thread*, Arcadia_Languages_InputFile*)) &Arcadia_Languages_PhysicalInputFile_getPathImpl;
  ((Arcadia_Languages_InputFileDispatch*)self)->getContents = (Arcadia_ByteArray* (*)(Arcadia_Thread*, Arcadia_Languages_InputFile*)) &Arcadia_Languages_PhysicalInputFile_getContentsImpl;
  ((Arcadia_Languages_InputFileDispatch*)self)->getLine = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Languages_InputFile*, Arcadia_SizeValue)) &Arcadia_Languages_PhysicalInputFile_getLineImpl;
}

static void
Arcadia_Languages_PhysicalInputFile_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  )
{
  if (self->name) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->name);
  }
  if (self->path) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->path);
  }
  if (self->lineMap) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->lineMap);
  }
  if (self->contents) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->contents);
  }
}

static void
Arcadia_Languages_PhysicalInputFile_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  )
{/*Intentionally empty.*/}

#define BINARY_OPERATION() \
  if (Arcadia_ValueStack_getSize(thread) < 3) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  if (2 != Arcadia_ValueStack_getNatural8Value(thread, 0)) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  Arcadia_Value x = Arcadia_ValueStack_getValue(thread, 2); \
  Arcadia_Value y = Arcadia_ValueStack_getValue(thread, 1); \
  Arcadia_ValueStack_popValues(thread, 3);

#define UNARY_OPERATION() \
  if (Arcadia_ValueStack_getSize(thread) < 2) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  if (1 != Arcadia_ValueStack_getNatural8Value(thread, 0)) { \
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); \
    Arcadia_Thread_jump(thread); \
  } \
  Arcadia_Value x = Arcadia_ValueStack_getValue(thread, 1); \
  Arcadia_ValueStack_popValues(thread, 2);

static void
isEqualToImpl
  (
    Arcadia_Thread* thread
  )
{
  BINARY_OPERATION();
  Arcadia_Object* a0 = Arcadia_Value_getObjectReferenceValue(&x);
  if (!Arcadia_Value_isObjectReferenceValue(&y)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
    return;
  }
  Arcadia_Object* b0 = Arcadia_Value_getObjectReferenceValue(&y);
  if (a0 == b0) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_True);
    return;
  }
  assert(NULL != b0);
  if (!Arcadia_Object_isInstanceOf(thread, b0, _Arcadia_Languages_PhysicalInputFile_getType(thread))) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
    return;
  }
  Arcadia_Languages_PhysicalInputFile* a1 = (Arcadia_Languages_PhysicalInputFile*)a0;
  Arcadia_Languages_PhysicalInputFile* b1 = (Arcadia_Languages_PhysicalInputFile*)b0;
  if (a1->hashValue != b1->hashValue) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
    return;
  }
  /* Both paths are absolute. */
  Arcadia_Value rhs = Arcadia_Value_makeObjectReferenceValue(b1);
  if (!Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)a1->path->root, &rhs)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
    return;
  }
  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)a1->path->fileNames) != Arcadia_Collection_getSize(thread, (Arcadia_Collection*)b1->path->fileNames)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
    return;
  }
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)a1->path->fileNames); i < n; ++i) {
    Arcadia_Object* x = Arcadia_List_getObjectReferenceValueAt(thread, a1->path->fileNames, i);
    Arcadia_Object* y = Arcadia_List_getObjectReferenceValueAt(thread, b1->path->fileNames, i);
    rhs = Arcadia_Value_makeObjectReferenceValue(y);
    if (!Arcadia_Object_isEqualTo(thread, x, &rhs)) {
      Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
      return;
    }
  }
  Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_True);
}

static void
getHashImpl
  (
    Arcadia_Thread* thread
  )
{
  UNARY_OPERATION();
  Arcadia_Languages_PhysicalInputFile* self = (Arcadia_Languages_PhysicalInputFile*)Arcadia_Value_getObjectReferenceValue(&x);
  Arcadia_ValueStack_pushSizeValue(thread, self->hashValue);
}

#undef BINARY_OPERATION
#undef UNARY_OPERATION

Arcadia_Languages_PhysicalInputFile*
Arcadia_Languages_PhysicalInputFile_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* diagnosticName,
    Arcadia_FilePath* path
  )
{
  _Arcadia_BeginCreate(Arcadia_Languages_PhysicalInputFile);
  if (diagnosticName) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)diagnosticName); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  if (path) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)path); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  _Arcadia_EndCreate(Arcadia_Languages_PhysicalInputFile);
}

Arcadia_FilePath*
Arcadia_Languages_PhysicalInputFile_getPath
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  )
{ return Arcadia_FilePath_clone(thread, self->path); }

Arcadia_ByteArray*
Arcadia_Languages_PhysicalInputFile_getContents
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self
  )
{
  if (NULL == self->contents) {
    doUpdateContents(thread, self);
  }
  return self->contents;
}

// A one-based line index.
Arcadia_SizeValue
Arcadia_Languages_PhysicalInputFile_getLine
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_PhysicalInputFile* self,
    Arcadia_SizeValue offset
  )
{ 
  if (NULL == self->lineMap) {
    doUpdateLineMap(thread, self);
  }
  return Arcadia_Languages_LineMap_getLine(thread, self->lineMap, offset);
}
