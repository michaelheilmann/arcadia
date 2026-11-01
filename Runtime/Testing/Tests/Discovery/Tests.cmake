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
foreach(mode IN ITEMS Valid Missing Duplicate Empty Error Diagnostic Collision)
  execute_process(COMMAND "${CMAKE_COMMAND}" "-DRUNTIME=${RUNTIME}" "-DMODE=${mode}"
      -P "${CMAKE_CURRENT_LIST_DIR}/Check.cmake"
    RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
  if(mode STREQUAL "Valid" OR mode STREQUAL "Missing")
    if(NOT "${status}" STREQUAL "0")
      message(FATAL_ERROR "${mode}: ${output}\n${error}")
    endif()
  else()
    if("${status}" STREQUAL "0")
      message(FATAL_ERROR "${mode} was not rejected")
    endif()
    if(mode STREQUAL "Duplicate")
      set(expected "Duplicate case name")
    elseif(mode STREQUAL "Empty")
      set(expected "Invalid case name")
    elseif(mode STREQUAL "Collision")
      set(expected "CTest case name collision")
    else()
      set(expected "Case discovery failed")
    endif()
    if(NOT "${error}" MATCHES "${expected}")
      message(FATAL_ERROR "${mode} failed for the wrong reason: ${error}")
    endif()
  endif()
endforeach()
message(STATUS "Discovery infrastructure: seven scenarios passed")
