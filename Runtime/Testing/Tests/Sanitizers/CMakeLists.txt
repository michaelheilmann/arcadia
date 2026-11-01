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
project(ArcadiaSanitizers LANGUAGES C)
get_filename_component(REPOSITORY_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE)
set(MyProjectName Arcadia)
option(ARCADIA_ENABLE_SANITIZERS "Instrument the dependency-free runtime test slice" ON)
if(ARCADIA_ENABLE_SANITIZERS)
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux" OR NOT CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    message(FATAL_ERROR "This sanitizer gate requires Linux and GCC/Clang; OFF is for harness wiring validation only")
  endif()
  add_compile_options(-fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined -fno-sanitize-recover=all)
endif()

# Use the actual libraries and maintained test sources, not substitute runtime
# implementations. No media/backend downloads or generated production inputs.
include("${REPOSITORY_SOURCE_DIR}/CMake/all.cmake")
add_subdirectory("${REPOSITORY_SOURCE_DIR}/Runtime/ARMS/Library" ARMS)
add_subdirectory("${REPOSITORY_SOURCE_DIR}/Runtime/Ring1/Library" Ring1)
add_subdirectory("${REPOSITORY_SOURCE_DIR}/Runtime/Testing/Library" Testing)
add_subdirectory("${REPOSITORY_SOURCE_DIR}/Runtime/Ring1/Tests" Ring1Tests)
add_subdirectory("${REPOSITORY_SOURCE_DIR}/Runtime/Testing/Tests" TestingTests)
