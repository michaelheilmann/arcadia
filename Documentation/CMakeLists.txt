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

BeginTemplateEngine(${MyProjectName}.Website.Generate "Website")

  # Build "index.html".
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/index.html.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/index.html"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")

  # Build "error.html".
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/error.html.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/error.html"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")

  # Build "index.css".
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/index.css.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/assets/index.css"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")

  # Build "reset.css".
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/reset.css.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/assets/reset.css"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")

  # Build "cxx-docs.css".
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/cxx-doc.css.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/assets/cxx-doc.css"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")

  # Build "about.html".
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/about.html.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/about.html"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")

  # Build "imprint.html".
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/imprint.html.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/imprint.html"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")

  # Build "data-protection-statement.html".
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/data-protection-statement.html.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/data-protection-statement.html"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")

  # Build "sitemap.xml" and ".htaccess".
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/sitemap.xml.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/sitemap.xml"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/.htaccess.te"
                    "${REPOSITORY_SOURCE_DIR}/.Website/.htaccess"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")

EndTemplateEngine()

add_subdirectory("Arcadia")
add_subdirectory("Specifications/Common Lexical Translations")
add_subdirectory("Specifications/MDNP")
add_subdirectory("Specifications/Data Definition Language")
add_subdirectory("Specifications/Data Definition Language Schema")
add_subdirectory("Specifications/MIL")
