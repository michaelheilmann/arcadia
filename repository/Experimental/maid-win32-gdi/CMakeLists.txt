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

project(maid)

set(SOURCES "")
list(APPEND SOURCES "maid-main.c")
list(APPEND SOURCES "maid-string.c" "maid-string.h")
list(APPEND SOURCES "maid-global-mutex.c" "maid-global-mutex.h")
list(APPEND SOURCES "maid-atexit.c" "maid-atexit.h")
list(APPEND SOURCES "maid-reference-counter.c" "maid-reference-counter.h")
list(APPEND SOURCES "gfx.c" "gfx.h")

add_executable(maid-win32-gdi WIN32 ${SOURCES})
