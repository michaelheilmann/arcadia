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

#if !defined(ARCADIA_DDL_READER_PARSER_H_INCLUDED)
#define ARCADIA_DDL_READER_PARSER_H_INCLUDED

#include "Arcadia/DDL/Nodes/Include.h"
#include "Arcadia/Languages/Include.h"
#include "Arcadia/DDL/Reader/Scanner.h"

/// @code
/// class Arcadia.DDL.Parser {
///   method run(input : Arcadia.String) : Arcadia.DDL.Node
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.DDL.Parser", Arcadia_DDL_Parser,
                          u8"Arcadia.Languages.Parser");

/// @brief Create a Data Definition Language parser.
/// @return A pointer to the Data Definition Language parser.
Arcadia_DDL_Parser*
Arcadia_DDL_Parser_create
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_Scanner* scanner
  );

#endif // ARCADIA_DDL_READER_PARSER_H_INCLUDED
