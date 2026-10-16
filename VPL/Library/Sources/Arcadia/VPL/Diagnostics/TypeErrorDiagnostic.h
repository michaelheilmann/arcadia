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

#if !defined(ARCADIA_VPL_DIAGNOSTICS_TYPEERRORDIAGNOSTIC_H_INCLUDED)
#define ARCADIA_VPL_DIAGNOSTICS_TYPEERRORDIAGNOSTIC_H_INCLUDED

#include "Arcadia/Languages/Include.h"

Arcadia_declareObjectType(u8"Arcadia.VPL.Diagnostics.TypeErrorDiagnostic", Arcadia_VPL_Diagnostics_TypeErrorDiagnostic,
                          u8"Arcadia.Languages.Diagnostic");

struct Arcadia_VPL_Diagnostics_TypeErrorDiagnosticDispatch {
  Arcadia_Languages_DiagnosticDispatch _parent;
};

struct Arcadia_VPL_Diagnostics_TypeErrorDiagnostic {
  Arcadia_Languages_Diagnostic _parent;
  Arcadia_String* message;
};

Arcadia_VPL_Diagnostics_TypeErrorDiagnostic*
Arcadia_VPL_Diagnostics_TypeErrorDiagnostic_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* message
  );

#endif // ARCADIA_VPL_DIAGNOSTICS_TYPEERRORDIAGNOSTIC_H_INCLUDED
