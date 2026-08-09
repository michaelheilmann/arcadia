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

#if !defined(ARCADIA_VISUALS_VPL_TREE_NODEFACTORY_H_INCLUDED)
#define ARCADIA_VISUALS_VPL_TREE_NODEFACTORY_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Tree/Node.h"
#include "Arcadia/VPL/Tree/ProcedureDefnNode.h"

Arcadia_VPL_Tree_Node*
Arcadia_VPL_Tree_NodeFactory_makeCallExpr
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* target
  );

Arcadia_VPL_Tree_Node*
Arcadia_VPL_Tree_NodeFactory_makeNumberExpr
  (
    Arcadia_Thread* thread,
    Arcadia_String* literal
  );

Arcadia_VPL_Tree_Node*
Arcadia_VPL_Tree_NodeFactory_makeAccessExpr
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* lhs,
    Arcadia_VPL_Tree_Node* rhs
  );

Arcadia_VPL_Tree_Node*
Arcadia_VPL_Tree_NodeFactory_makeAddExpr
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* lhs,
    Arcadia_VPL_Tree_Node* rhs
  );

Arcadia_VPL_Tree_Node*
Arcadia_VPL_Tree_NodeFactory_makeSubtractExpr
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* lhs,
    Arcadia_VPL_Tree_Node* rhs
  );

Arcadia_VPL_Tree_Node*
Arcadia_VPL_Tree_NodeFactory_makeMultiplyExpr
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* lhs,
    Arcadia_VPL_Tree_Node* rhs
  );

Arcadia_VPL_Tree_Node*
Arcadia_VPL_Tree_NodeFactory_makeDivideExpr
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* lhs,
    Arcadia_VPL_Tree_Node* rhs
  );

Arcadia_VPL_Tree_Node*
Arcadia_VPL_Tree_NodeFactory_makeNameExpr
  (
    Arcadia_Thread* thread,
    Arcadia_String* name
  );

Arcadia_VPL_Tree_Node*
Arcadia_VPL_Tree_NodeFactory_makeAssignmentExpr
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* lhs,
    Arcadia_VPL_Tree_Node* rhs
  );

Arcadia_VPL_Tree_Node*
Arcadia_VPL_Tree_NodeFactory_makeVariableDefn
  (
    Arcadia_Thread* thread,
    Arcadia_String* name,
    Arcadia_String* type
  );

Arcadia_VPL_Tree_ProcedureDefnNode*
Arcadia_VPL_Tree_NodeFactory_makeProcedureDefn
  (
    Arcadia_Thread* thread,
    Arcadia_List* modifiers,
    Arcadia_VPL_Tree_NameExprNode* returnValueType,
    Arcadia_String* name,
    Arcadia_List* parameters,
    Arcadia_List* body
  );


#endif // ARCADIA_VISUALS_VPL_TREE_NODEFACTORY_H_INCLUDED
