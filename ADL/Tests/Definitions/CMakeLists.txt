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

set(MyFolder ADL)

# Create test.
set(this ${MyProjectName}.ADL.Tests.Definitions)

if (NOT TARGET ${this})

  BeginProduct(${this} test)

  SetFolder(${this} ${MyFolder})

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia.ADL.Tests.Definitions/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia.ADL.Tests.Definitions/Configure.h.i)

  OnSourceFile(${this} Arcadia.ADL.Tests.Definitions/Main.c)
  OnSourceFile(${this} Arcadia.ADL.Tests.Definitions/Visuals/Color.c)
  OnHeaderFile(${this} Arcadia.ADL.Tests.Definitions/Visuals/Color.h)
  OnSourceFile(${this} Arcadia.ADL.Tests.Definitions/Visuals/Material.c)
  OnHeaderFile(${this} Arcadia.ADL.Tests.Definitions/Visuals/Material.h)
  OnSourceFile(${this} Arcadia.ADL.Tests.Definitions/Visuals/Mesh.c)
  OnHeaderFile(${this} Arcadia.ADL.Tests.Definitions/Visuals/Mesh.h)
  OnSourceFile(${this} Arcadia.ADL.Tests.Definitions/Visuals/Model.c)
  OnHeaderFile(${this} Arcadia.ADL.Tests.Definitions/Visuals/Model.h)
  OnSourceFile(${this} Arcadia.ADL.Tests.Definitions/Visuals/PixelBuffer.c)
  OnHeaderFile(${this} Arcadia.ADL.Tests.Definitions/Visuals/PixelBuffer.h)
  OnSourceFile(${this} Arcadia.ADL.Tests.Definitions/Visuals/Texture.c)
  OnHeaderFile(${this} Arcadia.ADL.Tests.Definitions/Visuals/Texture.h)

  OnSourceFile(${this} Arcadia.ADL.Tests.Definitions/DSP/Constant.c)
  OnHeaderFile(${this} Arcadia.ADL.Tests.Definitions/DSP/Constant.h)
  OnSourceFile(${this} Arcadia.ADL.Tests.Definitions/DSP/SawtoothWave.c)
  OnHeaderFile(${this} Arcadia.ADL.Tests.Definitions/DSP/SawtoothWave.h)
  OnSourceFile(${this} Arcadia.ADL.Tests.Definitions/DSP/SineWave.c)
  OnHeaderFile(${this} Arcadia.ADL.Tests.Definitions/DSP/SineWave.h)
  OnSourceFile(${this} Arcadia.ADL.Tests.Definitions/DSP/WhiteNoise.c)
  OnHeaderFile(${this} Arcadia.ADL.Tests.Definitions/DSP/WhiteNoise.h)

  OnSourceFile(${this} Arcadia.ADL.Tests.Definitions/Audials/SampleBuffer.c)
  OnHeaderFile(${this} Arcadia.ADL.Tests.Definitions/Audials/SampleBuffer.h)

  OnAssetsDirectory(${this}.CopyAssets ${CMAKE_CURRENT_SOURCE_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets)

  OnModuleDependency(${this} ${MyProjectName}.ADL PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.DDLS PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.DDL PRIVATE)

  EndProduct(${this})

  target_include_directories(${this} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Sources ${CMAKE_CURRENT_BINARY_DIR}/Sources)

  CopyProductAssets(${this} ${MyFolder} ${CMAKE_CURRENT_BINARY_DIR}/Assets)

endif()
