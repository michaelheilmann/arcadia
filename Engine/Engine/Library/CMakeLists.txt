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

set(this ${MyProjectName}.Engine)

if (NOT TARGET ${this})

  BeginProduct(${this} library)

  SetFolder(${this} "Engine")

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Engine/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Engine/Configure.h.i)

  OnAssetsDirectory(${this} ${CMAKE_CURRENT_SOURCE_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets)

  OnSourceFile(${this} Arcadia/Engine/Include.c)
  OnHeaderFile(${this} Arcadia/Engine/Include.h)
  
  OnSourceFile(${this} Arcadia/Engine/Application.c)
  OnHeaderFile(${this} Arcadia/Engine/Application.h)
  OnSourceFile(${this} Arcadia/Engine/ApplicationConfiguration.c)
  OnHeaderFile(${this} Arcadia/Engine/ApplicationConfiguration.h)
  OnSourceFile(${this} Arcadia/Engine/ApplicationHelper/Visuals.c)
  OnHeaderFile(${this} Arcadia/Engine/ApplicationHelper/Visuals.h)
  OnSourceFile(${this} Arcadia/Engine/ApplicationHelper/Audials.c)
  OnHeaderFile(${this} Arcadia/Engine/ApplicationHelper/Audials.h)

  OnSourceFile(${this} Arcadia/Engine/Engine.c)
  OnHeaderFile(${this} Arcadia/Engine/Engine.h)
  OnSourceFile(${this} Arcadia/Engine/Event.c)
  OnHeaderFile(${this} Arcadia/Engine/Event.h)

  OnSourceFile(${this} Arcadia/Engine/Backend.c)
  OnHeaderFile(${this} Arcadia/Engine/Backend.h)
  OnSourceFile(${this} Arcadia/Engine/BackendContext.c)
  OnHeaderFile(${this} Arcadia/Engine/BackendContext.h)

  OnSourceFile(${this} Arcadia/Engine/Node.c)
  OnHeaderFile(${this} Arcadia/Engine/Node.h)
  OnSourceFile(${this} Arcadia/Engine/NodeFactory.c)
  OnHeaderFile(${this} Arcadia/Engine/NodeFactory.h)

  # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>

  OnSourceFile(${this} Arcadia/Engine/Audials/Resource.c)
  OnHeaderFile(${this} Arcadia/Engine/Audials/Resource.h)
  OnSourceFile(${this} Arcadia/Engine/Audials/Resources/SoundSourceResource.c)
  OnHeaderFile(${this} Arcadia/Engine/Audials/Resources/SoundSourceResource.h)

  OnSourceFile(${this} Arcadia/Engine/Audials/Backend.c)
  OnHeaderFile(${this} Arcadia/Engine/Audials/Backend.h)
  OnSourceFile(${this} Arcadia/Engine/Audials/BackendContext.c)
  OnHeaderFile(${this} Arcadia/Engine/Audials/BackendContext.h)
  OnSourceFile(${this} Arcadia/Engine/Audials/Include.c)
  OnHeaderFile(${this} Arcadia/Engine/Audials/Include.h)
  OnSourceFile(${this} Arcadia/Engine/Audials/Node.c)
  OnHeaderFile(${this} Arcadia/Engine/Audials/Node.h)
  OnSourceFile(${this} Arcadia/Engine/Audials/NodeFactory.c)
  OnHeaderFile(${this} Arcadia/Engine/Audials/NodeFactory.h)
  OnSourceFile(${this} Arcadia/Engine/Audials/Nodes/SoundSourceNode.c)
  OnHeaderFile(${this} Arcadia/Engine/Audials/Nodes/SoundSourceNode.h)

  # <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>
  OnSourceFile(${this} Arcadia/Engine/Visuals/Backend.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Backend.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/BackendContext.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/BackendContext.h)

  OnSourceFile(${this} Arcadia/Engine/Visuals/BlendFunction.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/BlendFunction.h)

  OnSourceFile(${this} Arcadia/Engine/Visuals/CullMode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/CullMode.h)

  OnSourceFile(${this} Arcadia/Engine/Visuals/DepthCompareFunction.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/DepthCompareFunction.h)

  OnSourceFile(${this} Arcadia/Engine/Visuals/FillMode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/FillMode.h)

  OnSourceFile(${this} Arcadia/Engine/Visuals/Include.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Include.h)

  OnSourceFile(${this} Arcadia/Engine/Visuals/Node.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Node.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/NodeFactory.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/NodeFactory.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Nodes/CameraNode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Nodes/CameraNode.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Nodes/EnterPassNode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Nodes/EnterPassNode.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Nodes/FrameBufferNode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Nodes/FrameBufferNode.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Nodes/MaterialNode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Nodes/MaterialNode.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Nodes/MeshNode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Nodes/MeshNode.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Nodes/ModelNode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Nodes/ModelNode.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Nodes/PixelBufferNode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Nodes/PixelBufferNode.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Nodes/RenderScene.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Nodes/RenderScene.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Nodes/TextureNode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Nodes/TextureNode.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Nodes/ViewportNode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Nodes/ViewportNode.h)

  OnSourceFile(${this} Arcadia/Engine/Visuals/TextureAddressMode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/TextureAddressMode.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/TextureFilter.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/TextureFilter.h)

  OnSourceFile(${this} Arcadia/Engine/Visuals/WindingMode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/WindingMode.h)

  OnSourceFile(${this} Arcadia/Engine/Visuals/ApplicationEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/ApplicationEvent.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/ApplicationQuitRequestedEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/ApplicationQuitRequestedEvent.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/CanvasEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/CanvasEvent.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/CanvasActivationStateChangedEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/CanvasActivationStateChangedEvent.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/CanvasDPIChangedEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/CanvasDPIChangedEvent.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/CanvasSizeChangedEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/CanvasSizeChangedEvent.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/WindowClosedEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/WindowClosedEvent.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/WindowPositionChangedEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/WindowPositionChangedEvent.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/WindowSizeChangedEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/WindowSizeChangedEvent.h)

  OnSourceFile(${this} Arcadia/Engine/Input/KeyboardKey.c)
  OnHeaderFile(${this} Arcadia/Engine/Input/KeyboardKey.h)
  OnSourceFile(${this} Arcadia/Engine/Input/KeyboardKey.i)
  OnSourceFile(${this} Arcadia/Engine/Input/KeyboardKeyAction.c)
  OnHeaderFile(${this} Arcadia/Engine/Input/KeyboardKeyAction.h)
  OnSourceFile(${this} Arcadia/Engine/Input/KeyboardKeyEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/Input/KeyboardKeyEvent.h)
  OnSourceFile(${this} Arcadia/Engine/Input/MouseButtonAction.c)
  OnHeaderFile(${this} Arcadia/Engine/Input/MouseButtonAction.h)
  OnSourceFile(${this} Arcadia/Engine/Input/MouseButtonEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/Input/MouseButtonEvent.h)
  OnSourceFile(${this} Arcadia/Engine/Input/MousePointerAction.c)
  OnHeaderFile(${this} Arcadia/Engine/Input/MousePointerAction.h)
  OnSourceFile(${this} Arcadia/Engine/Input/MousePointerEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/Input/MousePointerEvent.h)

  OnSourceFile(${this} Arcadia/Engine/Visuals/Diagnostics.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Diagnostics.h)

  OnSourceFile(${this} Arcadia/Engine/Visuals/Icon.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Icon.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Window.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Window.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/WindowBackend.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/WindowBackend.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Configuration.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Configuration.h)

  OnSourceFile(${this} Arcadia/Engine/Visuals/DisplayMode.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/DisplayMode.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/DisplayDevice.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/DisplayDevice.h)

  OnSourceFile(${this} Arcadia/Engine/Visuals/Resource.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Resource.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Resources/ConstantBufferResource.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Resources/ConstantBufferResource.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Resources/VertexBufferResource.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Resources/VertexBufferResource.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Resources/ProgramResource.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Resources/ProgramResource.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Resources/EnterPassResource.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Resources/EnterPassResource.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Resources/MaterialResource.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Resources/MaterialResource.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Resources/ModelResource.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Resources/ModelResource.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Resources/FrameBufferResource.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Resources/FrameBufferResource.h)
  OnSourceFile(${this} Arcadia/Engine/Visuals/Resources/TextureResource.c)
  OnHeaderFile(${this} Arcadia/Engine/Visuals/Resources/TextureResource.h)

  # <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  OnModuleDependency(${this} ${MyProjectName}.VPL)
  OnModuleDependency(${this} ${MyProjectName}.Math)
  OnModuleDependency(${this} ${MyProjectName}.PixelBufferIO)
  
  OnModuleDependency(${this} ${MyProjectName}.Ring2 PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.DDL PRIVATE)

  EndProduct(${this})

endif()
