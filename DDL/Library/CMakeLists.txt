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
set(this ${MyProjectName}.DDL)

if (NOT TARGET ${this})
  BeginProduct(${this} library)

  SetFolder(${this} "DDL")

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/DDL/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/DDL/Configure.h.i)

  OnSourceFile(${this} Arcadia/DDL/Include.c)
  OnHeaderFile(${this} Arcadia/DDL/Include.h)
  OnSourceFile(${this} Arcadia/DDL/Writer/Unparser.c)
  OnHeaderFile(${this} Arcadia/DDL/Writer/Unparser.h)
  OnSourceFile(${this} Arcadia/DDL/Reader/DefaultReader.c)
  OnHeaderFile(${this} Arcadia/DDL/Reader/DefaultReader.h)
  OnSourceFile(${this} Arcadia/DDL/Reader/Keywords.c)
  OnHeaderFile(${this} Arcadia/DDL/Reader/Keywords.h)
  OnSourceFile(${this} Arcadia/DDL/Reader/Parser.c)
  OnHeaderFile(${this} Arcadia/DDL/Reader/Parser.h)
  OnSourceFile(${this} Arcadia/DDL/Reader/Scanner.c)
  OnHeaderFile(${this} Arcadia/DDL/Reader/Scanner.h)
  OnSourceFile(${this} Arcadia/DDL/Reader/WordType.c)
  OnHeaderFile(${this} Arcadia/DDL/Reader/WordType.h)
  OnHeaderFile(${this} Arcadia/DDL/Reader/WordType.i)

  OnSourceFile(${this} Arcadia/DDL/Nodes/Include.c)
  OnHeaderFile(${this} Arcadia/DDL/Nodes/Include.h)
  OnSourceFile(${this} Arcadia/DDL/Nodes/Node.c)
  OnHeaderFile(${this} Arcadia/DDL/Nodes/Node.h)

  OnSourceFile(${this} Arcadia/DDL/Nodes/BooleanNode.c)
  OnHeaderFile(${this} Arcadia/DDL/Nodes/BooleanNode.h)
  OnSourceFile(${this} Arcadia/DDL/Nodes/ListNode.c)
  OnHeaderFile(${this} Arcadia/DDL/Nodes/ListNode.h)
  OnSourceFile(${this} Arcadia/DDL/Nodes/MapNode.c)
  OnHeaderFile(${this} Arcadia/DDL/Nodes/MapNode.h)
  OnSourceFile(${this} Arcadia/DDL/Nodes/MapEntryNode.c)
  OnHeaderFile(${this} Arcadia/DDL/Nodes/MapEntryNode.h)
  OnSourceFile(${this} Arcadia/DDL/Nodes/NameNode.c)
  OnHeaderFile(${this} Arcadia/DDL/Nodes/NameNode.h)
  OnSourceFile(${this} Arcadia/DDL/Nodes/NodeType.c)
  OnHeaderFile(${this} Arcadia/DDL/Nodes/NodeType.h)
  OnHeaderFile(${this} Arcadia/DDL/Nodes/NodeType.i)
  OnSourceFile(${this} Arcadia/DDL/Nodes/NumberNode.c)
  OnHeaderFile(${this} Arcadia/DDL/Nodes/NumberNode.h)
  OnSourceFile(${this} Arcadia/DDL/Nodes/StringNode.c)
  OnHeaderFile(${this} Arcadia/DDL/Nodes/StringNode.h)
  OnSourceFile(${this} Arcadia/DDL/Nodes/VoidNode.c)
  OnHeaderFile(${this} Arcadia/DDL/Nodes/VoidNode.h)

  OnSourceFile(${this} Arcadia/DDL/SemanticalAnalysis.c)
  OnHeaderFile(${this} Arcadia/DDL/SemanticalAnalysis.h)

  OnModuleDependency(${this} ${MyProjectName}.Languages)
  OnModuleDependency(${this} ${MyProjectName}.Logging)
  OnModuleDependency(${this} ${MyProjectName}.Collections)
  OnModuleDependency(${this} ${MyProjectName}.Ring2)

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux})
    list(APPEND ${this}.Libraries m)
  endif()

  EndProduct(${this})

endif()
