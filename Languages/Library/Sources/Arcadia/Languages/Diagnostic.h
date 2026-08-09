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

#if !defined(ARCADIA_LANGUAGES_DIAGNOSTIC_H_INCLUDED)
#define ARCADIA_LANGUAGES_DIAGNOSTIC_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Languages/DiagnosticType.h"
typedef struct Arcadia_Languages_InputFile Arcadia_Languages_InputFile;

/// @brief A diagnostic message of the compiler.
Arcadia_declareObjectType(u8"Arcadia.Languages.Diagnostic", Arcadia_Languages_Diagnostic,
                          u8"Arcadia.Object");

struct Arcadia_Languages_DiagnosticDispatch {
  Arcadia_ObjectDispatch _parent;
  Arcadia_String *(*getMessage)(Arcadia_Thread*, Arcadia_Languages_Diagnostic*);
};

struct Arcadia_Languages_Diagnostic {
  Arcadia_Object _parent;
  /// @brief The diagnostic type.
  Arcadia_Languages_DiagnosticType type;
  /// @brief The input file, if any, to which this diagnostic applies.
  /// A pointer to the input file if there is an input file, null otherwise.
  Arcadia_Languages_InputFile* inputFile;
  /// @brief The input position, if any, to which this diagnostic applies.
  /// Arcadia.Size if there is an input position, Arcadia.Void otherwise.
  Arcadia_Value inputPosition;
};

/// @brief Get the input file, if any, to which this diagnostic applies.
/// @param thread A pointer to this thread.
/// @param self A pointer to this diagnostic.
/// @return The input file or null.
Arcadia_Languages_InputFile*
Arcadia_Languages_Diagnostic_getInputFile
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
  );

/// @brief Get the offset, if any, to which this diagnostic applies.
/// @param thread A pointer to this thread.
/// @param self A pointer to this diagnostic.
/// @return An Arcadia.Size value if specified, Arcadia.Void otherwise.
Arcadia_Value
Arcadia_Languages_Diagnostic_getInputPosition
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
  );

/// @brief Get the type of this diagnostic.
/// @param thread A pointer to this thread.
/// @param self A pointer to this diagnostic.
/// @return The diagnostic type.
Arcadia_Languages_DiagnosticType
Arcadia_Languages_Diagnostic_getType
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
  );

/// @brief Get the message of this diagnostic.
/// @param thread A pointer to this thread.
/// @param self A pointer to this diagnostic.
/// @return The diagnostic message.
Arcadia_String*
Arcadia_Languages_Diagnostic_getMessage
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostic* self
  );

#endif // ARCADIA_LANGUAGES_DIAGNOSTIC_H_INCLUDED
