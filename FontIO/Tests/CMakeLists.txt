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

# Reuse the repository's OFL-licensed Noto Sans asset; do not download/copy a
# platform font into the checkout. A missing explicit fixture is a genuine skip.
set(ARCADIA_FONT_TEST_FILE "${REPOSITORY_SOURCE_DIR}/.Website/assets/noto-new/Noto_Sans/NotoSans-VariableFont_wdth,wght.ttf"
  CACHE FILEPATH "Read-only font fixture for FontIO CTest cases")
add_subdirectory(CreateBitmapFontTests)
add_subdirectory(CreateAtlasBitmapFontTests)

set(this ${MyProjectName}.FontIO.Tests.Prerequisites)
BeginProduct(${this} test)
SetFolder(${this} "FontIO")
set(${this}.TestScript "${CMAKE_CURRENT_SOURCE_DIR}/Prerequisites.cmake")
set(${this}.TestArguments "-DBITMAP=$<TARGET_FILE:${MyProjectName}.FontIO.Tests.CreateBitmapFontTests>"
  "-DATLAS=$<TARGET_FILE:${MyProjectName}.FontIO.Tests.CreateAtlasBitmapFontTests>"
  "-DFONT=${ARCADIA_FONT_TEST_FILE}" "-DBUILD_ROOT=${CMAKE_CURRENT_BINARY_DIR}"
  "-DCTEST_COMMAND=${CMAKE_CTEST_COMMAND}")
set(${this}.TestDependencies ${MyProjectName}.FontIO.Tests.CreateBitmapFontTests ${MyProjectName}.FontIO.Tests.CreateAtlasBitmapFontTests)
set(${this}.TestLabels infrastructure integration requires-font)
set(${this}.TestTimeout 60)
EndProduct(${this})
