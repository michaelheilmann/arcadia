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
project(ArcadiaDiscoveryFixture LANGUAGES C)
enable_testing()
include("${DISCOVERY_MODULE}")
# Standalone harness: intentionally do not load the monorepo/dependency macros.
add_executable(Fixture Main.c)
# Exercise configuration-expanded .c.g LANGUAGE properties in both VS and Ninja.
get_filename_component(cmakeDirectory "${DISCOVERY_MODULE}" DIRECTORY)
get_filename_component(repositoryDirectory "${cmakeDirectory}" DIRECTORY)
include("${repositoryDirectory}/PDLC/Tests/GeneratedSources.cmake")
set(generated "${CMAKE_CURRENT_BINARY_DIR}/Generated/$<CONFIG>/Value.c.g")
file(GENERATE OUTPUT "${generated}" CONTENT "int\nfixtureGeneratedValue\n  (\n    void\n  )\n{\n  return 42;\n}\n")
Arcadia_markGeneratedTestSources("${generated}")
target_sources(Fixture PRIVATE "${generated}")
set(Fixture.WorkingDirectory "${CMAKE_CURRENT_BINARY_DIR}")
set(Fixture.TestArguments "missing font.ttf")
set(Fixture.TestEnvironment "ARCADIA_DISCOVERY_TEST=environment value")
set(Fixture.TestProperties SKIP_RETURN_CODE 77)
set(Fixture.TestLabels integration requires-font)
set(Fixture.TestTimeout 45)
set(Fixture.TestIsolated TRUE)
set(Fixture.TestFixtureDirectory "${CMAKE_CURRENT_SOURCE_DIR}/Assets")
set(Fixture.TestFixtureDestination Assets)
set(Fixture.TestParallelSafe TRUE)
Arcadia_registerTestCases(Fixture)
Arcadia_finalizeTestDiscovery()
