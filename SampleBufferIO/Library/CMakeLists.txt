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

set(this ${MyProjectName}.SampleBufferIO)

if (NOT TARGET ${this})
  BeginProduct(${this} library)

  # libFLAC.
  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Windows})
    # Windows.   
    if (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X64})
      set(${this}.libOGG.Libraries ${MyDependenciesDirectory}/x64/$<LOWER_CASE:$<CONFIG>>/ogg/lib/$<IF:$<STREQUAL:$<LOWER_CASE:$<CONFIG>>,debug>,ogg.lib,ogg.lib>)
      set(${this}.libOGG.HeaderFilesDirectory ${MyDependenciesDirectory}/x64/$<LOWER_CASE:$<CONFIG>>/ogg/include)
    elseif (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X86})
      set(${this}.libOGG.Libraries ${MyDependenciesDirectory}/x86/$<LOWER_CASE:$<CONFIG>>/ogg/lib/$<IF:$<STREQUAL:$<LOWER_CASE:$<CONFIG>>,debug>,ogg.lib,ogg.lib>)
      set(${this}.libOGG.HeaderFilesDirectory ${MyDependenciesDirectory}/x86/$<LOWER_CASE:$<CONFIG>>/ogg/include)
    else()
      message(FATAL_ERROR " - unsupported instruction set architecture")
    endif()
    list(APPEND ${this}.PrivateLibraries ${${this}.libOGG.Libraries})
    list(APPEND ${this}.IncludeDirectories ${${this}.libOGG.HeaderFilesDirectory})
    
    if (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X64})
      set(${this}.libFLAC.Libraries ${MyDependenciesDirectory}/x64/$<LOWER_CASE:$<CONFIG>>/flac/lib/$<IF:$<STREQUAL:$<LOWER_CASE:$<CONFIG>>,debug>,FLAC.lib,FLAC.lib>)
      set(${this}.libFLAC.HeaderFilesDirectory ${MyDependenciesDirectory}/x64/$<LOWER_CASE:$<CONFIG>>/flac/include)
    elseif (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X86})
      set(${this}.libFLAC.Libraries ${MyDependenciesDirectory}/x86/$<LOWER_CASE:$<CONFIG>>/flac/lib/$<IF:$<STREQUAL:$<LOWER_CASE:$<CONFIG>>,debug>,FLAC.lib,FLAC.lib>)
      set(${this}.libFLAC.HeaderFilesDirectory ${MyDependenciesDirectory}/x86/$<LOWER_CASE:$<CONFIG>>/flac/include)
    else()
      message(FATAL_ERROR " - unsupported instruction set architecture")
    endif()
    list(APPEND ${this}.PrivateLibraries ${${this}.libFLAC.Libraries})
    list(APPEND ${this}.IncludeDirectories ${${this}.libFLAC.HeaderFilesDirectory})
  else()
    # Linux.
    find_package(FLAC REQUIRED)
    if (NOT TARGET FLAC::FLAC)
      message(FATAL_ERROR "libFLAC not found")
    endif()
    list(APPEND ${this}.PrivateLibraries FLAC::FLAC)
  endif()

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/SampleBufferIO/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/SampleBufferIO/Configure.h.i)

  OnSourceFile(${this} Arcadia/SampleBufferIO/Include.c)
  OnHeaderFile(${this} Arcadia/SampleBufferIO/Include.h)

  OnModuleDependency(${this} ${MyProjectName}.Media)
  OnModuleDependency(${this} ${MyProjectName}.ADL)

  EndProduct(${this})

  set_target_properties(${this} PROPERTIES FOLDER "SampleBufferIO")

endif()
