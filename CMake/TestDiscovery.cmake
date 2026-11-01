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
include("${CMAKE_CURRENT_LIST_DIR}/TestPolicy.cmake")
set(ARCADIA_TEST_DISCOVERY_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

function(Arcadia_quoteCMake output value)
  set(equals "=")
  while("${value}" MATCHES "]${equals}]")
    string(APPEND equals "=")
  endwhile()
  set(${output} "[${equals}[${value}]${equals}]" PARENT_SCOPE)
endfunction()

# Serialize configuration-specific metadata here; execute --list only when CTest
# loads it, never during configure. The executable is the sole case-name source.
# Metadata settings must be supplied before EndProduct. Keep explicit fallback
# suites (notably the runner self-tests) separate from discovered consumer cases.
function(Arcadia_registerTestCases target)
  Arcadia_prepareTestPolicy(${target})
  set(prefix "${CMAKE_CURRENT_BINARY_DIR}/${target}.discovery")
  set(isolationRoot "")
  if(${target}.TestIsolated)
    set(isolationRoot "${CMAKE_CURRENT_BINARY_DIR}/Testing/Isolated/$<IF:$<BOOL:$<CONFIG>>,$<CONFIG>,Default>")
  endif()
  set(content "")
  foreach(pair IN ITEMS
      "arcadiaSuite|${target}"
      # CTest's include interpreter need not define CMAKE_COMMAND itself.
      "arcadiaCMakeCommand|${CMAKE_COMMAND}"
      "arcadiaExecutable|$<TARGET_FILE:${target}>"
      "arcadiaIsolationRoot|${isolationRoot}"
      "arcadiaFixtureDirectory|${${target}.TestFixtureDirectory}"
      "arcadiaFixtureDestination|${${target}.TestFixtureDestination}"
      "arcadiaParallelSafe|${${target}.TestParallelSafe}"
      "arcadiaTimeout|${${target}.TestTimeout}"
      "arcadiaWorkingDirectory|${${target}.WorkingDirectory}")
    string(FIND "${pair}" "|" split)
    string(SUBSTRING "${pair}" 0 ${split} key)
    math(EXPR split "${split} + 1")
    string(SUBSTRING "${pair}" ${split} -1 value)
    Arcadia_quoteCMake(quoted "${value}")
    string(APPEND content "set(${key} ${quoted})\n")
  endforeach()
  foreach(setting IN ITEMS Arguments Environment Properties Labels)
    string(APPEND content "set(arcadia${setting})\n")
    foreach(value IN LISTS ${target}.Test${setting})
      Arcadia_quoteCMake(quoted "${value}")
      string(APPEND content "list(APPEND arcadia${setting} ${quoted})\n")
    endforeach()
  endforeach()
  Arcadia_quoteCMake(runtime "${ARCADIA_TEST_DISCOVERY_DIRECTORY}/TestDiscoveryRuntime.cmake")
  string(APPEND content "if(NOT ARCADIA_DISCOVERY_METADATA_ONLY)\n  include(${runtime})\n  Arcadia_discoverCases()\nendif()\n")
  file(GENERATE OUTPUT "${prefix}-$<CONFIG>.cmake" CONTENT "${content}")

  # CTest chooses the requested configuration, not whichever executable was built last.
  if(CMAKE_CONFIGURATION_TYPES)
    set(configuration "\${CTEST_CONFIGURATION_TYPE}")
  else()
    set(configuration "${CMAKE_BUILD_TYPE}")
  endif()
  file(WRITE "${prefix}.cmake"
    "if(EXISTS \"${prefix}-${configuration}.cmake\")\n"
    "  include(\"${prefix}-${configuration}.cmake\")\n"
    "else()\n"
    "  add_test(${target}.CONFIGURATION_REQUIRED [==[${CMAKE_COMMAND}]==] -E false)\n"
    "  set_tests_properties(${target}.CONFIGURATION_REQUIRED PROPERTIES RUN_SERIAL TRUE)\n"
    "endif()\n")
  set_property(DIRECTORY APPEND PROPERTY TEST_INCLUDE_FILES "${prefix}.cmake")
  set_property(GLOBAL APPEND PROPERTY ARCADIA_DISCOVERY_METADATA "${prefix}-$<CONFIG>.cmake")
endfunction()

# Only product macros call this for ordinary suite/script registrations. Keep
# command/property handling centralized instead of add_test calls in modules.
function(Arcadia_registerSuiteTest target)
  Arcadia_prepareTestPolicy(${target})
  if(${target}.TestScript)
    add_test(NAME ${target} COMMAND "${CMAKE_COMMAND}" ${${target}.TestArguments}
      -P "${${target}.TestScript}")
  else()
    add_test(NAME ${target} COMMAND ${target} ${${target}.TestArguments})
  endif()
  set_tests_properties(${target} PROPERTIES WORKING_DIRECTORY "${${target}.WorkingDirectory}"
    SKIP_RETURN_CODE 77 LABELS "${${target}.TestLabels}" TIMEOUT "${${target}.TestTimeout}"
    ${${target}.TestProperties} RUN_SERIAL TRUE)
  if(${target}.TestEnvironment)
    set_tests_properties(${target} PROPERTIES ENVIRONMENT "${${target}.TestEnvironment}")
  endif()
endfunction()

function(Arcadia_finalizeTestDiscovery)
  get_property(metadata GLOBAL PROPERTY ARCADIA_DISCOVERY_METADATA)
  set(content "set(arcadiaMetadata)\n")
  foreach(path IN LISTS metadata)
    Arcadia_quoteCMake(quoted "${path}")
    string(APPEND content "list(APPEND arcadiaMetadata ${quoted})\n")
  endforeach()
  set(manifest "${CMAKE_BINARY_DIR}/Testing/ArcadiaDiscovery-$<CONFIG>.cmake")
  file(GENERATE OUTPUT "${manifest}" CONTENT "${content}")
  add_test(NAME Arcadia.Testing.DiscoveryConsistency
    COMMAND "${CMAKE_COMMAND}" "-DMANIFEST=${manifest}" "-DBUILD_DIRECTORY=${CMAKE_BINARY_DIR}"
      "-DCONFIGURATION=$<CONFIG>" "-DCTEST_COMMAND=${CMAKE_CTEST_COMMAND}"
      -P "${ARCADIA_TEST_DISCOVERY_DIRECTORY}/VerifyTestDiscovery.cmake")
  set_tests_properties(Arcadia.Testing.DiscoveryConsistency PROPERTIES RUN_SERIAL TRUE
    LABELS "infrastructure;integration;slow" TIMEOUT 300)
endfunction()
