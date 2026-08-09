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

set(MyTool ${MyProjectName}.MILC.CIL)

if (NOT TARGET ${MyTool})

  BeginProduct(${MyTool} executable)

  OnConfigurationFile(${MyTool} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/MIL/Compiler/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/MIL/Compiler/Configure.h.i)

  OnSourceFile(${MyTool} Arcadia/MIL/Compiler/Main.c)
  OnSourceFile(${MyTool} Arcadia/MIL/Compiler/HelpSystem.c)
  OnHeaderFile(${MyTool} Arcadia/MIL/Compiler/HelpSystem.h)

  OnModuleDependency(${MyTool} ${MyProjectName}.MILC PRIVATE)
  OnModuleDependency(${MyTool} ${MyProjectName}.DDL PRIVATE)
  OnModuleDependency(${MyTool} ${MyProjectName}.DDLS PRIVATE)

  EndProduct(${MyTool})

  set(COMMAND_LINE "--mil2mil --configuration=\\\"${CMAKE_CURRENT_SOURCE_DIR}/../../../Modules.mil\\\"")
  set_target_properties(${MyTool} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "${COMMAND_LINE}")
  set_target_properties(${MyTool} PROPERTIES FOLDER "MILC")

endif()
