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
project(ArcadiaAssetCopyFixture LANGUAGES NONE)
set(MyProjectName Arcadia)
include("${REPOSITORY_SOURCE_DIR}/CMake/all.cmake")

# Private build-tree inputs keep source fixtures immutable during incremental
# mutation checks. Destination directories must not exist before the first build.
set(inputs "${CMAKE_CURRENT_BINARY_DIR}/Inputs")
file(WRITE "${inputs}/Directory1/first.txt" "first\n")
file(WRITE "${inputs}/Directory2/nested/second.txt" "second\n")
add_library(AssetOwner INTERFACE)
set_property(TARGET AssetOwner PROPERTY ARCADIA_ASSETS_DIRECTORIES "${inputs};${inputs}")
set_property(TARGET AssetOwner PROPERTY ARCADIA_ASSETS_FILES "Directory1/first.txt;Directory2/nested/second.txt")
add_library(AssetConsumer INTERFACE)
target_link_libraries(AssetConsumer INTERFACE AssetOwner)
add_custom_target(Fixture)
set_property(TARGET Fixture PROPERTY LINK_LIBRARIES AssetConsumer)
CopyProductAssets(Fixture "" "${CMAKE_CURRENT_BINARY_DIR}/Assets")

# Script tests use the same product macros, without a generated C launcher.
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Work")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/VerifyScript.cmake" [=[
if(NOT EXPECTED STREQUAL "argument value" OR NOT "$ENV{SCRIPT_TEST_ENV}" STREQUAL "environment value")
  message(FATAL_ERROR "Script-test arguments/environment were not preserved")
endif()
if(NOT EXISTS "${INPUT}")
  message(FATAL_ERROR "Script-test build dependency did not prepare assets")
endif()
file(WRITE "observed.txt" "script ran\n")
]=])
BeginProduct(Fixture.Script test)
SetFolder(Fixture.Script "Tests")
set(Fixture.Script.TestScript "${CMAKE_CURRENT_BINARY_DIR}/VerifyScript.cmake")
set(Fixture.Script.TestArguments "-DEXPECTED=argument value" "-DINPUT=${CMAKE_CURRENT_BINARY_DIR}/Assets/Directory1/first.txt")
set(Fixture.Script.TestDependencies Fixture.CopyAssets)
set(Fixture.Script.WorkingDirectory "${CMAKE_CURRENT_BINARY_DIR}/Work")
set(Fixture.Script.TestEnvironment "SCRIPT_TEST_ENV=environment value")
set(Fixture.Script.TestLabels infrastructure integration)
set(Fixture.Script.TestTimeout 20)
EndProduct(Fixture.Script)
