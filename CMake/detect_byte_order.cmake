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

# "Unknown" Byte order.
set(Arcadia_ByteOrder_Unknown 0)

# "Big Endian" Byte order.
set(Arcadia_ByteOrder_BigEndian 1)

# "Little Endian" Byte order.
set(Arcadia_ByteOrder_LittleEndian 2)

function(Arcadia_DetectByteOrder result)
  if (CMAKE_C_BYTE_ORDER STREQUAL "BIG_ENDIAN")
    set(${result} ${Arcadia_ByteOrder_BigEndian} PARENT_SCOPE) 
  elseif (CMAKE_C_BYTE_ORDER STREQUAL "LITTLE_ENDIAN")
    set(${result} ${Arcadia_ByteOrder_LittleEndian} PARENT_SCOPE)   
  else()
    set(${result} ${Arcadia_ByteOrder_Unknown} PARENT_SCOPE) 
  endif()
endfunction()

# SUMMARY
# Define an enumeration of Byte orders and detect the Byte order.
#
# DETAIL
# Define an enumeration of Byte orders and detect the Byte order.
# The detailed steps are:
# a) The numeration constants ${target}_ByteOrder_(LittleEndian|BigEndian|Unknown) are defined.
#    Each constant identifies a Byte order.
# b) The constant ${target}_ByteOrder is set to the {target}_ByteOrder_* values denoting the detected Byte order.
# PARAM target The target.
macro(DetectByteOrder target)
  set(${target}_ByteOrder_Unknown ${Arcadia_ByteOrder_Unknown})
  set(${target}_ByteOrder_BigEndian ${Arcadia_ByteOrder_BigEndian})
  set(${target}_ByteOrder_LittleEndian ${Arcadia_ByteOrder_LittleEndian})
  
  # Initialize if not yet initialized.
  if (NOT DEFINED ${target}_ByteOrder)
    Arcadia_DetectByteOrder(${target}_ByteOrder)
  endif()
  
  set(VERBOSE TRUE)
  
  if (VERBOSE) 
    if (${target}_ByteOrder STREQUAL ${target}_ByteOrder_Unknown)
      message( STATUS " - ${target} Byte order: unknown")
    elseif (${target}_ByteOrder STREQUAL ${target}_ByteOrder_BigEndian)
      message( STATUS " - ${target} Byte order: big endian")
    elseif (${target}_ByteOrder STREQUAL ${target}_ByteOrder_LittleEndian)
      message( STATUS " - ${target} Byte order: little endian")
    else()
      message(FATAL_ERROR "<internal error>") 
    endif()
  endif()

endmacro()
