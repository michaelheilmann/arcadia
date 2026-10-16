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

set(this ${MyProjectName}.Games.Anima)

if (NOT TARGET ${this})

  BeginProduct(${this} executable)

  SetFolder(${this} "Games")

  OnAssetsDirectory(${this} ${CMAKE_CURRENT_SOURCE_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets)

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Games/Anima/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Games/Anima/Configure.h.i)

  OnSourceFile(${this} Arcadia/Games/Anima/Main.c)
  OnSourceFile(${this} Arcadia/Games/Anima/Application.c)
  OnHeaderFile(${this} Arcadia/Games/Anima/Application.h)
  OnSourceFile(${this} Arcadia/Games/Anima/MenuScene.c)
  OnHeaderFile(${this} Arcadia/Games/Anima/MenuScene.h)

  OnModuleDependency(${this} ${MyProjectName}.Engine PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.Engine.UI PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.ADL PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.DDL PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.FileSystem PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.Engine.Audials.Implementation PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.Engine.Visuals.Implementation PRIVATE)

  EndProduct(${this})

endif()
