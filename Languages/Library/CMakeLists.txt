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

# Create library.
set(this ${MyProjectName}.Languages)

if (NOT TARGET ${this})
  BeginProduct(${this} library)

  SetFolder(${this} "Languages")

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Languages/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Languages/Configure.h.i)

  OnSourceFile(${this} Arcadia/Languages/Include.c)
  OnHeaderFile(${this} Arcadia/Languages/Include.h)
  OnSourceFile(${this} Arcadia/Languages/StringTable.c)
  OnHeaderFile(${this} Arcadia/Languages/StringTable.h)
  OnSourceFile(${this} Arcadia/Languages/EncodingErrorException.c)
  OnHeaderFile(${this} Arcadia/Languages/EncodingErrorException.h)
  OnSourceFile(${this} Arcadia/Languages/LexicalErrorException.c)
  OnHeaderFile(${this} Arcadia/Languages/LexicalErrorException.h)
  OnSourceFile(${this} Arcadia/Languages/SyntacticalErrorException.c)
  OnHeaderFile(${this} Arcadia/Languages/SyntacticalErrorException.h)
  OnSourceFile(${this} Arcadia/Languages/Scanner.c)
  OnHeaderFile(${this} Arcadia/Languages/Scanner.h)
  OnSourceFile(${this} Arcadia/Languages/Scope.c)
  OnHeaderFile(${this} Arcadia/Languages/Scope.h)
  OnSourceFile(${this} Arcadia/Languages/Diagnostic.c)
  OnHeaderFile(${this} Arcadia/Languages/Diagnostic.h)
  OnSourceFile(${this} Arcadia/Languages/DiagnosticsOld.c)
  OnHeaderFile(${this} Arcadia/Languages/DiagnosticsOld.h)
  OnSourceFile(${this} Arcadia/Languages/Diagnostics.c)
  OnHeaderFile(${this} Arcadia/Languages/Diagnostics.h)
  OnSourceFile(${this} Arcadia/Languages/DiagnosticType.c)
  OnHeaderFile(${this} Arcadia/Languages/DiagnosticType.h)
  OnSourceFile(${this} Arcadia/Languages/Parser.c)
  OnHeaderFile(${this} Arcadia/Languages/Parser.h)
  OnSourceFile(${this} Arcadia/Languages/InputFileManager.c)
  OnHeaderFile(${this} Arcadia/Languages/InputFileManager.h)
  OnSourceFile(${this} Arcadia/Languages/InputFile.c)
  OnHeaderFile(${this} Arcadia/Languages/InputFile.h)
  OnSourceFile(${this} Arcadia/Languages/LineMap.c)
  OnHeaderFile(${this} Arcadia/Languages/LineMap.h)
  OnSourceFile(${this} Arcadia/Languages/mangleName.c)
  OnHeaderFile(${this} Arcadia/Languages/mangleName.h)

  OnSourceFile(${this} Arcadia/Languages/InputFileManager.c)
  OnHeaderFile(${this} Arcadia/Languages/InputFileManager.h)
  OnSourceFile(${this} Arcadia/Languages/InputFile.c)
  OnHeaderFile(${this} Arcadia/Languages/InputFile.h)
  OnSourceFile(${this} Arcadia/Languages/InputFileManager/PhysicalInputFile.c)
  OnHeaderFile(${this} Arcadia/Languages/InputFileManager/PhysicalInputFile.h)

  OnModuleDependency(${this} ${MyProjectName}.Collections)
  OnModuleDependency(${this} ${MyProjectName}.Logging)
  OnModuleDependency(${this} ${MyProjectName}.FileSystem)
  OnModuleDependency(${this} ${MyProjectName}.Ring2)
  OnModuleDependency(${this} ${MyProjectName}.Ring1)

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux})
    list(APPEND ${this}.Libraries m)
  endif()

  EndProduct(${this})

endif()
