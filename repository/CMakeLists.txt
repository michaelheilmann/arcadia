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

add_subdirectory(Integrations)

add_subdirectory(Engine.Demo)

add_subdirectory(Tools.ConcatenateFiles)
add_subdirectory(Tools.CompareFiles)

add_subdirectory(Math)
add_subdirectory(Visuals.Implementation)

add_subdirectory(Tools.IconGenerator)
add_subdirectory(Tools.GenerateRyuLookupTables)
