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

#if !defined(ARCADIA_TEMPLATEENGINE_PARSER_PARSER_H_INCLUDED)
#define ARCADIA_TEMPLATEENGINE_PARSER_PARSER_H_INCLUDED

#include "Arcadia.TemplateEngine/Scanner/Scanner.h"
#include "Arcadia.TemplateEngine/Parser/Tree.h"

Arcadia_declareObjectType(u8"Arcadia.TemplateEngine.Parser", Arcadia_TemplateEngine_Parser,
                          u8"Arcadia.Languages.Parser");

struct Arcadia_TemplateEngine_ParserDispatch {
  Arcadia_Languages_ParserDispatch _parent;
};

struct Arcadia_TemplateEngine_Parser {
  Arcadia_Languages_Parser _parent;
  /// The diagnostics.
  Arcadia_Languages_Diagnostics* diagnostics;
  /// The scanner.
  Arcadia_TemplateEngine_Scanner* scanner;
};

Arcadia_TemplateEngine_Parser*
Arcadia_TemplateEngine_Parser_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics,
	  Arcadia_String* file,
	  Arcadia_Natural64Value line,
    Arcadia_UnicodeCodePointReader* reader
  );

void
Arcadia_TemplateEngine_Parser_setPosition
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Parser* self,
    Arcadia_String* file,
    Arcadia_Natural64Value line
  );

#endif // ARCADIA_TEMPLATEENGINE_PARSER_PARSER_H_INCLUDED
