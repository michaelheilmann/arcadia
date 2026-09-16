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

if (NOT MyProjectIsDefined)
  if (NOT DEFINED MyProjectName)
    set(MyProjectName "Arcadia")
  endif()
  project(${MyProjectName})
  set(REPOSITORY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
  include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/all.cmake)
endif()

set(CPACK_PACKAGE_NAME "Arcadia")

set(CPACK_PACKAGE_VENDOR "Michael Heilmann")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Arcadia executable files")
set(CPACK_PACKAGE_CONTACT "contact@michaelheilmann.com")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://michaelheilmann.com/")

set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(CPACK_INSTRUCTIONSETARCHITECTURE "x64")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(CPACK_INSTRUCTIONSETARCHITECTURE "x86")
endif()

set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CPACK_INSTRUCTIONSETARCHITECTURE})

# Prepend custom modules directory to module path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/")

# Detect the operating system.
Arcadia_DetectOperatingSystem(operatingSystem)

# Detect the instruction set architecture.
Arcadia_DetectInstructionSetArchitecture(instructionSetArchitecture)

# For windows, we need to define our dependencies directory.
if (operatingSystem STREQUAL Arcadia_OperatingSystem_Windows)
  # TODO: Make this an option.
  set(MyDependenciesDirectory "${CMAKE_CURRENT_BINARY_DIR}/.Dependencies")
  set(MyDependenciesGitHubReleasePrefix "https://github.com/michaelheilmann/arcadia-dependencies/releases/download/arcadia-dependencies-v1.0.67")

  # Custom target that downloads and extracts the prebuilt dependencies.
  # Takes over the job of the `install-dependencies.ps1` script.
  # Build it explicitly, e.g. `cmake --target Arcadia.InstallDependencies`.
  add_custom_target(Arcadia.InstallDependencies
    COMMAND ${CMAKE_COMMAND}
            -D "MyDependenciesDirectory=${MyDependenciesDirectory}"
            -D "MyDependenciesGitHubReleasePrefix=${MyDependenciesGitHubReleasePrefix}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/CMake/download-dependencies.cmake"
    COMMENT "Downloading and extracting the Arcadia dependencies ..."
    VERBATIM)
endif()

add_subdirectory(Runtime/ARMS)
add_subdirectory(Runtime/Ring1)
add_subdirectory(Runtime/Ring2)
add_subdirectory(Runtime/Collections)
add_subdirectory(Runtime/FileSystem)
add_subdirectory(Runtime/Print)
add_subdirectory(Runtime/Logging)
add_subdirectory(Languages)
add_subdirectory(DDL)
add_subdirectory(DDLS)
add_subdirectory(ADL)
add_subdirectory(VPL)
add_subdirectory(Media)
add_subdirectory(PixelBufferIO)
add_subdirectory(SampleBufferIO)
add_subdirectory(FontIO)

add_subdirectory(Engine/Engine)
add_subdirectory(Engine/UI)
add_subdirectory(Engine/Audials.Implementation)

add_subdirectory(Documentation)
add_subdirectory(Tools/TemplateEngine)
add_subdirectory(Tools/PixelBufferGenerator)

add_subdirectory(repository)

include(CMakePackageConfigHelpers)


# Generate a configuration file from the template.
configure_package_config_file(
  "CMake/ArcadiaConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/ArcadiaConfig.cmake"
  INSTALL_DESTINATION
  "."
)

# Install generated configuration file.
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/ArcadiaConfig.cmake"
  DESTINATION "."
)

# Export the targets.
install(EXPORT ArcadiaTargets FILE ArcadiaTargets.cmake
        DESTINATION "CMake" NAMESPACE Arcadia::)

include(CPack)
