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

#if !defined(ARCADIA_MILC_ENVIRONMENT_H_INCLUDED)
#define ARCADIA_MILC_ENVIRONMENT_H_INCLUDED

#include "Arcadia/MILC/Symbols/Symbol.h"
#include "Arcadia/MILC/AST/CompilationUnitNode.h"

/// @brief An environment storing information on a symbol.
Arcadia_declareObjectType(u8"Arcadia.MILC.Environment", Arcadia_MILC_Environment,
                          u8"Arcadia.Object");

struct Arcadia_MILC_EnvironmentDispatch {
  Arcadia_ObjectDispatch _parent;
};

struct Arcadia_MILC_Environment {
  Arcadia_Object _parent;
  /// @brief The symbol this environment is associated with.
  Arcadia_MILC_Symbol* symbol;
  /// @brief The compilation unit the symbol is defined in.
  Arcadia_MILC_AST_CompilationUnitNode* compilationUnitNode;
};

Arcadia_MILC_Environment*
Arcadia_MILC_Environment_create
  (
    Arcadia_Thread* thread,
    Arcadia_MILC_Symbol* symbol,
    Arcadia_MILC_AST_CompilationUnitNode* compilationUnitNode
  );

#endif // ARCADIA_MILC_ENVIRONMENT_H_INCLUDED
