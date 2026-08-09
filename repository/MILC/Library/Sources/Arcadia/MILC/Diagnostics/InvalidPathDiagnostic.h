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

#if !defined(ARCADIA_MILC_DIAGNOSTICS_INVALIDPATHDIAGNOSTIC_H_INCLUDED)
#define ARCADIA_MILC_DIAGNOSTICS_INVALIDPATHDIAGNOSTIC_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia/MILC/WordType.h"

/// @brief A diagnostic message of the compiler.
/// This diagnostic, if emitted, yields the following error message
/// @code
/// error: string `<string>` is not a valid path
/// @endode
/// where @code{<string>} is a string.
Arcadia_declareObjectType(u8"Arcadia.MILC.Diagnostics.InvalidPathDiagnostic", Arcadia_MILC_Diagnostics_InvalidPathDiagnostic,
                          u8"Arcadia.Languages.Diagnostic");

struct Arcadia_MILC_Diagnostics_InvalidPathDiagnosticDispatch {
  Arcadia_Languages_DiagnosticDispatch _parent;
};

struct Arcadia_MILC_Diagnostics_InvalidPathDiagnostic {
  Arcadia_Languages_Diagnostic _parent;
  /// @brief The string.
  Arcadia_String* string;
};

Arcadia_MILC_Diagnostics_InvalidPathDiagnostic*
Arcadia_MILC_Diagnostics_InvalidPathDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticType type,
    Arcadia_String* string
  );

#endif // ARCADIA_MILC_DIAGNOSTICS_INVALIDPATHDIAGNOSTIC_H_INCLUDED
