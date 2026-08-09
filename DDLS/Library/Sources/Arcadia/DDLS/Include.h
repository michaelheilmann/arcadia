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

#if !defined(ARCADIA_DDLS_INCLUDE_H_INCLUDED)
#define ARCADIA_DDLS_INCLUDE_H_INCLUDED

// If a file x belongs to a module a and ARCADIA_a_MODULE is not defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_DDLS_MODULE")
#define ARCADIA_DDLS_MODULE (1)

// If a file x of a module a is not an export file of that module and ARCADIA_a_EXPORT is defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_DDLS_EXPORT")
#define ARCADIA_DDLS_EXPORT (1)

#include "Arcadia/DDLS/Syntactical/DefaultReader.h"
#include "Arcadia/DDLS/Symbols/SymbolReader.h"

#include "Arcadia/DDLS/Nodes/Include.h"

#include "Arcadia/DDLS/Implementation/ValidationContext.h"

#undef ARCADIA_DDLS_EXPORT
#pragma pop_macro("ARCADIA_DDLS_EXPORT")

#undef ARCADIA_DDLS_MODULE
#pragma pop_macro("ARCADIA_DDLS_MODULE")

#endif // ARCADIA_DDLS_INCLUDE_H_INCLUDED
