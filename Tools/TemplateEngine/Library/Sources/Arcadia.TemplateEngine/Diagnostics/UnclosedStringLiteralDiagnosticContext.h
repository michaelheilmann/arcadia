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

#if !defined(ARCADIA_TOOLS_TEMPLATEENGINE_DIAGNOSTICS_UNCLOSEDSTRINGLITERALDIAGNOSTICCONTEXT_H_INCLUDED)
#define ARCADIA_TOOLS_TEMPLATEENGINE_DIAGNOSTICS_UNCLOSEDSTRINGLITERALDIAGNOSTICCONTEXT_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia.TemplateEngine/Scanner/WordType.h"

/// @brief Enumeration denoting the reason why a string literal is not closed.
Arcadia_declareEnumerationType(u8"Arcadia.TemplateEngine.Diagnostics.UnclosedStringLiteralDiagnosticContext",
                               Arcadia_TemplateEngine_Diagnostics_UnclosedStringLiteralDiagnosticContext);

enum Arcadia_TemplateEngine_Diagnostics_UnclosedStringLiteralDiagnosticContext {
  /// @brief The end of the line was encountered.
  Arcadia_TemplateEngine_Diagnostics_UnclosedStringLiteralDiagnosticContext_EndOfLine,
  /// @brief The end of the input was encountered.
  Arcadia_TemplateEngine_Diagnostics_UnclosedStringLiteralDiagnosticContext_EndOfInput,
};

#endif // ARCADIA_TOOLS_TEMPLATEENGINE_DIAGNOSTICS_UNCLOSEDSTRINGLITERALDIAGNOSTICCONTEXT_H_INCLUDED
