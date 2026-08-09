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

#if !defined(ARCADIA_VPL_TREE_PROGRAMDEFNNODE_H_INCLUDED)
#define ARCADIA_VPL_TREE_PROGRAMDEFNNODE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Tree/Node.h"

// <program definition> : <program element definition>*
// <program element definition> : <constant block definition>
//                              / <constant scalar definition>
//                              / <variable scalar definition>
//                              / <procedure definition>
Arcadia_declareObjectType(u8"Arcadia.VPL.Tree.ProgramDefnNode", Arcadia_VPL_Tree_ProgramDefnNode,
                          u8"Arcadia.VPL.Tree.Node");

struct Arcadia_VPL_Tree_ProgramDefnNodeDispatch {
  Arcadia_VPL_Tree_NodeDispatch _parent;
};

struct Arcadia_VPL_Tree_ProgramDefnNode {
  Arcadia_VPL_Tree_Node _parent;
  Arcadia_List* elements;
};

Arcadia_VPL_Tree_ProgramDefnNode*
Arcadia_VPL_Tree_ProgramDefnNode_create
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_VPL_TREE_PROGRAMDEFNNODE_H_INCLUDED
