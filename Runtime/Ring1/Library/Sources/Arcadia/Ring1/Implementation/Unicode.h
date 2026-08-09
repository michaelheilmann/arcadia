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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_UNICODE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_UNICODE_H_INCLUDED

#include "Arcadia/Ring1/Implementation/StaticAssert.h"

#include "Arcadia/Ring1/Implementation/Unicode/Unicode.h"
#include "Arcadia/Ring1/Implementation/Unicode/isUTF8.h"
#include "Arcadia/Ring1/Implementation/Unicode/UTF8.h"
#include "Arcadia/Ring1/Implementation/Unicode/UTF8ArrayIterator.h"

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_Unicode_EncodeCodePointCallbackContext
typedef void Arcadia_Unicode_EncodeCodePointCallbackContext;

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_Unicode_EncodeCodePointCallbackFunction
typedef void (Arcadia_Unicode_EncodeCodePointCallbackFunction)(Arcadia_Thread*, Arcadia_Unicode_EncodeCodePointCallbackContext* context, const Arcadia_Natural8Value* bytes, Arcadia_SizeValue numberOfBytes);

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_Unicode_encodeCodePointsUTF8
void
Arcadia_Unicode_encodeCodePointsUTF8
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints,
    Arcadia_Unicode_EncodeCodePointCallbackContext* context,
    Arcadia_Unicode_EncodeCodePointCallbackFunction* function
  );

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_Unicode_encodeCodePointsUtf32Be
void
Arcadia_Unicode_encodeCodePointsUtf32Be
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints,
    Arcadia_Unicode_EncodeCodePointCallbackContext* context,
    Arcadia_Unicode_EncodeCodePointCallbackFunction* function
  );

// https://michaelheilmann.com/Arcadia.Ring1/#Arcadia_Unicode_encodeCodePointsUtf32Le
void
Arcadia_Unicode_encodeCodePointsUtf32Le
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value const* codePoints,
    Arcadia_SizeValue numberOfCodePoints,
    Arcadia_Unicode_EncodeCodePointCallbackContext* context,
    Arcadia_Unicode_EncodeCodePointCallbackFunction* function
  );

/// @brief
/// Validate a sequence of UTF8 Bytes and write the Bytes into.
/// @remarks
/// This function also validates if the sequence of Natural8 values is a valid code points.
void
Arcadia_Unicode_encodeBytesUTF8
  (
    Arcadia_Thread* thread,
    Arcadia_Natural8Value const* bytes,
    Arcadia_SizeValue numberOfBytes,
    Arcadia_Unicode_EncodeCodePointCallbackContext* context,
    Arcadia_Unicode_EncodeCodePointCallbackFunction* function
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_UNICODE_H_INCLUDED
