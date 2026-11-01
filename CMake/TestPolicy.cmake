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

include_guard(GLOBAL)

# Labels describe resources/coverage, not scheduling permission. Ordinary cases
# observed below one second get a generous 60-second watchdog. Nested configure/
# build infrastructure uses separately declared limits (180/300 seconds).
function(Arcadia_prepareTestPolicy target)
  set(labels "${${target}.TestLabels}")
  if(NOT labels)
    set(labels unit)
    if(${target}.TestIsolated OR target MATCHES "\\.(FontIO|Engine|Media|ADL|FileSystem|PixelBufferIO|SampleBufferIO)\\.Tests\\."
        OR target MATCHES "\\.PDLC\\.Tests\\.(Backend|CrossModules|ResourceModels|SemanticalAnalysis)$"
        OR target MATCHES "\\.Tools\\.TemplateEngine\\.Tests\\.")
      set(labels integration)
    endif()
    if(target MATCHES "\\.FontIO\\.Tests\\.")
      list(APPEND labels requires-font)
    elseif(target MATCHES "\\.VPL\\.Tests\\.GLSL\\.Driver$")
      set(labels integration requires-graphics)
    endif()
  endif()
  list(REMOVE_DUPLICATES labels)
  list(SORT labels)
  set(${target}.TestLabels "${labels}" PARENT_SCOPE)
  if(NOT DEFINED ${target}.TestTimeout OR "${${target}.TestTimeout}" STREQUAL "")
    set(${target}.TestTimeout 60 PARENT_SCOPE)
  endif()
endfunction()
