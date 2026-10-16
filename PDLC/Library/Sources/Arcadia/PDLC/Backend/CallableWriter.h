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

#if !defined(ARCADIA_PDLC_BACKEND_CALLABLEWRITER_H_INCLUDED)
#define ARCADIA_PDLC_BACKEND_CALLABLEWRITER_H_INCLUDED

#include "Arcadia/PDLC/Include.h"

void
Arcadia_PDLC_Backend_writeExecutable
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_Symbol* symbol,
    Arcadia_BooleanValue header,
    Arcadia_StringBuilder* output
  );

void
Arcadia_PDLC_Backend_validateExecutables
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  );

/// Verification-only C declarations with a backend-selected _PDLModel type suffix.
/// Native headers remain authoritative and are never replaced by this operation.
void
Arcadia_PDLC_Backend_writeNativeClassModel
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_ClassSymbol* symbol,
    Arcadia_StringBuilder* output
  );

void
Arcadia_PDLC_Backend_writeVariableType
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context,
    Arcadia_PDLC_VariableSymbol* variable,
    Arcadia_StringBuilder* output
  );

#endif // ARCADIA_PDLC_BACKEND_CALLABLEWRITER_H_INCLUDED
