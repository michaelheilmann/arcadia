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

#define ARCADIA_RING2_MODULE (1)
#include "Arcadia/Ring2/CommandLine/Include.h"

#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Ring2/Strings/Include.h"

static Arcadia_BooleanValue
isEnd
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* reader
  )
{ return !Arcadia_UnicodeCodePointReader_hasValue(thread, reader); }

static Arcadia_BooleanValue
isEqual
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* reader
  )
{
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    return Arcadia_BooleanValue_False;
  }
  return '=' == Arcadia_UnicodeCodePointReader_getValue(thread, reader);
}

static Arcadia_BooleanValue
isEqualOrEnd
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* reader
  )
{
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    return Arcadia_BooleanValue_True;
  }
  return '=' == Arcadia_UnicodeCodePointReader_getValue(thread, reader);
}

static Arcadia_BooleanValue
isDigit
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* reader
  )
{
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    return Arcadia_BooleanValue_False;
  }
  Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
  return '0' <= codePoint && codePoint <= '9';
}

static Arcadia_String*
parseInteger
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* reader,
    Arcadia_Unicode_Encoder* writer,
    Arcadia_ByteArrayBuilder* writerByteBuffer
  )
{
  Arcadia_Natural32Value codePoint;
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    return NULL;
  }
  // ('+'|'-')?
  codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
  if (codePoint == '+' || codePoint == '-') {
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
    Arcadia_Unicode_Encoder_encodeCodePoints(thread, writer, &codePoint, 1, writerByteBuffer);
  }
  // digit+
  if (!isDigit(thread, reader)) {
    return NULL;
  }
  codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
  do {
    codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
    Arcadia_Unicode_Encoder_encodeCodePoints(thread, writer, &codePoint, 1, writerByteBuffer);
  } while (isDigit(thread, reader));

  Arcadia_Value temporaryValue;
  Arcadia_Value_setObjectReferenceValue(&temporaryValue, (Arcadia_ObjectReferenceValue)writerByteBuffer);
  Arcadia_String* resultString = Arcadia_String_create(thread, temporaryValue);

  return resultString;
}

static Arcadia_String*
parseString
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* reader,
    Arcadia_Unicode_Encoder* writer,
    Arcadia_ByteArrayBuilder* writerByteBuffer
  )
{
  Arcadia_Natural32Value codePoint;
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    return NULL;
  }
  codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
  if (codePoint != '"') {
    return NULL;
  }
  Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
  Arcadia_BooleanValue lastWasSlash = Arcadia_BooleanValue_False;
  while (Arcadia_BooleanValue_True) {
    if (!Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
      return NULL;
    }
    codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    if (lastWasSlash) {
      switch (codePoint) {
        case 'r': {
          lastWasSlash = Arcadia_BooleanValue_False;
          codePoint = '\r';
          Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
          Arcadia_Unicode_Encoder_encodeCodePoints(thread, writer, &codePoint, 1, writerByteBuffer);
        } break;
        case 'n': {
          lastWasSlash = Arcadia_BooleanValue_False;
          codePoint = '\n';
          Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
          Arcadia_Unicode_Encoder_encodeCodePoints(thread, writer, &codePoint, 1, writerByteBuffer);
        } break;
        case 'v': {
          lastWasSlash = Arcadia_BooleanValue_False;
          codePoint = '\v';
          Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
          Arcadia_Unicode_Encoder_encodeCodePoints(thread, writer, &codePoint, 1, writerByteBuffer);
        } break;
        case 't': {
          lastWasSlash = Arcadia_BooleanValue_False;
          codePoint = '\t';
          Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
          Arcadia_Unicode_Encoder_encodeCodePoints(thread, writer, &codePoint, 1, writerByteBuffer);
        } break;
        case '"': {
          lastWasSlash = Arcadia_BooleanValue_False;
          codePoint = '"';
          Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
          Arcadia_Unicode_Encoder_encodeCodePoints(thread, writer, &codePoint, 1, writerByteBuffer);
        } break;
        case '\\': {
          lastWasSlash = Arcadia_BooleanValue_False;
          Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
          Arcadia_Unicode_Encoder_encodeCodePoints(thread, writer, &codePoint, 1, writerByteBuffer);
        } break;
        default: {
          return NULL;
        }
      };
    } else {
      if (codePoint == '"') {
        Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
        break;
      } else if (codePoint == '\\') {
        Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
        lastWasSlash = Arcadia_BooleanValue_True;
      } else {
        Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
        Arcadia_Unicode_Encoder_encodeCodePoints(thread, writer, &codePoint, 1, writerByteBuffer);
      }

    }
  }

  Arcadia_Value temporaryValue;
  Arcadia_Value_setObjectReferenceValue(&temporaryValue, (Arcadia_ObjectReferenceValue)writerByteBuffer);
  Arcadia_String* resultString = Arcadia_String_create(thread, temporaryValue);

  return resultString;
}

Arcadia_CommandLineArgument*
Arcadia_CommandLine_parseArgument
  (
    Arcadia_Thread* thread,
    Arcadia_UnicodeCodePointReader* reader
  )
{
  Arcadia_String* value1 = NULL, *key1 = NULL;
  Arcadia_Value temporaryValue;
  Arcadia_ByteArrayBuilder* writerByteBuffer = Arcadia_ByteArrayBuilder_create(thread);
  Arcadia_Unicode_Encoder* writer = (Arcadia_Unicode_Encoder*)Arcadia_Unicode_UTF8Encoder_create(thread);
  // '--'
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    return Arcadia_CommandLineArgument_create(thread, Arcadia_BooleanValue_True, NULL, NULL);
  }
  if ('-' != Arcadia_UnicodeCodePointReader_getValue(thread, reader)) {
    return Arcadia_CommandLineArgument_create(thread, Arcadia_BooleanValue_True, NULL, NULL);
  }
  Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    return Arcadia_CommandLineArgument_create(thread, Arcadia_BooleanValue_True, NULL, NULL);
  }
  if ('-' != Arcadia_UnicodeCodePointReader_getValue(thread, reader)) {
    return Arcadia_CommandLineArgument_create(thread, Arcadia_BooleanValue_True, NULL, NULL);
  }
  Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
  // <name>
  if (isEqualOrEnd(thread, reader)) {
    return Arcadia_CommandLineArgument_create(thread, Arcadia_BooleanValue_True, NULL, NULL);
  }
  do {
    Arcadia_Natural32Value codePoint = Arcadia_UnicodeCodePointReader_getValue(thread, reader);
    Arcadia_Unicode_Encoder_encodeCodePoints(thread, writer, &codePoint, 1, writerByteBuffer);
    Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
  } while (!isEqualOrEnd(thread, reader));
  Arcadia_Value_setObjectReferenceValue(&temporaryValue, (Arcadia_ObjectReferenceValue)writerByteBuffer);
  key1 = Arcadia_String_create(thread, temporaryValue);
  Arcadia_ByteArrayBuilder_clear(thread, writerByteBuffer);
  if (isEnd(thread, reader)) {
    return Arcadia_CommandLineArgument_create(thread, Arcadia_BooleanValue_False, key1, NULL);
  }
  // '=' <value>
  if (!isEqual(thread, reader)) {
    return Arcadia_CommandLineArgument_create(thread, Arcadia_BooleanValue_True, key1, NULL);
  }
  Arcadia_UnicodeCodePointReader_nextValue(thread, reader);
  if (!Arcadia_UnicodeCodePointReader_hasValue(thread, reader)) {
    return Arcadia_CommandLineArgument_create(thread, Arcadia_BooleanValue_True, key1, NULL);
  }
  if ('"' == Arcadia_UnicodeCodePointReader_getValue(thread, reader)) {
    value1 = parseString(thread, reader, writer, writerByteBuffer);
  } else {
    value1 = parseInteger(thread, reader, writer, writerByteBuffer);
  }
  if (!value1) {
    return Arcadia_CommandLineArgument_create(thread, Arcadia_BooleanValue_True, key1, NULL);
  }
  if (!isEnd(thread, reader)) {
    return Arcadia_CommandLineArgument_create(thread, Arcadia_BooleanValue_True, key1, NULL);
  }
  return Arcadia_CommandLineArgument_create(thread, Arcadia_BooleanValue_False, key1, value1);
}

void
Arcadia_CommandLine_raiseRequiredArgumentMissingError
  (
    Arcadia_Thread* thread,
    Arcadia_String* key,
    Arcadia_Log* log
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"required command-line argument `");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, key);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"` not specified\n");

  Arcadia_Log_error(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer)));

  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

void
Arcadia_CommandLine_raiseUnknownArgumentError
  (
    Arcadia_Thread* thread,
    Arcadia_String* key,
    Arcadia_String* value,
    Arcadia_Log* log
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"unknown command-line argument `");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, key);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"`\n");

  Arcadia_Log_error(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer)));

  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

void
Arcadia_CommandLine_raiseNoValueError
  (
    Arcadia_Thread* thread,
    Arcadia_String* key,
    Arcadia_Log* log
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"no value specifiekd for command-line argument `");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, key);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"`\n");

  Arcadia_Log_error(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer)));

  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

void
Arcadia_CommandLine_raiseValueInvalidError
  (
    Arcadia_Thread* thread,
    Arcadia_String* key,
    Arcadia_String* value,
    Arcadia_Log* log
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"value specified for command-line argument `");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, key);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"` is not valid\n");

  Arcadia_Log_error(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer)));

  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

void
Arcadia_CommandLine_raiseAlreadySpecifiedError
  (
    Arcadia_Thread* thread,
    Arcadia_String* key,
    Arcadia_Log* log
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"command-line argument `");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, key);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"` was already specified\n");

  Arcadia_Log_error(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer)));

  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}

void
Arcadia_CommandLine_invalidCommandLineArgumentError
  (
    Arcadia_Thread* thread,
    Arcadia_String* argument,
    Arcadia_Log* log
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);

  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"invalid command-line argument `");
  Arcadia_StringBuilder_insertBackString(thread, stringBuffer, argument);
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"`\n");

  Arcadia_Log_error(thread, log, Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer)));

  Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
  Arcadia_Thread_jump(thread);
}
