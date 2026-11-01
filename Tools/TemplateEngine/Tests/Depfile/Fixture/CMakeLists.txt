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
project(ArcadiaTemplateDepfileFixture LANGUAGES NONE)
cmake_path(IS_PREFIX REPOSITORY_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" NORMALIZE inCheckout)
if(inCheckout)
  message(FATAL_ERROR "Run Integration.cmake: this fixture writes inputs only in its disposable source copy, never the checkout")
endif()
set(MyProjectName Arcadia)
include("${REPOSITORY_SOURCE_DIR}/CMake/tool-file-copy.cmake")
include("${REPOSITORY_SOURCE_DIR}/CMake/tools-template-engine.cmake")
add_executable(Arcadia.TemplateEngine.CLI IMPORTED GLOBAL)
set_property(TARGET Arcadia.TemplateEngine.CLI PROPERTY IMPORTED_LOCATION "${CLI}")
set(inputs "${CMAKE_CURRENT_SOURCE_DIR}/Inputs")
file(WRITE "${inputs}/main file.t" "@{include(\"Inputs/middle file.t\")}")
file(WRITE "${inputs}/middle file.t" "@{include(\"Inputs/leaf file.t\")}")
file(WRITE "${inputs}/leaf file.t" "leaf-v1\n")
file(WRITE "${inputs}/environment.env" "{}\n")
BeginTemplateEngine(Render "")
OnTemplateEngine("${inputs}/main file.t" "${CMAKE_CURRENT_BINARY_DIR}/rendered.txt" "${inputs}/environment.env")
EndTemplateEngine()
