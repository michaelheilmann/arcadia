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

set(this ${MyProjectName}.Engine.UI.Tests.EventBases)
BeginProduct(${this} test)
SetFolder(${this} Engine)
OnSourceFile(${this} Arcadia/Engine/UI/Tests/EventBases/Main.c)
OnHeaderFile(${this} Arcadia/Engine/UI/Tests/EventBases/Interfaces.h)
OnSourceFile(${this} Arcadia/Engine/UI/Tests/EventBases/Interfaces.c)
OnHeaderFile(${this} Arcadia/Engine/UI/Tests/EventBases/Alignment.h)
OnSourceFile(${this} Arcadia/Engine/UI/Tests/EventBases/Alignment.c)
OnHeaderFile(${this} Arcadia/Engine/UI/Tests/EventBases/LayoutPanel.h)
OnSourceFile(${this} Arcadia/Engine/UI/Tests/EventBases/LayoutPanel.c)
OnHeaderFile(${this} Arcadia/Engine/UI/Tests/EventBases/LayoutPanelInput.h)
OnSourceFile(${this} Arcadia/Engine/UI/Tests/EventBases/LayoutPanelInput.c)
OnHeaderFile(${this} Arcadia/Engine/UI/Tests/EventBases/TabBar.h)
OnSourceFile(${this} Arcadia/Engine/UI/Tests/EventBases/TabBar.c)
OnHeaderFile(${this} Arcadia/Engine/UI/Tests/EventBases/CanvasPopup.h)
OnSourceFile(${this} Arcadia/Engine/UI/Tests/EventBases/CanvasPopup.c)
OnHeaderFile(${this} Arcadia/Engine/UI/Tests/EventBases/Dropdown.h)
OnSourceFile(${this} Arcadia/Engine/UI/Tests/EventBases/Dropdown.c)
OnHeaderFile(${this} Arcadia/Engine/UI/Tests/EventBases/InputBinding.h)
OnSourceFile(${this} Arcadia/Engine/UI/Tests/EventBases/InputBinding.c)
OnHeaderFile(${this} Arcadia/Engine/UI/Tests/EventBases/BindingRow.h)
OnSourceFile(${this} Arcadia/Engine/UI/Tests/EventBases/BindingRow.c)
OnHeaderFile(${this} Arcadia/Engine/UI/Tests/EventBases/StackLayout.h)
OnSourceFile(${this} Arcadia/Engine/UI/Tests/EventBases/StackLayout.c)
OnSourceFile(${this} Arcadia/Engine/UI/Tests/EventBases/Node.c)
OnHeaderFile(${this} Arcadia/Engine/UI/Tests/EventBases/Node.h)
OnSourceFile(${this} Arcadia/Engine/UI/Tests/EventBases/Populate.c)
OnHeaderFile(${this} Arcadia/Engine/UI/Tests/EventBases/Populate.h)
OnModuleDependency(${this} ${MyProjectName}.Engine.UI PRIVATE)
OnModuleDependency(${this} ${MyProjectName}.Testing PRIVATE)
EndProduct(${this})
