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

#if !defined(ARCADIA_MILC_DIAGNOSTICS_FILENOTFOUNDDIAGNOSTIC_H_INCLUDED)
#define ARCADIA_MILC_DIAGNOSTICS_FILENOTFOUNDDIAGNOSTIC_H_INCLUDED

#include "Arcadia/Languages/Include.h"
#include "Arcadia/MILC/WordType.h"
#include "Arcadia/MILC/FileType.h"
#include "Arcadia/FileSystem/Include.h"

/// @brief A diagnostic message of the compiler.
/// This diagnostic, if emitted, yields the following error message
/// @code
/// error: <file type> `<path>` not found
/// @endode
/// where @code{<file type>} is the file type and @code{<path>} is the path to the file.
Arcadia_declareObjectType(u8"Arcadia.MILC.Diagnostics.FileNotFoundDiagnostic", Arcadia_MILC_Diagnostics_FileNotFoundDiagnostic,
                          u8"Arcadia.Languages.Diagnostic");

struct Arcadia_MILC_Diagnostics_FileNotFoundDiagnosticDispatch {
  Arcadia_Languages_DiagnosticDispatch _parent;
};

struct Arcadia_MILC_Diagnostics_FileNotFoundDiagnostic {
  Arcadia_Languages_Diagnostic _parent;
  /// @brief The type of the file.
  Arcadia_MILC_FileType fileType;
};

Arcadia_MILC_Diagnostics_FileNotFoundDiagnostic*
Arcadia_MILC_Diagnostics_FileNotFoundDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_DiagnosticType diagnosticType,
    Arcadia_Languages_InputFile* file,
    Arcadia_MILC_FileType fileType
  );

#endif // ARCADIA_MILC_DIAGNOSTICS_FILENOTFOUNDDIAGNOSTIC_H_INCLUDED
