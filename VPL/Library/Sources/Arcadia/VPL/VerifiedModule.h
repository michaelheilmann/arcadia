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

#if !defined(ARCADIA_VPL_VERIFIEDMODULE_H_INCLUDED)
#define ARCADIA_VPL_VERIFIEDMODULE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Text.h"

Arcadia_declareObjectType(u8"Arcadia.VPL.VerifiedModule", Arcadia_VPL_VerifiedModule, u8"Arcadia.Object");

/// Publish an immutable canonical snapshot, independent of the mutable builder graph.
Arcadia_VPL_VerifiedModule*
Arcadia_VPL_VerifiedModule_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Module* module,
    Arcadia_Languages_Diagnostics* diagnostics
  );

/// Return immutable canonical text; no mutable graph or validity flag is exposed.
Arcadia_String*
Arcadia_VPL_VerifiedModule_getText
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_VerifiedModule* self
  );

/// Materialize a fresh, independently verified graph for a compiler consumer.
Arcadia_VPL_IR_Module*
Arcadia_VPL_VerifiedModule_materialize
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_VerifiedModule* self,
    Arcadia_Languages_Diagnostics* diagnostics
  );

#endif // ARCADIA_VPL_VERIFIEDMODULE_H_INCLUDED
