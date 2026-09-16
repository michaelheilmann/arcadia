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

# Create library.
set(this ${MyProjectName}.ADL)

if (NOT TARGET ${this})
  BeginProduct(${this} library)

  SetFolder(${this} ${MyFolder})

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/ADL/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/ADL/Configure.h.i)

  OnSourceFile(${this} Arcadia/ADL/Include.c)
  OnHeaderFile(${this} Arcadia/ADL/Include.h)

  OnSourceFile(${this} Arcadia/ADL/Definition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions.h)
  OnSourceFile(${this} Arcadia/ADL/Diagnostics.c)
  OnHeaderFile(${this} Arcadia/ADL/Diagnostics.h)
  OnSourceFile(${this} Arcadia/ADL/Reader.c)
  OnHeaderFile(${this} Arcadia/ADL/Reader.h)
  OnSourceFile(${this} Arcadia/ADL/Reader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Reader.module.h)
  OnSourceFile(${this} Arcadia/ADL/Context.c)
  OnHeaderFile(${this} Arcadia/ADL/Context.h)
  OnSourceFile(${this} Arcadia/ADL/Reference.c)
  OnHeaderFile(${this} Arcadia/ADL/Reference.h)

  OnSourceFile(${this} Arcadia/ADL/Definitions/DSP/Include.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/DSP/Include.h)

  OnSourceFile(${this} Arcadia/ADL/Definitions/DSP/ConstantDefinition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/DSP/ConstantDefinition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/DSP/ConstantReader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/DSP/ConstantReader.module.h)
  
  OnSourceFile(${this} Arcadia/ADL/Definitions/DSP/SawtoothWaveDefinition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/DSP/SawtoothWaveDefinition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/DSP/SawtoothWaveReader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/DSP/SawtoothWaveReader.module.h)
  
  OnSourceFile(${this} Arcadia/ADL/Definitions/DSP/SineWaveDefinition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/DSP/SineWaveDefinition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/DSP/SineWaveReader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/DSP/SineWaveReader.module.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/DSP/WhiteNoiseDefinition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/DSP/WhiteNoiseDefinition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/DSP/WhiteNoiseReader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/DSP/WhiteNoiseReader.module.h)

  OnSourceFile(${this} Arcadia/ADL/Definitions/Audials/Include.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Audials/Include.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Audials/SampleBufferDefinition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Audials/SampleBufferDefinition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Audials/SampleBufferReader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Audials/SampleBufferReader.module.h)

  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/Include.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/Include.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/CheckerboardFillOperationDefinition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/CheckerboardFillOperationDefinition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/CheckerboardFillOperationReader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/CheckerboardFillOperationReader.module.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/ColorDefinition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/ColorDefinition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/ColorReader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/ColorReader.module.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/FillOperationDefinition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/FillOperationDefinition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/FillOperationReader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/FillOperationReader.module.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/MaterialDefinition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/MaterialDefinition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/MaterialReader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/MaterialReader.module.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/MaterialType.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/MaterialType.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/AmbientColorSource.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/AmbientColorSource.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/MeshDefinition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/MeshDefinition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/MeshReader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/MeshReader.module.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/ModelDefinition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/ModelDefinition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/ModelReader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/ModelReader.module.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/PixelBufferDefinition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/PixelBufferDefinition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/PixelBufferReader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/PixelBufferReader.module.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/TextureDefinition.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/TextureDefinition.h)
  OnSourceFile(${this} Arcadia/ADL/Definitions/Visuals/TextureReader.module.c)
  OnHeaderFile(${this} Arcadia/ADL/Definitions/Visuals/TextureReader.module.h)

  OnModuleDependency(${this} ${MyProjectName}.DDLS)
  OnModuleDependency(${this} ${MyProjectName}.FileSystem)

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux})
    list(APPEND ${this}.Libraries m)
  endif()

  EndProduct(${this})

endif()
