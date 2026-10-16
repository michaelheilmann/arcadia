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

#if !defined(ARCADIA_VPL_IR_VERIFIER_H_INCLUDED)
#define ARCADIA_VPL_IR_VERIFIER_H_INCLUDED
#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Languages/Include.h"
#include "Arcadia/VPL/IR/Module.h"

Arcadia_declareObjectType(u8"Arcadia.VPL.IR.Verifier", Arcadia_VPL_IR_Verifier, u8"Arcadia.Object");

struct Arcadia_VPL_IR_VerifierDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_VPL_IR_Verifier {
  Arcadia_Object _parent;
  Arcadia_Languages_Diagnostics* diagnostics;
  Arcadia_VPL_IR_Module* module;
  Arcadia_VPL_IR_Stage* stage;
  Arcadia_VPL_IR_Function* function;
  Arcadia_List* registers;
  Arcadia_List* types;
  Arcadia_List* activeTypes;
  Arcadia_List* activeReferences;
  Arcadia_List* uniformTypes;
  Arcadia_List* blocks;
  Arcadia_Map* normalReturns;
};

Arcadia_VPL_IR_Verifier*
Arcadia_VPL_IR_Verifier_create
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics
  );

/// Verify the current graph; mutation afterwards requires re-verification.
/// Raises SemanticalError with a VerificationDiagnostic on the first invalid object.
void
Arcadia_VPL_IR_Verifier_run
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Module* module
  );

#endif // ARCADIA_VPL_IR_VERIFIER_H_INCLUDED
