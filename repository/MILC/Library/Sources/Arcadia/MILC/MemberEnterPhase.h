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

#if !defined(ARCADIA_MILC_MEMBERENTERPHASE_H_INCLUDED)
#define ARCADIA_MILC_MEMBERENTERPHASE_H_INCLUDED

#include "Arcadia/MILC/Context.h"
#include "Arcadia/MILC/AST/Include.h"
#include "Arcadia/MILC/Symbols/ModuleSymbol.h"

/// @brief The member enter phase.
/// @warning The context object passed to the compilation task object is modified by each invocation of Arcadia.MILC.EnterPhase.execute.
/// @remarks
/// - for class symbols enter the constructor, method, and fields symbols
/// - for enumeration symbols enter the enumeration element symbols
/// - for procedure symbols add their parameter variables
Arcadia_declareObjectType(u8"Arcadia.MILC.MemberEnterPhase", Arcadia_MILC_MemberEnterPhase,
                          u8"Arcadia.MILC.AST.Visitor");

struct Arcadia_MILC_MemberEnterPhaseDispatch {
  Arcadia_MILC_AST_VisitorDispatch _parent;
};

struct Arcadia_MILC_MemberEnterPhase {
  Arcadia_MILC_AST_Visitor _parent;
  /// @brief The context.
  Arcadia_MILC_Context* context;
  /// The completer for classes.
  Arcadia_MILC_Completer* classCompleter;
  /// The completer for enumerations.
  Arcadia_MILC_Completer* enumerationCompleter;
  /// The completer for procedures.
  Arcadia_MILC_Completer* procedureCompleter;
};

Arcadia_MILC_MemberEnterPhase*
Arcadia_MILC_MemberEnterPhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

Arcadia_MILC_MemberEnterPhase*
Arcadia_MILC_MemberEnterPhase_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

void
Arcadia_MILC_MemberEnterPhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_MemberEnterPhase* self
  );

#endif // ARCADIA_MILC_MEMBERENTERPHASE_H_INCLUDED
