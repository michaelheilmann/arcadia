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

set(this ${MyProjectName}.Engine.Audials.Implementation)

if (NOT TARGET ${this})
  option(${this}.OpenAL.Enabled "TRUE enables OpenAL support. FALSE disables OpenAL support. Default is FALSE" FALSE)

  BeginProduct(${this} library)

  SetFolder(${this} "Backends")

  if (${this}.OpenAL.Enabled)
    set(${this}.Configuration.OpenAL.Enabled 1)
  else()
    set(${this}.Configuration.OpenAL.Enabled 0)
  endif()

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Windows} AND ${${this}.OpenAL.Enabled})
    # Use OpenAL under Windows if enabled.
    set(${this}.Backend ${${this}.Backend.OpenAL})
    list(APPEND ${this}.PrivateLibraries ${OPENGL_LIBRARIES})
    if (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X64})
      set(${this}.OpenAL.Libraries ${MyDependenciesDirectory}/x64/$<LOWER_CASE:$<CONFIG>>/openal-soft/lib/OpenAL32.lib winmm.lib Avrt.lib)
      set(${this}.OpenAL.HeaderFilesDirectory ${MyDependenciesDirectory}/x64/$<LOWER_CASE:$<CONFIG>>/openal-soft/include)
    elseif (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X86})
      set(${this}.OpenAL.Libraries ${MyDependenciesDirectory}/x86/$<LOWER_CASE:$<CONFIG>>/openal-soft/lib/OpenAL32.lib winmm.lib Avrt.lib)
      set(${this}.OpenAL.HeaderFilesDirectory ${MyDependenciesDirectory}/x86/$<LOWER_CASE:$<CONFIG>>/openal-soft/include)
    else()
      message(FATAL_ERROR " - unsupported instruction set architecture")
    endif()
    message(STATUS " - ${this}.OpenAL.Libraries = ${${this}.OpenAL.Libraries}")
    list(APPEND ${this}.PrivateLibraries ${${this}.OpenAL.Libraries})
    message(STATUS " - ${this}.OpenAL.HeaderFilesDirectory = ${${this}.OpenAL.HeaderFilesDirectory}")
    list(APPEND ${this}.IncludeDirectories ${${this}.OpenAL.HeaderFilesDirectory})
  endif()

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux} AND ${${this}.OpenAL.Enabled})
    # Use OpenAL under Linux if enabled.
    find_package(OpenAL)
    if (NOT OPENAL_FOUND)
      message(FATAL_ERROR " - OpenAL not found")
    else()
      message(STATUS " - OpenAL found - OPENAL_LIBRARY = ${OPENAL_LIBRARY}")
      list(APPEND ${this}.PrivateLibraries ${OPENAL_LIBRARY})
      message(STATUS " - OpenAL found - OPENAL_INCLUDE_DIR = ${OPENAL_INCLUDE_DIR}")
      list(APPEND ${this}.IncludeDirectories ${OPENAL_INCLUDE_DIR})
    endif()
  endif()

  OnSourceFile(${this} Arcadia/Audials/Implementation/Include.c)
  OnHeaderFile(${this} Arcadia/Audials/Implementation/Include.h)

  if (${this}.OpenAL.Enabled)
    OnSourceFile(${this} Arcadia/Audials/Implementation/OpenAL/Backend.c)
    OnHeaderFile(${this} Arcadia/Audials/Implementation/OpenAL/Backend.h)

    OnSourceFile(${this} Arcadia/Audials/Implementation/OpenAL/Resources/SoundSourceResource.c)
    OnHeaderFile(${this} Arcadia/Audials/Implementation/OpenAL/Resources/SoundSourceResource.h)
    OnSourceFile(${this} Arcadia/Audials/Implementation/OpenAL/BackendContext.c)
    OnHeaderFile(${this} Arcadia/Audials/Implementation/OpenAL/BackendContext.h)
  endif()

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Audials/Implementation/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Audials/Implementation/Configure.h.i)

  OnModuleDependency(${this} ${MyProjectName}.Engine)
  
  EndProduct(${this})

endif()
