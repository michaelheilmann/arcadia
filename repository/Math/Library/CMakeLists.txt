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

set(this ${MyProjectName}.Math)

# Create library.
if (NOT TARGET ${this})
  BeginProduct(${this} library)

  OnTemplateEngineTemplateFile(${this} ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle.c.te PRIVATE)
  OnTemplateEngineTemplateFile(${this} ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle.h.te PRIVATE)
  OnTemplateEngineTemplateFile(${this} ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Matrix.c.te PRIVATE)
  OnTemplateEngineTemplateFile(${this} ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Matrix.h.te PRIVATE)
  OnTemplateEngineTemplateFile(${this} ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Color.c.te PRIVATE)
  OnTemplateEngineTemplateFile(${this} ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Color.h.te PRIVATE)
  OnTemplateEngineTemplateFile(${this} ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector.c.te PRIVATE)
  OnTemplateEngineTemplateFile(${this} ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector.h.te PRIVATE)

  BeginTemplateEngine(${this}.GenerateFiles "Math")

    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle.c.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle3Real32.c"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle3Real32.env")
    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle.h.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle3Real32.h"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle3Real32.env")

    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle.c.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle3Real64.c"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle3Real64.env")
    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle.h.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle3Real64.h"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/AxisAngle3Real64.env")

    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector.c.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Vector3Real32.c"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector3Real32.env")
    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector.h.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Vector3Real32.h"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector3Real32.env")

    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector.c.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Vector3Real64.c"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector3Real64.env")
    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector.h.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Vector3Real64.h"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector3Real64.env")

    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector.c.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Vector4Real32.c"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector4Real32.env")
    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector.h.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Vector4Real32.h"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector4Real32.env")

    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector.c.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Vector4Real64.c"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector4Real64.env")
    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector.h.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Vector4Real64.h"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Vector4Real64.env")

    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Matrix.c.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Matrix4Real32.c"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Matrix4Real32.env")
    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Matrix.h.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Matrix4Real32.h"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Matrix4Real32.env")

    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Matrix.c.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Matrix4Real64.c"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Matrix4Real64.env")
    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Matrix.h.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Matrix4Real64.h"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Matrix4Real64.env")

    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Color.c.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Color4Real64.c"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Color4Real64.env")
    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Color.h.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Color4Real64.h"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Color4Real64.env")

    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Color.c.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Color4Real32.c"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Color4Real32.env")
    OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Color.h.te"
                     "${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Color4Real32.h"
                     "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Color4Real32.env")

  EndTemplateEngine()


  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Math/Implementation/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Math/Implementation/Configure.h.i)

  OnSourceFile(${this} Arcadia/Math/Include.c)
  OnHeaderFile(${this} Arcadia/Math/Include.h)

  OnSourceFile(${this} Arcadia/Math/Implementation/QuaternionReal32.c)
  OnHeaderFile(${this} Arcadia/Math/Implementation/QuaternionReal32.h)

  OnSourceFile(${this} Arcadia/Math/Implementation/Color4Real32.c GENERATED)
  OnHeaderFile(${this} Arcadia/Math/Implementation/Color4Real32.h GENERATED)
  OnSourceFile(${this} Arcadia/Math/Implementation/Color4Real64.c GENERATED)
  OnHeaderFile(${this} Arcadia/Math/Implementation/Color4Real64.h GENERATED)

  OnSourceFile(${this} Arcadia/Math/Implementation/Vector3Real32.c GENERATED)
  OnHeaderFile(${this} Arcadia/Math/Implementation/Vector3Real32.h GENERATED)
  OnSourceFile(${this} Arcadia/Math/Implementation/Vector3Real64.c GENERATED)
  OnHeaderFile(${this} Arcadia/Math/Implementation/Vector3Real64.h GENERATED)
  OnSourceFile(${this} Arcadia/Math/Implementation/Vector4Real32.c GENERATED)
  OnHeaderFile(${this} Arcadia/Math/Implementation/Vector4Real32.h GENERATED)
  OnSourceFile(${this} Arcadia/Math/Implementation/Vector4Real64.c GENERATED)
  OnHeaderFile(${this} Arcadia/Math/Implementation/Vector4Real64.h GENERATED)

  OnSourceFile(${this} Arcadia/Math/Implementation/Matrix4Real32.c GENERATED)
  OnHeaderFile(${this} Arcadia/Math/Implementation/Matrix4Real32.h GENERATED)
  OnSourceFile(${this} Arcadia/Math/Implementation/Matrix4Real64.c GENERATED)
  OnHeaderFile(${this} Arcadia/Math/Implementation/Matrix4Real64.h GENERATED)

  OnSourceFile(${this} Arcadia/Math/Implementation/crossProduct.c)
  OnHeaderFile(${this} Arcadia/Math/Implementation/crossProduct.h)
  OnSourceFile(${this} Arcadia/Math/Implementation/setOrthographicProjection.c)
  OnHeaderFile(${this} Arcadia/Math/Implementation/setOrthographicProjection.h)
  OnSourceFile(${this} Arcadia/Math/Implementation/setPerspectiveProjection.c)
  OnHeaderFile(${this} Arcadia/Math/Implementation/setPerspectiveProjection.h)
  OnSourceFile(${this} Arcadia/Math/Implementation/setLookAt.c)
  OnHeaderFile(${this} Arcadia/Math/Implementation/setLookAt.h)
  OnSourceFile(${this} Arcadia/Math/Implementation/setRotationX.c)
  OnHeaderFile(${this} Arcadia/Math/Implementation/setRotationX.h)
  OnSourceFile(${this} Arcadia/Math/Implementation/setRotationY.c)
  OnHeaderFile(${this} Arcadia/Math/Implementation/setRotationY.h)
  OnSourceFile(${this} Arcadia/Math/Implementation/setRotationZ.c)
  OnHeaderFile(${this} Arcadia/Math/Implementation/setRotationZ.h)
  OnSourceFile(${this} Arcadia/Math/Implementation/setScaling.c)
  OnHeaderFile(${this} Arcadia/Math/Implementation/setScaling.h)
  OnSourceFile(${this} Arcadia/Math/Implementation/setTranslation.c)
  OnHeaderFile(${this} Arcadia/Math/Implementation/setTranslation.h)
  OnSourceFile(${this} Arcadia/Math/Implementation/trigonometry.c)
  OnHeaderFile(${this} Arcadia/Math/Implementation/trigonometry.h)
  OnSourceFile(${this} Arcadia/Math/Implementation/lerp.c)
  OnHeaderFile(${this} Arcadia/Math/Implementation/lerp.h)

  OnHeaderFile(${this} Arcadia/Math/Include.h)
  OnSourceFile(${this} Arcadia/Math/Include.c)
  OnHeaderFile(${this} Arcadia/Math/Include.h)
  OnSourceFile(${this} Arcadia/Math/Include.c)
  OnHeaderFile(${this} Arcadia/Math/Include.h)

  OnModuleDependency(${this} Arcadia.Ring2)

  if (${${this}.Compiler.Masm} STREQUAL ${${this}.Compiler.Masm.Ms})
	  if (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X64})
      elseif (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X86})
	  endif()
	  if (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X64})
      elseif (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X86})
	  endif()
  endif()

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux})
    list(APPEND ${this}.Libraries m)
  endif()

  EndProduct(${this})
  # @todo This is hackish, fixme.
  add_dependencies(${this} ${this}.GenerateFiles)
  set_target_properties(${this} PROPERTIES FOLDER "Math")
  target_include_directories(${this} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/Sources)

endif()
