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
if("${CONFIGURATION}" STREQUAL "")
  set(CONFIGURATION Debug)
endif()
function(run)
  execute_process(COMMAND ${ARGV} RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
  if(NOT "${status}" STREQUAL "0")
    message(FATAL_ERROR "${ARGV}\n${output}\n${error}")
  endif()
  set(output "${output}" PARENT_SCOPE)
endfunction()

# A fresh, uniquely scoped external tree proves configure never runs the executable.
string(RANDOM LENGTH 12 ALPHABET 0123456789abcdef unique)
set(build "${BUILD_ROOT}/discovery-${unique}")
set(platformArguments)
if(NOT "${PLATFORM}" STREQUAL "")
  list(APPEND platformArguments -A "${PLATFORM}")
endif()
run("${CMAKE_COMMAND}" -S "${CMAKE_CURRENT_LIST_DIR}/Fixture" -B "${build}"
  -G "${GENERATOR}" ${platformArguments} "-DCMAKE_BUILD_TYPE=${CONFIGURATION}"
  "-DDISCOVERY_MODULE=${MODULE}")
run("${CTEST_COMMAND}" --test-dir "${build}" -C "${CONFIGURATION}" -N)
if(NOT "${output}" MATCHES "Fixture.NOT_BUILT")
  message(FATAL_ERROR "Unbuilt fixture was not reported")
endif()
if(MULTI_CONFIG)
  run("${CTEST_COMMAND}" --test-dir "${build}" -N)
  if(NOT "${output}" MATCHES "Fixture.CONFIGURATION_REQUIRED")
    message(FATAL_ERROR "Missing configuration silently selected another binary")
  endif()
endif()
set(configurations "${CONFIGURATION}")
if(MULTI_CONFIG)
  if(CONFIGURATION STREQUAL "Release")
    list(APPEND configurations Debug)
  else()
    list(APPEND configurations Release)
  endif()
endif()
foreach(configuration IN LISTS configurations)
  run("${CMAKE_COMMAND}" --build "${build}" --config "${configuration}")
  run("${CTEST_COMMAND}" --test-dir "${build}" -C "${configuration}" --output-on-failure)
  if(NOT "${output}" MATCHES "100% tests passed" OR NOT "${output}" MATCHES "Skipped")
    message(FATAL_ERROR "Case execution/skip semantics were not preserved: ${output}")
  endif()
  # The writer fixture is isolated by case/configuration/invocation. A second
  # parallel, randomized run must not see the previous output.txt files.
  run("${CTEST_COMMAND}" --test-dir "${build}" -C "${configuration}" -j 4 --schedule-random --output-on-failure)
endforeach()
# Exercise the real shared asset helper on a cold tree. Ninja rejects an ordinary
# dependency on a missing destination directory even when the file copy creates
# it; warmed production trees and VS builds can hide that invalid graph.
get_filename_component(cmakeDirectory "${MODULE}" DIRECTORY)
get_filename_component(repositoryDirectory "${cmakeDirectory}" DIRECTORY)
set(assets "${build}/asset-copy")
run("${CMAKE_COMMAND}" -S "${CMAKE_CURRENT_LIST_DIR}/AssetFixture" -B "${assets}"
  -G "${GENERATOR}" ${platformArguments} "-DREPOSITORY_SOURCE_DIR=${repositoryDirectory}")
if(EXISTS "${assets}/Assets")
  message(FATAL_ERROR "Asset regression must start without destination directories")
endif()
run("${CMAKE_COMMAND}" --build "${assets}" --config "${CONFIGURATION}" --target Fixture.Script)
run("${CTEST_COMMAND}" --test-dir "${assets}" -C "${CONFIGURATION}" -R "^Fixture.Script$" --output-on-failure)
if(NOT EXISTS "${assets}/Work/observed.txt")
  message(FATAL_ERROR "Script-test product did not preserve working directory/execution")
endif()
file(READ "${assets}/Assets/Directory1/first.txt" first)
file(READ "${assets}/Assets/Directory2/nested/second.txt" second)
if(NOT first STREQUAL "first\n" OR NOT second STREQUAL "second\n")
  message(FATAL_ERROR "Cold asset copy lost inherited/nested inputs")
endif()
# Only delete outputs in this invocation's disposable tree, then prove rebuild
# recreates them and detects a changed input without a configure-time mkdir fix.
file(REMOVE_RECURSE "${assets}/Assets/Directory1")
file(APPEND "${assets}/Inputs/Directory2/nested/second.txt" "changed\n")
run("${CMAKE_COMMAND}" --build "${assets}" --config "${CONFIGURATION}" --target Fixture.CopyAssets)
file(READ "${assets}/Assets/Directory1/first.txt" first)
file(READ "${assets}/Assets/Directory2/nested/second.txt" second)
if(NOT first STREQUAL "first\n" OR NOT second STREQUAL "second\nchanged\n")
  message(FATAL_ERROR "Asset recreation/incremental source dependency failed")
endif()

# Only this invocation's known disposable fixture tree is removed.
file(REMOVE_RECURSE "${build}")
message(STATUS "Discovery configure/unbuilt/configuration/command/property integration checks passed")
