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

#if !defined(ARCADIA_LANGUAGES_PARSER_H_INCLUDED)
#define ARCADIA_LANGUAGES_PARSER_H_INCLUDED

#if !defined(ARCADIA_LANGUAGES_MODULE)
  #error("do not include directly, include `Arcadia/Languages/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Languages/StringTable.h"
#include "Arcadia/Languages/Diagnostics.h"

/// @brief The base of all parsers.
/// A parser is designed to perform syntactical analysis over a random-access finite immutable sequence of Bytes.
Arcadia_declareObjectType(u8"Arcadia.Languages.Parser", Arcadia_Languages_Parser,
                          u8"Arcadia.Object");

struct Arcadia_Languages_ParserDispatch {
  Arcadia_ObjectDispatch _parent;

  Arcadia_Languages_Diagnostics* (*getDiagnostics)(Arcadia_Thread* thread, Arcadia_Languages_Parser* self);
  Arcadia_UnicodeCodePointReader* (*getInput)(Arcadia_Thread* thread, Arcadia_Languages_Parser* self);
  Arcadia_Languages_StringTable* (*getStringTable)(Arcadia_Thread* thread, Arcadia_Languages_Parser* self);
  Arcadia_Value (*run)(Arcadia_Thread* thread, Arcadia_Languages_Parser* self);
  void (*setInput)(Arcadia_Thread* thread, Arcadia_Languages_Parser* self, Arcadia_UnicodeCodePointReader* input);
};

struct Arcadia_Languages_Parser {
  Arcadia_Object _parent;

  /// @brief The input string.
  Arcadia_String* input;
};

/// @brief Get the diagnostics used by this parser.
/// @param thread A pointer this thread.
/// @param self A pointer to this parser.
/// @return A pointer to the diagnostics.
Arcadia_Languages_Diagnostics*
Arcadia_Languages_Parser_getDiagnostics
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  );

/// @brief Get the input.
/// @param thread A pointer to this thread.
/// @param self A pointer to this parser.
/// @return A pointer to the input string.
Arcadia_UnicodeCodePointReader*
Arcadia_Languages_Parser_getInput
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  );

/// @brief Get the string table used by this parser.
/// @param thread A pointer this thread.
/// @param self A pointer to this parser.
/// @return A pointer to the string table.
Arcadia_Languages_StringTable*
Arcadia_Languages_Parser_getStringTable
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  );

/// @brief Run this parser on an input.
/// @param thread A pointer to this thread.
/// @param self A pointer to this parser.
/// @return A value representing the result.
/// This is usually an Arcadia.Object or derived type object representing the root of a conrete tree or an abstract syntax tree.
Arcadia_Value
Arcadia_Languages_Parser_run
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self
  );

/// @brief Set the input.
/// @param thread A pointer to this thread.
/// @param self A pointer to this parser.
/// @param input A pointer to the input string.
void
Arcadia_Languages_Parser_setInput
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Parser* self,
    Arcadia_UnicodeCodePointReader* input
  );

#endif // ARCADIA_LANGUAGES_PARSER_H_INCLUDED
