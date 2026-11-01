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
if(MODE STREQUAL "Duplicate")
  execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "same\nsame")
elseif(MODE STREQUAL "Empty")
  execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "")
elseif(MODE STREQUAL "Error")
  message(FATAL_ERROR "Deliberate discovery failure")
elseif(MODE STREQUAL "Diagnostic")
  message("Unexpected discovery diagnostic")
  execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "case")
else()
  if(NOT "$ENV{ARCADIA_DISCOVERY_TEST}" STREQUAL "environment value")
    message(FATAL_ERROR "Discovery environment was not preserved")
  endif()
  execute_process(COMMAND "${CMAKE_COMMAND}" -E echo "First\ntwo words\npunctuation[\";\\]")
endif()
