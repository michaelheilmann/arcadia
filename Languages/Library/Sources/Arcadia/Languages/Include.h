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

#if !defined(ARCADIA_LANGUAGES_INCLUDE_H_INCLUDED)
#define ARCADIA_LANGUAGES_INCLUDE_H_INCLUDED

// If a file x belongs to a module a and ARCADIA_a_MODULE is not defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_LANGUAGES_MODULE")
#define ARCADIA_LANGUAGES_MODULE (1)

// If a file x of a module a is not an export file of that module and ARCADIA_a_EXPORT is defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_LANGUAGES_EXPORT")
#define ARCADIA_LANGUAGES_EXPORT (1)

#include "Arcadia/Languages/Diagnostic.h"
#include "Arcadia/Languages/Diagnostics.h"
#include "Arcadia/Languages/DiagnosticsOld.h"
#include "Arcadia/Languages/DiagnosticType.h"
#include "Arcadia/Languages/InputFile.h"
#include "Arcadia/Languages/InputFileManager.h"
#include "Arcadia/Languages/LineMap.h"
#include "Arcadia/Languages/mangleName.h"
#include "Arcadia/Languages/Scanner.h"
#include "Arcadia/Languages/Parser.h"
#include "Arcadia/Languages/Scope.h"
#include "Arcadia/Languages/StringTable.h"

#undef ARCADIA_LANGUAGES_EXPORT
#pragma pop_macro("ARCADIA_LANGUAGES_EXPORT")

#undef ARCADIA_LANGUAGES_MODULE
#pragma pop_macro("ARCADIA_LANGUAGES_MODULE")

#endif // ARCADIA_LANGUAGES_INCLUDE_H_INCLUDED
