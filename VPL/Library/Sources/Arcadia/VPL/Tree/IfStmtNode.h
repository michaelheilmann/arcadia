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

#if !defined(ARCADIA_VPL_TREE_IFSTMTNODE_H_INCLUDED)
#define ARCADIA_VPL_TREE_IFSTMTNODE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Tree/Node.h"

Arcadia_declareObjectType(u8"Arcadia.VPL.Tree.IfStmtNode", Arcadia_VPL_Tree_IfStmtNode,
                          u8"Arcadia.VPL.Tree.Node");

struct Arcadia_VPL_Tree_IfStmtNodeDispatch {
  Arcadia_VPL_Tree_NodeDispatch _parent;
};

struct Arcadia_VPL_Tree_IfStmtNode {
  Arcadia_VPL_Tree_Node _parent;
  Arcadia_VPL_Tree_Node* condition;
  /// A list of nodes executed when the condition is true.
  Arcadia_List* thenBody;
  /// A list of nodes, or NULL if no else clause is present.
  Arcadia_List* elseBody;
};

Arcadia_VPL_Tree_IfStmtNode*
Arcadia_VPL_Tree_IfStmtNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* condition,
    Arcadia_List* thenBody,
    Arcadia_List* elseBody
  );

#endif // ARCADIA_VPL_TREE_IFSTMTNODE_H_INCLUDED
