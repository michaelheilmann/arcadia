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

#if !defined(ARCADIA_ADL_DEFINITIONS_UI_BUTTONREADER_MODULE_H_INCLUDED)
#define ARCADIA_ADL_DEFINITIONS_UI_BUTTONREADER_MODULE_H_INCLUDED

#if !defined(ARCADIA_ADL_MODULE)
  #error("do not include directly, include `Arcadia/ADL/Include.h` instead")
#endif
#include "Arcadia/ADL/Reader.h"
#include "Arcadia/DDLS/Include.h"

Arcadia_declareObjectType(u8"Arcadia.ADL.UI.ButtonReader", Arcadia_ADL_UI_ButtonReader,
                          u8"Arcadia.ADL.Reader");

struct Arcadia_ADL_UI_ButtonReaderDispatch {
  Arcadia_ADL_ReaderDispatch _parent;
};

struct Arcadia_ADL_UI_ButtonReader {
  Arcadia_ADL_Reader _parent;
  Arcadia_String* TYPE;
  Arcadia_String* NAME;
  Arcadia_String* LEFT;
  Arcadia_String* BOTTOM;
  Arcadia_String* WIDTH;
  Arcadia_String* HEIGHT;
  Arcadia_String* TEXT;
  Arcadia_String* HORIZONTAL_ALIGNMENT;
  Arcadia_String* VERTICAL_ALIGNMENT;
  Arcadia_String* BACKGROUND_COLOR;
  Arcadia_String* HOVERED_BACKGROUND_COLOR;
  Arcadia_String* PRESSED_BACKGROUND_COLOR;
  Arcadia_String* DISABLED_BACKGROUND_COLOR;
  Arcadia_String* FOCUSED_BACKGROUND_COLOR;
  Arcadia_String* BACKGROUND_RED;
  Arcadia_String* BACKGROUND_GREEN;
  Arcadia_String* BACKGROUND_BLUE;
  Arcadia_String* SCHEMANAME;
  Arcadia_String* TYPENAME;
  Arcadia_String* CENTER;
  Arcadia_DDLS_ValidationContext* validationContext;
};

Arcadia_ADL_UI_ButtonReader*
Arcadia_ADL_UI_ButtonReader_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_ADL_DEFINITIONS_UI_BUTTONREADER_MODULE_H_INCLUDED
