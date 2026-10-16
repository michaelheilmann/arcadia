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

include_guard(GLOBAL)
include("${CMAKE_CURRENT_LIST_DIR}/TestIsolation.cmake")

function(Arcadia_readCaseNames)
  execute_process(COMMAND "${arcadiaCMakeCommand}" -E env ${arcadiaEnvironment} --
      "${arcadiaExecutable}" ${arcadiaArguments} --list
    WORKING_DIRECTORY "${arcadiaWorkingDirectory}"
    RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error TIMEOUT 30)
  if(NOT "${status}" STREQUAL "0" OR NOT "${error}" STREQUAL "")
    message(FATAL_ERROR "Case discovery failed for ${arcadiaSuite}: ${status}\n${error}")
  endif()
  string(REPLACE "\r\n" "\n" output "${output}")
  if("${output}" STREQUAL "" OR NOT "${output}" MATCHES "\n$")
    message(FATAL_ERROR "Invalid --list output from ${arcadiaSuite}: expected nonempty newline-terminated names")
  endif()
  set(count 0)
  while(NOT "${output}" STREQUAL "")
    string(FIND "${output}" "\n" end)
    string(SUBSTRING "${output}" 0 ${end} name)
    math(EXPR next "${end} + 1")
    string(SUBSTRING "${output}" ${next} -1 output)
    if("${name}" STREQUAL "" OR "${name}" MATCHES "\r")
      message(FATAL_ERROR "Invalid case name from ${arcadiaSuite}")
    endif()
    if(count GREATER 0)
      math(EXPR last "${count} - 1")
      foreach(i RANGE 0 ${last})
        if("${name}" STREQUAL "${name_${i}}")
          message(FATAL_ERROR "Duplicate case name from ${arcadiaSuite}: ${name}")
        endif()
      endforeach()
    endif()
    set(name_${count} "${name}")
    set(arcadiaName_${count} "${name}" PARENT_SCOPE)
    math(EXPR count "${count} + 1")
  endwhile()
  set(arcadiaCount ${count} PARENT_SCOPE)
endfunction()

function(Arcadia_discoverCases)
  # Standalone infrastructure harnesses may supply metadata directly.
  if(NOT arcadiaTimeout)
    set(arcadiaTimeout 60)
  endif()
  if(NOT arcadiaLabels)
    set(arcadiaLabels unit)
  endif()
  if(NOT EXISTS "${arcadiaExecutable}")
    # Visible failure, not a silently omitted suite or a stale case cache.
    add_test("${arcadiaSuite}.NOT_BUILT" "${arcadiaCMakeCommand}" -E false)
    set_tests_properties("${arcadiaSuite}.NOT_BUILT" PROPERTIES RUN_SERIAL TRUE)
    return()
  endif()
  Arcadia_readCaseNames()
  math(EXPR last "${arcadiaCount} - 1")
  foreach(i RANGE 0 ${last})
    set(name "${arcadiaSuite}.${arcadiaName_${i}}")
    string(SHA256 key "${name}")
    get_property(registered GLOBAL PROPERTY "ARCADIA_REGISTERED_CASE_${key}")
    if(registered)
      message(FATAL_ERROR "CTest case name collision: ${name}")
    endif()
    set_property(GLOBAL PROPERTY "ARCADIA_REGISTERED_CASE_${key}" TRUE)
    Arcadia_prepareCaseDirectory(workingDirectory "${arcadiaName_${i}}")
    # CTest include files use add_test's positional signature.
    add_test("${name}" "${arcadiaExecutable}" ${arcadiaArguments} --case "${arcadiaName_${i}}")
    set_tests_properties("${name}" PROPERTIES
      WORKING_DIRECTORY "${workingDirectory}" SKIP_RETURN_CODE 77
      LABELS "${arcadiaLabels}" TIMEOUT "${arcadiaTimeout}" ${arcadiaProperties})
    set(environment "${arcadiaEnvironment}")
    if(arcadiaIsolationRoot)
      list(APPEND environment "TMP=${workingDirectory}" "TEMP=${workingDirectory}" "TMPDIR=${workingDirectory}")
    endif()
    if(environment)
      set_tests_properties("${name}" PROPERTIES ENVIRONMENT "${environment}")
    endif()
    # Opt-in only after the suite's external resources and output paths are audited.
    if(arcadiaParallelSafe)
      set_tests_properties("${name}" PROPERTIES RUN_SERIAL FALSE)
    else()
      set_tests_properties("${name}" PROPERTIES RUN_SERIAL TRUE)
    endif()
  endforeach()
endfunction()
