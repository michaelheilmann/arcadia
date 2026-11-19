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

# Build the documentation.
BeginTemplateEngine(${MyProjectName}.ADL.BuildDocs "ADL")

  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/index.html.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/Arcadia/ADL/index.html"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Introduction/index.html.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/Arcadia/ADL/Introduction/index.html"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Concepts-and-Terms/index.html.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/Arcadia/ADL/Concepts-and-Terms/index.html"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Module-Interface/index.html.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/Arcadia/ADL/Module-Interface/index.html"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/Specification/index.html.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/Arcadia/ADL/Specification/index.html"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")

  # Copy standalone diagrams referenced by the ADL specification.
  MyCopyFile(${MyProjectName}.ADL.BuildDocs
             "${CMAKE_CURRENT_SOURCE_DIR}/Specification/images/stack-orientation.svg"
             "${REPOSITORY_SOURCE_DIR}/.Website/Arcadia/ADL/Specification/images/stack-orientation.svg")
  MyCopyFile(${MyProjectName}.ADL.BuildDocs
             "${CMAKE_CURRENT_SOURCE_DIR}/Specification/images/cross-alignment.svg"
             "${REPOSITORY_SOURCE_DIR}/.Website/Arcadia/ADL/Specification/images/cross-alignment.svg")
  MyCopyFile(${MyProjectName}.ADL.BuildDocs
             "${CMAKE_CURRENT_SOURCE_DIR}/Specification/images/panel-size-policy.svg"
             "${REPOSITORY_SOURCE_DIR}/.Website/Arcadia/ADL/Specification/images/panel-size-policy.svg")
  MyCopyFile(${MyProjectName}.ADL.BuildDocs
             "${CMAKE_CURRENT_SOURCE_DIR}/Specification/images/slot-visibility.svg"
             "${REPOSITORY_SOURCE_DIR}/.Website/Arcadia/ADL/Specification/images/slot-visibility.svg")
  MyCopyFile(${MyProjectName}.ADL.BuildDocs
             "${CMAKE_CURRENT_SOURCE_DIR}/Specification/images/weighted-sizing.svg"
             "${REPOSITORY_SOURCE_DIR}/.Website/Arcadia/ADL/Specification/images/weighted-sizing.svg")
  MyCopyFile(${MyProjectName}.ADL.BuildDocs
             "${CMAKE_CURRENT_SOURCE_DIR}/Specification/images/stack-spacing.svg"
             "${REPOSITORY_SOURCE_DIR}/.Website/Arcadia/ADL/Specification/images/stack-spacing.svg")

EndTemplateEngine()