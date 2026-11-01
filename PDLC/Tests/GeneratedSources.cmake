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

function(Arcadia_markGeneratedTestSources)
  # Source properties do not expand $<CONFIG>. Set LANGUAGE on the actual paths,
  # otherwise VS lists .c.g outputs as None instead of compiling them.
  foreach(source IN LISTS ARGN)
    if(CMAKE_CONFIGURATION_TYPES)
      foreach(configuration IN LISTS CMAKE_CONFIGURATION_TYPES)
        string(REPLACE "$<CONFIG>" "${configuration}" path "${source}")
        set_source_files_properties("${path}" PROPERTIES GENERATED TRUE LANGUAGE C)
      endforeach()
    else()
      string(REPLACE "$<CONFIG>" "${CMAKE_BUILD_TYPE}" path "${source}")
      set_source_files_properties("${path}" PROPERTIES GENERATED TRUE LANGUAGE C)
    endif()
  endforeach()
endfunction()
