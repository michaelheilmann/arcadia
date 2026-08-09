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

#if !defined(ARCADIA_VISUALS_VPL_TREE_INCLUDE_H_INCLUDED)
#define ARCADIA_VISUALS_VPL_TREE_INCLUDE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif

#include "Arcadia/VPL/Tree/BinaryExprNode.h"
#include "Arcadia/VPL/Tree/CallExprNode.h"
#include "Arcadia/VPL/Tree/ConstantRecordDefnNode.h"
#include "Arcadia/VPL/Tree/NameExprNode.h"
#include "Arcadia/VPL/Tree/NumberExprNode.h"
#include "Arcadia/VPL/Tree/Node.h"
#include "Arcadia/VPL/Tree/NodeFactory.h"
#include "Arcadia/VPL/Tree/ProcedureDefnNode.h"
#include "Arcadia/VPL/Tree/ProgramDefnNode.h"
#include "Arcadia/VPL/Tree/VariableDefnNode.h"

#endif // ARCADIA_VISUALS_VPL_TREE_INCLUDE_H_INCLUDED
