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

set(this ${MyProjectName}.Tools.GenerateRyuLookupTables)

if (NOT TARGET ${this})

  BeginProduct(${this} executable)

  SetFolder(${this} "Tools")

  set(${this}.Windows.ResourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Resources.rc)

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia.Tools.GenerateRyuLookupTables/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia.Tools.GenerateRyuLookupTables/Configure.h.i)

  OnSourceFile(${this} Arcadia.Tools.GenerateRyuLookupTables/Main.c)

  OnSourceFile(${this} Arcadia.Tools.GenerateRyuLookupTables/generateReal64Table.c)
  OnHeaderFile(${this} Arcadia.Tools.GenerateRyuLookupTables/generateReal64Table.h)
  OnSourceFile(${this} Arcadia.Tools.GenerateRyuLookupTables/generateReal32Table.c)
  OnHeaderFile(${this} Arcadia.Tools.GenerateRyuLookupTables/generateReal32Table.h)
  OnSourceFile(${this} Arcadia.Tools.GenerateRyuLookupTables/Common.c)
  OnHeaderFile(${this} Arcadia.Tools.GenerateRyuLookupTables/Common.h)

  OnModuleDependency(${this} ${MyProjectName}.Logging PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.Ring2 PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.Ring1 PRIVATE)

  EndProduct(${this})

  set_target_properties(${this} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "--target=\"\"\"${CMAKE_CURRENT_BINARY_DIR}\"\"\"")

endif()
