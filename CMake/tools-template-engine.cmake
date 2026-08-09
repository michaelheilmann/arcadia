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

# Invoke the template engine with the following arguments.
# - `--source="${sourceFile}"`
# - `--target="${targetFile}"`
# - `--environment="${environmentFile}"`
#
# Furthermore
# - `${target}` is a target to which `${targetFile}` is added as a dependency to.
# - `${folder}` is only used in Visual Studio and can be left unspecified.
#    It is stored in `${target}.folder`. If specified then it will add generated vcxproj files in that folder in the IDE.

macro(BeginTemplateEngine target folder)

  if(NOT ${ARGC} EQUAL 2)
    message(FATAL_ERROR "BeginTemplateEngine: invalid number of arguments")
  endif()
  
  # Initialize the stack if it is not initialized.
  if (NOT g_templateEngineTargets)
    set(g_templateEngineTargets "")
  endif()
  
  # Push the file copy target on the stack of file copy targets.
  list(APPEND g_templateEngineTargets ${target})
  
  # File copy support.
  BeginFileCopy(${target})
  
  # Triples source, target, and environment files.
  set(${target}.templateEngine.sources "")
  set(${target}.templateEngine.targets "")
  set(${target}.templateEngine.environments "")
      
  # Store the target folder.
  set(${target}.folder ${folder})

endmacro()

macro(EndTemplateEngine)

  if(NOT ${ARGC} EQUAL 0)
    message(FATAL_ERROR "EndTemplatEngine: invalid number of arguments")
  endif()
  
  list(GET g_templateEngineTargets -1 target)
  
  list(LENGTH ${target}.templateEngine.sources l)
  math(EXPR l "${l} - 1")
  
  add_custom_target(${target} ALL)
  
  if (l GREATER -1)
    foreach (i RANGE ${l})
      list(GET ${target}.templateEngine.sources ${i} sourceFile)
      list(GET ${target}.templateEngine.targets ${i} targetFile)
      list(GET ${target}.templateEngine.environments ${i} environmentFile)
      
      cmake_path(IS_ABSOLUTE sourceFile isAbsolute)
      if (NOT isAbsolute)
        message(FATAL_ERROR "source file path is not absolute")
      endif()
      
      cmake_path(IS_ABSOLUTE targetFile isAbsolute)
      if (NOT isAbsolute)
        message(FATAL_ERROR "target file path is not absolute")
      endif()
    
      cmake_path(IS_ABSOLUTE environmentFile isAbsolute)
      if (NOT isAbsolute)
        message(FATAL_ERROR "environment file path is not absolute")
      endif()
      
      # Compute the path of the dependencies file from the source file.
      set(dependenciesFile "${sourceFile}")
      cmake_path(RELATIVE_PATH dependenciesFile BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE dependenciesFile)
      cmake_path(ABSOLUTE_PATH dependenciesFile BASE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} OUTPUT_VARIABLE dependenciesFile)
      cmake_path(APPEND_STRING dependenciesFile ".dependencies" OUTPUT_VARIABLE dependenciesFile)

      cmake_path(IS_ABSOLUTE dependenciesFile isAbsolute)
      if (NOT isAbsolute)
        message(FATAL_ERROR "dependencies file path is not absolute")
      endif()
      message(STATUS "dependencies file = ${dependenciesFile}")

      #message(STATUS " - ${target} copy file `${sourceFile}` to `${targetFile}")
      
      # Add custom command and custom target.
      add_custom_command(OUTPUT ${targetFile}
                         COMMAND $<TARGET_FILE:${MyProjectName}.TemplateEngine.CLI> --source="${sourceFile}" --target="${targetFile}" --environment="${environmentFile}" --dependencies="${dependenciesFile}"
                         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                         VERBATIM
                         DEPFILE "${dependenciesFile}"
                         COMMENT "${sourceFile} / ${environmentFile} => ${targetFile} / ${dependenciesFile}"
                         DEPENDS ${MyProjectName}.TemplateEngine.CLI ${sourceFile} ${environmentFile})
      set_source_files_properties(${targetFile} PROPERTIES GENERATED 1)
      string(RANDOM LENGTH 64 randomLength)
      string(UUID generatedTarget NAMESPACE "15340915-a7be-4950-9d26-b789c0eaf106" NAME ${randomLength} TYPE SHA1)
      set(generatedTarget template-engine-${generatedTarget})
      
      add_custom_target(${generatedTarget} DEPENDS ${targetFile})
      # Add the custom target to the enclosing target.
      add_dependencies(${target} ${generatedTarget})
      # Set the folder.
      if (${target}.folder)
        set_target_properties(${generatedTarget} PROPERTIES FOLDER ${${target}.folder})
        set_target_properties(${target} PROPERTIES FOLDER ${${target}.folder})
      endif()
    endforeach()
  endif()

  EndFileCopy()
  
  list(POP_BACK g_templateEngineTargets)

endmacro()

macro(OnTemplateEngine sourceFile targetFile environmentFile)

  if(NOT ${ARGC} EQUAL 3)
    message(FATAL_ERROR "OnTemplateEngine: invalid number of arguments")
  endif()

  list(GET g_templateEngineTargets -1 target)
  
  
  list(APPEND ${target}.templateEngine.sources ${sourceFile})
  list(APPEND ${target}.templateEngine.targets ${targetFile})
  list(APPEND ${target}.templateEngine.environments ${environmentFile})

endmacro()
