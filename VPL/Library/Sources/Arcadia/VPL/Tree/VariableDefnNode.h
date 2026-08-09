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

#if !defined(ARCADIA_VISUALS_VPL_TREE_VARIABLEDEFNNODE_H_INCLUDED)
#define ARCADIA_VISUALS_VPL_TREE_VARIABLEDEFNNODE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Tree/Node.h"
#include "Arcadia/VPL/Tree/NameExprNode.h"


// <variable definition> : <name> ':' <type name> <modifiers>
// <modifier>            : <modifier>*
// <modifier>            : 'constant'
//                       / 'variable'
Arcadia_declareObjectType(u8"Arcadia.VPL.Tree.VariableDefnNode", Arcadia_VPL_Tree_VariableDefnNode,
                          u8"Arcadia.VPL.Tree.Node");

struct Arcadia_VPL_Tree_VariableDefnNodeDispatch {
  Arcadia_VPL_Tree_NodeDispatch _parent;
};

struct Arcadia_VPL_Tree_VariableDefnNode {
  Arcadia_VPL_Tree_Node _parent;
  // The name of the variable.
  Arcadia_String* name;
  // List of modifiers of the variable.
  Arcadia_List* modifiers;
  // The type of the variable.
  Arcadia_String* type;
  // The symbol of the type.
  Arcadia_VPL_Symbols_Symbol* typeSymbol;
};

Arcadia_VPL_Tree_VariableDefnNode*
Arcadia_VPL_Tree_VariableDefnNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_String* type
  );

Arcadia_BooleanValue
Arcadia_VPL_Tree_VariableDefnNode_isVertex
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  );

Arcadia_BooleanValue
Arcadia_VPL_Tree_VariableDefnNode_isFragment
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  );

Arcadia_BooleanValue
Arcadia_VPL_Tree_VariableDefnNode_isPixel
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  );

Arcadia_BooleanValue
Arcadia_VPL_Tree_VariableDefnNode_isConstant
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  );

Arcadia_BooleanValue
Arcadia_VPL_Tree_VariableDefnNode_isVariable
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_VariableDefnNode* self
  );

#endif // ARCADIA_VISUALS_VPL_TREE_VARIABLEDEFNNODE_H_INCLUDED
