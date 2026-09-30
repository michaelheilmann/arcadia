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

#if !defined(ARCADIA_MILC_ENTERPHASE_H_INCLUDED)
#define ARCADIA_MILC_ENTERPHASE_H_INCLUDED

#include "Arcadia/MILC/Context.h"
#include "Arcadia/MILC/AST/Include.h"
#include "Arcadia/MILC/Symbols/ModuleSymbol.h"

/// @brief The enter phase.
/// @warning The context object passed to the compilation task object is modified by each invocation of Arcadia.MILC.EnterPhase.execute.
/// @remarks
/// step 1:
/// for each module node
/// - find all module definition nodes and enter the module symbols
/// - associate the module symbol with the module node
/// - ensure that there are no two module definitions nodes / module symbols under a module node / module symbol
/// step 2:
/// for each module node
/// - find all top level (class|enumeration|procedure) nodes and enter the symbols. Equip these symbols with their respective completers.
Arcadia_declareObjectType(u8"Arcadia.MILC.EnterPhase", Arcadia_MILC_EnterPhase,
                          u8"Arcadia.MILC.AST.Visitor");

struct Arcadia_MILC_EnterPhaseDispatch {
  Arcadia_MILC_AST_VisitorDispatch _parent;
};

struct Arcadia_MILC_EnterPhase {
  Arcadia_MILC_AST_Visitor _parent;
  /// @brief The context.
  Arcadia_MILC_Context* context;
  /// @brief The last module symbol discovered for the current module node.
  /// If this is not null and another module symbol for the current module node is discovered, then this is an error.
  Arcadia_MILC_ModuleSymbol* lastSymbol;
  /// @brief A deque to add non-module definition nodes to be processed after their corresponding module definition node.
  /// This spares us the hazzle of recursive descent of the syntax tree.
  Arcadia_Deque* deque;
};

Arcadia_MILC_EnterPhase*
Arcadia_MILC_EnterPhase_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

Arcadia_MILC_EnterPhase*
Arcadia_MILC_EnterPhase_getInstance
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Context* context
  );

void
Arcadia_MILC_EnterPhase_run
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_EnterPhase* self
  );

#endif // ARCADIA_MILC_ENTERPHASE_H_INCLUDED
