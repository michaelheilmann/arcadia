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

set(this ${MyProjectName}.Tools.TemplateEngine.Tests.RecursiveIncludes)

# Create test executable.
BeginProduct(${this} test)

SetFolder(${this} "TemplateEngine")

# Configuration files.
OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia.TemplateEngine.Tests.RecursiveIncludes/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia.TemplateEngine.Tests.RecursiveIncludes/Configure.h.i)

# Source and header files.
OnSourceFile(${this} Arcadia.TemplateEngine.Tests.RecursiveIncludes/Main.c)

# Module dependencies.
OnModuleDependency(${this} ${MyProjectName}.TemplateEngine PRIVATE)

EndProduct(${this})
# Copy assets.
add_custom_command(TARGET ${this} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Assets ${${this}.WorkingDirectory})
# Adjust working directory.
set(${this}.WorkingDirectory $<TARGET_FILE_DIR:${this}>)

