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

set(this ${MyProjectName}.Engine.Examples.CheckerboardTexture)

if (NOT TARGET ${this})
  set(${this}.Enabled FALSE)

  # This demo requires at least one visuals backend.
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

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Engine/Examples/CheckerboardTexture/Configure.h
                              ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Engine/Examples/CheckerboardTexture/Configure.h.i)

  OnSourceFile(${this} Arcadia/Engine/Examples/CheckerboardTexture/Main.c)

  OnSourceFile(${this} Arcadia/Engine/Examples/CheckerboardTexture/Scene.c)
  OnHeaderFile(${this} Arcadia/Engine/Examples/CheckerboardTexture/Scene.h)

  OnSourceFile(${this} Arcadia/Engine/Examples/CheckerboardTexture/Scenes/CheckerboardTextureScene.c)
  OnHeaderFile(${this} Arcadia/Engine/Examples/CheckerboardTexture/Scenes/CheckerboardTextureScene.h)

  OnSourceFile(${this} Arcadia/Engine/Examples/CheckerboardTexture/SceneManager.c)
  OnHeaderFile(${this} Arcadia/Engine/Examples/CheckerboardTexture/SceneManager.h)

  OnSourceFile(${this} Arcadia/Engine/Examples/CheckerboardTexture/AssetUtilities.c)
  OnHeaderFile(${this} Arcadia/Engine/Examples/CheckerboardTexture/AssetUtilities.h)

  OnSourceFile(${this} Arcadia/Engine/Examples/CheckerboardTexture/Application.c)
  OnHeaderFile(${this} Arcadia/Engine/Examples/CheckerboardTexture/Application.h)

  OnAssetsDirectory(${this} ${CMAKE_CURRENT_SOURCE_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets)

  OnModuleDependency(${this} ${MyProjectName}.Engine PRIVATE)
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
