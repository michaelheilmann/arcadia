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
function(run)
  execute_process(COMMAND ${ARGV} RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
  if(NOT "${status}" STREQUAL "0")
    message(FATAL_ERROR "${ARGV}\n${output}\n${error}")
  endif()
endfunction()
string(RANDOM LENGTH 12 ALPHABET 0123456789abcdef unique)
set(work "${BUILD_ROOT}/template-depfile-${unique}")
set(build "${work}/Build")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/Fixture/" DESTINATION "${work}/Source")
set(platformArguments)
if(PLATFORM)
  list(APPEND platformArguments -A "${PLATFORM}")
endif()
run("${CMAKE_COMMAND}" -S "${work}/Source" -B "${build}"
  -G "${GENERATOR}" ${platformArguments} "-DCLI=${CLI}" "-DREPOSITORY_SOURCE_DIR=${REPOSITORY_SOURCE_DIR}")
run("${CMAKE_COMMAND}" --build "${build}" --config "${CONFIGURATION}" --target Render)
file(READ "${build}/rendered.txt" output)
if(NOT output STREQUAL "leaf-v1\n")
  message(FATAL_ERROR "Unexpected nested template output: ${output}")
endif()
file(READ "${build}/Inputs/main file.t.dependencies" dependencies)
string(REPLACE "\r\n" "\n" dependencies "${dependencies}")
string(REGEX MATCHALL "\n" lines "${dependencies}")
list(LENGTH lines count)
if(NOT count EQUAL 1)
  message(FATAL_ERROR "Depfile must describe one rendered output, not rules for input templates: ${dependencies}")
endif()
foreach(input IN ITEMS "main\\ file.t" "middle\\ file.t" "leaf\\ file.t")
  string(FIND "${dependencies}" "${input}" index)
  if(index LESS 0)
    message(FATAL_ERROR "Missing transitive/escaped dependency ${input}: ${dependencies}")
  endif()
endforeach()
# A second build must accept the loaded depfile. Changing only the deepest input
# must rebuild the final output; listing only direct inputs is not sufficient.
run("${CMAKE_COMMAND}" --build "${build}" --config "${CONFIGURATION}" --target Render)
file(APPEND "${work}/Source/Inputs/leaf file.t" "leaf-v2\n")
run("${CMAKE_COMMAND}" --build "${build}" --config "${CONFIGURATION}" --target Render)
file(READ "${build}/rendered.txt" output)
if(NOT output STREQUAL "leaf-v1\nleaf-v2\n")
  message(FATAL_ERROR "Deep include change did not regenerate output: ${output}")
endif()
file(REMOVE_RECURSE "${work}")
message(STATUS "Nested template depfile, escaped paths, reload and transitive rebuild verified")
