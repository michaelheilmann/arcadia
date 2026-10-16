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

#if !defined(ARCADIA_VPL_IR_OUTPUTVERIFICATION_H_INCLUDED)
#define ARCADIA_VPL_IR_OUTPUTVERIFICATION_H_INCLUDED

#include "Arcadia/VPL/IR/Verifier.h"

void
_Arcadia_VPL_IR_verifyOutputs
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self
  );

void
_Arcadia_VPL_IR_verifyFunctionFlow
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_VPL_IR_Function* function
  );

void
_Arcadia_VPL_IR_raiseVerificationError
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_IR_Verifier* self,
    Arcadia_Object* subject,
    const char* message
  );

#endif // ARCADIA_VPL_IR_OUTPUTVERIFICATION_H_INCLUDED
