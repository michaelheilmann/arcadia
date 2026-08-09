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

set(this ${MyProjectName}.Media)

if (NOT TARGET ${this})
  BeginProduct(${this} library)

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Media/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Media/Configure.h.i)

  OnSourceFile(${this} Arcadia/Media/Include.c)
  OnHeaderFile(${this} Arcadia/Media/Include.h)

  OnSourceFile(${this} Arcadia/Media/PixelBuffer.c)
  OnHeaderFile(${this} Arcadia/Media/PixelBuffer.h)
  OnSourceFile(${this} Arcadia/Media/PixelFormat.c)
  OnHeaderFile(${this} Arcadia/Media/PixelFormat.h)
  OnSourceFile(${this} Arcadia/Media/PixelBufferOperation.c)
  OnHeaderFile(${this} Arcadia/Media/PixelBufferOperation.h)
  OnSourceFile(${this} Arcadia/Media/PixelBufferOperations/CheckerboardFill.c)
  OnHeaderFile(${this} Arcadia/Media/PixelBufferOperations/CheckerboardFill.h)
  OnSourceFile(${this} Arcadia/Media/PixelBufferOperations/Fill.c)
  OnHeaderFile(${this} Arcadia/Media/PixelBufferOperations/Fill.h)

  OnSourceFile(${this} Arcadia/Media/SampleBuffer.c)
  OnHeaderFile(${this} Arcadia/Media/SampleBuffer.h)
  OnSourceFile(${this} Arcadia/Media/SampleFormat.c)
  OnHeaderFile(${this} Arcadia/Media/SampleFormat.h)
  OnSourceFile(${this} Arcadia/Media/DSP.c)
  OnHeaderFile(${this} Arcadia/Media/DSP.h)
  OnSourceFile(${this} Arcadia/Media/DSP/SineWave.c)
  OnHeaderFile(${this} Arcadia/Media/DSP/SineWave.h)
  OnSourceFile(${this} Arcadia/Media/DSP/WhiteNoise.c)
  OnHeaderFile(${this} Arcadia/Media/DSP/WhiteNoise.h)

  OnSourceFile(${this} Arcadia/Media/Quantization.c)
  OnHeaderFile(${this} Arcadia/Media/Quantization.h)

  OnSourceFile(${this} Arcadia/Media/VertexBuffer.c)
  OnHeaderFile(${this} Arcadia/Media/VertexBuffer.h)

  OnSourceFile(${this} Arcadia/Media/VertexDescriptorBuilder.c)
  OnHeaderFile(${this} Arcadia/Media/VertexDescriptorBuilder.h)

  OnSourceFile(${this} Arcadia/Media/VertexDescriptor.c)
  OnHeaderFile(${this} Arcadia/Media/VertexDescriptor.h)
  OnSourceFile(${this} Arcadia/Media/VertexDescriptor.module.c)
  OnHeaderFile(${this} Arcadia/Media/VertexDescriptor.module.h)

  OnSourceFile(${this} Arcadia/Media/VertexElementDescriptor.c)
  OnHeaderFile(${this} Arcadia/Media/VertexElementDescriptor.h)

  OnSourceFile(${this} Arcadia/Media/VertexElementSemantics.c)
  OnHeaderFile(${this} Arcadia/Media/VertexElementSemantics.h)
  OnSourceFile(${this} Arcadia/Media/VertexElementSyntactics.c)
  OnHeaderFile(${this} Arcadia/Media/VertexElementSyntactics.h)

  list(APPEND ${this}.Libraries ${MyProjectName}.ADL)

  EndProduct(${this})

  set_target_properties(${this} PROPERTIES FOLDER "Media")

endif()
