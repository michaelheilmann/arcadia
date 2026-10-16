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

#if !defined(ARCADIA_PDLC_AST_INCLUDE_H_INCLUDED)
#define ARCADIA_PDLC_AST_INCLUDE_H_INCLUDED

#include "Arcadia/PDLC/AST/DefinitionNode.h"
#include "Arcadia/PDLC/AST/DefinitionStatementNode.h"
#include "Arcadia/PDLC/AST/InstructionNode.h"
#include "Arcadia/PDLC/AST/InstructionStatementNode.h"
#include "Arcadia/PDLC/AST/ModuleNode.h"
#include "Arcadia/PDLC/AST/Node.h"
#include "Arcadia/PDLC/AST/OperandNode.h"
#include "Arcadia/PDLC/AST/StatementNode.h"

#include "Arcadia/PDLC/AST/CompilationUnitNode.h"

#include "Arcadia/PDLC/AST/IdentifierNode.h"

#include "Arcadia/PDLC/AST/Instructions/BinaryInstructionNode.h"
#include "Arcadia/PDLC/AST/Instructions/ConstructInstructionNode.h"
#include "Arcadia/PDLC/AST/Instructions/InvokeInstructionNode.h"
#include "Arcadia/PDLC/AST/Instructions/JumpInstructionNode.h"
#include "Arcadia/PDLC/AST/Instructions/RaiseInstructionNode.h"
#include "Arcadia/PDLC/AST/Instructions/ReturnInstructionNode.h"
#include "Arcadia/PDLC/AST/Instructions/UnaryInstructionNode.h"

#include "Arcadia/PDLC/AST/Instructions/LoadArgumentInstructionNode.h"
#include "Arcadia/PDLC/AST/Instructions/StoreArgumentInstructionNode.h"
#include "Arcadia/PDLC/AST/Instructions/LoadFieldInstructionNode.h"
#include "Arcadia/PDLC/AST/Instructions/StoreFieldInstructionNode.h"

#include "Arcadia/PDLC/AST/Literals/BooleanLiteralNode.h"
#include "Arcadia/PDLC/AST/Literals/IntegerLiteralNode.h"
#include "Arcadia/PDLC/AST/Literals/RealLiteralNode.h"
#include "Arcadia/PDLC/AST/Literals/StringLiteralNode.h"
#include "Arcadia/PDLC/AST/Literals/VoidLiteralNode.h"

#include "Arcadia/PDLC/AST/Operands/LiteralOperandNode.h"
#include "Arcadia/PDLC/AST/Operands/RegisterOperandNode.h"
#include "Arcadia/PDLC/AST/Operands/VariableOperandNode.h"

#include "Arcadia/PDLC/AST/Definitions/ClassDefinitionNode.h"
#include "Arcadia/PDLC/AST/Definitions/ConstructorDefinitionNode.h"
#include "Arcadia/PDLC/AST/Definitions/EnumerationConstantDefinitionNode.h"
#include "Arcadia/PDLC/AST/Definitions/EnumerationDefinitionNode.h"
#include "Arcadia/PDLC/AST/Definitions/FieldDefinitionNode.h"
#include "Arcadia/PDLC/AST/Definitions/InterfaceDefinitionNode.h"
#include "Arcadia/PDLC/AST/Definitions/MethodDefinitionNode.h"
#include "Arcadia/PDLC/AST/Definitions/ModuleDefinitionNode.h"
#include "Arcadia/PDLC/AST/Definitions/ProcedureDefinitionNode.h"

#include "Arcadia/PDLC/AST/DefinitionStatements/LabelDefinitionStatementNode.h"
#include "Arcadia/PDLC/AST/DefinitionStatements/VariableDefinitionStatementNode.h"

#include "Arcadia/PDLC/AST/DefinitionNode.h"
#include "Arcadia/PDLC/AST/DefinitionStatementNode.h"
#include "Arcadia/PDLC/AST/InstructionNode.h"
#include "Arcadia/PDLC/AST/InstructionStatementNode.h"
#include "Arcadia/PDLC/AST/ModuleNode.h"
#include "Arcadia/PDLC/AST/Node.h"
#include "Arcadia/PDLC/AST/OperandNode.h"
#include "Arcadia/PDLC/AST/StatementNode.h"

#include "Arcadia/PDLC/AST/Visitor.h"

#endif // ARCADIA_PDLC_AST_INCLUDE_H_INCLUDED
