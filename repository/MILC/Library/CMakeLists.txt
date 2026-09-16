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

# Create library.
set(this ${MyProjectName}.MILC)

if (NOT TARGET ${this})
  BeginProduct(${this} library)

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/MILC/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/MILC/Configure.h.i)

  OnSourceFile(${this} Arcadia/MILC/Include.c)
  OnHeaderFile(${this} Arcadia/MILC/Include.h)

  OnSourceFile(${this} Arcadia/MILC/CompilationTask.c)
  OnHeaderFile(${this} Arcadia/MILC/CompilationTask.h)
  OnSourceFile(${this} Arcadia/MILC/CompilationFailedException.c)
  OnHeaderFile(${this} Arcadia/MILC/CompilationFailedException.h)

  OnSourceFile(${this} Arcadia/MILC/EnterPhase.c)
  OnHeaderFile(${this} Arcadia/MILC/EnterPhase.h)
  OnSourceFile(${this} Arcadia/MILC/Environment.c)
  OnHeaderFile(${this} Arcadia/MILC/Environment.h)


  OnSourceFile(${this} Arcadia/MILC/MemberEnterPhase.c)
  OnHeaderFile(${this} Arcadia/MILC/MemberEnterPhase.h)
    
  OnSourceFile(${this} Arcadia/MILC/TypeResolutionPhase.c)
  OnHeaderFile(${this} Arcadia/MILC/TypeResolutionPhase.h)
  OnSourceFile(${this} Arcadia/MILC/TypeResolutionPhase/ClassCompleter.c)
  OnHeaderFile(${this} Arcadia/MILC/TypeResolutionPhase/ClassCompleter.h)
  OnSourceFile(${this} Arcadia/MILC/TypeResolutionPhase/ConstructorCompleter.c)
  OnHeaderFile(${this} Arcadia/MILC/TypeResolutionPhase/ConstructorCompleter.h)
  OnSourceFile(${this} Arcadia/MILC/TypeResolutionPhase/EnumerationCompleter.c)
  OnHeaderFile(${this} Arcadia/MILC/TypeResolutionPhase/EnumerationCompleter.h)
  OnSourceFile(${this} Arcadia/MILC/TypeResolutionPhase/MethodCompleter.c)
  OnHeaderFile(${this} Arcadia/MILC/TypeResolutionPhase/MethodCompleter.h)
  OnSourceFile(${this} Arcadia/MILC/TypeResolutionPhase/ProcedureCompleter.c)
  OnHeaderFile(${this} Arcadia/MILC/TypeResolutionPhase/ProcedureCompleter.h)
    
  OnSourceFile(${this} Arcadia/MILC/Context.c)
  OnHeaderFile(${this} Arcadia/MILC/Context.h)

  OnSourceFile(${this} Arcadia/MILC/FileType.c)
  OnHeaderFile(${this} Arcadia/MILC/FileType.h)

  OnSourceFile(${this} Arcadia/MILC/Names.c)
  OnHeaderFile(${this} Arcadia/MILC/Names.h)

  OnSourceFile(${this} Arcadia/MILC/Symbols/Include.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/Include.h)

  OnSourceFile(${this} Arcadia/MILC/Symbols/Symbol.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/Symbol.h)
  OnSourceFile(${this} Arcadia/MILC/Symbols/SymbolKind.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/SymbolKind.h)

  OnSourceFile(${this} Arcadia/MILC/Symbols/ClassSymbol.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/ClassSymbol.h)  
  OnSourceFile(${this} Arcadia/MILC/Symbols/ConstructorSymbol.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/ConstructorSymbol.h) 
  OnSourceFile(${this} Arcadia/MILC/Symbols/EnumerationSymbol.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/EnumerationSymbol.h)
  OnSourceFile(${this} Arcadia/MILC/Symbols/EnumerationConstantSymbol.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/EnumerationConstantSymbol.h)
  OnSourceFile(${this} Arcadia/MILC/Symbols/MethodSymbol.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/MethodSymbol.h)
  OnSourceFile(${this} Arcadia/MILC/Symbols/ModuleSymbol.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/ModuleSymbol.h)
  OnSourceFile(${this} Arcadia/MILC/Symbols/PrimitiveTypeSymbol.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/PrimitiveTypeSymbol.h)
  OnSourceFile(${this} Arcadia/MILC/Symbols/ProcedureSymbol.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/ProcedureSymbol.h)
  OnSourceFile(${this} Arcadia/MILC/Symbols/ProcedureSymbol.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/ProcedureSymbol.h)
  OnSourceFile(${this} Arcadia/MILC/Symbols/Symbols.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/Symbols.h)
  OnSourceFile(${this} Arcadia/MILC/Symbols/VariableSymbol.c)
  OnHeaderFile(${this} Arcadia/MILC/Symbols/VariableSymbol.h)


  OnSourceFile(${this} Arcadia/MILC/Completer.c)
  OnHeaderFile(${this} Arcadia/MILC/Completer.h)

  OnSourceFile(${this} Arcadia/MILC/MemberEnterPhase/ClassCompleter.c)
  OnHeaderFile(${this} Arcadia/MILC/MemberEnterPhase/ClassCompleter.h)
  OnSourceFile(${this} Arcadia/MILC/MemberEnterPhase/EnumerationCompleter.c)
  OnHeaderFile(${this} Arcadia/MILC/MemberEnterPhase/EnumerationCompleter.h)
  OnSourceFile(${this} Arcadia/MILC/MemberEnterPhase/ProcedureCompleter.c)
  OnHeaderFile(${this} Arcadia/MILC/MemberEnterPhase/ProcedureCompleter.h)

  OnSourceFile(${this} Arcadia/MILC/Diagnostics/ConfigurationFileInvalidDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/ConfigurationFileInvalidDiagnostic.h)
  OnSourceFile(${this} Arcadia/MILC/Diagnostics/FileNotFoundDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/FileNotFoundDiagnostic.h)
  OnSourceFile(${this} Arcadia/MILC/Diagnostics/IntegerLiteralOutOfRangeDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/IntegerLiteralOutOfRangeDiagnostic.h)
  OnSourceFile(${this} Arcadia/MILC/Diagnostics/InvalidPathDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/InvalidPathDiagnostic.h)
  OnSourceFile(${this} Arcadia/MILC/Diagnostics/MissingInitializerDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/MissingInitializerDiagnostic.h)
  OnSourceFile(${this} Arcadia/MILC/Diagnostics/MissingModuleDefinitionDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/MissingModuleDefinitionDiagnostic.h)
  OnSourceFile(${this} Arcadia/MILC/Diagnostics/MultipleModuleDefinitionsDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/MultipleModuleDefinitionsDiagnostic.h)
  OnSourceFile(${this} Arcadia/MILC/Diagnostics/Lexical/UnexpectedSymbolDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/Lexical/UnexpectedSymbolDiagnostic.h)
  OnSourceFile(${this} Arcadia/MILC/Diagnostics/Syntactical/UnexpectedWordDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/Syntactical/UnexpectedWordDiagnostic.h)
  OnSourceFile(${this} Arcadia/MILC/Diagnostics/SymbolIsAlreadyDefinedDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/SymbolIsAlreadyDefinedDiagnostic.h)
  OnSourceFile(${this} Arcadia/MILC/Diagnostics/SymbolIsNotDefinedDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/SymbolIsNotDefinedDiagnostic.h)
  OnSourceFile(${this} Arcadia/MILC/Diagnostics/CyclicInheritanceDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/CyclicInheritanceDiagnostic.h)
  OnSourceFile(${this} Arcadia/MILC/Diagnostics/SymbolIsNoClassDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/SymbolIsNoClassDiagnostic.h)
  OnSourceFile(${this} Arcadia/MILC/Diagnostics/SymbolIsNoMethodDiagnostic.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/SymbolIsNoMethodDiagnostic.h)

  OnSourceFile(${this} Arcadia/MILC/Diagnostics/Include.c)
  OnHeaderFile(${this} Arcadia/MILC/Diagnostics/Include.h)

  OnSourceFile(${this} Arcadia/MILC/WordType.c)
  OnHeaderFile(${this} Arcadia/MILC/WordType.h)

  OnSourceFile(${this} Arcadia/MILC/Scanner.c)
  OnHeaderFile(${this} Arcadia/MILC/Scanner.h)
  OnSourceFile(${this} Arcadia/MILC/Parser.c)
  OnHeaderFile(${this} Arcadia/MILC/Parser.h)
  OnSourceFile(${this} Arcadia/MILC/Keywords.c)
  OnHeaderFile(${this} Arcadia/MILC/Keywords.h)
  
  OnSourceFile(${this} Arcadia/MILC/AST/Include.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Include.h)

  OnSourceFile(${this} Arcadia/MILC/AST/Visitor.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Visitor.h)

  OnSourceFile(${this} Arcadia/MILC/AST/Node.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Node.h)
  
  OnSourceFile(${this} Arcadia/MILC/AST/ModuleNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/ModuleNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/CompilationUnitNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/CompilationUnitNode.h)

  OnSourceFile(${this} Arcadia/MILC/AST/InstructionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/InstructionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/StatementNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/StatementNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/OperandNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/OperandNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/DefinitionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/DefinitionNode.h)

  OnSourceFile(${this} Arcadia/MILC/AST/IdentifierNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/IdentifierNode.h)

  OnSourceFile(${this} Arcadia/MILC/AST/Literals/BooleanLiteralNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Literals/BooleanLiteralNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Literals/IntegerLiteralNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Literals/IntegerLiteralNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Literals/RealLiteralNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Literals/RealLiteralNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Literals/StringLiteralNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Literals/StringLiteralNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Literals/VoidLiteralNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Literals/VoidLiteralNode.h)

  OnSourceFile(${this} Arcadia/MILC/AST/Instructions/BinaryInstructionKind.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Instructions/BinaryInstructionKind.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Instructions/BinaryInstructionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Instructions/BinaryInstructionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Instructions/UnaryInstructionKind.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Instructions/UnaryInstructionKind.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Instructions/UnaryInstructionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Instructions/UnaryInstructionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Instructions/InvokeInstructionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Instructions/InvokeInstructionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Instructions/JumpInstructionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Instructions/JumpInstructionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Instructions/RaiseInstructionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Instructions/RaiseInstructionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Instructions/ReturnInstructionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Instructions/ReturnInstructionNode.h)
  
  OnSourceFile(${this} Arcadia/MILC/AST/Instructions/LoadFieldInstructionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Instructions/LoadFieldInstructionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Instructions/StoreFieldInstructionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Instructions/StoreFieldInstructionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Instructions/LoadArgumentInstructionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Instructions/LoadArgumentInstructionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Instructions/StoreArgumentInstructionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Instructions/StoreArgumentInstructionNode.h)

  OnSourceFile(${this} Arcadia/MILC/AST/DefinitionStatementNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/DefinitionStatementNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/InstructionStatementNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/InstructionStatementNode.h)

  OnSourceFile(${this} Arcadia/MILC/AST/Operands/LiteralOperandNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Operands/LiteralOperandNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Operands/RegisterOperandNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Operands/RegisterOperandNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Operands/VariableOperandNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Operands/VariableOperandNode.h)

  OnSourceFile(${this} Arcadia/MILC/AST/Definitions/ConstructorDefinitionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Definitions/ConstructorDefinitionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Definitions/ClassDefinitionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Definitions/ClassDefinitionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Definitions/EnumerationConstantDefinitionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Definitions/EnumerationConstantDefinitionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Definitions/EnumerationDefinitionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Definitions/EnumerationDefinitionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Definitions/FieldDefinitionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Definitions/FieldDefinitionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Definitions/MethodDefinitionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Definitions/MethodDefinitionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Definitions/ModuleDefinitionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Definitions/ModuleDefinitionNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/Definitions/ProcedureDefinitionNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/Definitions/ProcedureDefinitionNode.h)

  OnSourceFile(${this} Arcadia/MILC/AST/DefinitionStatements/VariableDefinitionStatementNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/DefinitionStatements/VariableDefinitionStatementNode.h)
  OnSourceFile(${this} Arcadia/MILC/AST/DefinitionStatements/LabelDefinitionStatementNode.c)
  OnHeaderFile(${this} Arcadia/MILC/AST/DefinitionStatements/LabelDefinitionStatementNode.h)

  OnSourceFile(${this} Arcadia/MILC/Backend/EnumerationConstantSymbolInfo.c)
  OnHeaderFile(${this} Arcadia/MILC/Backend/EnumerationConstantSymbolInfo.h)
  OnSourceFile(${this} Arcadia/MILC/Backend/SymbolInfo.c)
  OnHeaderFile(${this} Arcadia/MILC/Backend/SymbolInfo.h)
  OnSourceFile(${this} Arcadia/MILC/Backend/Implementation.c)
  OnHeaderFile(${this} Arcadia/MILC/Backend/Implementation.h)
  OnSourceFile(${this} Arcadia/MILC/Backend/SymbolWriter.c)
  OnHeaderFile(${this} Arcadia/MILC/Backend/SymbolWriter.h)
  OnSourceFile(${this} Arcadia/MILC/Backend/ClassSymbolWriter.c)
  OnHeaderFile(${this} Arcadia/MILC/Backend/ClassSymbolWriter.h)
  OnSourceFile(${this} Arcadia/MILC/Backend/ClassSymbolInfo.c)
  OnHeaderFile(${this} Arcadia/MILC/Backend/ClassSymbolInfo.h)
  OnSourceFile(${this} Arcadia/MILC/Backend/EnumerationSymbolInfo.c)
  OnHeaderFile(${this} Arcadia/MILC/Backend/EnumerationSymbolInfo.h)
  OnSourceFile(${this} Arcadia/MILC/Backend/EnumerationSymbolWriter.c)
  OnHeaderFile(${this} Arcadia/MILC/Backend/EnumerationSymbolWriter.h)
  OnSourceFile(${this} Arcadia/MILC/Backend/ModuleSymbolInfo.c)
  OnHeaderFile(${this} Arcadia/MILC/Backend/ModuleSymbolInfo.h)
  OnSourceFile(${this} Arcadia/MILC/Backend/PrimitiveTypeSymbolInfo.c)
  OnHeaderFile(${this} Arcadia/MILC/Backend/PrimitiveTypeSymbolInfo.h)
  OnSourceFile(${this} Arcadia/MILC/Backend/ProcedureSymbolInfo.c)
  OnHeaderFile(${this} Arcadia/MILC/Backend/ProcedureSymbolInfo.h)
  OnSourceFile(${this} Arcadia/MILC/Backend/CXXFileType.c)
  OnHeaderFile(${this} Arcadia/MILC/Backend/CXXFileType.h)

  OnModuleDependency(${this} ${MyProjectName}.Languages)
  OnModuleDependency(${this} ${MyProjectName}.Logging)
  OnModuleDependency(${this} ${MyProjectName}.Ring2)

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux})
    list(APPEND ${this}.Libraries m)
  endif()

  EndProduct(${this})
  set_target_properties(${this} PROPERTIES FOLDER "MILC")

endif()
