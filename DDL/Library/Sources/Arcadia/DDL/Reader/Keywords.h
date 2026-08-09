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

#if !defined(ARCADIA_DDL_READER_KEYWORDS_H_INCLUDED)
#define ARCADIA_DDL_READER_KEYWORDS_H_INCLUDED

#include "Arcadia/DDL/Reader/WordType.h"

/// @code
/// class Arcadia.DataDefinitionLanguage.Keywords
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.DataDefinitionLanguage.Keywords", Arcadia_DataDefinitionLanguage_Keywords,
                          u8"Arcadia.Object");

/// @brief Create a Data Definition Language keywords object.
/// @return A pointer to this Data Definition Language keywords object.
Arcadia_DataDefinitionLanguage_Keywords*
Arcadia_DataDefinitionLanguage_Keywords_create
  (
    Arcadia_Thread* thread
  );

/// @brief Add a Data Definition Language keyword.
/// @param string The string of the keyword.
/// @param type The type of the keyword
/// @error Arcadia_Status_Exists The keyword already was added.
void
Arcadia_DataDefinitionLanguage_Keywords_add
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Keywords* self,
    Arcadia_String* string,
    Arcadia_Natural32Value type
  );

/// @brief Get the token type.
/// @param self A pointer to this Data Definition Language keyword scanner.
/// @param string The string to be tested for being a keyword.
/// @param tokenType A pointer to a variable that is assigned the keyword's token type if the string is a keyword. Not modified otherwise.
/// @return Arcadia_BooleanValue_True if the specified string was a keyword. #Arcadia_BooleanValue_False otherwise.
Arcadia_BooleanValue
Arcadia_DataDefinitionLanguage_Keywords_scan
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Keywords* self,
    Arcadia_String* string,
    Arcadia_Natural32Value* tokenType
  );

#endif // ARCADIA_DDL_READER_KEYWORDS_H_INCLUDED

