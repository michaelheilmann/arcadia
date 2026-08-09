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

#if !defined(ARCADIA_LANGUAGES_MANGLENAME_H_INCLUDED)
#define ARCADIA_LANGUAGES_MANGLENAME_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

// The name mangler encodes the sequence of UTF-8 Bytes (x_1, ..., x_n) of an input string
// as a prefix 'x' plus a sequence of hexadecimal numbers y_1, ..., y_, that is as ('x', y_1, ..., y_n).
// Each hexadecimal numbers are using small caps and is padded with a zero
// if its number of hex digits is smaller than 2.
// For example:
// a) the single Byte sequence (0) would be represented as ('x', '00')
// b) the single Byte sequence (7) would be represented by ('x', '07')
// c) the single Byte sequence (15) would be represented as ('x','0f')
// d) the single Byte sequence (128) would be represented as ('x', '80')
// e) the single Byte sequence (255) would be represented as xff.
// f) the two Byte sequence (1, 128) would be encoded as ('x', '01', '80').
// @param source The value of type Arcadia.String or Arcadia.StringBuilder.
Arcadia_String*
Arcadia_Languages_mangleName
  (
    Arcadia_Thread* thread,
    Arcadia_Value source
  );

#endif // ARCADIA_LANGUAGES_MANGLENAME_H_INCLUDED
