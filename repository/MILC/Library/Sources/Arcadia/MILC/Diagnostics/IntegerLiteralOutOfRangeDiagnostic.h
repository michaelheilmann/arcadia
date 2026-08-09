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

#if !defined(ARCADIA_MILC_DIAGNOSTICS_INTEGERLITERALOUTOFRANGEDIAGNOSTIC_H_INCLUDED)
#define ARCADIA_MILC_DIAGNOSTICS_INTEGERLITERALOUTOFRANGEDIAGNOSTIC_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia/FileSystem/Include.h"
#include "Arcadia/MILC/Symbols/EnumerationConstantSymbol.h"

/// @brief A diagnostic message of the compiler.
/// This diagnostic, if emitted, yields the following error message
/// @code
/// error: value `<value>` of initializer of enumeration constant `<name>` is out of range
/// @endcode
/// where @code{<value>} is the value of the initializer of an enumeration constant
/// and @code{<name>} is the fully qualified name of the enumeration constant.
Arcadia_declareObjectType(u8"Arcadia.MILC.Diagnostics.IntegerLiteralOutOfRangeDiagnostic", Arcadia_MILC_Diagnostics_IntegerLiteralOutOfRangeDiagnostic,
                          u8"Arcadia.Languages.Diagnostic");

struct Arcadia_MILC_Diagnostics_IntegerLiteralOutOfRangeDiagnosticDispatch {
  Arcadia_Languages_DiagnosticDispatch _parent;
};

struct Arcadia_MILC_Diagnostics_IntegerLiteralOutOfRangeDiagnostic {
  Arcadia_Languages_Diagnostic _parent;
  /// @brief The symbol of the enumeration constant.
  Arcadia_MILC_EnumerationConstantSymbol* symbol;
  /// @brief The textual value of the initializer of the enumeration constant.
  Arcadia_String* value;
};

Arcadia_MILC_Diagnostics_IntegerLiteralOutOfRangeDiagnostic*
Arcadia_MILC_Diagnostics_IntegerLiteralOutOfRangeDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticType type,
    Arcadia_Languages_InputFile* file,
    Arcadia_SizeValue offset,
    Arcadia_MILC_EnumerationConstantSymbol* symbol,
    Arcadia_String* value
  );

#endif // ARCADIA_MILC_DIAGNOSTICS_INTEGERLITERALOUTOFRANGEDIAGNOSTIC_H_INCLUDED
