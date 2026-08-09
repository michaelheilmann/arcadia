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
#include "Arcadia/Languages/LineMap.h"

#include <assert.h>

/// The offsets at which lines start in ascending order.
/// There is always at least the offset 0 of line 1 in the that array. 
static Arcadia_List*
doComputeLineMap
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray* contents
  );

static void
Arcadia_Languages_LineMap_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_LineMap* self
  );

static void
Arcadia_Languages_LineMap_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_LineMapDispatch* self
  );

static void
Arcadia_Languages_LineMap_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_LineMap* self
  );

static void
Arcadia_Languages_LineMap_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_LineMap* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Languages_LineMap_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Languages_LineMap_destruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Languages_LineMap_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Languages_LineMap_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Languages.LineMap", Arcadia_Languages_LineMap,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static Arcadia_List*
doComputeLineMap
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray* contents
  )
{
  Arcadia_List* lineMap = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_List_insertBackSizeValue(thread, lineMap, 0);
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArray_ByteReader_create(thread, contents));
  Arcadia_SizeValue offset = 0;

  while (Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    Arcadia_SizeValue dummy, length;
    Arcadia_UnicodeCodePointReader_getByteRange(thread, reader, &dummy, &length);

    if (Arcadia_UnicodeCodePointReader_hasError(thread, reader)) {
      offset += length;
      Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
      continue;
    }

    Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    offset += length;
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);

    if (codePoint == '\n' || codePoint == '\r') {
      if (Arcadia_UnicodeCodePointReader_hasValue(thread, reader) && !Arcadia_UnicodeCodePointReader_hasError(thread, reader)) {
        Arcadia_Natural32Value nextCodePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
        if ((nextCodePoint == '\n' || nextCodePoint == '\r') && codePoint != nextCodePoint) {
          Arcadia_UnicodeCodePointReader_getByteRange(thread, reader, &dummy, &length);
          offset += length;
          Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
        }
      }
      Arcadia_List_insertBackSizeValue(thread, lineMap, offset);
    }
  }
  return lineMap;
}

static void
Arcadia_Languages_LineMap_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_LineMap* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Languages_LineMap);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ByteArray* contents = (Arcadia_ByteArray*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_ByteArray_getType(thread));
  self->lineMap = doComputeLineMap(thread, contents);
  Arcadia_LeaveConstructor(Arcadia_Languages_LineMap);
}

static void
Arcadia_Languages_LineMap_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_LineMapDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Languages_LineMap_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_LineMap* self
  )
{
  if (self->lineMap) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->lineMap);
  }
}

static void
Arcadia_Languages_LineMap_destruct
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_LineMap* self
  )
{/*Intentionally empty.*/}

Arcadia_Languages_LineMap*
Arcadia_Languages_LineMap_create
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArray* contents
  )
{
  _Arcadia_BeginCreate(Arcadia_Languages_LineMap);
  if (contents) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)contents); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Languages_LineMap);
}

Arcadia_SizeValue
Arcadia_Languages_LineMap_getLine
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_LineMap* self,
    Arcadia_SizeValue offset
  )
{ 
  Arcadia_SizeValue lowIndex = Arcadia_SizeValue_Literal(0);
  assert(Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->lineMap) > 0);
  Arcadia_SizeValue highIndex = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->lineMap);
  while (lowIndex < highIndex) {
    Arcadia_SizeValue middleIndex = lowIndex + (highIndex - lowIndex) / 2;
    Arcadia_SizeValue middleValue = Arcadia_List_getSizeValueAt(thread, self->lineMap, middleIndex);
    if (middleValue <= offset) {
      lowIndex = middleIndex + 1;
    } else {
      highIndex = middleIndex;
    }
  }
  return lowIndex;
}
