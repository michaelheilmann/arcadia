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

#if !defined(ARCADIA_PDLC_MEMBERENTERPHASE_H_INCLUDED)
#define ARCADIA_PDLC_MEMBERENTERPHASE_H_INCLUDED

#include "Arcadia/PDLC/Context.h"
#include "Arcadia/PDLC/AST/Include.h"
#include "Arcadia/PDLC/Symbols/ModuleSymbol.h"

/// @brief The member enter phase.
/// @warning The context object passed to the compilation task object is modified by each invocation of Arcadia.PDLC.EnterPhase.execute.
/// @remarks
/// - for class symbols enter the constructor, method, and fields symbols
/// - for enumeration symbols enter the enumeration element symbols
/// - for procedure symbols add their parameter variables
Arcadia_declareObjectType(u8"Arcadia.PDLC.MemberEnterPhase", Arcadia_PDLC_MemberEnterPhase,
                          u8"Arcadia.PDLC.AST.Visitor");

struct Arcadia_PDLC_MemberEnterPhaseDispatch {
  Arcadia_PDLC_AST_VisitorDispatch _parent;
};

struct Arcadia_PDLC_MemberEnterPhase {
  Arcadia_PDLC_AST_Visitor _parent;
  /// @brief The context.
  Arcadia_PDLC_Context* context;
  /// The completer for classes.
  Arcadia_PDLC_Completer* classCompleter;
  /// The completer for enumerations.
  Arcadia_PDLC_Completer* enumerationCompleter;
  /// The completer for interface types.
  Arcadia_PDLC_Completer* interfaceCompleter;
  /// The completer for procedures.
  Arcadia_PDLC_Completer* procedureCompleter;
};

Arcadia_PDLC_MemberEnterPhase*
Arcadia_PDLC_MemberEnterPhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  );

Arcadia_PDLC_MemberEnterPhase*
Arcadia_PDLC_MemberEnterPhase_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_Context* context
  );

void
Arcadia_PDLC_MemberEnterPhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_PDLC_MemberEnterPhase* self
  );

#endif // ARCADIA_PDLC_MEMBERENTERPHASE_H_INCLUDED
