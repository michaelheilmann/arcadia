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

#if !defined(ARCADIA_RING2_UNICODE_UTF8ENCODER_H_INCLUDED)
#define ARCADIA_RING2_UNICODE_UTF8ENCODER_H_INCLUDED

#if !defined(ARCADIA_RING2_MODULE)
  #error("do not include directly, include `Arcadia/Ring2/Include.h` instead")
#endif

#include "Arcadia/Ring2/Unicode/Encoder.h"
typedef struct Arcadia_ByteArrayBuilder Arcadia_ByteArrayBuilder;

Arcadia_declareObjectType(u8"Arcadia.Unicode.UTF8Encoder", Arcadia_Unicode_UTF8Encoder,
                          u8"Arcadia.Unicode.Encoder");

struct Arcadia_Unicode_UTF8EncoderDispatch {
  Arcadia_Unicode_EncoderDispatch _parent;
};

struct Arcadia_Unicode_UTF8Encoder {
  Arcadia_Unicode_Encoder parent;
};

Arcadia_Unicode_UTF8Encoder*
Arcadia_Unicode_UTF8Encoder_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_RING2_UNICODE_UTF8ENCODER_H_INCLUDED
