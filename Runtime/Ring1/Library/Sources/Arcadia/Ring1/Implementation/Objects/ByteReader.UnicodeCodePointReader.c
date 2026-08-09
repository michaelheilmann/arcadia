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

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/Implementation/Objects/ByteReader.UnicodeCodePointReader.h"

#include "Arcadia/Ring1/Include.h"
#include <memory.h>

static void
Arcadia_ByteReader_UnicodeCodePointReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  );

static void
Arcadia_ByteReader_UnicodeCodePointReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReaderDispatch* self
  );

static void
Arcadia_ByteReader_UnicodeCodePointReader_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  );

static void
next
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  );

static void
Arcadia_ByteReader_UnicodeCodePointReader_nextValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  );

static Arcadia_Natural32Value
Arcadia_ByteReader_UnicodeCodePointReader_getValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  );

static Arcadia_BooleanValue
Arcadia_ByteReader_UnicodeCodePointReader_hasValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  );

static Arcadia_BooleanValue
Arcadia_ByteReader_UnicodeCodePointReader_hasErrorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  );

static void
Arcadia_ByteReader_UnicodeCodePointReader_getByteRangeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self,
    Arcadia_SizeValue* start,
    Arcadia_SizeValue* length
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_ByteReader_UnicodeCodePointReader_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_ByteReader_UnicodeCodePointReader_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_ByteReader_UnicodeCodePointReader_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.ByteReader.UnicodeCodePointReader", Arcadia_ByteReader_UnicodeCodePointReader,
                         u8"Arcadia.UnicodeCodePointReader", Arcadia_UnicodeCodePointReader,
                         &_typeOperations);
                         
#define FlagsNone (1<<0)
#define FlagsEnd (1<<1)
#define FlagsShortRead (1<<2)
#define FlagsError (1<<3)                         

static void
Arcadia_ByteReader_UnicodeCodePointReader_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  )
{
  Arcadia_EnterConstructor(Arcadia_ByteReader_UnicodeCodePointReader);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->flags = FlagsNone;
  self->codePoint = 0xfffd;
  self->codePointByteIndex = 0;
  self->codePointByteLength = 0;
  self->source = (Arcadia_ByteReader*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_ByteReader_getType(thread));
  self->numberOfBytes = 0;
  self->bytes[0] = self->bytes[1] = self->bytes[2] = self->bytes[3] = 0;
  next(thread, self);
  Arcadia_LeaveConstructor(Arcadia_ByteReader_UnicodeCodePointReader);
}

static void
Arcadia_ByteReader_UnicodeCodePointReader_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReaderDispatch* self
  )
{
  ((Arcadia_UnicodeCodePointReaderDispatch*)self)->hasError = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_UnicodeCodePointReader*)) & Arcadia_ByteReader_UnicodeCodePointReader_hasErrorImpl;
  ((Arcadia_UnicodeCodePointReaderDispatch*)self)->getValue = (Arcadia_Natural32Value(*)(Arcadia_Thread*, Arcadia_UnicodeCodePointReader*)) & Arcadia_ByteReader_UnicodeCodePointReader_getValueImpl;
  ((Arcadia_UnicodeCodePointReaderDispatch*)self)->hasValue = (Arcadia_BooleanValue(*)(Arcadia_Thread*, Arcadia_UnicodeCodePointReader*)) & Arcadia_ByteReader_UnicodeCodePointReader_hasValueImpl;
  ((Arcadia_UnicodeCodePointReaderDispatch*)self)->nextValue = (void (*)(Arcadia_Thread*, Arcadia_UnicodeCodePointReader*)) & Arcadia_ByteReader_UnicodeCodePointReader_nextValueImpl;
  ((Arcadia_UnicodeCodePointReaderDispatch*)self)->getByteRange = (void (*)(Arcadia_Thread*, Arcadia_UnicodeCodePointReader*, Arcadia_SizeValue*, Arcadia_SizeValue*)) & Arcadia_ByteReader_UnicodeCodePointReader_getByteRangeImpl;
}

static void
Arcadia_ByteReader_UnicodeCodePointReader_visit
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  )
{
  if (self->source) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->source);
  }
}

static void
next
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  )
{ 
  if (self->flags == FlagsError || self->flags == FlagsShortRead) {
    // Advance to the next Byte.
    // Clear the error flag.
    self->flags = FlagsNone;
    self->codePointByteIndex += self->codePointByteLength;
    self->codePointByteLength = 0;
  } else if (self->flags == FlagsNone) {
    // Advance to the next Byte.
    self->codePointByteIndex += self->codePointByteLength;
    self->codePointByteLength = 0;
  } else if (self->flags == FlagsEnd) {
    return;
  }

  // Try to read at least one Byte.
  if (!self->numberOfBytes) {
    if (!Arcadia_ByteReader_hasValue(thread, self->source)) {
      self->flags = FlagsEnd;
      self->codePoint = 0xfffd;
      self->codePointByteLength = 0;
      return;
    }
    self->bytes[self->numberOfBytes++] = Arcadia_ByteReader_getValue(thread, self->source);
    Arcadia_ByteReader_nextValue(thread, self->source);
  }
  // If there are no Bytes, we reached the end or have an error. */
  if (self->numberOfBytes == 0) {
    if (Arcadia_ByteReader_hasError(thread, self->source)) {
      self->flags = FlagsError;
      self->codePoint = 0xfffd;
    } else {
      self->flags = FlagsEnd;
      self->codePoint = 0xfffd;
    }
    memmove(&self->bytes[0], &self->bytes[self->codePointByteLength], self->numberOfBytes - self->codePointByteLength);
    self->numberOfBytes -= self->codePointByteLength;
    return;
  }
  uint8_t byte = self->bytes[0];
  self->codePointByteLength = 0;
  if ((byte & 0x80) == 0x00) {
    // To determine if the first Byte is in the range 0xxx xxxx,
    // mask the Byte with 1000 0000 / 0x80. If the result is 0,
    // then the first Byte is in the range 0xxx xxxx.
    self->codePointByteLength = 1;
  } else if ((byte & 0xE0) == 0xC0) {
    // To determine if the first Byte is in the range 110x xxxx,
    // mask the Byte with 11100000 / 0xE0. If the result is 1100 0000 / 0xC0,
    // then the first Byte is in the range 110x xxxx.
    self->codePointByteLength = 2;
  } else if ((byte & 0xF0) == 0xE0) {
    // To determine if the first Byte is in the range 1110 xxxx,
    // mask the Byte with 1111 0000 / 0xF0. If the result is 1110 0000 / 0xE0,
    // then the first Byte is in the range 1110 xxxx.
    self->codePointByteLength = 3;
  } else if ((byte & 0xF8) == 0xF0) {
    // To determine if the first Byte is in the range 1111 0xxx,
    // mask the Byte with 1111 1000 / 0xF8. If the result is 1111 0000 / 0xF0,
    // then the first Byte is in th range 1111 0xxx.
    self->codePointByteLength = 4;
  } else {
    // This ensures that we skip this Byte.
    self->flags = FlagsError;
    self->codePoint = 0xfffd;
    self->codePointByteLength = 1;
    return;
  }
  while (self->codePointByteLength > self->numberOfBytes && Arcadia_ByteReader_hasValue(thread, self->source)) {
    self->bytes[self->numberOfBytes++] = Arcadia_ByteReader_getValue(thread, self->source);
    Arcadia_ByteReader_nextValue(thread, self->source); 
  }
  if (self->codePointByteLength > self->numberOfBytes) {
    // This is actually a short read and might be recoverable.
    self->flags = FlagsShortRead;
    self->codePoint = 0xfffd;
    self->codePointByteLength = 0;
    return;  
  }
  
  // The masks to remove the prefix bits from the first Byte.
  static const Arcadia_Natural8Value mask[] = {
    0b11111111,
    0b00011111,
    0b00001111,
    0b00000111,
  };
  
  self->codePoint = self->bytes[0] & mask[self->codePointByteLength - 1];
  for (Arcadia_SizeValue i = 1; i < self->codePointByteLength; ++i) {
    byte = self->bytes[0 + i]; // We need to mask with 0011 1111
    if (0x80 != (byte & 0xC0)) {
      self->flags = FlagsError; // Invalid 
      self->codePoint = 0xfffd;
      self->codePointByteLength = 0;
      memmove(&self->bytes[0], &self->bytes[self->codePointByteLength], self->numberOfBytes - self->codePointByteLength);
      self->numberOfBytes -= self->codePointByteLength;
      return;
    }
    byte &= 0x3F;
    self->codePoint <<= 6;
    self->codePoint |= byte;
  }
  
  if (self->codePointByteLength == 1 && self->codePoint <= 0x7f) {
  } else if (self->codePointByteLength == 2 && 0x80 <= self->codePoint && self->codePoint <= 0x7ff) {
  } else if (self->codePointByteLength == 3 && 0x800 <= self->codePoint && self->codePoint <= 0xffff) {
  } else if (self->codePointByteLength == 4 && 0x10000 <= self->codePoint && self->codePoint > 0x10ffff) {
  } else {
    self->flags = FlagsError; // Overlong encoding error.
    self->codePoint = 0xfffd;
    memmove(&self->bytes[0], &self->bytes[self->codePointByteLength], self->numberOfBytes - self->codePointByteLength);
    self->numberOfBytes -= self->codePointByteLength;
    return;
  }
  memmove(&self->bytes[0], &self->bytes[self->codePointByteLength], self->numberOfBytes - self->codePointByteLength);
  self->numberOfBytes -= self->codePointByteLength;
}

static void
Arcadia_ByteReader_UnicodeCodePointReader_nextValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  )
{
  if (self->flags == FlagsEnd) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  next(thread, self);
}

static Arcadia_Natural32Value
Arcadia_ByteReader_UnicodeCodePointReader_getValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  )
{
  if (self->flags == FlagsShortRead || self->flags == FlagsError || self->flags == FlagsEnd) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  return self->codePoint;
}

static Arcadia_BooleanValue
Arcadia_ByteReader_UnicodeCodePointReader_hasValueImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  )
{
  return !(self->flags == FlagsShortRead || self->flags == FlagsError || self->flags == FlagsEnd);
}

static Arcadia_BooleanValue
Arcadia_ByteReader_UnicodeCodePointReader_hasErrorImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self
  )
{
  return self->flags == FlagsError
      || self->flags == FlagsShortRead;
}

static void
Arcadia_ByteReader_UnicodeCodePointReader_getByteRangeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader_UnicodeCodePointReader* self,
    Arcadia_SizeValue* start,
    Arcadia_SizeValue* length
  )
{
  *start = self->codePointByteIndex;
  *length = self->codePointByteLength;
}

Arcadia_ByteReader_UnicodeCodePointReader*
Arcadia_ByteReader_UnicodeCodePointReader_create
  (
    Arcadia_Thread* thread,
    Arcadia_ByteReader* source
  )
{
  _Arcadia_BeginCreate(Arcadia_ByteReader_UnicodeCodePointReader);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)source);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_ByteReader_UnicodeCodePointReader);
}
