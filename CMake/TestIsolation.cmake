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

function(Arcadia_prepareCaseDirectory output caseName)
  if(NOT arcadiaIsolationRoot)
    set(${output} "${arcadiaWorkingDirectory}" PARENT_SCOPE)
    return()
  endif()
  string(SHA256 key "${arcadiaSuite}.${caseName}")
  set(root "${arcadiaIsolationRoot}/${key}")
  file(MAKE_DIRECTORY "${root}")
  # Serialize allocation, not execution: concurrent CTest processes may list the
  # same case. Never reuse a previous invocation's potentially mutated inputs.
  file(LOCK "${root}/allocation.lock" GUARD FUNCTION TIMEOUT 30)
  while(TRUE)
    string(RANDOM LENGTH 24 ALPHABET 0123456789abcdef unique)
    set(directory "${root}/run-${unique}")
    if(NOT EXISTS "${directory}")
      break()
    endif()
  endwhile()
  file(MAKE_DIRECTORY "${directory}")
  if(arcadiaFixtureDirectory)
    if(NOT IS_DIRECTORY "${arcadiaFixtureDirectory}")
      message(FATAL_ERROR "Missing pristine fixture directory: ${arcadiaFixtureDirectory}")
    endif()
    string(REPLACE "\\" "/" destination "${arcadiaFixtureDestination}")
    if(IS_ABSOLUTE "${destination}" OR "${destination}" MATCHES "(^|/)\\.\\.(/|$)")
      message(FATAL_ERROR "Fixture destination must remain inside the private case directory")
    endif()
    file(COPY "${arcadiaFixtureDirectory}/" DESTINATION "${directory}/${destination}")
  endif()
  set(${output} "${directory}" PARENT_SCOPE)
endfunction()
