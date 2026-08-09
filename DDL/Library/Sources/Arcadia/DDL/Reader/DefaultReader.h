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

#if !defined(ARCADIA_DDL_READER_DEFAULTREADER_H_INCLUDED)
#define ARCADIA_DDL_READER_DEFAULTREADER_H_INCLUDED

#include "Arcadia/DDL/Nodes/Include.h"
#include "Arcadia/DDL/Reader/Parser.h"

/// @code
/// class Arcadia.DDL.DefaultReader
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.DDL.DefaultReader", Arcadia_DDL_DefaultReader,
                          u8"Arcadia.Object");

struct Arcadia_DDL_DefaultReaderDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_DDL_DefaultReader {
  Arcadia_Object parent;
  Arcadia_DDL_Parser* parser;
};

/// @brief Create a Data Definition Language default reader.
/// @param thread A pointer to this thread.
/// @return A pointer to the Data Definition Language default reader.
Arcadia_DDL_DefaultReader*
Arcadia_DDL_DefaultReader_create
  (
    Arcadia_Thread* thread
  );

/// @brief Run this DDL default reader.
/// @param thread A pointer to this thread.
/// @param self A pointer to this DDL default reader.
/// @return A pointer to the result DDL tree node.
Arcadia_DDL_Node*
Arcadia_DDL_DefaultReader_run
  (
    Arcadia_Thread* thread,
    Arcadia_DDL_DefaultReader* self,
    Arcadia_UnicodeCodePointReader* input
  );

#endif // ARCADIA_DDL_READER_DEFAULTREADER_H_INCLUDED
