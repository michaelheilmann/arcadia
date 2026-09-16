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

#if !defined(ARCADIA_MILC_AST_INCLUDE_H_INCLUDED)
#define ARCADIA_MILC_AST_INCLUDE_H_INCLUDED

#include "Arcadia/MILC/AST/DefinitionNode.h"
#include "Arcadia/MILC/AST/DefinitionStatementNode.h"
#include "Arcadia/MILC/AST/InstructionNode.h"
#include "Arcadia/MILC/AST/InstructionStatementNode.h"
#include "Arcadia/MILC/AST/ModuleNode.h"
#include "Arcadia/MILC/AST/Node.h"
#include "Arcadia/MILC/AST/OperandNode.h"
#include "Arcadia/MILC/AST/StatementNode.h"

#include "Arcadia/MILC/AST/CompilationUnitNode.h"

#include "Arcadia/MILC/AST/IdentifierNode.h"

#include "Arcadia/MILC/AST/Instructions/BinaryInstructionNode.h"
#include "Arcadia/MILC/AST/Instructions/InvokeInstructionNode.h"
#include "Arcadia/MILC/AST/Instructions/JumpInstructionNode.h"
#include "Arcadia/MILC/AST/Instructions/RaiseInstructionNode.h"
#include "Arcadia/MILC/AST/Instructions/ReturnInstructionNode.h"
#include "Arcadia/MILC/AST/Instructions/UnaryInstructionNode.h"

#include "Arcadia/MILC/AST/Instructions/LoadArgumentInstructionNode.h"
#include "Arcadia/MILC/AST/Instructions/StoreArgumentInstructionNode.h"
#include "Arcadia/MILC/AST/Instructions/LoadFieldInstructionNode.h"
#include "Arcadia/MILC/AST/Instructions/StoreFieldInstructionNode.h"

#include "Arcadia/MILC/AST/Literals/BooleanLiteralNode.h"
#include "Arcadia/MILC/AST/Literals/IntegerLiteralNode.h"
#include "Arcadia/MILC/AST/Literals/RealLiteralNode.h"
#include "Arcadia/MILC/AST/Literals/StringLiteralNode.h"
#include "Arcadia/MILC/AST/Literals/VoidLiteralNode.h"

#include "Arcadia/MILC/AST/Operands/LiteralOperandNode.h"
#include "Arcadia/MILC/AST/Operands/RegisterOperandNode.h"
#include "Arcadia/MILC/AST/Operands/VariableOperandNode.h"

#include "Arcadia/MILC/AST/Definitions/ClassDefinitionNode.h"
#include "Arcadia/MILC/AST/Definitions/ConstructorDefinitionNode.h"
#include "Arcadia/MILC/AST/Definitions/EnumerationConstantDefinitionNode.h"
#include "Arcadia/MILC/AST/Definitions/EnumerationDefinitionNode.h"
#include "Arcadia/MILC/AST/Definitions/FieldDefinitionNode.h"
#include "Arcadia/MILC/AST/Definitions/MethodDefinitionNode.h"
#include "Arcadia/MILC/AST/Definitions/ModuleDefinitionNode.h"
#include "Arcadia/MILC/AST/Definitions/ProcedureDefinitionNode.h"

#include "Arcadia/MILC/AST/DefinitionStatements/LabelDefinitionStatementNode.h"
#include "Arcadia/MILC/AST/DefinitionStatements/VariableDefinitionStatementNode.h"

#include "Arcadia/MILC/AST/DefinitionNode.h"
#include "Arcadia/MILC/AST/DefinitionStatementNode.h"
#include "Arcadia/MILC/AST/InstructionNode.h"
#include "Arcadia/MILC/AST/InstructionStatementNode.h"
#include "Arcadia/MILC/AST/ModuleNode.h"
#include "Arcadia/MILC/AST/Node.h"
#include "Arcadia/MILC/AST/OperandNode.h"
#include "Arcadia/MILC/AST/StatementNode.h"

#include "Arcadia/MILC/AST/Visitor.h"

#endif // ARCADIA_MILC_AST_INCLUDE_H_INCLUDED
