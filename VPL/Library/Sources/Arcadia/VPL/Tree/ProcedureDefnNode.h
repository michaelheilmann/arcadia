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

#if !defined(ARCADIA_VISUALS_VPL_TREE_PROCEDUREDEFNNODE_H_INCLUDED)
#define ARCADIA_VISUALS_VPL_TREE_PROCEDUREDEFNNODE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Tree/Node.h"
typedef struct Arcadia_VPL_Tree_NameExprNode Arcadia_VPL_Tree_NameExprNode;

// <procedure defn> : <modifiers> 'procedure' <name> '(' �<parameters> ')' ':' <return value type> <body>?
// <modifiers> : <modifier>*
// <modifier> : 'builtin' / 'fragment' / 'pixel' / 'vertex'
Arcadia_declareObjectType(u8"Arcadia.VPL.Tree.ProcedureDefnNode", Arcadia_VPL_Tree_ProcedureDefnNode,
                          u8"Arcadia.VPL.Tree.Node");

struct Arcadia_VPL_Tree_ProcedureDefnNodeDispatch {
  Arcadia_VPL_Tree_NodeDispatch _parent;
};

struct Arcadia_VPL_Tree_ProcedureDefnNode {
  Arcadia_VPL_Tree_Node _parent;
  Arcadia_List* modifiers;
  Arcadia_VPL_Tree_NameExprNode* returnValueType;
  Arcadia_String* name;
  Arcadia_List* parameters;
  Arcadia_List* body;
};

Arcadia_VPL_Tree_ProcedureDefnNode*
Arcadia_VPL_Tree_ProcedureDefnNode_create
  (
    Arcadia_Thread* thread,
    Arcadia_List* modifiers,
    Arcadia_VPL_Tree_NameExprNode* returnValueType,
    Arcadia_String* name,
    Arcadia_List* parameters,
    Arcadia_List* body
  );

Arcadia_BooleanValue
Arcadia_VPL_Tree_ProcedureDefnNode_isVertex
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_ProcedureDefnNode* self
  );

Arcadia_BooleanValue
Arcadia_VPL_Tree_ProcedureDefnNode_isFragment
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_ProcedureDefnNode* self
  );

Arcadia_BooleanValue
Arcadia_VPL_Tree_ProcedureDefnNode_isPixel
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_ProcedureDefnNode* self
  );

#endif // ARCADIA_VISUALS_VPL_TREE_PROCEDUREDEFNNODE_H_INCLUDED
