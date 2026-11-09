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

set(this ${MyProjectName}.Network.Tests.Core)
BeginProduct(${this} test)
SetFolder(${this} "Network")
OnSourceFile(${this} Arcadia/Network/Tests/Main.c)
OnModuleDependency(${this} ${MyProjectName}.Network PRIVATE)
OnModuleDependency(${this} ${MyProjectName}.Testing PRIVATE)
if(UNIX)
  find_package(Threads REQUIRED)
  list(APPEND ${this}.PrivateLibraries Threads::Threads)
endif()
EndProduct(${this})
