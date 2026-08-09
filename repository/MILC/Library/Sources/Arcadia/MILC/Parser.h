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

#if !defined(ARCADIA_MILC_PARSER_H_INCLUDED)
#define ARCADIA_MILC_PARSER_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia/FileSystem/Include.h"
typedef struct Arcadia_MILC_Context Arcadia_MILC_Context;

/// @code
/// class Arcadia.MILC.Parser extends Arcadia.Languages.Parser {
///   method run(input : Arcadia.String) : Arcadia.MILC.AST.CompilationUnit
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MIL.Parser", Arcadia_MILC_Parser,
                          u8"Arcadia.Languages.Parser");

/// @brief Get or create the MIL parser in this context.
/// @param thread A pointer to this thread.
/// @param context A pointer to this context.
/// @return A pointer to the MIL parser.
Arcadia_MILC_Parser*
Arcadia_MILC_Parser_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

void
Arcadia_MILC_Parser_setFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Parser* self,
    Arcadia_Languages_InputFile* file
  );

#endif // ARCADIA_MILC_PARSER_H_INCLUDED

