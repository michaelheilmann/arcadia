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

set(this ${MyProjectName}.Engine.Visuals.Implementation)

if (NOT TARGET ${this})
  option(${this}.Direct3D12.Enabled "TRUE enables Direct3D 12 support. FALSE disables Direct3D 12 support. Default is FALSE" FALSE)
  option(${this}.OpenGL4.Enabled "TRUE enables OpenGL 4 support. FALSE disables OpenGL 4 support. Default is FALSE" FALSE)
  option(${this}.Vulkan.Enabled "TRUE enables Vulkan support. FALSE disables Vulkan support. Default is FALSE" FALSE)

  BeginProduct(${this} library)

  if (${this}.Direct3D12.Enabled)
    set(${this}.Direct3D12.Enabled 1)
  else()
    set(${this}.Direct3D12.Enabled 0)
  endif()

  if (${this}.OpenGL4.Enabled)
    set(${this}.OpenGL4.Enabled 1)
  else()
    set(${this}.OpenGL4.Enabled 0)
  endif()

  if (${this}.Vulkan.Enabled)
    set(${this}.Vulkan.Enabled 1)
  else()
    set(${this}.Vulkan.Enabled 0)
  endif()

  # X11/OpenGL under Linux.
  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux} AND ${this}.OpenGL4.Enabled)
    # Find X11.
    find_package(X11)
    if (NOT X11_FOUND)
      message(FATAL_ERROR " - X11 not found")
    else()
      message(STATUS " - X11 found - X11_LIBRARIES = ${X11_LIBRARIES}")
      list(APPEND ${this}.PrivateLibraries ${X11_LIBRARIES})
    endif()
    if (NOT X11_Xrandr_FOUND)
      message(FATAL_ERROR " - X11 XRANDR not found")
    else()
      message(STATUS " - X11 XRANDR found - X11_Xrandr_LIB = ${X11_Xrandr_LIB}")
      list(APPEND ${this}.PrivateLibraries ${X11_Xrandr_LIB})
    endif()
    # Find OpenGL.
    find_package(OpenGL)
    if (NOT OPENGL_FOUND)
      message(FATAL_ERROR " - OpenGL not found")
    else()
      #message(STATUS " - OpenGL found - OPENGL_LIBRARIES = ${OPENGL_LIBRARIES}")
      list(APPEND ${this}.PrivateLibraries ${OPENGL_LIBRARIES})
    endif()
    set(${this}.Backend ${${this}.Backend.OpenGl})
  endif()

  # Always use libpng under Linux.
  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux})
    find_package(PNG)
    if (NOT TARGET PNG::PNG)
      message(FATAL_ERROR "libPNG not found")
    endif()
    list(APPEND ${this}.PrivateLibraries PNG::PNG)
  endif()

  # Always use Shcore.lib under Windows.
  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Windows})
    list(APPEND ${this}.PrivateLibraries Shcore.lib)
  endif()

  # Enable WGL/OpenGL 4 under Windows.
  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Windows} AND ${${this}.OpenGL4.Enabled})
    find_package(OpenGL REQUIRED)
    if (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X64})
      set(${this}.OpenGL.HeaderFilesDirectory ${MyDependenciesDirectory}/x64/$<LOWER_CASE:$<CONFIG>>/opengl/includes)
    elseif (${${this}_InstructionSetArchitecture} STREQUAL ${${this}_InstructionSetArchitecture_X86})
      set(${this}.OpenGL.HeaderFilesDirectory ${MyDependenciesDirectory}/x86/$<LOWER_CASE:$<CONFIG>>/opengl/includes)
    else()
      message(FATAL_ERROR " - unsupported instruction set architecture")
    endif()
    message(STATUS " - ${this}.OpenGl.HeaderFilesDirectory = ${${this}.OpenGl.HeaderFilesDirectory}")
    list(APPEND ${this}.PrivateLibraries ${OPENGL_LIBRARIES})
    list(APPEND ${this}.IncludeDirectories ${${this}.OpenGL.HeaderFilesDirectory})
    set(${this}.Backend ${${this}.Backend.OpenGL})
  endif()

  # Enable OpenGL 4.
  if (${${this}.OpenGL4.Enabled})
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/ConstantBufferResource.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/ConstantBufferResource.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/VertexBufferResource.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/VertexBufferResource.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/ProgramResource.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/ProgramResource.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/MaterialResource.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/MaterialResource.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/ModelResource.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/ModelResource.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/FrameBufferResource.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/FrameBufferResource.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/EnterPassResource.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/EnterPassResource.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/TextureResource.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Resources/TextureResource.h)

    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/BackendIncludes.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/BackendIncludes.h)

    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Functions.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Functions.h)
    OnInlayFile(${this}  Arcadia/Visuals/Implementation/OpenGL4/Functions.i)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Backend.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/Backend.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/BackendContext.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/BackendContext.h)
  endif()

  # Enable Direct3D 12 under Windows.
  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Windows} AND ${${this}.Direct3D12.Enabled})
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Direct3D12/Backend.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Direct3D12/Backend.h)
  endif()

  # Enable Vulkan under Windows.
  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Windows} AND ${${this}.Vulkan.Enabled})
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Vulkan/Backend.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Vulkan/Backend.h)
  endif()

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia/Visuals/Implementation/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia/Visuals/Implementation/Configure.h.i)

  OnSourceFile(${this} Arcadia/Visuals/Implementation/Include.c)
  OnHeaderFile(${this} Arcadia/Visuals/Implementation/Include.h)

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux})
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Linux/_edid.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Linux/_edid.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Linux/DisplayDevice.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Linux/DisplayDevice.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Linux/DisplayMode.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Linux/DisplayMode.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Linux/Include.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Linux/Include.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Linux/Icon.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Linux/Icon.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Linux/ErrorHandler.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Linux/ErrorHandler.h)
  endif()

  if (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Windows})
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Windows/DisplayDevice.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Windows/DisplayDevice.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Windows/DisplayMode.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Windows/DisplayMode.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Windows/Bitmap.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Windows/Bitmap.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Windows/Icon.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Windows/Icon.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Windows/_WindowText.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Windows/_WindowText.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Windows/_CharConv.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Windows/_CharConv.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/Windows/_GetDisplayDevices.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/Windows/_GetDisplayDevices.h)
  endif()

  if (${${this}.OpenGL4.Enabled} AND (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Linux}))
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/GLX/_Extensions.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/GLX/_Extensions.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/GLX/_Configurations.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/GLX/_Configurations.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/GLX/WindowBackend.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/GLX/WindowBackend.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/GLX/BackendContext.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/GLX/BackendContext.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/GLX/Functions.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/GLX/Functions.h)
    OnInlayFile(${this} Arcadia/Visuals/Implementation/OpenGL4/GLX/Functions.i)
  endif()

  if (${${this}.OpenGL4.Enabled} AND (${${this}_OperatingSystem} STREQUAL ${${this}_OperatingSystem_Windows}))
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/WGL/SystemWindow.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/WGL/SystemWindow.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/WGL/FactoryContext.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/WGL/FactoryContext.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/WGL/FactoryWindow.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/WGL/FactoryWindow.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/WGL/BackendContext.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/WGL/BackendContext.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/WGL/WindowBackend.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/WGL/WindowBackend.h)
    OnSourceFile(${this} Arcadia/Visuals/Implementation/OpenGL4/WGL/Functions.c)
    OnHeaderFile(${this} Arcadia/Visuals/Implementation/OpenGL4/WGL/Functions.h)
    OnInlayFile(${this} Arcadia/Visuals/Implementation/OpenGL4/WGL/Functions.i)
  endif()

  OnModuleDependency(${this} ${MyProjectName}.Engine)
  OnModuleDependency(${this} ${MyProjectName}.PixelBufferIO)
  
  OnModuleDependency(${this} ${MyProjectName}.Math PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.Ring2 PRIVATE)
  OnModuleDependency(${this} ${MyProjectName}.DDL PRIVATE)

  EndProduct(${this})

  set_target_properties(${this} PROPERTIES FOLDER "Backends")

endif()
