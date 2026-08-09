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

#if !defined(ARCADIA_TEMPLATEENGINE_SCANNER_SCANNER_H_INCLUDED)
#define ARCADIA_TEMPLATEENGINE_SCANNER_SCANNER_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia.TemplateEngine/Scanner/WordType.h"

Arcadia_declareObjectType(u8"Arcadia.TemplateEngine.Scanner", Arcadia_TemplateEngine_Scanner,
                          u8"Arcadia.Languages.Scanner");

struct Arcadia_TemplateEngine_ScannerDispatch {
  Arcadia_Languages_ScannerDispatch _parent;
};

struct Arcadia_TemplateEngine_Scanner {
  Arcadia_Languages_Scanner _parent;

  /// @brief @a true the scanner is in text mode.
  /// @a false the scanner is in directive mode.
  bool textMode;

  /// @brief The diagnostics used by this scanner.
  Arcadia_Languages_Diagnostics* diagnostics;

  /// @brief The string table used by this scanner.
  Arcadia_Languages_StringTable* stringTable;

  // The file.
  Arcadia_String* file;
  // The line.
  Arcadia_Natural64Value line;
  // The reader.
  Arcadia_UnicodeCodePointReader* reader;

  Arcadia_TemplateEngine_WordType wordType;
  // Scanner: Writer to word text buffer.
  Arcadia_Unicode_Encoder* temporary;
  // Scanner: Word text buffer.
  Arcadia_ByteArrayBuilder* temporaryBuffer;
};

Arcadia_TemplateEngine_Scanner*
Arcadia_TemplateEngine_Scanner_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics,
    Arcadia_String* file,
    Arcadia_Natural64Value line,
    Arcadia_UnicodeCodePointReader* reader
  );

void
Arcadia_TemplateEngine_Scanner_setPosition
  (
    Arcadia_Thread* thread,
    Arcadia_TemplateEngine_Scanner* self,
    Arcadia_String* file,
    Arcadia_Natural64Value line
  );

#endif // ARCADIA_TEMPLATEENGINE_SCANNER_SCANNER_H_INCLUDED
