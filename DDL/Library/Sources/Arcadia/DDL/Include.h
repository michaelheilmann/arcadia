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

#if !defined(ARCADIA_DDL_INCLUDE_H_INCLUDED)
#define ARCADIA_DDL_INCLUDE_H_INCLUDED

// If a file x belongs to a module a and ARCADIA_a_MODULE is not defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_DDL_MODULE")
#define ARCADIA_DDL_MODULE (1)

// If a file x of a module a is not an export file of that module and ARCADIA_a_EXPORT is defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_DDL_EXPORT")
#define ARCADIA_DDL_EXPORT (1)

#include "Arcadia/DDL/Reader/Keywords.h"
#include "Arcadia/DDL/Reader/DefaultReader.h"
#include "Arcadia/DDL/Reader/Parser.h"
#include "Arcadia/DDL/Reader/Scanner.h"
#include "Arcadia/DDL/Reader/WordType.h"
#include "Arcadia/DDL/SemanticalAnalysis.h"
#include "Arcadia/DDL/Writer/Unparser.h"

#undef ARCADIA_DDL_EXPORT
#pragma pop_macro("ARCADIA_DDL_EXPORT")

#undef ARCADIA_DDL_MODULE
#pragma pop_macro("ARCADIA_DDL_MODULE")

#endif // ARCADIA_DDL_INCLUDE_H_INCLUDED
