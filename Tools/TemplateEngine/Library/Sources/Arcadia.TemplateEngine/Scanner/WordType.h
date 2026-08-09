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

#if !defined(ARCADIA_TEMPLATEENGINE_SCANNER_WORDTYPE_H_INCLUDED)
#define ARCADIA_TEMPLATEENGINE_SCANNER_WORDTYPE_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

Arcadia_declareEnumerationType(u8"Arcadia.TemplateEngine.WordType", Arcadia_TemplateEngine_WordType);

enum Arcadia_TemplateEngine_WordType {
  Arcadia_TemplateEngine_WordType_StartOfInput,
  Arcadia_TemplateEngine_WordType_EndOfInput,
  Arcadia_TemplateEngine_WordType_Error,

  Arcadia_TemplateEngine_WordType_AtLiteral,
  Arcadia_TemplateEngine_WordType_Name,
  Arcadia_TemplateEngine_WordType_StringLiteral,
  
  Arcadia_TemplateEngine_WordType_Comma,

  Arcadia_TemplateEngine_WordType_LeftCurlyBracket,
  Arcadia_TemplateEngine_WordType_LeftParenthesis,
  
  Arcadia_TemplateEngine_WordType_RightCurlyBracket,
  Arcadia_TemplateEngine_WordType_RightParenthesis,

  Arcadia_TemplateEngine_WordType_StartOfDirective,
  Arcadia_TemplateEngine_WordType_Text,
  
};

#endif // ARCADIA_TEMPLATEENGINE_SCANNER_WORDTYPE_H_INCLUDED
