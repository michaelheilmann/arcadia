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

set(MyTool ${MyProjectName}.Tools.PixelBufferGenerator)

if (NOT TARGET ${MyTool})

  BeginProduct(${MyTool} executable)

  OnConfigurationFile(${MyTool} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Tools/PixelBufferGenerator/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Tools/PixelBufferGenerator/Configure.h.i)

  OnSourceFile(${MyTool} Tools/PixelBufferGenerator/Main.c)

  OnAssetsDirectory(${MyTool} ${CMAKE_CURRENT_SOURCE_DIR}/Assets)

  OnModuleDependency(${MyTool} ${MyProjectName}.ADL PRIVATE)
  OnModuleDependency(${MyTool} ${MyProjectName}.PixelBufferIO PRIVATE)
  OnModuleDependency(${MyTool} ${MyProjectName}.DDL PRIVATE)
  OnModuleDependency(${MyTool} ${MyProjectName}.Print PRIVATE)

  EndProduct(${MyTool})

  CopyProductAssets(${MyTool} "Tools/PixelBufferGenerator" ${CMAKE_CURRENT_BINARY_DIR}/Assets)

  set(COMMAND_LINE_ARGS)
  list(APPEND COMMAND_LINE_ARGS "--target=\\\"helloworld.png\\\"")
  list(APPEND COMMAND_LINE_ARGS "--definition=\\\"Assets/Black.adl\\\"")
  list(APPEND COMMAND_LINE_ARGS "--definition=\\\"Assets/White.adl\\\"")
  list(APPEND COMMAND_LINE_ARGS "--definition=\\\"Assets/FillCheckerboardBlackWhite.adl\\\"")
  list(APPEND COMMAND_LINE_ARGS "--definition=\\\"Assets/Example1.adl\\\"")
  list(APPEND COMMAND_LINE_ARGS "--start=\\\"Example1\\\"")
  list(JOIN COMMAND_LINE_ARGS " " COMMAND_LINE)

  set_target_properties(${MyTool} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "${COMMAND_LINE}")

  #install(TARGETS ${MyTool} RUNTIME DESTINATION bin/$<CONFIG>) # Wrap into *Product API.
  set_target_properties(${MyTool} PROPERTIES FOLDER "Tools/PixelBufferGenerator") # Wrap into *Product API.

endif()
