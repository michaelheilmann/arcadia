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

set(this ${MyProjectName}.FontIO)

if (NOT TARGET ${this})
  BeginProduct(${this} library)

  SetFolder(${this} "FontIO")

  # Always use freetype under Linux.
  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux})
    find_package(Freetype REQUIRED)
    if (NOT TARGET Freetype::Freetype)
      message(FATAL_ERROR "freetype not found")
    endif()
    list(APPEND ${this}.PrivateLibraries Freetype::Freetype)
  endif()

  # Always use freetype under Windows.
  # The freetype library was built with support for PNG-compressed glyph bitmaps
  # and gzip compressed font files and requires zlib and libpng.
  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Windows})
    # zlib.
    if (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X64})
      set(${this}.zlib.Libraries ${MyDependenciesDirectory}/x64/$<LOWER_CASE:$<CONFIG>>/zlib/lib/$<IF:$<STREQUAL:$<LOWER_CASE:$<CONFIG>>,debug>,libzsd.lib,libzs.lib>)
      set(${this}.zlib.HeaderFilesDirectory ${MyDependenciesDirectory}/x64/$<LOWER_CASE:$<CONFIG>>/zlib/include)
    elseif (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X86})
      set(${this}.zlib.Libraries ${MyDependenciesDirectory}/x86/$<LOWER_CASE:$<CONFIG>>/zlib/lib/$<IF:$<STREQUAL:$<LOWER_CASE:$<CONFIG>>,debug>,libzsd.lib,libzs.lib>)
      set(${this}.zlib.HeaderFilesDirectory ${MyDependenciesDirectory}/x86/$<LOWER_CASE:$<CONFIG>>/zlib/include)
    else()
      message(FATAL_ERROR " - unsupported instruction set architecture")
    endif()
    list(APPEND ${this}.PrivateLibraries ${${this}.zlib.Libraries})
    list(APPEND ${this}.IncludeDirectories ${${this}.zlib.HeaderFilesDirectory})

    # libpng.
    if (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X64})
      set(${this}.libPNG.Libraries ${MyDependenciesDirectory}/x64/$<LOWER_CASE:$<CONFIG>>/libpng/lib/$<IF:$<STREQUAL:$<LOWER_CASE:$<CONFIG>>,debug>,libpng18_staticd.lib,libpng18_static.lib>)
      set(${this}.libPNG.HeaderFilesDirectory ${MyDependenciesDirectory}/x64/$<LOWER_CASE:$<CONFIG>>/libpng/include)
    elseif (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X86})
      set(${this}.libPNG.Libraries ${MyDependenciesDirectory}/x86/$<LOWER_CASE:$<CONFIG>>/libpng/lib/$<IF:$<STREQUAL:$<LOWER_CASE:$<CONFIG>>,debug>,libpng18_staticd.lib,libpng18_static.lib>)
      set(${this}.libPNG.HeaderFilesDirectory ${MyDependenciesDirectory}/x86/$<LOWER_CASE:$<CONFIG>>/libpng/include)
    else()
      message(FATAL_ERROR " - unsupported instruction set architecture")
    endif()
    list(APPEND ${this}.PrivateLibraries ${${this}.libPNG.Libraries})
    list(APPEND ${this}.IncludeDirectories ${${this}.libPNG.HeaderFilesDirectory})

    # freetype.
    if (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X64})
      set(${this}.FreeType.Libraries ${MyDependenciesDirectory}/x64/$<LOWER_CASE:$<CONFIG>>/freetype/lib/$<IF:$<STREQUAL:$<LOWER_CASE:$<CONFIG>>,debug>,freetyped.lib,freetype.lib>)
      set(${this}.FreeType.HeaderFilesDirectory ${MyDependenciesDirectory}/x64/$<LOWER_CASE:$<CONFIG>>/freetype/include/freetype2)
    elseif (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X86})
      set(${this}.FreeType.Libraries ${MyDependenciesDirectory}/x86/$<LOWER_CASE:$<CONFIG>>/freetype/lib/$<IF:$<STREQUAL:$<LOWER_CASE:$<CONFIG>>,debug>,freetyped.lib,freetype.lib>)
      set(${this}.FreeType.HeaderFilesDirectory ${MyDependenciesDirectory}/x86/$<LOWER_CASE:$<CONFIG>>/freetype/include/freetype2)
    else()
      message(FATAL_ERROR " - unsupported instruction set architecture")
    endif()
    list(APPEND ${this}.PrivateLibraries ${${this}.FreeType.Libraries})
    list(APPEND ${this}.IncludeDirectories ${${this}.FreeType.HeaderFilesDirectory})
  endif()

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/FontIO/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/FontIO/Configure.h.i)

  OnSourceFile(${this} Arcadia/FontIO/Include.c)
  OnHeaderFile(${this} Arcadia/FontIO/Include.h)

  OnSourceFile(${this} Arcadia/FontIO/BitmapFont.c)
  OnHeaderFile(${this} Arcadia/FontIO/BitmapFont.h)
  OnHeaderFile(${this} Arcadia/FontIO/GlyphInformation.h)
  OnSourceFile(${this} Arcadia/FontIO/AtlasBitmapFont.c)
  OnHeaderFile(${this} Arcadia/FontIO/AtlasBitmapFont.h)
  OnHeaderFile(${this} Arcadia/FontIO/AtlasGlyphInformation.h)

  OnModuleDependency(${this} ${MyProjectName}.Media)

  EndProduct(${this})

endif()
