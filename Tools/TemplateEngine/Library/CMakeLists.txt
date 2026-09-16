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

set(this ${MyProjectName}.TemplateEngine)

if (NOT TARGET ${this})

  BeginProduct(${this} library)

  SetFolder(${this} "TemplateEngine")

  OnConfigurationFile(${this} ${CMAKE_CURRENT_BINARY_DIR}/Sources/Arcadia.TemplateEngine/Configure.h ${CMAKE_CURRENT_SOURCE_DIR}/Sources/Arcadia.TemplateEngine/Configure.h.i)

  OnSourceFile(${this} Arcadia.TemplateEngine/Diagnostics/UnexpectedSymbolDiagnostic.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/Diagnostics/UnexpectedSymbolDiagnostic.h)
  OnSourceFile(${this} Arcadia.TemplateEngine/Diagnostics/UnexpectedWordDiagnostic.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/Diagnostics/UnexpectedWordDiagnostic.h)

  OnSourceFile(${this} Arcadia.TemplateEngine/Diagnostics/UnclosedStringLiteralDiagnostic.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/Diagnostics/UnclosedStringLiteralDiagnostic.h)
  OnSourceFile(${this} Arcadia.TemplateEngine/Diagnostics/UnclosedStringLiteralDiagnosticContext.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/Diagnostics/UnclosedStringLiteralDiagnosticContext.h)

  OnSourceFile(${this} Arcadia.TemplateEngine/Scanner/Scanner.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/Scanner/Scanner.h)
  OnSourceFile(${this} Arcadia.TemplateEngine/Scanner/WordType.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/Scanner/WordType.h)

  OnSourceFile(${this} Arcadia.TemplateEngine/Parser/Parser.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/Parser/Parser.h)
  OnSourceFile(${this} Arcadia.TemplateEngine/Parser/Tree.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/Parser/Tree.h)
  OnSourceFile(${this} Arcadia.TemplateEngine/Parser/TreeType.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/Parser/TreeType.h)

  OnSourceFile(${this} Arcadia.TemplateEngine/Context.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/Context.h)
  OnSourceFile(${this} Arcadia.TemplateEngine/Environment.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/Environment.h)
  OnSourceFile(${this} Arcadia.TemplateEngine/FileContext.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/FileContext.h)
  OnSourceFile(${this} Arcadia.TemplateEngine/Library.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/Library.h)
  OnSourceFile(${this} Arcadia.TemplateEngine/DependenciesContext.c)
  OnHeaderFile(${this} Arcadia.TemplateEngine/DependenciesContext.h)

  OnModuleDependency(${this} ${MyProjectName}.DDL)
  OnModuleDependency(${this} ${MyProjectName}.Ring2)

  EndProduct(${this})

endif()
