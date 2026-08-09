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

# Create a library.
set(this ${MyProjectName}.ARMS)
if (NOT TARGET ${this})

  BeginProduct(${this} library)

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/ARMS/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/ARMS/Configure.h.i)

  OnSourceFile(${this} Arcadia/ARMS/Include.c)
  OnHeaderFile(${this} Arcadia/ARMS/Include.h)
  OnSourceFile(${this} Arcadia/ARMS/ARMS.c)
  OnHeaderFile(${this} Arcadia/ARMS/ARMS.h)
  OnSourceFile(${this} Arcadia/ARMS/MemoryManager.c)
  OnHeaderFile(${this} Arcadia/ARMS/MemoryManager.h)
  OnSourceFile(${this} Arcadia/ARMS/SizeType.c)
  OnHeaderFile(${this} Arcadia/ARMS/SizeType.h)
  OnSourceFile(${this} Arcadia/ARMS/Natural8Type.c)
  OnHeaderFile(${this} Arcadia/ARMS/Natural8Type.h)
  OnSourceFile(${this} Arcadia/ARMS/StatusType.c)
  OnHeaderFile(${this} Arcadia/ARMS/StatusType.h)
  OnSourceFile(${this} Arcadia/ARMS/TypeName.c)
  OnHeaderFile(${this} Arcadia/ARMS/TypeName.h)
  OnSourceFile(${this} Arcadia/ARMS/NotifyDestroy.c)
  OnHeaderFile(${this} Arcadia/ARMS/NotifyDestroy.h)
  OnSourceFile(${this} Arcadia/ARMS/Internal/MemoryManager.private.c)
  OnHeaderFile(${this} Arcadia/ARMS/Internal/MemoryManager.private.h)
  OnSourceFile(${this} Arcadia/ARMS/Internal/DefaultMemoryManager.c)
  OnHeaderFile(${this} Arcadia/ARMS/Internal/DefaultMemoryManager.h)
  OnSourceFile(${this} Arcadia/ARMS/Internal/SlabMemoryManager.c)
  OnHeaderFile(${this} Arcadia/ARMS/Internal/SlabMemoryManager.h)
  OnSourceFile(${this} Arcadia/ARMS/Internal/Common.c)
  OnHeaderFile(${this} Arcadia/ARMS/Internal/Common.h)
  OnSourceFile(${this} Arcadia/ARMS/Internal/Statistics.c)
  OnHeaderFile(${this} Arcadia/ARMS/Internal/Statistics.h)
  OnSourceFile(${this} Arcadia/ARMS/Internal/Tag.c)
  OnHeaderFile(${this} Arcadia/ARMS/Internal/Tag.h)
  OnSourceFile(${this} Arcadia/ARMS/Internal/TypeName.c)
  OnHeaderFile(${this} Arcadia/ARMS/Internal/TypeName.h)
  OnSourceFile(${this} Arcadia/ARMS/Internal/ReferenceCounter.c)
  OnHeaderFile(${this} Arcadia/ARMS/Internal/ReferenceCounter.h)
  OnSourceFile(${this} Arcadia/ARMS/Internal/NotifyDestroy.c)
  OnHeaderFile(${this} Arcadia/ARMS/Internal/NotifyDestroy.h)

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux})
    list(APPEND ${this}.Libraries m)
  endif()

  set(${this}.Install TRUE)

  EndProduct(${this})

  set_target_properties(${this} PROPERTIES FOLDER "Runtime/ARMS")

endif()
