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
include("${ISOLATION_MODULE}")
string(RANDOM LENGTH 24 ALPHABET 0123456789abcdef unique)
set(root "${BUILD_ROOT}/isolation-check-${unique}")
set(arcadiaSuite "Fixture")
set(arcadiaIsolationRoot "${root}/Debug")
set(arcadiaFixtureDirectory "${root}/Inputs")
set(arcadiaFixtureDestination Assets)
file(MAKE_DIRECTORY "${arcadiaFixtureDirectory}/Nested")
file(WRITE "${arcadiaFixtureDirectory}/Nested/Type.pdl" "pristine")
file(WRITE "${arcadiaFixtureDirectory}/Blocked" "file, not directory")
Arcadia_prepareCaseDirectory(first "case[\";\\]")
file(WRITE "${first}/Assets/Nested/Type.pdl" "mutated")
file(WRITE "${first}/Assets/Nested/Generated.h" "stale")
Arcadia_prepareCaseDirectory(second "case[\";\\]")
Arcadia_prepareCaseDirectory(other "Other")
set(arcadiaIsolationRoot "${root}/Release")
Arcadia_prepareCaseDirectory(release "case[\";\\]")
foreach(directory IN ITEMS "${second}" "${other}" "${release}")
  if("${first}" STREQUAL "${directory}")
    message(FATAL_ERROR "Reused case directory")
  endif()
  file(READ "${directory}/Assets/Nested/Type.pdl" contents)
  if(NOT contents STREQUAL "pristine" OR EXISTS "${directory}/Assets/Nested/Generated.h"
      OR IS_DIRECTORY "${directory}/Assets/Blocked")
    message(FATAL_ERROR "Fixture copy was stale or changed directory/file topology")
  endif()
endforeach()
file(READ "${arcadiaFixtureDirectory}/Nested/Type.pdl" contents)
if(NOT contents STREQUAL "pristine")
  message(FATAL_ERROR "Source fixture was modified")
endif()
string(FIND "${release}" "${root}/Release/" start)
if(NOT start EQUAL 0)
  message(FATAL_ERROR "Configurations share outputs")
endif()
set(arcadiaIsolationRoot "")
set(arcadiaWorkingDirectory "${root}/Shared")
Arcadia_prepareCaseDirectory(shared "ReadOnly")
if(NOT shared STREQUAL arcadiaWorkingDirectory OR EXISTS "${shared}")
  message(FATAL_ERROR "Read-only suite unexpectedly staged fixtures")
endif()
# Only this script's newly allocated, known fixture tree is disposable.
file(REMOVE_RECURSE "${root}")
message(STATUS "Case/configuration/invocation isolation and pristine-copy checks passed")
