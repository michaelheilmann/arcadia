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

#if !defined(ARCADIA_PDLC_SCANNER_H_INCLUDED)
#define ARCADIA_PDLC_SCANNER_H_INCLUDED

#include "Arcadia/PDLC/WordType.h"
#include "Arcadia/Languages/Include.h"
#include "Arcadia/FileSystem/Include.h"
typedef struct Arcadia_PDLC_Context Arcadia_PDLC_Context;

/// @brief A scanner for the MIL compiler.
/// @code
/// class Arcadia.PDLC.Scanner extends Arcadia.Languages.Scanner {
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.PDLC.Scanner", Arcadia_PDLC_Scanner,
                          u8"Arcadia.Languages.Scanner");


/// @brief Get or create the MIL scanner in this context.
/// @param thread A pointer to this thread.
/// @param context A pointer to this context.
/// @return A pointer to the MIL scanner in this context.
Arcadia_PDLC_Scanner*
Arcadia_PDLC_Scanner_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  );

/// @remarks A copy of the file path is stored.
void
Arcadia_PDLC_Scanner_setFile
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Scanner* self,
    Arcadia_Languages_InputFile* file
  );

/// @remarks A copy of the file path is returned.
Arcadia_Languages_InputFile*
Arcadia_PDLC_Scanner_getFile
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Scanner* self
  );

Arcadia_SizeValue
Arcadia_PDLC_Scanner_getStartOffset
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Scanner* self
  );

#endif // ARCADIA_PDLC_SCANNER_H_INCLUDED
