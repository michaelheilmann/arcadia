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

#if !defined(ARCADIA_MILC_KEYWORDS_H_INCLUDED)
#define ARCADIA_MILC_KEYWORDS_H_INCLUDED

#include "Arcadia/MILC/WordType.h"
typedef struct Arcadia_MILC_Context Arcadia_MILC_Context;

/// @code
/// class Arcadia.MILC.Keywords
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.Keywords", Arcadia_MILC_Keywords,
                          u8"Arcadia.Object");

/// @brief Create a MIL keywords object.
/// @param context The context.
/// @return A pointer to this MIL keywords object.
Arcadia_MILC_Keywords*
Arcadia_MILC_Keywords_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

/// @brief Add a MIL keyword.
/// @param string The string of the keyword.
/// @param wordType The word type of the keyword. Must not be of type "void".
/// @error Arcadia_Status_Exists The keyword already was added.
void
Arcadia_MILC_Keywords_add
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Keywords* self,
    Arcadia_String* string,
    Arcadia_Value wordType
  );

/// @brief Get the token type.
/// @param self A pointer to this MIL keyword scanner.
/// @param string The string to be tested for being a keyword.
/// @param wordType A pointer to a variable that is assigned the keyword's token type if the string is a keyword. Not modified otherwise.
/// @return A value of a type other than "void" denoting the word type. A value of type "void" otherwise.
Arcadia_Value
Arcadia_MILC_Keywords_scan
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Keywords* self,
    Arcadia_String* string
  );

#endif // ARCADIA_MILC_KEYWORDS_H_INCLUDED

