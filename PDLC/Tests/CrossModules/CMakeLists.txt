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

set(this ${MyProjectName}.PDLC.Tests.CrossModules.Generator)
BeginProduct(${this} executable)
SetFolder(${this} PDLC)
OnSourceFile(${this} Generator.c)
OnModuleDependency(${this} ${MyProjectName}.PDLC PRIVATE)
EndProduct(${this})
set(generator ${this})
set(root "${CMAKE_CURRENT_BINARY_DIR}/Sources/Generated/$<CONFIG>")
set(aRoot "${root}/A/Library/Sources")
set(bRoot "${root}/B/Library/Sources")
set(aSources "${aRoot}/Example/A/Base.c.g" "${aRoot}/Example/A/Types/Flavor.c" "${aRoot}/Example/A/Root.c" "${aRoot}/Example/A/answer.c.g")
set(bSources "${bRoot}/Example/B/Derived.c.g" "${bRoot}/Example/B/Holder.c.g" "${bRoot}/Example/B/Child.c" "${bRoot}/Example/B/answer.c.g" "${bRoot}/Example/B/answerFromA.c.g")
list(APPEND bSources "${bRoot}/Example/B/defaultFlavor.c.g")
list(APPEND aSources "${aRoot}/Example/A.c.g")
list(APPEND bSources "${bRoot}/Example/B.c.g")
set(headers "${aRoot}/Example/A/Base.h" "${aRoot}/Example/A/Types/Flavor.h" "${aRoot}/Example/A/Root.h"
            "${bRoot}/Example/B/Derived.h" "${bRoot}/Example/B/Holder.h" "${bRoot}/Example/B/Child.h"
            "${aRoot}/Example/A/answer.h" "${bRoot}/Example/B/answer.h" "${bRoot}/Example/B/answerFromA.h"
             "${aRoot}/Example/A/Include.h" "${bRoot}/Example/B/Include.h")
list(APPEND headers "${aRoot}/Example/A.h" "${bRoot}/Example/B.h")
list(APPEND headers "${bRoot}/Example/B/defaultFlavor.h")
file(GLOB_RECURSE inputs CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Assets/*")
add_custom_command(
  OUTPUT ${aSources} ${bSources} ${headers}
  COMMAND ${CMAKE_COMMAND} -E rm -rf "${root}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Assets" "${root}"
  COMMAND $<TARGET_FILE:${generator}> "${root}"
  COMMAND ${CMAKE_COMMAND} "-DSOURCE_ROOT=${aRoot}" -DMODULE_PATH=Example/A -DPROCEDURE=answer
          -P "${CMAKE_CURRENT_SOURCE_DIR}/../VerifyProcedurePaths.cmake"
  COMMAND ${CMAKE_COMMAND} "-DSOURCE_ROOT=${bRoot}" -DMODULE_PATH=Example/B -DPROCEDURE=answer
          -P "${CMAKE_CURRENT_SOURCE_DIR}/../VerifyProcedurePaths.cmake"
  DEPENDS ${generator} ${inputs} "${CMAKE_CURRENT_SOURCE_DIR}/../VerifyProcedurePaths.cmake"
  VERBATIM
)
Arcadia_markGeneratedTestSources(${aSources} ${bSources})
add_custom_target(${generator}.Run DEPENDS ${aSources} ${bSources} ${headers})

set(this ${MyProjectName}.PDLC.Tests.CrossModules.A)
BeginProduct(${this} library)
SetFolder(${this} PDLC)
list(APPEND ${this}.SourceFiles ${aSources})
OnModuleDependency(${this} ${MyProjectName}.Ring1 PUBLIC)
EndProduct(${this})
add_dependencies(${this} ${generator}.Run)
target_include_directories(${this} PUBLIC "${aRoot}")

set(this ${MyProjectName}.PDLC.Tests.CrossModules.B)
BeginProduct(${this} library)
SetFolder(${this} PDLC)
list(APPEND ${this}.SourceFiles ${bSources})
OnModuleDependency(${this} ${MyProjectName}.PDLC.Tests.CrossModules.A PUBLIC)
EndProduct(${this})
add_dependencies(${this} ${generator}.Run)
target_include_directories(${this} PUBLIC "${bRoot}")

set(this ${MyProjectName}.PDLC.Tests.CrossModules)
BeginProduct(${this} test)
SetFolder(${this} PDLC)
OnSourceFile(${this} Main.c)
OnModuleDependency(${this} ${MyProjectName}.PDLC.Tests.CrossModules.B PRIVATE)
OnModuleDependency(${this} ${MyProjectName}.Testing PRIVATE)
EndProduct(${this})
