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

set(this ${MyProjectName}.Application)

if (NOT TARGET ${this})

  BeginProduct(${this} library)

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Application/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Application/Configure.h.i)

  OnSourceFile(${this} Arcadia/Application/Include.c)
  OnHeaderFile(${this} Arcadia/Application/Include.h)
  OnSourceFile(${this} Arcadia/Application/Application.c)
  OnHeaderFile(${this} Arcadia/Application/Application.h)

  OnModuleDependency(${this} ${MyProjectName}.Engine)
  OnModuleDependency(${this} ${MyProjectName}.PixelBufferIO)
  OnModuleDependency(${this} ${MyProjectName}.Math)
  OnModuleDependency(${this} ${MyProjectName}.Ring2)

  EndProduct(${this})

  set_target_properties(${this} PROPERTIES FOLDER "Engine")

endif()
