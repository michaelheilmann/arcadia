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

#if !defined(ARCADIA_VPL_TREE_TREEVISITOR_H_INCLUDED)
#define ARCADIA_VPL_TREE_TREEVISITOR_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/VPL/Tree/Node.h"
#include "Arcadia/VPL/Tree/BinaryExprNode.h"
#include "Arcadia/VPL/Tree/ConstantRecordDefnNode.h"
#include "Arcadia/VPL/Tree/CallExprNode.h"
#include "Arcadia/VPL/Tree/ProcedureDefnNode.h"
#include "Arcadia/VPL/Tree/ProgramDefnNode.h"
#include "Arcadia/VPL/Tree/VariableDefnNode.h"

Arcadia_declareObjectType(u8"Arcadia.VPL.Tree.TreeVisitor", Arcadia_VPL_Tree_NodeVisitor,
                          u8"Arcadia.Object");

struct Arcadia_VPL_Tree_NodeVisitorDispatch {
  Arcadia_ObjectDispatch _parent;
  Arcadia_Value (*onBinaryExprNode)(Arcadia_Thread* thread, Arcadia_VPL_Tree_NodeVisitor* self, Arcadia_Value context, Arcadia_VPL_Tree_BinaryExprNode* tree);
  Arcadia_Value (*onCallExprNode)(Arcadia_Thread* thread, Arcadia_VPL_Tree_NodeVisitor* self, Arcadia_Value context, Arcadia_VPL_Tree_CallExprNode* tree);
  Arcadia_Value (*onConstantRecordDefnNode)(Arcadia_Thread* thread, Arcadia_VPL_Tree_NodeVisitor* self, Arcadia_Value context, Arcadia_VPL_Tree_ConstantRecordDefnNode* tree);
  Arcadia_Value (*onProcedureDefnNode)(Arcadia_Thread* thread, Arcadia_VPL_Tree_NodeVisitor* self, Arcadia_Value context, Arcadia_VPL_Tree_ProcedureDefnNode* tree);
  Arcadia_Value (*onProgramDefnNode)(Arcadia_Thread* thread, Arcadia_VPL_Tree_NodeVisitor* self, Arcadia_Value context, Arcadia_VPL_Tree_ProgramDefnNode* tree);
  Arcadia_Value (*onVariableDefnNode)(Arcadia_Thread* thread, Arcadia_VPL_Tree_NodeVisitor* self, Arcadia_Value context, Arcadia_VPL_Tree_VariableDefnNode* tree);
};

struct Arcadia_VPL_Tree_NodeVisitor {
  Arcadia_Object _parent;
};

Arcadia_Value
Arcadia_VPL_Tree_NodeVisitor_onBinaryExprNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_NodeVisitor* self,
    Arcadia_Value context,
    Arcadia_VPL_Tree_BinaryExprNode* tree
  );

Arcadia_Value
Arcadia_VPL_Tree_NodeVisitor_onCallExprNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_NodeVisitor* self,
    Arcadia_Value context,
    Arcadia_VPL_Tree_CallExprNode* tree
  );

Arcadia_Value
Arcadia_VPL_Tree_NodeVisitor_onProcedureDefnNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_NodeVisitor* self,
    Arcadia_Value context,
    Arcadia_VPL_Tree_ProcedureDefnNode* tree
  );

Arcadia_Value
Arcadia_VPL_Tree_NodeVisitor_onProgramDefnNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_NodeVisitor* self,
    Arcadia_Value context,
    Arcadia_VPL_Tree_ProgramDefnNode* tree
  );

Arcadia_Value
Arcadia_VPL_Tree_NodeVisitor_onVariableDefnNode
  (
    Arcadia_Thread* thread,
    Arcadia_VPL_Tree_NodeVisitor* self,
    Arcadia_Value context,
    Arcadia_VPL_Tree_VariableDefnNode* tree
  );

#endif // ARCADIA_VPL_TREE_TREEVISITOR_H_INCLUDED
