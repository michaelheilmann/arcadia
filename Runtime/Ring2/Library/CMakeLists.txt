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
set(this ${MyProjectName}.Ring2)

if (NOT TARGET ${this})
  BeginProduct(${this} library)

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Ring2/Implementation/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Ring2/Implementation/Configure.h.i)

  OnSourceFile(${this} Arcadia/Ring2/CommandLine/CommandLineArgument.c)
  OnHeaderFile(${this} Arcadia/Ring2/CommandLine/CommandLineArgument.h)
  OnSourceFile(${this} Arcadia/Ring2/CommandLine/Include.c)
  OnHeaderFile(${this} Arcadia/Ring2/CommandLine/Include.h)

  OnSourceFile(${this} Arcadia/Ring2/Exceptions/Include.c)
  OnHeaderFile(${this} Arcadia/Ring2/Exceptions/Include.h)
  OnSourceFile(${this} Arcadia/Ring2/Exceptions/Exception.c)
  OnHeaderFile(${this} Arcadia/Ring2/Exceptions/Exception.h)

  OnSourceFile(${this} Arcadia/Ring2/Implementation/ArgumentsValidation.c)
  OnHeaderFile(${this} Arcadia/Ring2/Implementation/ArgumentsValidation.h)

  OnSourceFile(${this} Arcadia/Ring2/Unicode/Include.c)
  OnHeaderFile(${this} Arcadia/Ring2/Unicode/Include.h)
  OnSourceFile(${this} Arcadia/Ring2/Unicode/Encoder.c)
  OnHeaderFile(${this} Arcadia/Ring2/Unicode/Encoder.h)
  OnSourceFile(${this} Arcadia/Ring2/Unicode/UTF8Encoder.c)
  OnHeaderFile(${this} Arcadia/Ring2/Unicode/UTF8Encoder.h)

  OnSourceFile(${this} Arcadia/Ring2/Strings/Include.c)
  OnHeaderFile(${this} Arcadia/Ring2/Strings/Include.h)
  OnSourceFile(${this} Arcadia/Ring2/Strings/StringExtensions.c)
  OnHeaderFile(${this} Arcadia/Ring2/Strings/StringExtensions.h)

  OnSourceFile(${this} Arcadia/Ring2/Time/Include.c)
  OnHeaderFile(${this} Arcadia/Ring2/Time/Include.h)
  OnSourceFile(${this} Arcadia/Ring2/Time/PointInTime.c)
  OnHeaderFile(${this} Arcadia/Ring2/Time/PointInTime.h)
  OnSourceFile(${this} Arcadia/Ring2/Time/TimeSpecification.c)
  OnHeaderFile(${this} Arcadia/Ring2/Time/TimeSpecification.h)

  OnSourceFile(${this} Arcadia/Ring2/Logging/Include.c)
  OnHeaderFile(${this} Arcadia/Ring2/Logging/Include.h)
  OnSourceFile(${this} Arcadia/Ring2/Logging/Log.c)
  OnHeaderFile(${this} Arcadia/Ring2/Logging/Log.h)
  OnSourceFile(${this} Arcadia/Ring2/Logging/LogCategory.c)
  OnHeaderFile(${this} Arcadia/Ring2/Logging/LogCategory.h)

  OnSourceFile(${this} Arcadia/Ring2/Include.c)
  OnHeaderFile(${this} Arcadia/Ring2/Include.h)

  OnModuleDependency(${this} ${MyProjectName}.Ring1)

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux})
    list(APPEND ${this}.Libraries m)
  endif()

  set(${this}.Install TRUE)

  EndProduct(${this})

  set_target_properties(${this} PROPERTIES FOLDER "Runtime/Ring2")

endif()
