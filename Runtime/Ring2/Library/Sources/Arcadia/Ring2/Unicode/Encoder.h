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

#if !defined(ARCADIA_RING2_UNICODE_ENCODER_H_INCLUDED)
#define ARCADIA_RING2_UNICODE_ENCODER_H_INCLUDED

#if !defined(ARCADIA_RING2_MODULE)
  #error("do not include directly, include `Arcadia/Ring2/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
typedef struct Arcadia_ByteArrayBuilder Arcadia_ByteArrayBuilder;
typedef struct Arcadia_String Arcadia_String;

/// @code
/// class Arcadia.Unicode.Encoder {
///   
///   constructor()
///   
///   method encodeCodePoints(codePoints:ImmutableForeignMemory, numberOfCodePoints:Arcadia.SizeValue, target:Arcadia.ByteArrayBuilder)
///   
///   method encodeImmutableByteArray(source:Arcadia.ImmutableByteArray, target:Arcadia.ByteArrayBuilder)
///   
///   method encodeImmutableString(source:Arcadia.ImmutableString, target:Arcadia.ByteArrayBuilder)
///   
///   method encodeString(source:Arcadia.String, target:Arcadia.ByteArrayBuilder)
///   
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.Unicode.Encoder", Arcadia_Unicode_Encoder,
                          u8"Arcadia.Object");

struct Arcadia_Unicode_EncoderDispatch {
  Arcadia_ObjectDispatch _parent;
  void (*encodeCodePoints)(Arcadia_Thread*, Arcadia_Unicode_Encoder* self, Arcadia_Natural32Value const* codePoints, Arcadia_SizeValue numberOfCodePoints, Arcadia_ByteArrayBuilder* target);
  void (*encodeImmutableByteArray)(Arcadia_Thread* thread, Arcadia_Unicode_Encoder* self, Arcadia_RuntimeByteArray* source, Arcadia_ByteArrayBuilder* target);
  void (*encodeRuntimeUTF8String)(Arcadia_Thread* thread, Arcadia_Unicode_Encoder* self, Arcadia_RuntimeUTF8String* source, Arcadia_ByteArrayBuilder* target);
  void (*encodeString)(Arcadia_Thread* thread, Arcadia_Unicode_Encoder* self, Arcadia_String* source, Arcadia_ByteArrayBuilder* target);
};

struct Arcadia_Unicode_Encoder {
  Arcadia_Object _parent;
};

/// @brief Encode Unicode code points.
void
Arcadia_Unicode_Encoder_encodeCodePoints
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_Encoder* self,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints,
    Arcadia_ByteArrayBuilder* target
  );

/// @brief Encode an immutable Byte array.
void
Arcadia_Unicode_Encoder_encodeImmutableByteArray
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_Encoder* self,
    Arcadia_RuntimeByteArray* source,
    Arcadia_ByteArrayBuilder* target
  );

/// @brief Encode and immutable UTF8 string.
void
Arcadia_Unicode_Encoder_encodeRuntimeUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_Encoder* self,
    Arcadia_RuntimeUTF8String* source,
    Arcadia_ByteArrayBuilder* target
  );

/// @brief Encode a string.
void
Arcadia_Unicode_Encoder_encodeString
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_Encoder* self,
    Arcadia_String* source,
    Arcadia_ByteArrayBuilder* target
  );

#endif // ARCADIA_RING2_UNICODE_ENCODER_H_INCLUDED
