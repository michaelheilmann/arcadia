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

set(this ${MyProjectName}.Engine.Examples.SpaceshipAmbienceStreaming)

if (NOT TARGET ${this})
  set(${this}.Enabled FALSE)

  if (${${MyProjectName}.Engine.Audials.Implementation.OpenAL.Enabled} AND
      (${${MyProjectName}.Engine.Visuals.Implementation.OpenGL4.Enabled} OR
       ${${MyProjectName}.Engine.Visuals.Implementation.Direct3D12.Enabled} OR
       ${${MyProjectName}.Engine.Visuals.Implementation.Vulkan.Enabled}))
    set(${this}.Enabled TRUE)
  endif()

  BeginProduct(${this} executable)

  SetFolder(${this} "Engine/Examples")

  OnSourceFile(${this} Arcadia/Engine/Examples/SpaceshipAmbienceStreaming/Main.c)
  OnSourceFile(${this} Arcadia/Engine/Examples/SpaceshipAmbienceStreaming/Application.c)
  OnHeaderFile(${this} Arcadia/Engine/Examples/SpaceshipAmbienceStreaming/Application.h)

  OnModuleDependency(${this} ${MyProjectName}.Engine PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.Engine.Audials.Implementation PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.Engine.Visuals.Implementation PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.Media PRIVATE)

  EndProduct(${this})
endif()
