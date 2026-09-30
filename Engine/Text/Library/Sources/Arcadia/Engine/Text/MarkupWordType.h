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

#if !defined(ARCADIA_ENGINE_TEXT_MARKUPWORDTYPE_H_INCLUDED)
#define ARCADIA_ENGINE_TEXT_MARKUPWORDTYPE_H_INCLUDED

#if !defined(ARCADIA_ENGINE_TEXT_PRIVATE) || 1 != ARCADIA_ENGINE_TEXT_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Text/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"

Arcadia_declareEnumerationType(u8"Arcadia.Engine.Text.MarkupWordType", Arcadia_Engine_Text_MarkupWordType);

typedef enum Arcadia_Engine_Text_MarkupWordType {
  Arcadia_Engine_Text_MarkupWordType_StartOfInput,
  Arcadia_Engine_Text_MarkupWordType_EndOfInput,
  Arcadia_Engine_Text_MarkupWordType_LeftBracket,
  Arcadia_Engine_Text_MarkupWordType_RightBracket,
  Arcadia_Engine_Text_MarkupWordType_LeftBrace,
  Arcadia_Engine_Text_MarkupWordType_RightBrace,
  Arcadia_Engine_Text_MarkupWordType_EqualsSign,
  Arcadia_Engine_Text_MarkupWordType_Slash,
  Arcadia_Engine_Text_MarkupWordType_LineBreak,
  Arcadia_Engine_Text_MarkupWordType_Name,
  Arcadia_Engine_Text_MarkupWordType_Text,
} Arcadia_Engine_Text_MarkupWordType;

#endif // ARCADIA_ENGINE_TEXT_MARKUPWORDTYPE_H_INCLUDED
