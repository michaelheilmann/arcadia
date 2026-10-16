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
string(RANDOM LENGTH 12 ALPHABET 0123456789abcdef unique)
set(work "${BUILD_ROOT}/prerequisites-${unique}")
file(MAKE_DIRECTORY "${work}")
file(WRITE "${work}/corrupt.ttf" "not a font\n")
function(invoke expected)
  execute_process(COMMAND ${ARGN} WORKING_DIRECTORY "${work}" TIMEOUT 30
    RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
  if(NOT "${status}" STREQUAL "${expected}")
    message(FATAL_ERROR "Expected ${expected}, got ${status}: ${ARGN}\n${output}\n${error}")
  endif()
  set(output "${output}" PARENT_SCOPE)
  set(error "${error}" PARENT_SCOPE)
endfunction()

foreach(kind IN ITEMS BITMAP ATLAS)
  if(kind STREQUAL "BITMAP")
    set(case CreateBitmapFontTests)
  else()
    set(case CreateAtlasBitmapFontTests)
  endif()
  invoke(0 "${${kind}}" "${work}/missing.ttf" --list)
  string(REPLACE "\r\n" "\n" output "${output}")
  if(NOT output STREQUAL "${case}\n" OR NOT error STREQUAL "")
    message(FATAL_ERROR "Discovery performed font setup: ${output}\n${error}")
  endif()
  invoke(77 "${${kind}}" "${work}/missing.ttf")
  if(NOT output MATCHES "\\[  SKIPPED \\]" OR output MATCHES "\\[       OK \\]")
    message(FATAL_ERROR "Missing font was reported as exercised functionality: ${output}")
  endif()
  invoke(77 "${${kind}}" "${work}/missing.ttf" --case "${case}")
  invoke(1 "${${kind}}" "${work}/missing.ttf" --case MissingCase)
  invoke(1 "${${kind}}" "${work}/missing.ttf" --case)
  # Existing but invalid data is a failure, never an unavailable-prerequisite skip.
  invoke(1 "${${kind}}" "${work}/corrupt.ttf" --case "${case}")
  if(EXISTS "${FONT}")
    invoke(0 "${${kind}}" "${FONT}" --case "${case}")
  endif()
endforeach()

# Invoke real FontIO executables under CTest from an unrelated cwd. Preserve a
# deliberate failure and verify JUnit differentiates it from an actual skip.
file(WRITE "${work}/CTestTestfile.cmake" "")
foreach(state IN ITEMS Passed Skipped Failed)
  if(state STREQUAL "Passed")
    if(NOT EXISTS "${FONT}")
      continue()
    endif()
    set(path "${FONT}")
  elseif(state STREQUAL "Skipped")
    set(path "${work}/missing.ttf")
  else()
    set(path "${work}/corrupt.ttf")
  endif()
  file(APPEND "${work}/CTestTestfile.cmake"
    "add_test(Font${state} [==[${BITMAP}]==] [==[${path}]==] --case CreateBitmapFontTests)\n"
    "set_tests_properties(Font${state} PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 30)\n")
endforeach()
invoke(8 "${CTEST_COMMAND}" --test-dir "${work}" --output-junit "${work}/results.xml" --output-on-failure)
file(READ "${work}/results.xml" xml)
foreach(state IN ITEMS Passed Skipped Failed)
  if(state STREQUAL "Passed" AND NOT EXISTS "${FONT}")
    continue()
  endif()
  string(FIND "${xml}" "<testcase name=\"Font${state}\"" start)
  if(start LESS 0)
    message(FATAL_ERROR "Missing JUnit case Font${state}: ${xml}")
  endif()
  string(SUBSTRING "${xml}" ${start} -1 entry)
  string(FIND "${entry}" "</testcase>" end)
  string(SUBSTRING "${entry}" 0 ${end} entry)
  if((state STREQUAL "Skipped" AND NOT entry MATCHES "<skipped")
      OR (state STREQUAL "Failed" AND NOT entry MATCHES "<failure")
      OR (state STREQUAL "Passed" AND entry MATCHES "<(failure|skipped)"))
    message(FATAL_ERROR "Incorrect JUnit state for Font${state}: ${entry}")
  endif()
endforeach()
# Only remove this harness invocation's known disposable directory after success.
file(REMOVE_RECURSE "${work}")
if(EXISTS "${FONT}")
  message(STATUS "Font discovery, missing/corrupt/valid inputs and CTest/JUnit states verified")
else()
  message(STATUS "Font discovery, missing/corrupt inputs and JUnit skip/failure verified; valid-font checks not run (fixture unavailable)")
endif()
