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

set(this ${MyProjectName}.Network)

if (NOT TARGET ${this})
  BeginProduct(${this} library)
  SetFolder(${this} "Network")

  OnHeaderFile(${this} Arcadia/Network/Include.h)
  OnHeaderFile(${this} Arcadia/Network/Result.h)
  OnSourceFile(${this} Arcadia/Network/Frame.c)
  OnHeaderFile(${this} Arcadia/Network/Frame.h)
  OnSourceFile(${this} Arcadia/Network/FrameDecoder.c)
  OnHeaderFile(${this} Arcadia/Network/FrameDecoder.h)
  OnSourceFile(${this} Arcadia/Network/ByteQueue.c)
  OnHeaderFile(${this} Arcadia/Network/ByteQueue.h)
  OnSourceFile(${this} Arcadia/Network/Event.c)
  OnHeaderFile(${this} Arcadia/Network/Event.h)
  OnSourceFile(${this} Arcadia/Network/EventQueue.c)
  OnHeaderFile(${this} Arcadia/Network/EventQueue.h)
  OnHeaderFile(${this} Arcadia/Network/Internal/Mutex.h PRIVATE)

  if(WIN32)
    OnSourceFile(${this} Arcadia/Network/Internal/MutexWindows.c)
  elseif(UNIX)
    OnSourceFile(${this} Arcadia/Network/Internal/MutexPosix.c)
    find_package(Threads REQUIRED)
    list(APPEND ${this}.PrivateLibraries Threads::Threads)
  else()
    message(FATAL_ERROR "Arcadia.Network requires a native mutex implementation on this platform")
  endif()

  EndProduct(${this})
endif()
