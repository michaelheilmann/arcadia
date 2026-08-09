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

#if !defined(ARCADIA_MILC_SCANNER_H_INCLUDED)
#define ARCADIA_MILC_SCANNER_H_INCLUDED

#include "Arcadia/MILC/WordType.h"
#include "Arcadia/Languages/Include.h"
#include "Arcadia/FileSystem/Include.h"
typedef struct Arcadia_MILC_Context Arcadia_MILC_Context;

/// @brief A scanner for the MIL compiler.
/// @code
/// class Arcadia.MILC.Scanner extends Arcadia.Languages.Scanner {
/// }
/// @endcode
Arcadia_declareObjectType(u8"Arcadia.MILC.Scanner", Arcadia_MILC_Scanner,
                          u8"Arcadia.Languages.Scanner");


/// @brief Get or create the MIL scanner in this context.
/// @param thread A pointer to this thread.
/// @param context A pointer to this context.
/// @return A pointer to the MIL scanner in this context.
Arcadia_MILC_Scanner*
Arcadia_MILC_Scanner_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

/// @remarks A copy of the file path is stored.
void
Arcadia_MILC_Scanner_setFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self,
    Arcadia_Languages_InputFile* file
  );

/// @remarks A copy of the file path is returned.
Arcadia_Languages_InputFile*
Arcadia_MILC_Scanner_getFile
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

Arcadia_SizeValue
Arcadia_MILC_Scanner_getStartOffset
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Scanner* self
  );

#endif // ARCADIA_MILC_SCANNER_H_INCLUDED
