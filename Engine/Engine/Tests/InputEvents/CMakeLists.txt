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

set(this ${MyProjectName}.Engine.Tests.InputEvents)
BeginProduct(${this} test)
SetFolder(${this} Engine)
OnSourceFile(${this} Arcadia/Engine/Tests/InputEvents/Main.c)
OnHeaderFile(${this} Arcadia/Engine/Tests/InputEvents/Cases.h)
OnSourceFile(${this} Arcadia/Engine/Tests/InputEvents/MouseEvents.c)
OnSourceFile(${this} Arcadia/Engine/Tests/InputEvents/KeyboardEvent.c)
OnModuleDependency(${this} ${MyProjectName}.Engine PRIVATE)
OnModuleDependency(${this} ${MyProjectName}.Testing PRIVATE)
EndProduct(${this})
