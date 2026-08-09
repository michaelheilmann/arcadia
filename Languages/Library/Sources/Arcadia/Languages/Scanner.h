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

#if !defined(ARCADIA_LANGUAGES_SCANNER_H_INCLUDED)
#define ARCADIA_LANGUAGES_SCANNER_H_INCLUDED

#if !defined(ARCADIA_LANGUAGES_MODULE)
  #error("do not include directly, include `Arcadia/Languages/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Languages/StringTable.h"
#include "Arcadia/Languages/Diagnostics.h"

/// @brief The base of all scanners.
Arcadia_declareObjectType(u8"Arcadia.Languages.Scanner", Arcadia_Languages_Scanner,
                          u8"Arcadia.Object");

struct Arcadia_Languages_ScannerDispatch {
  Arcadia_ObjectDispatch _parent;

  Arcadia_String* (*getWordText)(Arcadia_Thread* thread, Arcadia_Languages_Scanner* self);

  Arcadia_Integer32Value(*getWordType)(Arcadia_Thread* thread, Arcadia_Languages_Scanner* self);

  Arcadia_Natural32Value(*getWordStart)(Arcadia_Thread* thread, Arcadia_Languages_Scanner* self);
  Arcadia_Natural32Value(*getWordLength)(Arcadia_Thread* thread, Arcadia_Languages_Scanner* self);

  void (*step)(Arcadia_Thread* thread, Arcadia_Languages_Scanner* self);

  void (*setInput)(Arcadia_Thread* thread, Arcadia_Languages_Scanner* self, Arcadia_UnicodeCodePointReader* reader);
  Arcadia_UnicodeCodePointReader* (*getInput)(Arcadia_Thread* thread, Arcadia_Languages_Scanner* self);

  Arcadia_Languages_StringTable* (*getStringTable)(Arcadia_Thread* thread, Arcadia_Languages_Scanner* self);
  Arcadia_Languages_Diagnostics* (*getDiagnostics)(Arcadia_Thread* thread, Arcadia_Languages_Scanner* self);
};

struct Arcadia_Languages_Scanner {
  Arcadia_Object _parent;
};

/// @brief Get the text of the word.
/// @param thread A pointer to this thread.
/// @param self A pointer to this scanner.
/// @return The text of the word.
Arcadia_String*
Arcadia_Languages_Scanner_getWordText
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scanner* self
  );

/// @brief Get the type of the word.
/// @param thread A pointer to this thread.
/// @param self A pointer to this scanner.
/// @return The type of the word.
Arcadia_Integer32Value
Arcadia_Languages_Scanner_getWordType
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scanner* self
  );

/// @brief Get the start of the word as an offset in Bytes from the beginning of the input.
/// @param thread A pointer to this thread.
/// @param self A pointer to this scanner.
/// @return The start of the word as an offset in Bytes from the beginning of the input.
Arcadia_Natural32Value
Arcadia_Languages_Scanner_getWordStart
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scanner* self
  );

/// @brief Get the length of the current word in Bytes.
/// @param thread A pointer to this thread.
/// @param self A pointer to this scanner.
/// @return The length of the word as a length in Bytes.
Arcadia_Natural32Value
Arcadia_Languages_Scanner_getWordLength
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scanner* self
  );

/// @brief Move to next word.
/// @param thread A pointer to this thread.
/// @param self A pointer to this scanner.
void
Arcadia_Languages_Scanner_step
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scanner* self
  );

/// @brief Set the input.
/// @param thread A pointer to this thread.
/// @param self A pointer to this scanner.
/// @param input A pointer to the input string.
void
Arcadia_Languages_Scanner_setInput
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scanner* self,
    Arcadia_UnicodeCodePointReader* input
  );

/// @brief Get the input.
/// @param thread A pointer to this thread.
/// @param self A pointer to this scanner.
/// @return A pointer to the input string.
Arcadia_UnicodeCodePointReader*
Arcadia_Languages_Scanner_getInput
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scanner* self
  );

/// @brief Get the string table used by this scanner.
/// @param thread A pointer to this thread.
/// @param self A pointer to this scanner.
/// @return A pointer to the string table used by this scanner.
Arcadia_Languages_StringTable*
Arcadia_Languages_Scanner_getStringTable
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scanner* self
  );

/// @brief Get the diagnostics used by this scanner.
/// @param thread A pointer to this thread.
/// @param self A pointer to this scanner.
/// @return A pointer to the diagnostics used by this scanner.
Arcadia_Languages_Diagnostics*
Arcadia_Languages_Scanner_getDiagnostics
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Scanner* self
  );

#endif // ARCADIA_LANGUAGES_SCANNER_H_INCLUDED
