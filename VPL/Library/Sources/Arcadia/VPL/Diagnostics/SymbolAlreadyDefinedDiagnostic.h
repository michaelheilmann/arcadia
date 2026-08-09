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

#if !defined(ARCADIA_VPL_DIAGNOSTICS_SYMBOLALREADYDEFINEDDIAGNOSTIC_H_INCLUDED)
#define ARCADIA_VPL_DIAGNOSTICS_SYMBOLALREADYDEFINEDDIAGNOSTIC_H_INCLUDED

#include "Arcadia/Languages/Include.h"

/// @brief A diagnostic message of the compiler.
/// This diagnostic, if emitted, yields the following error message
/// @code
/// semantical error: the symbol `<symbol>` is already defined
/// @endode
/// where @code{<symbol>} is replaced by the name of the symbol.
Arcadia_declareObjectType(u8"Arcadia.VPL.Diagnostics.SymbolAlreadyDefinedDiagnostic", Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic,
                          u8"Arcadia.Languages.Diagnostic");

struct Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnosticDispatch {
  Arcadia_Languages_DiagnosticDispatch _parent;
};

struct Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic {
  Arcadia_Languages_Diagnostic _parent;
  /// @brief The name of the symbol.
  Arcadia_String* name;
};

Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic*
Arcadia_VPL_Diagnostics_SymbolAlreadyDefinedDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticType type,
    Arcadia_String* name
  );

#endif // ARCADIA_VPL_DIAGNOSTICS_SYMBOLALREADYDEFINEDDIAGNOSTIC_H_INCLUDED
