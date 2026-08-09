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

# SUMMARY
# Configure warnings and errors for a static libraries, module libraries, and executables.
# 
# DETAIL
# Configure warnings and errors for a static libraries, module libraries, and executables.
# For GCC:  Turn several warnings into errors.
# For MSVC: Turn several warnings into errors.
#
# PARAM target The target.
macro(ConfigureWarningsAndErrors target)
  if (NOT DEFINED ${target}_Compiler_C)
    message(FATAL_ERROR "please execute DetectCompiler before ConfigureWarningsAndErrors")
  endif()
  if (${${target}_Compiler_C} EQUAL ${${target}_Compiler_C_GCC})
    target_compile_options(${target} BEFORE PRIVATE "-Werror=implicit-function-declaration")
  endif()

  if(${${target}_Compiler_C} EQUAL ${${target}_Compiler_C_MSVC})
    set(${target}.CompileOptions "")
    # C4090:  'opertion' : different 'modifier' qualifiers
    list(APPEND ${target}.CompileOptions "/we4090")
    # C4133: 'function' incompatible types - from 'type1' to 'type2'
    list(APPEND ${target}.CompileOptions "/we4133")
    # C4020_ 'function': too many actual parameters
    list(APPEND ${target}.CompileOptions "/we4020")
    # C4013: 'function': undefined; assuming extern returning int
    list(APPEND ${target}.CompileOptions "/we4013")
    # C4024: 'function': different types for formal and actual parameter 'number'
    list(APPEND ${target}.CompileOptions "/we4024")
    # C4716: 'function' must return a value
    list(APPEND ${target}.CompileOptions "/we4716")
    # C4013: 'function' undefined; assuming extern returning int
    list(APPEND ${target}.CompileOptions "/we4013")
    # C4028: formal parameter 'number' different from declaration
    list(APPEND ${target}.CompileOptions "/we4028")

    # C4189: 'identifier' : local variable is initialized but not referenced
    list(APPEND ${target}.CompileOptions "/we4189")
    
    # C4113: 'identifier1' differs in parameter lists from 'identifier2'
    list(APPEND ${target}.CompileOptions "/we4113")
    
    # C4002: 'function' : pointer mismatch for actual parameter 'number'
    list(APPEND ${target}.CompileOptions "/we4002")
    
    # C4047: 'operator' : 'identifier1' differs in levels of indirection from 'identifier2'
    list(APPEND ${target}.CompileOptions "/we4047")
    
    # C4029: declared formal parameter list different from definition
    list(APPEND ${target}.CompileOptions "/we4029")
    
    # C4717: 'function' : recursive on all control paths, function will cause runtime stack overflow
    list(APPEND ${target}.CompileOptions "/we4717")

    # The quotes around "${${target}.CompileOptions}" concatenate the list elements to a single string separated by semicolons.
    set_source_files_properties(${${target}.SourceFiles} PROPERTIES COMPILE_OPTIONS "${${target}.CompileOptions}")
    set_source_files_properties(${${target}.HeaderFiles} PROPERTIES COMPILE_OPTIONS "${${target}.CompileOptions}")

    # warning level 4
    target_compile_options(${target} PRIVATE /W1)

  endif()

endmacro()
