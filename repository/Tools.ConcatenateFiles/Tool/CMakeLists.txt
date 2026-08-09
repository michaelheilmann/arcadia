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

set(MyTool ${MyProjectName}.Tools.ConcatenateFiles)

if (NOT TARGET ${MyTool})

  BeginProduct(${MyTool} executable)

  OnConfigurationFile(${MyTool} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Tools/ConcatenateFiles/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Tools/ConcatenateFiles/Configure.h.i)

  OnSourceFile(${MyTool} Tools/ConcatenateFiles/Main.c)

  OnModuleDependency(${MyTool} ${MyProjectName}.Ring2 PRIVATE)
  OnModuleDependency(${MyTool} ${MyProjectName}.FileSystem PRIVATE)

  EndProduct(${MyTool})

  set_target_properties(${MyTool} PROPERTIES FOLDER "Tools")

endif()
