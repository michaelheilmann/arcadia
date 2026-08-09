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

#include "Arcadia/Languages/InputFile.h"

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

#if 0
// We re-read the code points of the file until we reach its end or an error.
// In either case, we stop. 
// The initial line length is l = 0;
// We start reading code points. 
// - If the read code point is an error or we reach the end of the file.
//   push l to the line map and stop.
// - If the read code point is
//  - '\n' or '\r' then l += 1 and advance to the next code point.
//  - if the next code point is '\n' or '\r' or it is an invalid code point and not equal to the previous code point then l += 1.
//  - push l to the line map and continue.
// @return An array of Arcadia.Size values.
// Each value denotes the offset, in Bytes from the beginnin of the input file, to the start of the line.
// (Consequently, the first value is always 0).
static Arcadia_List*
builtLineMap
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManager* self,
    Arcadia_FilePath* path,
    Arcadia_Languages_Diagnostics* diagnostics
   )
{
  Arcadia_List* lineMap = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_UnicodeCodePointReader* reader = Arcadia_Languages_InputFileManager_getFileReader(thread, self, path);
  Arcadia_SizeValue offset = 0;
  while (Arcadia_BooleanValue_True) {
    if (Arcadia_UnicodeCodePointReader_hasError(thread, reader)) {
      Arcadia_List_insertBackNatural32Value(thread,lineMap, offset);
      break;
    } else if (!Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
      Arcadia_List_insertBackNatural32Value(thread, lineMap, offset);
      break;
    } else {
      Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
      if (codePoint == '\n' || codePoint == '\r') {
        if (Arcadia_SizeValue_Maximum - offset < 1) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); /* The file is just too big. */
          Arcadia_Thread_jump(thread);
          break;
        }
        offset++;
        Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
        if (Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
          Arcadia_Natural32Value oldCodePoint = codePoint;
          codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
          if ((codePoint == '\n' || codePoint == '\r') && codePoint != oldCodePoint) {
            if (Arcadia_SizeValue_Maximum - offset < 1) {
              Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); /* The file is just too big. */
              Arcadia_Thread_jump(thread);
            }
            offset++;
            Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
            Arcadia_List_insertBackNatural32Value(thread, lineMap, offset);
          }
        }
      } else {
        Arcadia_SizeValue codePointLength;
        Arcadia_UnicodeCodePointReader_getByteRange(thread, reader, NULL, &codePointLength);
        if (Arcadia_SizeValue_Maximum - offset < codePointLength) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid); /* The file is just too big. */
          Arcadia_Thread_jump(thread);
        }
        offset += codePointLength;
        Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
      }
    }
  }
  return lineMap;
}
#endif

Arcadia_UnicodeCodePointReader*
Arcadia_Languages_InputFileManager_getFileReader
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_InputFileManager* self,
    Arcadia_FilePath* path
  )
{
  Arcadia_Languages_InputFile* inputFile = Arcadia_Languages_InputFile_create(thread, path);
  Arcadia_ByteArray* inputFileContents = Arcadia_Languages_InputFile_getContents(thread, inputFile);
  return (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, inputFileContents));
}
