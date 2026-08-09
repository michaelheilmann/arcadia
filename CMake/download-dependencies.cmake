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

if (NOT DEFINED MyDependenciesDirectory OR "${MyDependenciesDirectory}" STREQUAL "")
  message(FATAL_ERROR "MyDependenciesDirectory is not defined")
endif()

if (NOT DEFINED MyDependenciesGitHubReleasePrefix OR "${MyDependenciesGitHubReleasePrefix}" STREQUAL "")
  message(FATAL_ERROR "MyDependenciesGitHubReleasePrefix is not defined")
endif()

file(MAKE_DIRECTORY "${MyDependenciesDirectory}")

foreach (architecture x64 x86)
  set(archive "${MyDependenciesDirectory}/dependencies-${architecture}.zip")
  set(url "${MyDependenciesGitHubReleasePrefix}/dependencies-${architecture}.zip")
  set(destination "${MyDependenciesDirectory}/${architecture}")

  message(STATUS "Downloading ${url} to ${archive}")
  file(DOWNLOAD
       "${url}"
       "${archive}"
       SHOW_PROGRESS
       STATUS status)

  list(GET status 0 statusCode)
  list(GET status 1 statusMessage)
  if (NOT statusCode EQUAL 0)
    message(FATAL_ERROR "Downloading ${url} failed: ${statusMessage}")
  endif()

  file(REMOVE_RECURSE "${destination}")
  file(MAKE_DIRECTORY "${destination}")

  message(STATUS "Extracting ${archive} to ${destination}")
  file(ARCHIVE_EXTRACT
       INPUT "${archive}"
       DESTINATION "${destination}")
endforeach()
