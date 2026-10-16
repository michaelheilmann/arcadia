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

foreach(required INPUT_DIRECTORY OUTPUT_FILE TEMPLATE_FILE)
  if(NOT DEFINED ${required})
    message(FATAL_ERROR "EmbedPrograms: ${required} is required")
  endif()
endforeach()

set(names MeshAmbientColor VertexAmbientColor TextureAmbientColor TextureVertexAmbientColor)
set(files mesh-ambient-color.vpl vertex-ambient-color.vpl texture-ambient-color.vpl texture-vertex-ambient-color.vpl)
set(VPL_BUILTIN_PROGRAM_DATA "")
foreach(index RANGE 0 3)
  list(GET names ${index} name)
  list(GET files ${index} file)
  file(READ "${INPUT_DIRECTORY}/${file}" bytes HEX)
  string(APPEND VPL_BUILTIN_PROGRAM_DATA "static const unsigned char _Arcadia_VPL_BuiltinPrograms_${name}[] = {\n")
  string(LENGTH "${bytes}" length)
  if(length EQUAL 0)
    message(FATAL_ERROR "EmbedPrograms: ${file} is empty")
  endif()
  math(EXPR last "${length} - 1")
  foreach(offset RANGE 0 ${last} 128)
    string(SUBSTRING "${bytes}" ${offset} 128 chunk)
    string(REGEX REPLACE "([0-9a-fA-F][0-9a-fA-F])" "0x\\1," chunk "${chunk}")
    string(APPEND VPL_BUILTIN_PROGRAM_DATA "  ${chunk}\n")
  endforeach()
  string(APPEND VPL_BUILTIN_PROGRAM_DATA "};\n\n")
endforeach()

cmake_path(GET OUTPUT_FILE PARENT_PATH output_directory)
file(MAKE_DIRECTORY "${output_directory}")
configure_file("${TEMPLATE_FILE}" "${OUTPUT_FILE}" @ONLY)
# Keep the build rule up to date even if an input was touched without changing its bytes.
file(TOUCH "${OUTPUT_FILE}")
