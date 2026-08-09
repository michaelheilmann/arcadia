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

#if !defined(ARCADIA_TOOLS_TEMPLATEENGINE_DIAGNOSTICS_UNEXPECTEDSYMBOLDIAGNOSTIC_H_INCLUDED)
#define ARCADIA_TOOLS_TEMPLATEENGINE_DIAGNOSTICS_UNEXPECTEDSYMBOLDIAGNOSTIC_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia.TemplateEngine/Scanner/WordType.h"

/// @brief A diagnostic message of the compiler.
/// This diagnostic, if emitted, yields the following error message
/// @code
/// lexical error: received <symbol>
/// @endode
/// where @code{<symbol>} is replaced by a textual description of the received symbol.
Arcadia_declareObjectType(u8"Arcadia.TemplateEngine.Diagnostics.UnexpectedSymbolDiagnostic", Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic,
                          u8"Arcadia.Languages.Diagnostic");

struct Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnosticDispatch {
  Arcadia_Languages_DiagnosticDispatch _parent;
};

struct Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic {
  Arcadia_Languages_Diagnostic _parent;
  /// @brief The received symbol.
  Arcadia_Natural32Value received;
};

Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic*
Arcadia_TemplateEngine_Diagnostics_UnexpectedSymbolDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticType type,
    Arcadia_Natural32Value received
  );

#endif // ARCADIA_TOOLS_TEMPLATEENGINE_DIAGNOSTICS_UNEXPECTEDSYMBOLDIAGNOSTIC_H_INCLUDED
