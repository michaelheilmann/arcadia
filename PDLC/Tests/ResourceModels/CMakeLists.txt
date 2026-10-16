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

set(this ${MyProjectName}.PDLC.Tests.ResourceModels.Generator)
BeginProduct(${this} executable)
SetFolder(${this} PDLC)
OnSourceFile(${this} Arcadia/PDLC/Tests/ResourceModels/Generator.c)
OnModuleDependency(${this} ${MyProjectName}.PDLC PRIVATE)
EndProduct(${this})
set(generator ${this})

set(stage "${CMAKE_CURRENT_BINARY_DIR}/Staged/$<CONFIG>")
set(inputStage "${CMAKE_CURRENT_BINARY_DIR}/Inputs/$<CONFIG>")
set(stagedInputs)
foreach(module Engine Media Math Collections ADL)
  if(module STREQUAL Engine)
    set(source "${REPOSITORY_SOURCE_DIR}/Engine/Engine")
  elseif(module STREQUAL Media)
    set(source "${REPOSITORY_SOURCE_DIR}/Media")
  elseif(module STREQUAL Collections)
    set(source "${REPOSITORY_SOURCE_DIR}/Runtime/Collections")
  elseif(module STREQUAL ADL)
    set(source "${REPOSITORY_SOURCE_DIR}/ADL")
  else()
    set(source "${REPOSITORY_SOURCE_DIR}/repository/Math")
  endif()
  file(GLOB_RECURSE inputs CONFIGURE_DEPENDS RELATIVE "${source}" "${source}/Library/Sources/*.pdl")
  list(APPEND inputs Module.pdl)
  foreach(input IN LISTS inputs)
    set(destination "${inputStage}/${module}/${input}")
    get_filename_component(parent "${destination}" DIRECTORY)
    add_custom_command(OUTPUT "${destination}"
      COMMAND ${CMAKE_COMMAND} -E make_directory "${parent}"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${source}/${input}" "${destination}"
      DEPENDS "${source}/${input}" VERBATIM)
    list(APPEND stagedInputs "${destination}")
  endforeach()
endforeach()

set(generatedRoot "${CMAKE_CURRENT_BINARY_DIR}/Sources/Generated/$<CONFIG>")
set(contracts "${generatedRoot}/Contracts.c.g")
add_custom_command(OUTPUT "${contracts}"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${generatedRoot}"
  COMMAND ${CMAKE_COMMAND} -E rm -rf "${stage}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${inputStage}" "${stage}"
  COMMAND $<TARGET_FILE:${generator}> "${stage}" "${contracts}"
  DEPENDS ${generator} ${stagedInputs} VERBATIM)
add_custom_target(${generator}.Run DEPENDS "${contracts}")
set_property(TARGET ${generator}.Run PROPERTY FOLDER PDLC)

set(this ${MyProjectName}.PDLC.Tests.ResourceModels)
BeginProduct(${this} test)
SetFolder(${this} PDLC)
OnSourceFile(${this} Arcadia/PDLC/Tests/ResourceModels/Main.c)
OnModuleDependency(${this} ${MyProjectName}.Engine PRIVATE)
OnModuleDependency(${this} ${MyProjectName}.Testing PRIVATE)
EndProduct(${this})
add_dependencies(${this} ${generator}.Run)
target_include_directories(${this} PRIVATE "${generatedRoot}")
