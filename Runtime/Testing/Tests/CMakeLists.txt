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

set(this ${MyProjectName}.Testing.Tests.Runner)
BeginProduct(${this} test)
SetFolder(${this} "Runtime/Testing")
OnSourceFile(${this} Arcadia.Testing.Tests.Runner/Main.c)
OnModuleDependency(${this} ${MyProjectName}.Testing PRIVATE)
OnModuleDependency(${this} ${MyProjectName}.Ring1 PRIVATE)
# Keep the runner's own regression orchestration as one independent CTest suite.
set(${this}.TestDiscovery FALSE)
EndProduct(${this})

set(this ${MyProjectName}.Testing.DiscoveryInfrastructure)
BeginProduct(${this} test)
SetFolder(${this} "Runtime/Testing")
set(${this}.TestScript "${CMAKE_CURRENT_SOURCE_DIR}/Discovery/Tests.cmake")
set(${this}.TestArguments "-DRUNTIME=${REPOSITORY_SOURCE_DIR}/CMake/TestDiscoveryRuntime.cmake")
set(${this}.TestLabels infrastructure unit)
set(${this}.TestTimeout 60)
EndProduct(${this})

get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
set(this ${MyProjectName}.Testing.DiscoveryIntegration)
BeginProduct(${this} test)
SetFolder(${this} "Runtime/Testing")
set(${this}.TestScript "${CMAKE_CURRENT_SOURCE_DIR}/Discovery/Integration.cmake")
set(${this}.TestArguments "-DMODULE=${REPOSITORY_SOURCE_DIR}/CMake/TestDiscovery.cmake"
  "-DGENERATOR=${CMAKE_GENERATOR}" "-DPLATFORM=${CMAKE_GENERATOR_PLATFORM}"
  "-DMULTI_CONFIG=${isMultiConfig}" "-DCONFIGURATION=$<CONFIG>"
  "-DCTEST_COMMAND=${CMAKE_CTEST_COMMAND}" "-DBUILD_ROOT=${CMAKE_CURRENT_BINARY_DIR}")
set(${this}.TestLabels infrastructure integration slow)
set(${this}.TestTimeout 180)
EndProduct(${this})

set(this ${MyProjectName}.Testing.FixtureIsolation)
BeginProduct(${this} test)
SetFolder(${this} "Runtime/Testing")
set(${this}.TestScript "${CMAKE_CURRENT_SOURCE_DIR}/Discovery/Isolation.cmake")
set(${this}.TestArguments "-DISOLATION_MODULE=${REPOSITORY_SOURCE_DIR}/CMake/TestIsolation.cmake"
  "-DBUILD_ROOT=${CMAKE_CURRENT_BINARY_DIR}")
set(${this}.TestLabels infrastructure unit)
set(${this}.TestTimeout 60)
EndProduct(${this})

set(this ${MyProjectName}.Testing.TestPolicy)
BeginProduct(${this} test)
SetFolder(${this} "Runtime/Testing")
set(${this}.TestScript "${CMAKE_CURRENT_SOURCE_DIR}/Discovery/Policy.cmake")
set(${this}.TestArguments "-DPOLICY_MODULE=${REPOSITORY_SOURCE_DIR}/CMake/TestPolicy.cmake")
set(${this}.TestLabels infrastructure unit)
set(${this}.TestTimeout 60)
EndProduct(${this})
