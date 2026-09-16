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

#if !defined(ARCADIA_MILC_DIAGNOSTICS_SYMBOLISNOCLASSDIAGNOSTIC_H_INCLUDED)
#define ARCADIA_MILC_DIAGNOSTICS_SYMBOLISNOCLASSDIAGNOSTIC_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia/FileSystem/Include.h"

/// @brief A diagnostic message of the compiler.
/// This diagnostic, if emitted, yields the following error message
/// @code
/// semantical error: the symbol `<name>` is no class
/// @endode
/// where @code{<class>} is replaced by the name of the symbol.
Arcadia_declareObjectType(u8"Arcadia.MILC.Diagnostics.SymbolIsNoClassDiagnostic", Arcadia_MILC_Diagnostics_SymbolIsNoClassDiagnostic,
                          u8"Arcadia.Languages.Diagnostic");

struct Arcadia_MILC_Diagnostics_SymbolIsNoClassDiagnosticDispatch {
  Arcadia_Languages_DiagnosticDispatch _parent;
};

struct Arcadia_MILC_Diagnostics_SymbolIsNoClassDiagnostic {
  Arcadia_Languages_Diagnostic _parent;
  /// @brief The name of the symbol.
  Arcadia_String* name;
};

Arcadia_MILC_Diagnostics_SymbolIsNoClassDiagnostic*
Arcadia_MILC_Diagnostics_SymbolIsNoClassDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticType type,
    Arcadia_Languages_InputFile* file,
    Arcadia_SizeValue offset,
    Arcadia_String* name
  );

#endif // ARCADIA_MILC_DIAGNOSTICS_SYMBOLISNOCLASSDIAGNOSTIC_H_INCLUDED
