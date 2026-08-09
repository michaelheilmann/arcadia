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
#include "Arcadia/FileSystem/FilePath.h"

#include "Arcadia/FileSystem/Include.h"

#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h> // for GetFullPathName
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  #include <stdlib.h> // for realpath
  #include <linux/limits.h> // for PATH_MAX
  #include <string.h> // for strlen
#endif

typedef struct Context {
  Arcadia_Unicode_Encoder* temporaryWriter;
  Arcadia_ByteArrayBuilder* temporaryBuffer;
  Arcadia_UnicodeCodePointReader* reader;
  Arcadia_FilePath* target;
} Context;

static void
next
  (
    Arcadia_Thread* thread,
    Context* context
  );

static void
saveAndNext
  (
    Arcadia_Thread* thread,
    Context* context
  );

static Arcadia_BooleanValue
isEnd
  (
    Arcadia_Thread* thread,
    Context* context
  );

static Arcadia_BooleanValue
isDriveLetter
  (
    Arcadia_Thread* thread,
    Context* context
  );

static Arcadia_BooleanValue
isColon
  (
    Arcadia_Thread* thread,
    Context* context
  );

static Arcadia_BooleanValue
isDirectorySeparator
  (
    Arcadia_Thread* thread,
    Context* context
  );

static Arcadia_BooleanValue
isSlash
  (
    Arcadia_Thread* thread,
    Context* context
  );

static void
normalize
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  );

/* https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file#fully-qualified-vs-relative-paths */
static void
parseWindowsFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* target,
    Arcadia_ByteArrayBuilder* source
  );

static void
parseUnixFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* target,
    Arcadia_ByteArrayBuilder* source
  );

static void
parseGenericFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* target,
    Arcadia_ByteArrayBuilder* source
  );

#if defined (_DEBUG)

#include <assert.h>

// invariant:
// - a path has a root and zero or more components.
// - a path has one or more components.
static void
checkInvariant
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  );

#else

#define checkInvariant(thread, self)

#endif

static void
next
  (
    Arcadia_Thread* thread,
    Context* context
  )
{ Arcadia_UnicodeCodePointReader_nextValue(thread, context->reader); }

static void
saveAndNext
  (
    Arcadia_Thread* thread,
    Context* context
  )
{
  Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, context->reader);
  Arcadia_Unicode_Encoder_encodeCodePoints(thread, context->temporaryWriter, &codePoint, 1, context->temporaryBuffer);
  next(thread, context);
}

static Arcadia_BooleanValue
isEnd
  (
    Arcadia_Thread* thread,
    Context* context
  )
{ return !Arcadia_UnicodeCodePointReader_hasValue(thread, context->reader); }

static Arcadia_BooleanValue
isDriveLetter
  (
    Arcadia_Thread* thread,
    Context* context
  )
{
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, context->reader)) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, context->reader);
  return ('a' <= codePoint && codePoint <= 'z')
      || ('A' <= codePoint && codePoint <= 'Z');
}

static Arcadia_BooleanValue
isColon
  (
    Arcadia_Thread* thread,
    Context* context
  )
{
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, context->reader)) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, context->reader);
  return ':' == codePoint;
}

static Arcadia_BooleanValue
isDirectorySeparator
  (
    Arcadia_Thread* thread,
    Context* context
  )
{
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, context->reader)) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, context->reader);
  return ('\\' == codePoint || '/' == codePoint);
}

static Arcadia_BooleanValue
isSlash
  (
    Arcadia_Thread* thread,
    Context* context
  )
{
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, context->reader)) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, context->reader);
  return '/' == codePoint;
}

// <filename> <dot-dot> => delete this
// <dot> => delete this
static void
normalize
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{
  Arcadia_Value dot = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8"."));
  Arcadia_Value dotdot = Arcadia_Value_makeObjectReferenceValue(Arcadia_String_createFromCxxString(thread, u8".."));
  Arcadia_SizeValue previous = 0, current = 1;
  while (current < Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames)) {
    Arcadia_Value t;
    t = Arcadia_List_getAt(thread, self->fileNames, previous);
    Arcadia_String* previousString = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&t);
    t = Arcadia_List_getAt(thread, self->fileNames, current);
    Arcadia_String* currentString = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&t);
    if (!Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)previousString, &dotdot) && !Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)previousString, &dot) &&
         Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)currentString, &dotdot)) {
      // Remove previous and current.
      Arcadia_List_removeAt(thread, self->fileNames, previous, 2);
      previous--; current--;
    } else if (Arcadia_Object_isEqualTo(thread, (Arcadia_Object*)currentString, &dot)) {
      // Remove current.
      Arcadia_List_removeAt(thread, self->fileNames, current, 1);
    } else {
      previous++;
      current++;
    }
  }
  if (!self->root && Arcadia_Collection_isEmpty(thread, (Arcadia_Collection*)self->fileNames)) {
    // If the path is empty, then the path is `.`.
    Arcadia_List_insertBackObjectReferenceValue(thread, self->fileNames, (Arcadia_ObjectReferenceValue)Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8".", sizeof(u8".") - 1)));
  }
}

static void
parseWindowsFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* target,
    Arcadia_ByteArrayBuilder* source
  )
{
  Context context;
  context.reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArrayBuilder_ByteReader_create(thread, source));
  context.temporaryBuffer = Arcadia_ByteArrayBuilder_create(thread);
  context.temporaryWriter = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  context.target = target;

  target->relative = Arcadia_BooleanValue_False;
  target->root = NULL;
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)target->fileNames);

  if (isDriveLetter(thread, &context)) {
    saveAndNext(thread, &context);
    if (isColon(thread, &context)) {
      next(thread, &context);
      if (isDirectorySeparator(thread, &context)) {
        // absolute.
        context.target->relative = Arcadia_BooleanValue_False;
        next(thread, &context);
        Arcadia_Value temporaryValue;
        Arcadia_Value_setObjectReferenceValue(&temporaryValue, (Arcadia_ObjectReferenceValue)context.temporaryBuffer);
        context.target->root = Arcadia_String_create(thread, temporaryValue);
        Arcadia_ByteArrayBuilder_clear(thread, context.temporaryBuffer);
      } else {
        // relative to "root" drive and the diretory on the drive
        context.target->relative = Arcadia_BooleanValue_True;
        Arcadia_Value temporaryValue;
        Arcadia_Value_setObjectReferenceValue(&temporaryValue, (Arcadia_ObjectReferenceValue)context.temporaryBuffer);
        context.target->root = Arcadia_String_create(thread, temporaryValue);
        Arcadia_ByteArrayBuilder_clear(thread, context.temporaryBuffer);
      }
    } else {
      while (!isEnd(thread, &context) && !isDirectorySeparator(thread, &context)) {
        saveAndNext(thread, &context);
      }
      if (Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, context.temporaryBuffer)) {
        Arcadia_Value temporary;
        Arcadia_Value_setObjectReferenceValue(&temporary, (Arcadia_ObjectReferenceValue)context.temporaryBuffer);
        Arcadia_Value_setObjectReferenceValue(&temporary, Arcadia_String_create(thread, temporary));
        Arcadia_List_insertBack(thread, context.target->fileNames, temporary);
        Arcadia_ByteArrayBuilder_clear(thread, context.temporaryBuffer);
      }
      target->relative = Arcadia_BooleanValue_True;
    }
  } else if (isDirectorySeparator(thread, &context)) {
    // relative to the root of the current drive.
    context.target->relative = Arcadia_BooleanValue_True;
    next(thread, &context);
  } else {
    context.target->relative = Arcadia_BooleanValue_True;
  }
  // read the remaining directories
  Arcadia_ByteArrayBuilder_clear(thread, context.temporaryBuffer);
  while (!isEnd(thread, &context)) {
    if (isDirectorySeparator(thread, &context)) {
      next(thread, &context);
      if (Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, context.temporaryBuffer)) {
        Arcadia_Value temporary;
        Arcadia_Value_setObjectReferenceValue(&temporary, (Arcadia_ObjectReferenceValue)context.temporaryBuffer);
        Arcadia_Value_setObjectReferenceValue(&temporary, Arcadia_String_create(thread, temporary));
        Arcadia_List_insertBack(thread, context.target->fileNames, temporary);
        Arcadia_ByteArrayBuilder_clear(thread, context.temporaryBuffer);
      }
    } else {
      saveAndNext(thread, &context);
    }
  }
  if (Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, context.temporaryBuffer)) {
    Arcadia_Value temporary;
    Arcadia_Value_setObjectReferenceValue(&temporary, (Arcadia_ObjectReferenceValue)context.temporaryBuffer);
    Arcadia_Value_setObjectReferenceValue(&temporary, Arcadia_String_create(thread, temporary));
    Arcadia_List_insertBack(thread, context.target->fileNames, temporary);
    Arcadia_ByteArrayBuilder_clear(thread, context.temporaryBuffer);
  }
}

static void
parseUnixFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* target,
    Arcadia_ByteArrayBuilder* source
  )
{
  Context context;
  context.reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArrayBuilder_ByteReader_create(thread, source));
  context.temporaryBuffer = Arcadia_ByteArrayBuilder_create(thread);
  context.temporaryWriter = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  context.target = target;

  target->relative = Arcadia_BooleanValue_True;
  target->root = NULL;
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)target->fileNames);

  if (isSlash(thread, &context)) {
    target->relative = Arcadia_BooleanValue_False;
    target->root = Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"/", sizeof(u8"/") - 1));
    next(thread, &context);
  }
  // read the remaining directories
  Arcadia_ByteArrayBuilder_clear(thread, context.temporaryBuffer);
  while (!isEnd(thread, &context)) {
    if (isSlash(thread, &context)) {
      next(thread, &context);
      if (Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, context.temporaryBuffer)) {
        Arcadia_Value temporary;
        Arcadia_Value_setObjectReferenceValue(&temporary, (Arcadia_ObjectReferenceValue)context.temporaryBuffer);
        Arcadia_Value_setObjectReferenceValue(&temporary, Arcadia_String_create(thread, temporary));
        Arcadia_List_insertBack(thread, context.target->fileNames, temporary);
        Arcadia_ByteArrayBuilder_clear(thread, context.temporaryBuffer);
      }
    } else {
      saveAndNext(thread, &context);
    }
  }
  if (Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, context.temporaryBuffer)) {
    Arcadia_Value temporary;
    Arcadia_Value_setObjectReferenceValue(&temporary, (Arcadia_ObjectReferenceValue)context.temporaryBuffer);
    Arcadia_Value_setObjectReferenceValue(&temporary, Arcadia_String_create(thread, temporary));
    Arcadia_List_insertBack(thread, context.target->fileNames, temporary);
    Arcadia_ByteArrayBuilder_clear(thread, context.temporaryBuffer);
  }
}

static void
parseGenericFilePath
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* target,
    Arcadia_ByteArrayBuilder* source
  )
{
  Context context;
  context.reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_ByteArrayBuilder_ByteReader_create(thread, source));
  context.temporaryBuffer = Arcadia_ByteArrayBuilder_create(thread);
  context.temporaryWriter = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  context.target = target;

  target->relative = Arcadia_BooleanValue_True;
  target->root = NULL;
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)target->fileNames);

  if (isSlash(thread, &context)) {
    target->relative = Arcadia_BooleanValue_False;
    target->root = Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, u8"/", sizeof(u8"/") - 1));
    next(thread, &context);
  }
  // read the remaining directories
  Arcadia_ByteArrayBuilder_clear(thread, context.temporaryBuffer);
  while (!isEnd(thread, &context)) {
    if (isDirectorySeparator(thread, &context)) {
      next(thread, &context);
      if (Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, context.temporaryBuffer)) {
        Arcadia_Value temporary;
        Arcadia_Value_setObjectReferenceValue(&temporary, (Arcadia_ObjectReferenceValue)context.temporaryBuffer);
        Arcadia_Value_setObjectReferenceValue(&temporary, Arcadia_String_create(thread, temporary));
        Arcadia_List_insertBack(thread, context.target->fileNames, temporary);
        Arcadia_ByteArrayBuilder_clear(thread, context.temporaryBuffer);
      }
    } else {
      saveAndNext(thread, &context);
    }
  }
  if (Arcadia_ByteArrayBuilder_getNumberOfBytes(thread, context.temporaryBuffer)) {
    Arcadia_Value temporary;
    Arcadia_Value_setObjectReferenceValue(&temporary, (Arcadia_ObjectReferenceValue)context.temporaryBuffer);
    Arcadia_Value_setObjectReferenceValue(&temporary, Arcadia_String_create(thread, temporary));
    Arcadia_List_insertBack(thread, context.target->fileNames, temporary);
    Arcadia_ByteArrayBuilder_clear(thread, context.temporaryBuffer);
  }
}

static void
Arcadia_FilePath_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  );

static void
Arcadia_FilePath_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FilePathDispatch* self
  );

static void
Arcadia_FilePath_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  );

static void
Arcadia_FilePath_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_FilePath_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_FilePath_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_FilePath_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_FilePath_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.FilePath", Arcadia_FilePath,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

#if defined (_DEBUG)

static void
checkInvariant
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{
  Arcadia_BooleanValue isValid = Arcadia_BooleanValue_True;
  isValid &= NULL != self->root || Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames) > 0;
  assert(isValid);
}

#endif

static void
Arcadia_FilePath_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{
  Arcadia_EnterConstructor(Arcadia_FilePath);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->fileNames = NULL;
  self->relative = Arcadia_BooleanValue_False;
  self->root = NULL;
  self->fileNames = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_LeaveConstructor(Arcadia_FilePath);
}

static void
Arcadia_FilePath_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FilePathDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_FilePath_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_FilePath_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{
  if (self->fileNames) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->fileNames);
  }
  if (self->root) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->root);
  }
}

Arcadia_FilePath*
Arcadia_FilePath_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_FilePath);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_FilePath);
}

Arcadia_FilePath*
Arcadia_FilePath_clone
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{
  Arcadia_FilePath* clone = Arcadia_FilePath_create(thread);
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames); i < n; ++i) {
    Arcadia_List_insertBack(thread, clone->fileNames, Arcadia_List_getAt(thread, self->fileNames, i));
  }
  clone->root = self->root;
  clone->relative = self->relative;
  return clone;
}

Arcadia_FilePath*
Arcadia_FilePath_parseWindows
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  )
{
  Arcadia_FilePath* self = Arcadia_FilePath_create(thread);
  self->fileNames = NULL;
  self->relative = Arcadia_BooleanValue_False;
  self->root = NULL;
  self->fileNames = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ByteArrayBuilder* byteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, byteBuffer, Arcadia_String_getBytes(thread, string), Arcadia_String_getNumberOfBytes(thread, string));
  parseWindowsFilePath(thread, self, byteBuffer);
  normalize(thread, self);
  return self;
}

Arcadia_FilePath*
Arcadia_FilePath_parseUnix
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  )
{
  Arcadia_FilePath* self = Arcadia_FilePath_create(thread);
  self->fileNames = NULL;
  self->relative = Arcadia_BooleanValue_False;
  self->root = NULL;
  self->fileNames = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ByteArrayBuilder* byteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, byteBuffer, Arcadia_String_getBytes(thread, string), Arcadia_String_getNumberOfBytes(thread, string));
  parseUnixFilePath(thread, self, byteBuffer);
  normalize(thread, self);
  return self;
}

Arcadia_FilePath*
Arcadia_FilePath_parseNative
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  )
 {
#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  return Arcadia_FilePath_parseWindows(thread, string);
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  return Arcadia_FilePath_parseUnix(thread, string);
#else
  #error("operating system not (yet) supported")
#endif
}

Arcadia_String*
Arcadia_FilePath_toNative
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self,
    Arcadia_BooleanValue appendZeroTerminator
  )
{
  Arcadia_ByteArrayBuilder* temporaryBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Unicode_Encoder* temporary = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
#if Arcadia_Configuration_OperatingSystem_Windows == Arcadia_Configuration_OperatingSystem
  Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames);

  if (self->root) {
    Arcadia_Unicode_Encoder_encodeString(thread, temporary, self->root, temporaryBuffer);
    Arcadia_Natural32Value x;
    x = ':';
    Arcadia_Unicode_Encoder_encodeCodePoints(thread, temporary, &x, 1, temporaryBuffer);
    if (!self->relative) {
      x = '\\';
      Arcadia_Unicode_Encoder_encodeCodePoints(thread, temporary, &x, 1, temporaryBuffer);
    } else {
      /* Intentionally empty. */
    }
  }
  if (n > 0) {
    Arcadia_Value e = Arcadia_List_getAt(thread, self->fileNames, 0);
    Arcadia_String* fileName = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&e);
    Arcadia_Unicode_Encoder_encodeString(thread, temporary, fileName, temporaryBuffer);
    i++;

    for (; i < n; ++i) {
      Arcadia_Natural32Value x;
      x = '\\';
      Arcadia_Unicode_Encoder_encodeCodePoints(thread, temporary, &x, 1, temporaryBuffer);

      Arcadia_Value e = Arcadia_List_getAt(thread, self->fileNames, i);
      Arcadia_String* fileName = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&e);
      Arcadia_Unicode_Encoder_encodeString(thread, temporary, fileName, temporaryBuffer);
    }
  }
#elif Arcadia_Configuration_OperatingSystem_Linux == Arcadia_Configuration_OperatingSystem
  Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames);
  if (self->root) {
    Arcadia_Unicode_Encoder_encodeString(thread, temporary, self->root, temporaryBuffer);
  }
  if (n > 0) {
    Arcadia_Value e = Arcadia_List_getAt(thread, self->fileNames, 0);
    Arcadia_String* fileName = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&e);
    Arcadia_Unicode_Encoder_encodeString(thread, temporary, fileName, temporaryBuffer);
    i++;

    for (; i < n; ++i) {
      Arcadia_Natural32Value x;
      x = '/';
      Arcadia_Unicode_Encoder_encodeCodePoints(thread, temporary, &x, 1, temporaryBuffer);

      Arcadia_Value e = Arcadia_List_getAt(thread, self->fileNames, i);
      Arcadia_String* fileName = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&e);
      Arcadia_Unicode_Encoder_encodeString(thread, temporary, fileName, temporaryBuffer);
    }
  }
#else
  #error("operating system not (yet) supported")
#endif
  if (appendZeroTerminator) {
    Arcadia_Natural32Value x = '\0';
    Arcadia_Unicode_Encoder_encodeCodePoints(thread, temporary, &x, 1, temporaryBuffer);
  }
  Arcadia_Value temporaryValue;
  Arcadia_Value_setObjectReferenceValue(&temporaryValue, (Arcadia_ObjectReferenceValue)temporaryBuffer);
  return Arcadia_String_create(thread, temporaryValue);
}

Arcadia_FilePath*
Arcadia_FilePath_parseGeneric
  (
    Arcadia_Thread* thread,
    Arcadia_String* string
  )
{
  Arcadia_FilePath* self = Arcadia_FilePath_create(thread);
  self->fileNames = NULL;
  self->relative = Arcadia_BooleanValue_False;
  self->root = NULL;
  self->fileNames = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_ByteArrayBuilder* byteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, byteBuffer, Arcadia_String_getBytes(thread, string), Arcadia_String_getNumberOfBytes(thread, string));
  parseGenericFilePath(thread, self, byteBuffer);
  normalize(thread, self);
  return self;
}

Arcadia_String*
Arcadia_FilePath_toGeneric
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{
  /// @todo Cache in file system.
  Arcadia_String* directorySeparator = Arcadia_String_createFromCxxString(thread, u8"/");

  Arcadia_ByteArrayBuilder* temporaryBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Unicode_Encoder* encoder = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames);
  if (self->root) {
    Arcadia_Unicode_Encoder_encodeString(thread, encoder, directorySeparator, temporaryBuffer);
    Arcadia_Unicode_Encoder_encodeString(thread, encoder, self->root, temporaryBuffer);
    if (n > 0) {
      Arcadia_Unicode_Encoder_encodeString(thread, encoder, directorySeparator, temporaryBuffer);
    }
  }
  if (n > 0) {
    Arcadia_Value e = Arcadia_List_getAt(thread, self->fileNames, 0);
    Arcadia_String* fileName = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&e);
    Arcadia_Unicode_Encoder_encodeString(thread, encoder, fileName, temporaryBuffer);
    i++;

    for (; i < n; ++i) {
      Arcadia_Unicode_Encoder_encodeString(thread, encoder, directorySeparator, temporaryBuffer);
      Arcadia_Value e = Arcadia_List_getAt(thread, self->fileNames, i);
      Arcadia_String* fileName = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&e);
      Arcadia_Unicode_Encoder_encodeString(thread, encoder, fileName, temporaryBuffer);
    }
  }
  Arcadia_Value temporaryValue;
  Arcadia_Value_setObjectReferenceValue(&temporaryValue, (Arcadia_ObjectReferenceValue)temporaryBuffer);
  return Arcadia_String_create(thread, temporaryValue);
}

Arcadia_FilePath*
Arcadia_FilePath_getFullPath
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{
  Arcadia_FilePath* path = Arcadia_FilePath_clone(thread, self);
  // (1) if the path is relative, make it absolute using the working directory.
  if (Arcadia_FilePath_isRelative(thread, path)) {
    Arcadia_FileSystem* fileSystem = Arcadia_FileSystem_getOrCreate(thread);
    Arcadia_FilePath* newPath = Arcadia_FileSystem_getWorkingDirectory(thread, fileSystem);
    Arcadia_FilePath_append(thread, newPath, path);
    path = newPath;
  }
  // (2) normalize the path
  normalize(thread, path);
  // (3) return the normalized path
  return path;
}

Arcadia_BooleanValue
Arcadia_FilePath_isEqualTo
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self,
    Arcadia_FilePath* other
  )
{
  self = Arcadia_FilePath_getFullPath(thread, self);
  other = Arcadia_FilePath_getFullPath(thread, other);
  if (self == other) {
    return Arcadia_BooleanValue_True;
  }
  if (self->relative != other->relative) {
    return Arcadia_BooleanValue_False;
  }
  if (Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames) != Arcadia_Collection_getSize(thread, (Arcadia_Collection*)other->fileNames)) {
    return Arcadia_BooleanValue_False;
  }
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames); i < n; ++i) {
    Arcadia_Value x = Arcadia_List_getAt(thread, self->fileNames, i);
    Arcadia_Value y = Arcadia_List_getAt(thread, other->fileNames, i);
    if (!Arcadia_Value_isEqualTo(thread, &x, &y)) {
      return Arcadia_BooleanValue_False;
    }
  }
  if ((self->root == NULL && other->root != NULL) || (self->root != NULL && other->root == NULL)) {
    return Arcadia_BooleanValue_False;
  }
  if (self->root != NULL) {
    Arcadia_Value args[2] = { Arcadia_Value_Initializer(),
                              Arcadia_Value_Initializer() };
    Arcadia_Value_setObjectReferenceValue(&args[0], self->root);
    Arcadia_Value_setObjectReferenceValue(&args[1], other->root);
    if (!Arcadia_Value_isEqualTo(thread, &args[0], &args[1])) {
      return Arcadia_BooleanValue_False;
    }
  }
  return Arcadia_BooleanValue_True;
}

Arcadia_BooleanValue
Arcadia_FilePath_isAbsolute
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{ return !self->relative; }

Arcadia_BooleanValue
Arcadia_FilePath_isRelative
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{ return self->relative; }

void
Arcadia_FilePath_append
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self,
    Arcadia_FilePath* other
  )
{
  if (!Arcadia_FilePath_isRelative(thread, other)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)other->fileNames); i < n; ++i) {
    Arcadia_Value v = Arcadia_List_getAt(thread, other->fileNames, i);
    Arcadia_List_insertBack(thread, self->fileNames, v);
  }
  normalize(thread, self);
}

Arcadia_FilePath*
Arcadia_FilePath_getRootPath
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{
  if (!self->root) {
    return NULL;
  }
  Arcadia_FilePath* clone = Arcadia_FilePath_clone(thread, self);
  Arcadia_Collection_clear(thread, (Arcadia_Collection*)clone->fileNames);
  return clone;
}

Arcadia_FilePath*
Arcadia_FilePath_getRelativePath
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{
  Arcadia_FilePath* clone = Arcadia_FilePath_clone(thread, self);
  clone->root = NULL;
  clone->relative = Arcadia_BooleanValue_True;
  return clone;
}

Arcadia_FilePath*
Arcadia_FilePath_getParent
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{
  checkInvariant(thread, self);
  if (self->root) {
    if (!Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames)) {
      // If there are not components, we return NULL.
      return NULL;
    } else {
      Arcadia_FilePath* clone = Arcadia_FilePath_clone(thread, self);
      Arcadia_List_removeBack(thread, clone->fileNames, 1);
      return clone;
    }
  } else {
    if (1 == Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames)) {
      return NULL;
    } else {
      Arcadia_FilePath* clone = Arcadia_FilePath_clone(thread, self);
      Arcadia_List_removeBack(thread, clone->fileNames, 1);
      return clone;
    }
  }
}

Arcadia_String*
Arcadia_FilePath_getExtension
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self
  )
{
  if (!Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames)) {
    return NULL;
  }
  Arcadia_String* fileName = (Arcadia_String*)Arcadia_List_getObjectReferenceValueAt(thread, self->fileNames, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames) - 1);
  Arcadia_Value dotPositionValue = Arcadia_String_findLastOccurrence(thread, fileName, '.');
  if (Arcadia_Value_isVoidValue(&dotPositionValue)) {
    return NULL;
  }
  Arcadia_SizeValue dotPosition = Arcadia_Value_getSizeValue(&dotPositionValue);
  return Arcadia_String_substring(thread, fileName, dotPosition, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
}

void
Arcadia_FilePath_addOrReplaceExtension
  (
    Arcadia_Thread* thread,
    Arcadia_FilePath* self,
    Arcadia_String* extension
  )
{ 
  Arcadia_UnicodeCodePointReader* reader = (Arcadia_UnicodeCodePointReader*)Arcadia_ByteReader_UnicodeCodePointReader_create(thread, (Arcadia_ByteReader*)Arcadia_String_ByteReader_create(thread, extension));
  // Must not be empty.
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  // Must not contains period or colon, slash or backslash.
  do {
    Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    switch (codePoint) {
      case '.':
      case ':':
      case '/':
      case '\\': {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
        Arcadia_Thread_jump(thread);
      } break;
    };
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
  } while (Arcadia_UnicodeCodePointReader_hasValue(thread, reader));

  if (!Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames)) {
    Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
    Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, '.');
    Arcadia_StringBuilder_insertBackString(thread, stringBuffer, extension);
    Arcadia_List_insertBack(thread, self->fileNames, Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer))));
  } else {
    Arcadia_String* fileName = (Arcadia_String*)Arcadia_List_getObjectReferenceValueAt(thread, self->fileNames, Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->fileNames) - 1);
    Arcadia_Value dotPositionValue = Arcadia_String_findLastOccurrence(thread, fileName, '.');
    if (Arcadia_Value_isVoidValue(&dotPositionValue)) {
      Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
      Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, '.');
      Arcadia_StringBuilder_insertBackString(thread, stringBuffer, extension);
      Arcadia_List_insertBack(thread, self->fileNames, Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer))));
    } else {
      Arcadia_SizeValue dotPosition = Arcadia_Value_getSizeValue(&dotPositionValue);
      Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
      Arcadia_StringBuilder_insertBackString(thread, stringBuffer, Arcadia_String_substring(thread, fileName, 0, Arcadia_Value_makeSizeValue(dotPosition)));
      Arcadia_StringBuilder_insertBackCodePoint(thread, stringBuffer, '.');
      Arcadia_StringBuilder_insertBackString(thread, stringBuffer, extension);
      Arcadia_List_removeBack(thread, self->fileNames, 1);
      Arcadia_List_insertBack(thread, self->fileNames, Arcadia_Value_makeObjectReferenceValue(Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer))));
    }
  }
}
