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

BeginTemplateEngine(${MyProjectName}.Website.Arcadia.EngineConcepts "Website/Arcadia")

  # Build "index.html".
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/index.html.te"
                   "${REPOSITORY_SOURCE_DIR}/.Build/Arcadia/engine-concepts/index.html"
                   "${CMAKE_CURRENT_SOURCE_DIR}/../../website.env")

EndTemplateEngine()
