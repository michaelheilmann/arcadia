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
include("${POLICY_MODULE}")
function(check target expected)
  Arcadia_prepareTestPolicy(${target})
  if(NOT "${${target}.TestLabels}" STREQUAL "${expected}" OR NOT "${${target}.TestTimeout}" STREQUAL "60")
    message(FATAL_ERROR "Incorrect default policy for ${target}: ${${target}.TestLabels}/${${target}.TestTimeout}")
  endif()
endfunction()
check(Arcadia.Ring1.Tests.SafeAdd unit)
check(Arcadia.FontIO.Tests.CreateBitmapFontTests "integration;requires-font")
check(Arcadia.FontIO.Tests.CreateAtlasBitmapFontTests "integration;requires-font")
check(Arcadia.VPL.Tests.GLSL.Driver "integration;requires-graphics")
check(Arcadia.PDLC.Tests.SemanticalAnalysis integration)
check(Arcadia.Engine.Tests.Nodes integration)
check(Arcadia.Tools.TemplateEngine.Tests.HelloWorld integration)
set(Custom.TestIsolated TRUE)
check(Custom integration)
set(Custom.TestLabels slow integration slow)
set(Custom.TestTimeout 45)
Arcadia_prepareTestPolicy(Custom)
if(NOT Custom.TestLabels STREQUAL "integration;slow" OR NOT Custom.TestTimeout EQUAL 45)
  message(FATAL_ERROR "Explicit labels/timeout were not preserved and canonicalized")
endif()
message(STATUS "Default resource labels, isolated classification and explicit timeout/label overrides verified")
