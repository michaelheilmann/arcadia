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

#if !defined(ARCADIA_ADL_INCLUDE_H_INCLUDED)
#define ARCADIA_ADL_INCLUDE_H_INCLUDED

// If a file x belongs to a module a and ARCADIA_a_MODULE is not defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_ADL_MODULE")
#define ARCADIA_ADL_MODULE (1)

// If a file x of a module a is not an export file of that module and ARCADIA_a_EXPORT is defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_ADL_EXPORT")
#define ARCADIA_ADL_EXPORT (1)

#include "Arcadia/ADL/Context.h"
#include "Arcadia/ADL/Definition.h"
#include "Arcadia/ADL/Definitions.h"
#include "Arcadia/ADL/Diagnostics.h"
#include "Arcadia/ADL/Reader.h"

#include "Arcadia/ADL/Definitions/DSP/Include.h"
#include "Arcadia/ADL/Definitions/Audials/Include.h"
#include "Arcadia/ADL/Definitions/Visuals/Include.h"

#undef ARCADIA_ADL_EXPORT
#pragma pop_macro("ARCADIA_ADL_EXPORT")

#undef ARCADIA_ADL_MODULE
#pragma pop_macro("ARCADIA_ADL_MODULE")

#endif // ARCADIA_ADL_INCLUDE_H_INCLUDED
