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

set(this ${MyProjectName}.FileSystem.Tests.DirectoryIteratorTests)

# Create test executable.
BeginProduct(${this} test)

OnSourceFile(${this} Main.c)

OnAssetsDirectory(${this} ${CMAKE_CURRENT_SOURCE_DIR}/Assets)

OnModuleDependency(${this} ${MyProjectName}.FileSystem)
OnModuleDependency(${this} ${MyProjectName}.Ring2)

EndProduct(${this})

set_target_properties(${this} PROPERTIES FOLDER "Runtime/FileSystem")

# Adjust working directory.
set(${this}.WorkingDirectory $<TARGET_FILE_DIR:${this}>)

# Copy assets.
CopyProductAssets(${this} "Runtime/FileSystem" ${CMAKE_CURRENT_BINARY_DIR}/Assets)

