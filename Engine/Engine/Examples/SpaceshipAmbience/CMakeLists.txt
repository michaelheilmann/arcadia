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

set(this ${MyProjectName}.Engine.Examples.SpaceshipAmbience)

if (NOT TARGET ${this})
  BeginProduct(${this} executable)
  
  SetFolder(${this} "Engine/Examples")

  set(${this}.Enabled FALSE)

  # This demo requires at least one audials backend.
  if (${${MyProjectName}.Engine.Audials.Implementation.OpenAL.Enabled})
    set(${this}.Enabled TRUE)
  endif()
  
  set(${this}.Windows.ResourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Resources.rc)

  OnSourceFile(${this} Arcadia/Engine/Examples/SpaceshipAmbience.c)

  OnModuleDependency(${this} ${MyProjectName}.Engine.Audials.Implementation PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.Media PRIVATE)

  EndProduct(${this})
endif()
