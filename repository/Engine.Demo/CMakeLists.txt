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

set(this ${MyProjectName}.Engine.Demo)

if (NOT TARGET ${this})
  #message(STATUS "${this}: ${MyProjectName}.Visuals.Implementation.OpenGL4.Enabled = ${${MyProjectName}.Visuals.Implementation.OpenGL4.Enabled}")
  set(${this}.Enabled FALSE)

  # This demo requires at least one rendering backend.
  if (${${MyProjectName}.Engine.Visuals.Implementation.OpenGL4.Enabled})
    set(${this}.Enabled TRUE)
  endif()
  if (${${MyProjectName}.Engine.Visuals.Implementation.Direct3D12.Enabled})
    set(${this}.Enabled TRUE)
  endif()
  if (${${MyProjectName}.Engine.Visuals.Implementation.Vulkan.Enabled})
    set(${this}.Enabled TRUE)
  endif()

  BeginProduct(${this} executable)

  SetFolder(${this} "Engine/Examples")

  set(${this}.Windows.ResourceFile ${CMAKE_CURRENT_SOURCE_DIR}/Resources.rc)

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Engine/Demo/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Engine/Demo/Configure.h.i)

  OnSourceFile(${this} Arcadia/Engine/Demo/Main.c)

  OnSourceFile(${this} Arcadia/Starship/Viewer3D.c)
  OnHeaderFile(${this} Arcadia/Starship/Viewer3D.h)
  OnSourceFile(${this} Arcadia/Starship/OrientationChangeInputEvent.c)
  OnHeaderFile(${this} Arcadia/Starship/OrientationChangeInputEvent.h)
  OnSourceFile(${this} Arcadia/Starship/PositionChangeInputEvent.c)
  OnHeaderFile(${this} Arcadia/Starship/PositionChangeInputEvent.h)

  OnSourceFile(${this} Arcadia/Engine/Demo/Scene.c)
  OnHeaderFile(${this} Arcadia/Engine/Demo/Scene.h)

  OnSourceFile(${this} Arcadia/Engine/Demo/Scenes/ArcadiaLogoScene.c)
  OnHeaderFile(${this} Arcadia/Engine/Demo/Scenes/ArcadiaLogoScene.h)
  OnSourceFile(${this} Arcadia/Engine/Demo/Scenes/MainScene.c)
  OnHeaderFile(${this} Arcadia/Engine/Demo/Scenes/MainScene.h)
  OnSourceFile(${this} Arcadia/Engine/Demo/Scenes/MainMenuScene.c)
  OnHeaderFile(${this} Arcadia/Engine/Demo/Scenes/MainMenuScene.h)

  OnSourceFile(${this} Arcadia/Engine/Demo/SceneManager.c)
  OnHeaderFile(${this} Arcadia/Engine/Demo/SceneManager.h)

  OnSourceFile(${this} Arcadia/Engine/Demo/AssetUtilities.c)
  OnHeaderFile(${this} Arcadia/Engine/Demo/AssetUtilities.h)

  OnSourceFile(${this} Arcadia/Engine/Demo/Application.c)
  OnHeaderFile(${this} Arcadia/Engine/Demo/Application.h)

  OnAssetsDirectory(${this} ${CMAKE_CURRENT_SOURCE_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets)

  OnModuleDependency(${this} ${MyProjectName}.Engine.UI PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.ADL PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.DDL PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.Engine.Audials.Implementation PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.Engine.Visuals.Implementation PRIVATE)

  EndProduct(${this})

  # Copy assets.
  if (${this}.Enabled)
    CopyProductAssets(${this} "Engine/Examples" ${CMAKE_CURRENT_BINARY_DIR}/Assets)
  endif()

endif()
