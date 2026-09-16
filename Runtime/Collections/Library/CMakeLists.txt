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

# Create library.
set(this ${MyProjectName}.Collections)

if (NOT TARGET ${this})
  BeginProduct(${this} library)

  SetFolder(${this} "Runtime/Collections")

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Collections/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Collections/Configure.h.i)

  OnSourceFile(${this} Arcadia/Collections/Include.c)
  OnHeaderFile(${this} Arcadia/Collections/Include.h)
  OnSourceFile(${this} Arcadia/Collections/ArrayList.c)
  OnHeaderFile(${this} Arcadia/Collections/ArrayList.h)
  OnSourceFile(${this} Arcadia/Collections/ArrayDeque.c)
  OnHeaderFile(${this} Arcadia/Collections/ArrayDeque.h)
  OnSourceFile(${this} Arcadia/Collections/ArrayStack.c)
  OnHeaderFile(${this} Arcadia/Collections/ArrayStack.h)
  OnSourceFile(${this} Arcadia/Collections/Collection.c)
  OnHeaderFile(${this} Arcadia/Collections/Collection.h)
  OnSourceFile(${this} Arcadia/Collections/Deque.c)
  OnHeaderFile(${this} Arcadia/Collections/Deque.h)
  OnSourceFile(${this} Arcadia/Collections/HashMap.c)
  OnHeaderFile(${this} Arcadia/Collections/HashMap.h)
  OnSourceFile(${this} Arcadia/Collections/HashSet.c)
  OnHeaderFile(${this} Arcadia/Collections/HashSet.h)
  OnSourceFile(${this} Arcadia/Collections/ImmutableHashMap.c)
  OnHeaderFile(${this} Arcadia/Collections/ImmutableHashMap.h)
  OnSourceFile(${this} Arcadia/Collections/ImmutableList.c)
  OnHeaderFile(${this} Arcadia/Collections/ImmutableList.h)
  OnSourceFile(${this} Arcadia/Collections/ImmutableSet.c)
  OnHeaderFile(${this} Arcadia/Collections/ImmutableSet.h)
  OnSourceFile(${this} Arcadia/Collections/List.c)
  OnHeaderFile(${this} Arcadia/Collections/List.h)
  OnSourceFile(${this} Arcadia/Collections/Map.c)
  OnHeaderFile(${this} Arcadia/Collections/Map.h)
  OnSourceFile(${this} Arcadia/Collections/Set.c)
  OnHeaderFile(${this} Arcadia/Collections/Set.h)
  OnSourceFile(${this} Arcadia/Collections/Stack.c)
  OnHeaderFile(${this} Arcadia/Collections/Stack.h)

  OnSourceFile(${this} Arcadia/Collections/Include.c)
  OnHeaderFile(${this} Arcadia/Collections/Include.h)

  OnModuleDependency(${this} ${MyProjectName}.Ring2)

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux})
    list(APPEND ${this}.Libraries m)
  endif()

  set(${this}.Install TRUE)

  EndProduct(${this})

endif()
