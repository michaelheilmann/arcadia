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
set(this ${MyProjectName}.DDLS)

if (NOT TARGET ${this})
  BeginProduct(${this} library)

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/DDLS/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/DDLS/Configure.h.i)

  OnSourceFile(${this} Arcadia/DDLS/Include.c)
  OnHeaderFile(${this} Arcadia/DDLS/Include.h)
  OnSourceFile(${this} Arcadia/DDLS/Syntactical/DefaultReader.c)
  OnHeaderFile(${this} Arcadia/DDLS/Syntactical/DefaultReader.h)

  OnSourceFile(${this} Arcadia/DDLS/Symbols/AnySymbol.c)
  OnHeaderFile(${this} Arcadia/DDLS/Symbols/AnySymbol.h)
  OnSourceFile(${this} Arcadia/DDLS/Symbols/ChoiceSymbol.c)
  OnHeaderFile(${this} Arcadia/DDLS/Symbols/ChoiceSymbol.h)
  OnSourceFile(${this} Arcadia/DDLS/Symbols/ListSymbol.c)
  OnHeaderFile(${this} Arcadia/DDLS/Symbols/ListSymbol.h)
  OnSourceFile(${this} Arcadia/DDLS/Symbols/MapSymbol.c)
  OnHeaderFile(${this} Arcadia/DDLS/Symbols/MapSymbol.h)
  OnSourceFile(${this} Arcadia/DDLS/Symbols/MapEntrySymbol.c)
  OnHeaderFile(${this} Arcadia/DDLS/Symbols/MapEntrySymbol.h)
  OnSourceFile(${this} Arcadia/DDLS/Symbols/ScalarSymbol.c)
  OnHeaderFile(${this} Arcadia/DDLS/Symbols/ScalarSymbol.h)
  OnSourceFile(${this} Arcadia/DDLS/Symbols/SchemaSymbol.c)
  OnHeaderFile(${this} Arcadia/DDLS/Symbols/SchemaSymbol.h)
  OnSourceFile(${this} Arcadia/DDLS/Symbols/SchemaReferenceSymbol.c)
  OnHeaderFile(${this} Arcadia/DDLS/Symbols/SchemaReferenceSymbol.h)
  OnSourceFile(${this} Arcadia/DDLS/Symbols/Scope.c)
  OnHeaderFile(${this} Arcadia/DDLS/Symbols/Scope.h)
  OnSourceFile(${this} Arcadia/DDLS/Symbols/Symbol.c)
  OnHeaderFile(${this} Arcadia/DDLS/Symbols/Symbol.h)
  OnSourceFile(${this} Arcadia/DDLS/Symbols/SymbolReader.c)
  OnHeaderFile(${this} Arcadia/DDLS/Symbols/SymbolReader.h)

  OnSourceFile(${this} Arcadia/DDLS/Implementation/Diagnostics.c)
  OnHeaderFile(${this} Arcadia/DDLS/Implementation/Diagnostics.h)
  OnSourceFile(${this} Arcadia/DDLS/Implementation/ValidationContext.c)
  OnHeaderFile(${this} Arcadia/DDLS/Implementation/ValidationContext.h)

  OnSourceFile(${this} Arcadia/DDLS/Extensions.c)
  OnHeaderFile(${this} Arcadia/DDLS/Extensions.h)

  OnSourceFile(${this} Arcadia/DDLS/Nodes/Include.c)
  OnHeaderFile(${this} Arcadia/DDLS/Nodes/Include.h)
  OnSourceFile(${this} Arcadia/DDLS/Nodes/AnyNode.c)
  OnHeaderFile(${this} Arcadia/DDLS/Nodes/AnyNode.h)
  OnSourceFile(${this} Arcadia/DDLS/Nodes/ChoiceNode.c)
  OnHeaderFile(${this} Arcadia/DDLS/Nodes/ChoiceNode.h)
  OnSourceFile(${this} Arcadia/DDLS/Nodes/ListNode.c)
  OnHeaderFile(${this} Arcadia/DDLS/Nodes/ListNode.h)
  OnSourceFile(${this} Arcadia/DDLS/Nodes/MapNode.c)
  OnHeaderFile(${this} Arcadia/DDLS/Nodes/MapNode.h)
  OnSourceFile(${this} Arcadia/DDLS/Nodes/Node.c)
  OnHeaderFile(${this} Arcadia/DDLS/Nodes/Node.h)
  OnSourceFile(${this} Arcadia/DDLS/Nodes/ScalarNode.c)
  OnHeaderFile(${this} Arcadia/DDLS/Nodes/ScalarNode.h)
  OnSourceFile(${this} Arcadia/DDLS/Nodes/SchemaNode.c)
  OnHeaderFile(${this} Arcadia/DDLS/Nodes/SchemaNode.h)
  OnSourceFile(${this} Arcadia/DDLS/Nodes/SchemaReferenceNode.c)
  OnHeaderFile(${this} Arcadia/DDLS/Nodes/SchemaReferenceNode.h)

  OnModuleDependency(${this} ${MyProjectName}.DDL)
  OnModuleDependency(${this} ${MyProjectName}.Languages)
  OnModuleDependency(${this} ${MyProjectName}.FileSystem)
  OnModuleDependency(${this} ${MyProjectName}.Ring2)

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux})
    list(APPEND ${this}.Libraries m)
  endif()

  EndProduct(${this})
  set_target_properties(${this} PROPERTIES FOLDER "DDLS")

endif()
