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

set(this ${MyProjectName}.Engine.UI)

if (NOT TARGET ${this})

  BeginProduct(${this} library)

  SetFolder(${this} "Engine/UI")

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Engine/UI/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Engine/UI/Configure.h.i)

  OnSourceFile(${this} Arcadia/Engine/UI/Include.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Include.h)

  OnSourceFile(${this} Arcadia/Engine/UI/Event.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Event.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Activatable.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Activatable.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Activatable.mil)
  OnSourceFile(${this} Arcadia/Engine/UI/Container.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Container.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Container.mil)
  OnSourceFile(${this} Arcadia/Engine/UI/DefinitionFactory.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/DefinitionFactory.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Focusable.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Focusable.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Focusable.mil)
  OnSourceFile(${this} Arcadia/Engine/UI/HitTestable.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/HitTestable.h)
  OnSourceFile(${this} Arcadia/Engine/UI/HitTestable.mil)
  OnSourceFile(${this} Arcadia/Engine/UI/KeyboardInput.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/KeyboardInput.h)
  OnSourceFile(${this} Arcadia/Engine/UI/KeyboardInput.mil)
  OnSourceFile(${this} Arcadia/Engine/UI/PointerInput.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/PointerInput.h)
  OnSourceFile(${this} Arcadia/Engine/UI/PointerInput.mil)
  OnSourceFile(${this} Arcadia/Engine/UI/Renderable.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Renderable.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Renderable.mil)
  OnSourceFile(${this} Arcadia/Engine/UI/Scrollable.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Scrollable.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Scrollable.mil)
  OnSourceFile(${this} Arcadia/Engine/UI/ValueControl.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/ValueControl.h)
  OnSourceFile(${this} Arcadia/Engine/UI/ValueControl.mil)
  OnSourceFile(${this} Arcadia/Engine/UI/Node.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Node.h)
  OnSourceFile(${this} Arcadia/Engine/UI/TextUtilities.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/TextUtilities.h)

  OnSourceFile(${this} Arcadia/Engine/UI/WidgetEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/WidgetEvent.h)
  OnSourceFile(${this} Arcadia/Engine/UI/WidgetNode.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/WidgetNode.h)

  OnSourceFile(${this} Arcadia/Engine/UI/Events/WidgetPositionChangedEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Events/WidgetPositionChangedEvent.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Events/WidgetSizeChangedEvent.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Events/WidgetSizeChangedEvent.h)

  OnSourceFile(${this} Arcadia/Engine/UI/Nodes/CanvasNode.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Nodes/CanvasNode.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Nodes/ButtonNode.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Nodes/ButtonNode.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Nodes/LabelNode.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Nodes/LabelNode.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Nodes/PanelNode.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Nodes/PanelNode.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Nodes/ScrollPanelNode.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Nodes/ScrollPanelNode.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Nodes/SegmentedChoiceNode.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Nodes/SegmentedChoiceNode.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Nodes/SliderNode.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Nodes/SliderNode.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Nodes/StackPanelNode.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Nodes/StackPanelNode.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Nodes/StackPanelNodeAlignment.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Nodes/StackPanelNodeAlignment.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Nodes/ToggleNode.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Nodes/ToggleNode.h)

  OnModuleDependency(${this} ${MyProjectName}.ADL)
  OnModuleDependency(${this} ${MyProjectName}.Math)
  OnModuleDependency(${this} ${MyProjectName}.Engine)
  OnModuleDependency(${this} ${MyProjectName}.Engine.Text)
  OnModuleDependency(${this} ${MyProjectName}.FontIO)

  EndProduct(${this})

endif()
