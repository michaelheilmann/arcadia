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

#if !defined(ARCADIA_DDL_READER_SCANNER_H_INCLUDED)
#define ARCADIA_DDL_READER_SCANNER_H_INCLUDED

#include "Arcadia/DDL/Reader/WordType.h"
#include "Arcadia/Languages/Include.h"

/// @brief A scanner for DDL.
/// @code
/// class Arcadia.DDL.Scanner
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.DDL.Scanner", Arcadia_DDL_Scanner,
                          u8"Arcadia.Languages.Scanner");

/// @brief Create a DDL scanner.
/// @param thread A pointer to this thread.
/// @param stringTable A pointer to the string table.
/// @param diagnostics A pointer to the diagnostics.
/// @return A pointer to the DDL scanner.
Arcadia_DDL_Scanner*
Arcadia_DDL_Scanner_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_StringTable* stringTable,
    Arcadia_Languages_Diagnostics* diagnostics
  );

#endif // ARCADIA_DDL_READER_SCANNER_H_INCLUDED
