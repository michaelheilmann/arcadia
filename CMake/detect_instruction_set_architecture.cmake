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

# An unknown instruction set architecture.
set(Arcadia_InstructionSetArchitecture_Unknown 0)

# The x86 instruction set architecture.
set(Arcadia_InstructionSetArchitecture_X86 1)

# The x64 instruction set architecture.
set(Arcadia_InstructionSetArchitecture_X64 2)

# SUMMARY
# Detect the instruction set architecture.
#
# DETAIL
# Detect the instruction set architecture.
# The variable named by ${result} in the parent scope is assigned one of ${Arcadia_InstructionSetArchitecture_X86}, ${Arcadia_InstructionSetArchitecture_X64}, or ${Arcadia_InstructionSetArchitecture_Unknown}. 
#
# PARAM result The name of the variable to assign in the parent scope the instruction set architecture to.
function(Arcadia_DetectInstructionSetArchitecture result)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(${result} ${Arcadia_InstructionSetArchitecture_X64} PARENT_SCOPE)
  elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(${result} ${Arcadia_InstructionSetArchitecture_X86} PARENT_SCOPE)
  else()
    set(${result} ${Arcadia_InstructionSetArchitecture_Unknown} PARENT_SCOPE)
  endif()
endfunction()

# SUMMARY
# Define an enumeration of instruction set architectures and detect the instruction set architecture.
#
# DETAIL
# Define an enumeration of instruction set architectures and detect the instruction set architecture.
# The detailed steps are:
# a) Define an enumeration of instruction set architectures.
#    The enumeration constants ${target}_InstructionSetArchitecture_(Unknown|X86|X64) are defined.
#    Each constant is a string of an unique name identifying an instruction set architecture.
# b) The constant ${target}_InstructionSetArchitecture is defined to the ${target}_InstructionSetArchitecture_* value denoting the detected instruction set architecture.
#
# PARAM target The target.
macro(DetectInstructionSetArchitecture target)
  set(${target}_InstructionSetArchitecture_Unknown ${Arcadia_InstructionSetArchitecture_Unknown})

  set(${target}_InstructionSetArchitecture_X86 ${Arcadia_InstructionSetArchitecture_X86})

  set(${target}_InstructionSetArchitecture_X64 ${Arcadia_InstructionSetArchitecture_X64})

  # Initialize if not yet initialized.
  if (NOT DEFINED ${target}_InstructionSetArchitecture)
    Arcadia_DetectInstructionSetArchitecture(${target}_InstructionSetArchitecture)
  endif()
  set(VERBOSE FALSE)
  if (VERBOSE)
    if (${target}_InstructionSetArchitecture STREQUAL ${target}_InstructionSetArchitecture_Unknown)
      message( STATUS " - ${target} instruction set architecture: unknown")
    elseif (${target}_InstructionSetArchitecture STREQUAL ${target}_InstructionSetArchitecture_X64)
      message( STATUS " - ${target} instruction set architecture: X64")
    elseif (${target}_InstructionSetArchitecture STREQUAL ${target}_InstructionSetArchitecture_X86)
      message( STATUS " - ${target} instruction set architecture: X64")
    else()
      message(FATAL_ERROR "<internal error>")
    endif()
  endif()
endmacro()
