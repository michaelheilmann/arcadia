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

# This is the canonical method of disabling in-source builds.
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
  message( FATAL_ERROR
           "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
           "Please remove any files from the source directory which this call might have generated (in particular, CMakeCache.txt and CMakeFiles)." )
endif()

include(${CMAKE_CURRENT_LIST_DIR}/configure_warnings_and_errors.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detect_compiler.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detect_byte_order.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detect_instruction_set_architecture.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/detect_operating_system.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/tool-file-copy.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/tools-template-engine.cmake)

enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(FetchContent)
include(CheckLanguage)

check_language(C)
if (CMAKE_C_COMPILER)
  enable_language(C)
endif()

check_language(ASM_MASM)
if (CMAKE_ASM_MASM_COMPILER)
  enable_language(ASM_MASM)
endif()

# @param folder The IDE folder under which the targets are generated. Pass the empty string for the IDE root folder.
# @todo Move into list directory as a library function.
macro(CopyProductAssets target folder targetDirectory)
 set(targetFiles "")
 foreach (sourceDirectory sourceFile IN ZIP_LISTS ${target}.Assets.Directories ${target}.Assets.Files)
   message(STATUS "(sourceDirectory, sourceFile) := (${sourceDirectory}, ${sourceFile})")
   set(targetFile ${sourceFile})
   message(STATUS "(sourceDirectory, sourceFile, targetDirectory, targetFile) := (${sourceDirectory}, ${sourceFile}, ${targetDirectory}, ${targetFile})")

   get_filename_component(targetFileDirectory ${targetDirectory}/${targetFile} DIRECTORY)
   #message(STATUS "=> target file directory: ${targetFileDirectory}")
   add_custom_command(OUTPUT ${targetDirectory}/${targetFile}
                      COMMAND ${CMAKE_COMMAND} -E make_directory ${targetFileDirectory}
                      COMMAND ${CMAKE_COMMAND} -E copy ${sourceDirectory}/${sourceFile} ${targetDirectory}/${targetFile}
                      COMMAND ${CMAKE_COMMAND} -E touch ${targetDirectory}/${targetFile}
                      DEPENDS ${sourceDirectory}/${sourceFile}
                      VERBATIM
                      COMMENT "copy file `${sourceDirectory}/${sourceFile}` to `${targetDirectory}/${targetFile}`")

   list(APPEND targetFiles "${targetFileDirectory}" "${targetDirectory}/${targetFile}")

 endforeach()

 add_custom_target(${target}.CopyAssets ALL DEPENDS ${targetFiles})
 add_dependencies(${target} ${target}.CopyAssets)


 # If a folder was specified.
 if (NOT "${folder}" STREQUAL "")
   set_target_properties(${target}.CopyAssets PROPERTIES FOLDER ${folder})
 endif()

endmacro()

# Begin a product (library, executable, or test).
#
# - if ${type} is `library`, `executable`, or `test`:
#   The following variables are defined
#   - ${target}.SourceFiles
#     List of C/C++ source files. Initially defined but empty
#   - ${target}.HeaderFiles
#     List of C/C++ header files. Initially defined but empty
#   - ${target}.Assets.(Files|Directories)
#     At position i in ${target}.AssetFiles is a relative path to a file.
#     Position i in ${target}.AssetDirectories contains the absolute directory prefix of that file.
#   - ${target}.InlayFiles
#     List of inlay files. Initially defined but empty
#   - ${target}.WorkingDirectory
#     for tests and executables only: the working directory. Defaults to ${CMAKE_CURRENT_BINARY_DIR}.
#   - ${target}.Libaries
#     List of libraries. Initially defined but empty
#   - ${target}.PrivateLibaries
#     List of libraries only visible from within ${target}. Initially defined but empty
#   - ${target}.Enabled
#     TRUE if the product should be built. FALSE otherwise. Defaults to TRUE.
#
# - if ${type} is `template`
macro(BeginProduct target type)
  set(${target}.SourceFiles "")
  set(${target}.HeaderFiles "")
  set(${target}.InlayFiles "")
  
  set(${target}.ConfigurationTemplateFiles "")
  set(${target}.ConfigurationFiles "")
  
  set(${target}.Assets.Files "")
  set(${target}.Assets.Directories "")
  
  set(${target}.AssemblerFiles "")
  
  set(${target}.Libraries "")
  set(${target}.PrivateLibraries "")
  
  set(${target}.IncludeDirectories "")
  
  set(${target}.TemplateEngine.TemplateFiles)
  set(${target}.TemplateEngine.EnvironmentFiles)
  
  set(${target}.Enabled TRUE)
  DetectCompilerC(${target})
  DetectCompilerASM(${target})
  DetectByteOrder(${target})
  DetectOperatingSystem(${target})
  DetectInstructionSetArchitecture(${target})

  BeginFileCopy(${target})

  # Store the product type.
  set(${target}.Type ${type})

endmacro()

# Determine if a string haystack starts with a string needle.
# Return the result in a variable result.
# Usage:
#   string_starts_with(result ${haystack} ${needle})
#   if (${result})
#     ...
#   endif()
function(string_starts_with result haystack needle)
  string(FIND "${haystack}" "${needle}" index)
  if("${index}" EQUAL 0)
    set(${result} 1 PARENT_SCOPE)
  else()
    set(${result} 0 PARENT_SCOPE)
  endif()
endfunction()

macro(EndProduct target)
  if (${${target}.Enabled})
    if (${${target}.Type} STREQUAL library)
      add_library(${target})
    elseif (${${target}.Type} STREQUAL executable)
      set(${target}.WorkingDirectory ${CMAKE_CURRENT_BINARY_DIR})
      add_executable(${target})
    elseif (${${target}.Type} STREQUAL test)
      set(${target}.WorkingDirectory ${CMAKE_CURRENT_BINARY_DIR})
      add_executable(${target})
    else()
      message(FATAL_ERROR "unknown/unsupported product type")
    endif()

    # Configure warnings and errors.
    ConfigureWarningsAndErrors(${target})

    # Configure installation directory.
    set(${target}.InstallationDirectory "${target}")
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (is_multi_config)
      set(${target}.InstallationDirectory "${${target}.InstallationDirectory}/$<CONFIG>")
    else()
      set(${target}.InstallationDirectory "${${target}.InstallationDirectory}/${CMAKE_BUILD_TYPE}")
    endif()
    if (${target}_InstructionSetArchitecture STREQUAL ${target}_InstructionSetArchitecture_X64)
      set(${target}.InstallationDirectory "${${target}.InstallationDirectory}/x64")
    elseif (${target}_InstructionSetArchitecture STREQUAL ${target}_InstructionSetArchitecture_X86)
      set(${target}.InstallationDirectory "${${target}.InstallationDirectory}/x86")
    else()
      message(FATAL_ERROR " - unsupported instruction set architecture")
    endif()
    set(${target}.InstallationDirectories.Binaries  ${${target}.InstallationDirectory}/bin)
    set(${target}.InstallationDirectories.Libraries ${${target}.InstallationDirectory}/lib)
    set(${target}.InstallationDirectories.Archives  ${${target}.InstallationDirectory}/lib)
    set(${target}.InstallationDirectories.Includes  ${${target}.InstallationDirectory}/include)

    # Combine all file lists into a single list.
    set (allFiles "")
    foreach (e ${${target}.HeaderFiles})
      list(APPEND allFiles ${e})
    endforeach()
    foreach (e ${${target}.SourceFiles})
      list(APPEND allFiles ${e})
    endforeach()
    foreach (e ${${target}.InlayFiles})
      list(APPEND allFiles ${e})
    endforeach()
    foreach (e ${${target}.ConfigurationTemplateFiles})
      list(APPEND allFiles ${e})
    endforeach()
    foreach (e ${${target}.ConfigurationFiles})
      list(APPEND allFiles ${e})
    endforeach()
    foreach (e ${${target}.AssemblerFiles})
      list(APPEND allFiles ${e})
    endforeach()
    foreach (e ${${target}.TemplateEngine.TemplateFiles})
      list(APPEND allFiles ${e})
    endforeach()
    foreach (e ${${target}.TemplateEngine.EnvironmentFiles})
      list(APPEND allFiles ${e})
    endforeach()

    # Support for resource files under windows.
    if (${${target}.OperatingSystem} STREQUAL ${${target}.OperatingSystem.Windows} AND ${target}.Windows.ResourceFile)
      list(APPEND allFiles ${${target}.Windows.ResourceFile})
    endif()

    set(${target}.PublicFiles "")
    set(${target}.PrivateFiles "")
    # source files, assembler files, and configuration template files are ALWAYS private.
    # Header files, inlay files, and configuration files are by default public and can be marked as private.
    foreach (file ${${target}.InlayFiles})
      get_source_file_property(_isPrivate ${file} PRIVATE)
      if (NOT _isPrivate)
        list(APPEND ${target}.PublicFiles ${file})
      else()
        list(APPEND ${target}.PrivateFiles ${file})
      endif()
    endforeach()
    foreach (file ${${target}.HeaderFiles})
      get_source_file_property(_isPrivate ${file} PRIVATE)
      if (NOT _isPrivate)
        list(APPEND ${target}.PublicFiles ${file})
      else()
        list(APPEND ${target}.PrivateFiles ${file})
      endif()
    endforeach()
    foreach (file ${${target}.ConfigurationFiles})
      get_source_file_property(_isPrivate ${file} PRIVATE)
      if (NOT _isPrivate)
        list(APPEND ${target}.PublicFiles ${file})
      else()
        list(APPEND ${target}.PrivateFiles ${file})
      endif()
    endforeach()
    foreach (file ${${target}.TemplateEngine.TemplateFiles})
      get_source_file_property(_isPrivate ${file} PRIVATE)
      if (NOT _isPrivate)
        list(APPEND ${target}.PublicFiles ${file})
      else()
        list(APPEND ${target}.PrivateFiles ${file})
      endif()
    endforeach()

    target_sources(${target}
                   PUBLIC FILE_SET publicHeaders
                   TYPE HEADERS
                   BASE_DIRS
                   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sources>
                   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/Sources>
                   FILES ${${target}.PublicFiles}

                   PRIVATE
                   ${${target}.PrivateFiles}
                   ${${target}.AssemblerFiles}
                   ${${target}.ConfigurationTemplateFiles}
                   ${${target}.SourceFiles})

    # Assign source groups to the files.
    # For this to work, the files must reside in ${CMAKE_CURRENT_SOURCE_DIR} or in ${CMAKE_CURRENT_BINARY_DIR}.
    foreach (file ${allFiles})
      string_starts_with(isInSourceDir ${file} ${CMAKE_CURRENT_SOURCE_DIR}/Sources)
      string_starts_with(isInBinaryDir ${file} ${CMAKE_CURRENT_BINARY_DIR}/Sources)
      if (${isInSourceDir})
        source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Sources PREFIX "Sources" FILES ${file})
      elseif (${isInBinaryDir})
        source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/Sources PREFIX "Sources" FILES ${file})
      else()
        message(FATAL_ERROR "unknown/unsupported source directory: ${file}")
      endif()
    endforeach()

    set_property(TARGET ${target} PROPERTY C_STANDARD 17)

    target_link_libraries(${target} PRIVATE ${${target}.PrivateLibraries})
    target_link_libraries(${target} PUBLIC ${${target}.Libraries})

    if (${${target}.Type} STREQUAL library)

      target_include_directories(${target} PRIVATE ${${target}.IncludeDirectories})
      target_include_directories(${target} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sources> $<INSTALL_INTERFACE:${${target}.InstallationDirectories.Includes}>)
      target_include_directories(${target} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/Sources>)

    elseif (${${target}.Type} STREQUAL executable)

      target_include_directories(${target} PRIVATE ${${target}.IncludeDirectories})
      target_include_directories(${target} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sources> $<INSTALL_INTERFACE:${${target}.InstallationDirectories.Includes}>)
      target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/Sources)

      set_property(TARGET ${target} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${${target}.WorkingDirectory})

    elseif (${${target}.Type} STREQUAL test)

      target_include_directories(${target} PRIVATE ${${target}.IncludeDirectories})
      target_include_directories(${target} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sources> $<INSTALL_INTERFACE:${${target}.InstallationDirectories.Includes}>)
      target_include_directories(${target} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/Sources>)

      # Possible alternative is `add_test(NAME ${target} COMMAND ${target} WORKING_DIRECTORY $<TARGET_FILE_DIR:${target}>)`.
      add_test(NAME ${target} COMMAND ${target} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
      set_property(TARGET ${target} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${${target}.WorkingDirectory})

      # `add_test` must already have been invoked for `set_test_properties` to be successful.
      set_tests_properties(${target} PROPERTIES WORKING_DIRECTORY ${${target}.WorkingDirectory})

    else()
      message(FATAL_ERROR "unknown/unsupported product type")
    endif()
  endif()

  if( CMAKE_SIZEOF_VOID_P EQUAL 4 AND CMAKE_ASM_MASM_COMPILER)
    set_source_files_properties(${${target}.AssemblerFiles}}
                                PROPERTIES
                                COMPILE_FLAGS "/safeseh")
  endif()

  EndFileCopy()

  if (${target}.Install)


    if (${${target}.Type} STREQUAL library)

      install(TARGETS ${target}
              EXPORT ArcadiaTargets
              RUNTIME
                DESTINATION ${${target}.InstallationDirectories.Binaries}
              ARCHIVE
                DESTINATION ${${target}.InstallationDirectories.Archives}
              LIBRARY
                DESTINATION ${${target}.InstallationDirectory.Libraries}
              FILE_SET publicHeaders
                DESTINATION ${${target}.InstallationDirectories.Includes})

    elseif (${${target}.Type} STREQUAL executable)

      install(TARGETS ${target}
              RUNTIME DESTINATION ${${target}.InstallationDirectory}/Binaries)

    elseif (${${target}.Type} STREQUAL test)

    install(TARGETS ${target}
              RUNTIME DESTINATION ${${target}.InstallationDirectory}/Binaries)

    else()
      message(FATAL_ERROR "unknown/unsupported product type")
    endif()

  endif()

endmacro()


# @param target the target to which targetFile becomes a dependency to
# @param sourceFile the source file.
# @param targetFile the target file. to which the target file is a dependency
macro(MyCopyFile target sourceFile targetFile)
  list(APPEND ${target}.copyFiles.sources ${sourceFile})
  list(APPEND ${target}.copyFiles.targets ${targetFile})
endmacro()

# @remark Can only be used between `BeginProduct` and `EndProduct`.
# @todo Raise an error if the file does not exist.
macro(OnAssetsDirectory target directory)
 file(GLOB_RECURSE files RELATIVE ${directory} "${directory}/*")
 foreach (_file ${files})
   message(STATUS "(directory, file) := (${directory}, ${_file})")
   list(APPEND ${target}.Assets.Directories ${directory})
   list(APPEND ${target}.Assets.Files ${_file})
 endforeach()
endmacro()

# Define a template engine template file.
# The path is relative to `${CMAKE_CURRENT_SOURCE_DIR}/Sources`.
# If the optional argument `GENERATED` is supplied, then the path is relative to `${CMAKE_CURRENT_BINARY_DIR}/Sources`.
# @remark Can only be used between `BeginProduct` and `EndProduct`.
# @todo Raise an error if the file does not exist.
macro(OnTemplateEngineTemplateFile target file)
  set (extra_args ${ARGN})
  list(LENGTH extra_args extra_args_count)
  if (${extra_args_count} GREATER 0)
    list(GET extra_args 0 optional_arg)
  endif()

  if (GENERATED IN_LIST extra_args)
    cmake_path(SET _temporary ${CMAKE_CURRENT_BINARY_DIR}/Sources/${file})
    set_source_files_properties(${_temporary} PROPERTIES GENERATED ON)
  else()
    cmake_path(SET _temporary ${CMAKE_CURRENT_SOURCE_DIR}/Sources/${file})
    #set_source_files_properties(${_temporary} PROPERTIES GENERATED OFF)
  endif()

  list(APPEND ${target}.TemplateEngine.TemplateFiles ${file})

  unset(optional_arg)
endmacro()

# Can be used between `BeginProduct` and `EndProduct`.
macro(OnConfigurationFile target configurationFile configurationTemplateFile)
  configure_file(${configurationTemplateFile} ${configurationFile} @ONLY)
  list(APPEND ${target}.ConfigurationTemplateFiles ${configurationTemplateFile})
  list(APPEND ${target}.ConfigurationFiles ${configurationFile})
endmacro()

# Define an inlay file.
# The path is relative to `${CMAKE_CURRENT_SOURCE_DIR}/Sources`.
# If the optional argument `GENERATED` is supplied, then the path is relative to `${CMAKE_CURRENT_BINARY_DIR}/Sources`.
# @remark Can only be used between `BeginProduct` and `EndProduct`.
# @todo Raise an error if the file does not exist.
macro(OnInlayFile target file)
  set (extra_args ${ARGN})
  list(LENGTH extra_args extra_args_count)
  if (${extra_args_count} GREATER 0)
    list(GET extra_args 0 optional_arg)
  endif()

  if (GENERATED IN_LIST extra_args)
    cmake_path(SET _temporary ${CMAKE_CURRENT_BINARY_DIR}/Sources/${file})
    set_source_files_properties(${_temporary} PROPERTIES GENERATED ON)
  else()
    cmake_path(SET _temporary ${CMAKE_CURRENT_SOURCE_DIR}/Sources/${file})
    #set_source_files_properties(${_temporary} PROPERTIES GENERATED OFF)
  endif()

  if (PRIVATE IN_LIST extra_args)
    set_source_files_properties(${_temporary} PROPERTIES PRIVATE ON)
  endif()

  list(APPEND ${target}.InlayFiles ${_temporary})

  unset(optional_arg)
endmacro()


# Define a header file.
# The path is relative to `${CMAKE_CURRENT_SOURCE_DIR}/Sources`.
# If the optional argument `GENERATED` is supplied, then the path is relative to `${CMAKE_CURRENT_BINARY_DIR}/Sources`.
# @remark Can only be used between `BeginProduct` and `EndProduct`.
# @todo Raise an error if the file does not exist.
macro(OnHeaderFile target file)
  set (extra_args ${ARGN})
  list(LENGTH extra_args extra_args_count)
  if (${extra_args_count} GREATER 0)
    list(GET extra_args 0 optional_arg)
  endif()

  if (GENERATED IN_LIST extra_args)
    cmake_path(SET _temporary ${CMAKE_CURRENT_BINARY_DIR}/Sources/${file})
    set_source_files_properties(${_temporary} PROPERTIES GENERATED ON)
  else()
    cmake_path(SET _temporary ${CMAKE_CURRENT_SOURCE_DIR}/Sources/${file})
    #set_source_files_properties(${_temporary} PROPERTIES GENERATED OFF)
  endif()

  if (PRIVATE IN_LIST extra_args)
    set_source_files_properties(${_temporary} PROPERTIES PRIVATE ON)
  endif()

  list(APPEND ${target}.HeaderFiles ${_temporary})

  unset(optional_arg)
endmacro()

# Define a source file.
# The path is relative to `${CMAKE_CURRENT_SOURCE_DIR}/Sources`.
# If the optional argument `GENERATED` is supplied, then the path is relative to `${CMAKE_CURRENT_BINARY_DIR}/Sources`.
# @remark Can only be used between `BeginProduct` and `EndProduct`.
# @todo Raise an error if the file does not exist.
macro(OnSourceFile target file)
  set (extra_args ${ARGN})
  list(LENGTH extra_args extra_args_count)
  if (${extra_args_count} GREATER 0)
    list(GET extra_args 0 optional_arg)
  endif()

  if (GENERATED IN_LIST extra_args)
    cmake_path(SET _temporary ${CMAKE_CURRENT_BINARY_DIR}/Sources/${file})
    set_source_files_properties(${_temporary} PROPERTIES GENERATED ON)
  else()
    cmake_path(SET _temporary ${CMAKE_CURRENT_SOURCE_DIR}/Sources/${file})
    #set_source_files_properties(${_temporary} PROPERTIES GENERATED OFF)
  endif()

  if (PRIVATE IN_LIST extra_args)
    set_source_files_properties(${_temporary} PROPERTIES PRIVATE ON)
  endif()

  list(APPEND ${target}.SourceFiles ${_temporary})

  unset(optional_arg)
endmacro()

# Define a dependency to a module.
# @remark Can only be used between `BeginProduct` and `EndProduct`.
# @todo Raise an error if the file does not exist.
macro(OnModuleDependency target module)
  set (extra_args ${ARGN})
  list(LENGTH extra_args extra_args_count)
  if (${extra_args_count} GREATER 0)
    list(GET extra_args 0 optional_arg)
  endif()

  if (PRIVATE IN_LIST extra_args)
    message(STATUS "adding ${module} to ${target}.PrivateLibraries")
    list(APPEND ${target}.PrivateLibraries ${module})
  else()
    message(STATUS "adding ${module} to ${target}.Libraries")
    list(APPEND ${target}.Libraries ${module}) 
  endif()

  unset(optional_arg)
endmacro()
