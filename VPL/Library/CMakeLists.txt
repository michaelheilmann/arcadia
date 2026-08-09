# Arcadia
# Copyright (C) 2024-2026 Michael Heilmann
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU Affero General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
# details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.29)

set(this ${MyProjectName}.VPL)

if (NOT TARGET ${this})

  BeginProduct(${this} library)

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/VPL/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/VPL/Configure.h.i)

  OnSourceFile(${this} Arcadia/VPL/Symbols/BuiltinType.c)
  OnHeaderFile(${this} Arcadia/VPL/Symbols/BuiltinType.h)
  OnSourceFile(${this} Arcadia/VPL/Symbols/Constant.c)
  OnHeaderFile(${this} Arcadia/VPL/Symbols/Constant.h)
  OnSourceFile(${this} Arcadia/VPL/Symbols/ConstantKind.c)
  OnHeaderFile(${this} Arcadia/VPL/Symbols/ConstantKind.h)
  OnSourceFile(${this} Arcadia/VPL/Symbols/ConstantRecord.c)
  OnHeaderFile(${this} Arcadia/VPL/Symbols/ConstantRecord.h)
  OnSourceFile(${this} Arcadia/VPL/Symbols/ConstantScalar.c)
  OnHeaderFile(${this} Arcadia/VPL/Symbols/ConstantScalar.h)
  OnSourceFile(${this} Arcadia/VPL/Symbols/Include.c)
  OnHeaderFile(${this} Arcadia/VPL/Symbols/Include.h)
  OnSourceFile(${this} Arcadia/VPL/Symbols/Procedure.c)
  OnHeaderFile(${this} Arcadia/VPL/Symbols/Procedure.h)
  OnSourceFile(${this} Arcadia/VPL/Symbols/Program.c)
  OnHeaderFile(${this} Arcadia/VPL/Symbols/Program.h)
  OnSourceFile(${this} Arcadia/VPL/Symbols/Stage.c)
  OnHeaderFile(${this} Arcadia/VPL/Symbols/Stage.h)
  OnSourceFile(${this} Arcadia/VPL/Symbols/Symbol.c)
  OnHeaderFile(${this} Arcadia/VPL/Symbols/Symbol.h)
  OnSourceFile(${this} Arcadia/VPL/Symbols/VariableScalar.c)
  OnHeaderFile(${this} Arcadia/VPL/Symbols/VariableScalar.h)
  OnSourceFile(${this} Arcadia/VPL/Symbols/Variable.c)
  OnHeaderFile(${this} Arcadia/VPL/Symbols/Variable.h)

  OnSourceFile(${this} Arcadia/VPL/EnterPhase.c)
  OnHeaderFile(${this} Arcadia/VPL/EnterPhase.h)
  OnSourceFile(${this} Arcadia/VPL/ModifiersUtilities.c)
  OnHeaderFile(${this} Arcadia/VPL/ModifiersUtilities.h)
  OnSourceFile(${this} Arcadia/VPL/ResolvePhase.c)
  OnHeaderFile(${this} Arcadia/VPL/ResolvePhase.h)

  OnSourceFile(${this} Arcadia/VPL/Tree/BinaryExprNode.c)
  OnHeaderFile(${this} Arcadia/VPL/Tree/BinaryExprNode.h)
  OnSourceFile(${this} Arcadia/VPL/Tree/CallExprNode.c)
  OnHeaderFile(${this} Arcadia/VPL/Tree/CallExprNode.h)
  OnSourceFile(${this} Arcadia/VPL/Tree/ConstantRecordDefnNode.c)
  OnHeaderFile(${this} Arcadia/VPL/Tree/ConstantRecordDefnNode.h)
  OnSourceFile(${this} Arcadia/VPL/Tree/ProcedureDefnNode.c)
  OnHeaderFile(${this} Arcadia/VPL/Tree/ProcedureDefnNode.h)
  OnSourceFile(${this} Arcadia/VPL/Tree/NameExprNode.c)
  OnHeaderFile(${this} Arcadia/VPL/Tree/NameExprNode.h)
  OnSourceFile(${this} Arcadia/VPL/Tree/Include.c)
  OnHeaderFile(${this} Arcadia/VPL/Tree/Include.h)
  OnSourceFile(${this} Arcadia/VPL/Tree/Node.c)
  OnHeaderFile(${this} Arcadia/VPL/Tree/Node.h)
  OnSourceFile(${this} Arcadia/VPL/Tree/NodeFactory.c)
  OnHeaderFile(${this} Arcadia/VPL/Tree/NodeFactory.h)
  OnSourceFile(${this} Arcadia/VPL/Tree/NodeVisitor.c)
  OnHeaderFile(${this} Arcadia/VPL/Tree/NodeVisitor.h)
  OnSourceFile(${this} Arcadia/VPL/Tree/NumberExprNode.c)
  OnHeaderFile(${this} Arcadia/VPL/Tree/NumberExprNode.h)
  OnSourceFile(${this} Arcadia/VPL/Tree/ProgramDefnNode.c)
  OnHeaderFile(${this} Arcadia/VPL/Tree/ProgramDefnNode.h)
  OnSourceFile(${this} Arcadia/VPL/Tree/VariableDefnNode.c)
  OnHeaderFile(${this} Arcadia/VPL/Tree/VariableDefnNode.h)
  OnSourceFile(${this} Arcadia/VPL/Include.c)
  OnHeaderFile(${this} Arcadia/VPL/Include.h)
  OnHeaderFile(${this} Arcadia/VPL/TreeBuilder.h)

  OnSourceFile(${this} Arcadia/VPL/Diagnostics/SymbolAlreadyDefinedDiagnostic.c)
  OnHeaderFile(${this} Arcadia/VPL/Diagnostics/SymbolAlreadyDefinedDiagnostic.h)

  OnSourceFile(${this} Arcadia/VPL/GLSL/Include.c)
  OnHeaderFile(${this} Arcadia/VPL/GLSL/Include.h)
  OnSourceFile(${this} Arcadia/VPL/GLSL/Transpiler.c)
  OnHeaderFile(${this} Arcadia/VPL/GLSL/Transpiler.h)

  OnSourceFile(${this} Arcadia/VPL/Context.c)
  OnHeaderFile(${this} Arcadia/VPL/Context.h)

  OnModuleDependency(${this} ${MyProjectName}.Math)
  OnModuleDependency(${this} ${MyProjectName}.Languages)

  OnModuleDependency(${this} ${MyProjectName}.Ring2 PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.DDL PRIVATE)

  EndProduct(${this})

  set_target_properties(${this} PROPERTIES FOLDER "VPL")

endif()
