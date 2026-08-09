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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_IMMUTABLEUTF8STRINGEXTENSIONS_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_IMMUTABLEUTF8STRINGEXTENSIONS_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String.h"
#include "Arcadia/Ring1/Implementation/Value.h"

/// @brief Get the first occurence of a code point in a string.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @param codePoint the code point.
/// @return An Arcadia.Size value denoting the zero-based code point index from the start of the string if the code point was found, Arcadia.Void otherwise.
Arcadia_Value
Arcadia_RuntimeUTF8String_findFirstOccurrence
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* self,
    Arcadia_Natural32Value codePoint
  );

/// @brief Get the first occurence of a code point in a string.
/// @param thread A pointer to this thread.
/// @param self A pointer to this string.
/// @param codePoint the code point.
/// @return An Arcadia.Size value denoting the zero-based code point index from the start of the string if the code point was found, Arcadia.Void otherwise.
Arcadia_Value
Arcadia_RuntimeUTF8String_findLastOccurrence
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* self,
    Arcadia_Natural32Value codePoint
  );

/// @brief Create an empty string.
/// @param thread A pointer to this thread.
/// @return A pointer to the string.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_createEmpty
  (
    Arcadia_Thread* thread
  );

/// @brief Create a string that is a substring of the specified string.
/// @param start The code point index of at which the substring starts at.
/// @param length The length, in code points, of the substring as a Arcadia_SizeValue or an Arcadia_Void value.
/// @return A pointer to the substring.
Arcadia_RuntimeUTF8String*
Arcadia_RuntimeUTF8String_substring
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* self,
    Arcadia_SizeValue start,
    Arcadia_Value length
  );

#endif // ARCADIA_RING1_IMPLEMENTATION_IMMUTABLEUTF8STRINGEXTENSIONS_H_INCLUDED
