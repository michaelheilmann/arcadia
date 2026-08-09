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

#if !defined(ARCADIA_TOOLS_TEMPLATEENGINE_DIAGNOSTICS_UNCLOSEDSTRINGLITERALDIAGNOSTIC_H_INCLUDED)
#define ARCADIA_TOOLS_TEMPLATEENGINE_DIAGNOSTICS_UNCLOSEDSTRINGLITERALDIAGNOSTIC_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia.TemplateEngine/Diagnostics/UnclosedStringLiteralDiagnosticContext.h"

/// @brief A diagnostic message of the compiler.
/// This diagnostic, if emitted, yields the following error message
/// @code
/// lexical error: unclosed string literal, <reason>
/// @endode
/// where @code{<reason>} is replaced by a textual description of the reason which is usually @code{encountered end of line} or @code{encountered end of input}
Arcadia_declareObjectType(u8"Arcadia.TemplateEngine.Diagnostics.UnclosedStringLiteralDiagnostic", Arcadia_TemplateEngine_Diagnostics_UnclosedStringLiteralDiagnostic,
                          u8"Arcadia.Languages.Diagnostic");

struct Arcadia_TemplateEngine_Diagnostics_UnclosedStringLiteralDiagnosticDispatch {
  Arcadia_Languages_DiagnosticDispatch _parent;
};

struct Arcadia_TemplateEngine_Diagnostics_UnclosedStringLiteralDiagnostic {
  Arcadia_Languages_Diagnostic _parent;
  /// @brief The received symbol.
  Arcadia_Natural32Value received;
};

Arcadia_TemplateEngine_Diagnostics_UnclosedStringLiteralDiagnostic*
Arcadia_TemplateEngine_Diagnostics_UnclosedStringLiteralDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticType type,
    Arcadia_TemplateEngine_Diagnostics_UnclosedStringLiteralDiagnosticContext context
  );

#endif // ARCADIA_TOOLS_TEMPLATEENGINE_DIAGNOSTICS_UNCLOSEDSTRINGLITERALDIAGNOSTIC_H_INCLUDED
