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

#if !defined(ARCADIA_DDL_WRITER_UNPARSER_H_INCLUDED)
#define ARCADIA_DDL_WRITER_UNPARSER_H_INCLUDED

#if !defined(ARCADIA_DDL_MODULE) || 1 != ARCADIA_DDL_MODULE
  #error("do not include directly, include `Arcadia/DDL/Include.h` instead")
#endif

#include "Arcadia/DDL/Nodes/Include.h"

/// @code
/// class Arcadia.DataDefinitionLanguage.Unparser
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.DataDefinitionLanguage.Unparser", Arcadia_DataDefinitionLanguage_Unparser,
                          u8"Arcadia.Object");

/// @brief Create a Data Definition Language unparser.
/// @param thread A pointer to this thread.
/// @param encoder The encoder to encode the output.
/// @return A pointer to the Data Definition Language unparser.
Arcadia_DataDefinitionLanguage_Unparser*
Arcadia_DataDefinitionLanguage_Unparser_create
  (
    Arcadia_Thread* thread,
    Arcadia_Unicode_Encoder* encoder
  );

/// @brief Move to next token.
/// @param thread A pointer to the thread.
/// @param self A pointer to this Data Definition Language unparser.
/// @param node A pointer to the node to write.
/// @param target The Byte array builder to which the output is appended to.
/// @error Arcadia_Status_SemanticalError two map contains two entries with the same key
/// @error Arcadia_Status_LexicalError a literal string (boolean, number, string, void) does not represent a valid literal
void
Arcadia_DataDefinitionLanguage_Unparser_run
  (
    Arcadia_Thread* thread,
    Arcadia_DataDefinitionLanguage_Unparser* self,
    Arcadia_DDL_Node* node,
    Arcadia_ByteArrayBuilder* targetBuffer
  );

#endif // ARCADIA_DDL_WRITER_UNPARSER_H_INCLUDED
