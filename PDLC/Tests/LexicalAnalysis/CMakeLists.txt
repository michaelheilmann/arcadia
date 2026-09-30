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

set(MyTestFolder PDLC)
set(this ${MyProjectName}.PDLC.Tests.LexicalAnalysis)

if (NOT TARGET ${this})

  BeginProduct(${this} test)

  SetFolder(${this} ${MyTestFolder})

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/MILC/Tests/LexicalAnalysis/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/MILC/Tests/LexicalAnalysis/Configure.h.i)

  OnSourceFile(${this} Arcadia/MILC/Tests/LexicalAnalysis/Main.c)

  OnModuleDependency(${this} ${MyProjectName}.PDLC PRIVATE)

  EndProduct(${this})

endif()
