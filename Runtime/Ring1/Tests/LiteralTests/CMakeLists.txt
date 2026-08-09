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

set(MyTestName ${MyProjectName}.Ring1.Tests.LiteralTests)

# Create test executable.
add_executable(${MyTestName})
BeginProduct(${MyTestName})

add_test(NAME ${MyTestName} COMMAND ${MyTestName} WORKING_DIRECTORY $<TARGET_FILE_DIR:${MyTestName}>)

list(APPEND ${MyTestName}.SourceFiles ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia.Ring1.Tests.LiteralTests/Main.c)

target_link_libraries(${MyTestName} ${MyProjectName}.Ring1 ${MyProjectName}.Arms)

set_target_properties(${MyTestName} PROPERTIES FOLDER "Arcadia")

EndProduct(${MyTestName} executable)
