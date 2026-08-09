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

#if !defined(ARCADIA_LANGUAGES_DIAGNOSTICS_H_INCLUDED)
#define ARCADIA_LANGUAGES_DIAGNOSTICS_H_INCLUDED

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"
typedef struct Arcadia_Languages_Diagnostic Arcadia_Languages_Diagnostic;

Arcadia_declareObjectType(u8"Arcadia.Languages.Diagnostics", Arcadia_Languages_Diagnostics,
                          u8"Arcadia.Object");

struct Arcadia_Languages_DiagnosticsDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_Languages_Diagnostics {
  Arcadia_Object _parent;
  /// @brief List of diagnostics (Arcadia.Languages.Diagnostic or derived type objects).
  Arcadia_List* diagnostics;
  /// @brief The log.
  Arcadia_Log* log;
};

Arcadia_Languages_Diagnostics*
Arcadia_Languages_Diagnostics_create
  (
    Arcadia_Thread* thread,
	  Arcadia_Log* log
  );

Arcadia_BooleanValue
Arcadia_Languages_Diagnostics_hasErrors
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self
  );
  
void
Arcadia_Languages_Diagnostics_add
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self,
    Arcadia_Languages_Diagnostic* diagnostic
  );

void
Arcadia_Languages_Diagnostics_emit
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* self
  );

#endif // ARCADIA_LANGUAGES_DIAGNOSTICS_H_INCLUDED
