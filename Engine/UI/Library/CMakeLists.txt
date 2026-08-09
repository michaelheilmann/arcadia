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

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Engine/UI/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Engine/UI/Configure.h.i)

  OnSourceFile(${this} Arcadia/Engine/UI/Include.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Include.h)

  OnSourceFile(${this} Arcadia/Engine/UI/Event.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Event.h)
  OnSourceFile(${this} Arcadia/Engine/UI/Node.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Node.h)

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
  OnSourceFile(${this} Arcadia/Engine/UI/Nodes/PanelNode.c)
  OnHeaderFile(${this} Arcadia/Engine/UI/Nodes/PanelNode.h)

  OnModuleDependency(${this} ${MyProjectName}.Math)
  OnModuleDependency(${this} ${MyProjectName}.Engine)

  EndProduct(${this})

  set_target_properties(${this} PROPERTIES FOLDER "Engine")

endif()
