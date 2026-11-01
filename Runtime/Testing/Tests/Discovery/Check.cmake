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
include("${RUNTIME}")

# Capture the positional API CTest provides to its TEST_INCLUDE_FILES.
function(add_test)
  get_property(count GLOBAL PROPERTY TEST_COUNT)
  if(NOT count)
    set(count 0)
  endif()
  foreach(i RANGE 0 ${ARGC})
    set_property(GLOBAL PROPERTY "COMMAND_${count}_${i}" "${ARGV${i}}")
  endforeach()
  set_property(GLOBAL PROPERTY "COMMAND_SIZE_${count}" "${ARGC}")
  math(EXPR count "${count} + 1")
  set_property(GLOBAL PROPERTY TEST_COUNT ${count})
endfunction()

function(set_tests_properties name properties)
  string(SHA256 key "${name}")
  math(EXPR last "${ARGC} - 1")
  foreach(i RANGE 2 ${last} 2)
    math(EXPR value "${i} + 1")
    set_property(GLOBAL PROPERTY "PROPERTY_${key}_${ARGV${i}}" "${ARGV${value}}")
  endforeach()
endfunction()

set(arcadiaSuite "Fixture")
set(arcadiaCMakeCommand "${CMAKE_COMMAND}")
set(arcadiaExecutable "${CMAKE_COMMAND}")
set(arcadiaArguments "-DMODE=${MODE}" -P "${CMAKE_CURRENT_LIST_DIR}/Output.cmake")
set(arcadiaWorkingDirectory "${CMAKE_CURRENT_BINARY_DIR}")
set(arcadiaEnvironment "ARCADIA_DISCOVERY_TEST=environment value")
set(arcadiaProperties SKIP_RETURN_CODE 77)
if(MODE STREQUAL "Missing")
  set(arcadiaExecutable "${CMAKE_CURRENT_BINARY_DIR}/not-built-executable")
endif()
Arcadia_discoverCases()
get_property(count GLOBAL PROPERTY TEST_COUNT)
if(MODE STREQUAL "Missing")
  get_property(name GLOBAL PROPERTY COMMAND_0_0)
  if(NOT count EQUAL 1 OR NOT name STREQUAL "Fixture.NOT_BUILT")
    message(FATAL_ERROR "Missing executable was silently omitted")
  endif()
elseif(MODE STREQUAL "Collision")
  Arcadia_discoverCases()
elseif(MODE STREQUAL "Valid")
  if(NOT count EQUAL 3)
    message(FATAL_ERROR "Wrong number of registered cases")
  endif()
  set(expected_0 "First")
  set(expected_1 "two words")
  set(expected_2 "punctuation[\";\\]")
  foreach(i RANGE 0 2)
    get_property(name GLOBAL PROPERTY "COMMAND_${i}_0")
    get_property(size GLOBAL PROPERTY "COMMAND_SIZE_${i}")
    get_property(flag GLOBAL PROPERTY "COMMAND_${i}_5")
    get_property(selected GLOBAL PROPERTY "COMMAND_${i}_6")
    if(NOT "${name}" STREQUAL "Fixture.${expected_${i}}" OR NOT size EQUAL 7
        OR NOT flag STREQUAL "--case" OR NOT "${selected}" STREQUAL "${expected_${i}}")
      message(FATAL_ERROR "Case name/argument was altered: ${name}, ${selected}")
    endif()
    string(SHA256 key "${name}")
    foreach(pair IN ITEMS "RUN_SERIAL|TRUE" "SKIP_RETURN_CODE|77"
        "WORKING_DIRECTORY|${arcadiaWorkingDirectory}" "ENVIRONMENT|${arcadiaEnvironment}")
      string(REPLACE "|" ";" pair "${pair}")
      list(GET pair 0 property)
      list(GET pair 1 expected)
      get_property(actual GLOBAL PROPERTY "PROPERTY_${key}_${property}")
      if(NOT "${actual}" STREQUAL "${expected}")
        message(FATAL_ERROR "Property ${property} was not preserved")
      endif()
    endforeach()
  endforeach()
else()
  message(FATAL_ERROR "Invalid discovery unexpectedly succeeded")
endif()
