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
set(this ${MyProjectName}.Tools.TemplateEngine.Tests.Depfile)
BeginProduct(${this} test)
SetFolder(${this} "TemplateEngine")
set(${this}.TestScript "${CMAKE_CURRENT_SOURCE_DIR}/Integration.cmake")
set(${this}.TestArguments "-DCLI=$<TARGET_FILE:${MyProjectName}.TemplateEngine.CLI>"
  "-DGENERATOR=${CMAKE_GENERATOR}" "-DPLATFORM=${CMAKE_GENERATOR_PLATFORM}"
  "-DCONFIGURATION=$<CONFIG>" "-DREPOSITORY_SOURCE_DIR=${REPOSITORY_SOURCE_DIR}"
  "-DBUILD_ROOT=${CMAKE_CURRENT_BINARY_DIR}")
set(${this}.TestDependencies ${MyProjectName}.TemplateEngine.CLI)
set(${this}.TestLabels infrastructure integration)
set(${this}.TestTimeout 120)
EndProduct(${this})
