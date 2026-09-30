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

#if !defined(ARCADIA_ENGINE_TEXT_MARKUPTEXTRUN_H_INCLUDED)
#define ARCADIA_ENGINE_TEXT_MARKUPTEXTRUN_H_INCLUDED

#if !defined(ARCADIA_ENGINE_TEXT_PRIVATE) || 1 != ARCADIA_ENGINE_TEXT_PRIVATE
  #error("do not include directly, include `Arcadia/Engine/Text/Include.h` instead")
#endif

#include "Arcadia/Engine/Text/TextAlignment.h"
#include "Arcadia/Ring2/Include.h"

Arcadia_declareObjectType(u8"Arcadia.Engine.Text.MarkupTextRun", Arcadia_Engine_Text_MarkupTextRun,
                          u8"Arcadia.Object");

struct Arcadia_Engine_Text_MarkupTextRunDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_Engine_Text_MarkupTextRun {
  Arcadia_Object parent;
  Arcadia_String* text;
  Arcadia_String* glyphSource;
  Arcadia_String* dynamicToken;
  Arcadia_Natural32Value pixelSize;
  Arcadia_String* colorName;
  Arcadia_Engine_Text_TextAlignment alignment;
  Arcadia_BooleanValue hasRectangle;
  Arcadia_Integer32Value rectangleLeft;
  Arcadia_Integer32Value rectangleBottom;
  Arcadia_Integer32Value rectangleWidth;
  Arcadia_Integer32Value rectangleHeight;
  Arcadia_BooleanValue fixedPlacement;
};

Arcadia_Engine_Text_MarkupTextRun*
Arcadia_Engine_Text_MarkupTextRun_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* text,
    Arcadia_String* glyphSource,
    Arcadia_String* dynamicToken,
    Arcadia_Natural32Value pixelSize,
    Arcadia_String* colorName,
    Arcadia_Engine_Text_TextAlignment alignment,
    Arcadia_BooleanValue fixedPlacement
  );

#endif // ARCADIA_ENGINE_TEXT_MARKUPTEXTRUN_H_INCLUDED
