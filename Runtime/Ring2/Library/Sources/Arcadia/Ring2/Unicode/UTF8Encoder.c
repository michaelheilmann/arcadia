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
#include "Arcadia/Ring2/Unicode/UTF8Encoder.h"

#include "Arcadia/Ring2/Implementation/ArgumentsValidation.h"
#include "Arcadia/Ring1/Include.h"

static void
Arcadia_Unicode_UTF8Encoder_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_UTF8Encoder* self
  );

static void
Arcadia_Unicode_UTF8Encoder_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_UTF8EncoderDispatch* self
  );

static void
Arcadia_Unicode_UTF8Encoder_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_UTF8Encoder* self
  );

static void
Arcadia_Unicode_UTF8Encoder_encodeCodePointsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_UTF8Encoder* self,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints,
    Arcadia_ByteArrayBuilder* target
  );

static void
Arcadia_Unicode_UTF8Encoder_encodeRuntimeUTF8StringImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_UTF8Encoder* self,
    Arcadia_RuntimeUTF8String* string,
    Arcadia_ByteArrayBuilder* target
  );

static void
Arcadia_Unicode_UTF8Encoder_encodeStringImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_UTF8Encoder* self,
    Arcadia_String* string,
    Arcadia_ByteArrayBuilder* target
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Unicode_UTF8Encoder_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Unicode_UTF8Encoder_visit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Unicode_UTF8Encoder_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Unicode.ByteBufferEncoder", Arcadia_Unicode_UTF8Encoder,
                         u8"Arcadia.Unicode.Encoder", Arcadia_Unicode_Encoder,
                         &_typeOperations);

static void
Arcadia_Unicode_UTF8Encoder_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_UTF8Encoder* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Unicode_UTF8Encoder);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Unicode_UTF8Encoder);
}

static void
Arcadia_Unicode_UTF8Encoder_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_UTF8EncoderDispatch* self
  )
{
  ((Arcadia_Unicode_EncoderDispatch*)self)->encodeCodePoints = (void (*)(Arcadia_Thread*, Arcadia_Unicode_Encoder*, Arcadia_Natural32Value const*, Arcadia_SizeValue, Arcadia_ByteArrayBuilder*)) & Arcadia_Unicode_UTF8Encoder_encodeCodePointsImpl;
  ((Arcadia_Unicode_EncoderDispatch*)self)->encodeRuntimeUTF8String = (void (*)(Arcadia_Thread*, Arcadia_Unicode_Encoder*, Arcadia_RuntimeUTF8String*, Arcadia_ByteArrayBuilder*)) & Arcadia_Unicode_UTF8Encoder_encodeRuntimeUTF8StringImpl;
  ((Arcadia_Unicode_EncoderDispatch*)self)->encodeString = (void (*)(Arcadia_Thread*, Arcadia_Unicode_Encoder*, Arcadia_String*, Arcadia_ByteArrayBuilder*)) & Arcadia_Unicode_UTF8Encoder_encodeStringImpl;
}

static void
Arcadia_Unicode_UTF8Encoder_visit
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_UTF8Encoder* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Unicode_UTF8Encoder_encodeCodePointsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_UTF8Encoder* self,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints,
    Arcadia_ByteArrayBuilder* target
  )
{
  Arcadia_Unicode_encodeCodePointsUTF8(thread, codePoints, numberOfCodePoints, target, (Arcadia_Unicode_EncodeCodePointCallbackFunction*)&Arcadia_ByteArrayBuilder_insertBackBytes);
}

static void
Arcadia_Unicode_UTF8Encoder_encodeRuntimeUTF8StringImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_UTF8Encoder* self,
    Arcadia_RuntimeUTF8String* string,
    Arcadia_ByteArrayBuilder* target
  )
{
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, target, Arcadia_RuntimeUTF8String_getBytes(thread, string), Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, string));
}

static void
Arcadia_Unicode_UTF8Encoder_encodeStringImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_UTF8Encoder* self,
    Arcadia_String* string,
    Arcadia_ByteArrayBuilder* target
  )
{
  Arcadia_ByteArrayBuilder_insertBackBytes(thread, target, Arcadia_String_getBytes(thread, string), Arcadia_String_getNumberOfBytes(thread, string));
}

Arcadia_Unicode_UTF8Encoder*
Arcadia_Unicode_UTF8Encoder_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Unicode_UTF8Encoder);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Unicode_UTF8Encoder);
}
