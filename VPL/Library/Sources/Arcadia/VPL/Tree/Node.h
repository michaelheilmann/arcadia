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

#if !defined(ARCADIA_VISUALS_VPL_TREE_NODE_H_INCLUDED)
#define ARCADIA_VISUALS_VPL_TREE_NODE_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"
#include "Arcadia/Collections/Include.h"

typedef struct Arcadia_VPL_Tree_NameExprNode Arcadia_VPL_Tree_NameExprNode;
typedef struct Arcadia_VPL_Tree_ProcedureDefnNode Arcadia_VPL_Tree_ProcedureDefnNode;

typedef struct Arcadia_VPL_Tree_NodeVisitor Arcadia_VPL_Tree_NodeVisitor;

// The empty tree. Has neither children nor a value.
#define Arcadia_VPL_Tree_NodeFlags_Empty (0)

// See Arcadia.VPL.Tree.BinaryExpr for more information.
#define Arcadia_VPL_Tree_NodeFlags_BinaryExpr (1)

// See Arcadia.VPL.Tree.NameExpr for more information.
#define Arcadia_VPL_Tree_NodeFlags_NameExpr (2)

// See Arcadia.VPL.Tree.NumberExpr for more information.
#define Arcadia_VPL_Tree_NodeFlags_NumberExpr (3)

// See Arcadia.VPL.Tree.CallExpr for more information.
#define Arcadia_VPL_Tree_NodeFlags_CallExpr (4)

// See Arcadia.VPL.Tree.VariableDefinition for more information.
#define Arcadia_VPL_Tree_NodeFlags_VariableDefinition (5)

// See Arcadia.VPL.Tree.ProcedureDefnNode for more information.
#define Arcadia_VPL_Tree_NodeFlags_ProcedureDefn (6)

// See Arcadia.VPL.Tree.ConstantRecordDefn for more information.
#define Arcadia_VPL_Tree_NodeFlags_ConstantRecordDefn (7)

Arcadia_declareObjectType(u8"Arcadia.VPL.Tree.Node", Arcadia_VPL_Tree_Node,
                          u8"Arcadia.Object");

struct Arcadia_VPL_Tree_NodeDispatch {
  Arcadia_ObjectDispatch _parent;
  void (*visit)(Arcadia_Thread* thread, Arcadia_VPL_Tree_Node* self, Arcadia_VPL_Tree_NodeVisitor* visitor);
};

// The base of all abstract syntax tree nodes.
// Abstract syntax tree nodes represent the abstract syntax of the program and contain only elements related to that abstract syntax.
// Stages which need to associate additional data with tree nodes, use maps from Arcadia.VPL.Tree.Node to Arcadia.VPL.TreeNodeInfo objects.
struct Arcadia_VPL_Tree_Node {
  Arcadia_Object _parent;
  Arcadia_Integer32Value flags;
  /// Pointer to the parent node or the null pointer.
  Arcadia_VPL_Tree_Node* parent;
};

// Determines the node type and invokes the corresponding visitor method,
// For example,for the tree node type Arcadia_VPL_Tree_NodeFlags_VariableDefinition, invoke the visitor function Arcadia_VPL_Tree_NodeVisitor_visitVariableDefinition.
void
Arcadia_VPL_TreeNode_visit
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_Node* self,
    Arcadia_VPL_Tree_NodeVisitor* visitor
  );

#endif // ARCADIA_VISUALS_VPL_TREE_NODE_H_INCLUDED
