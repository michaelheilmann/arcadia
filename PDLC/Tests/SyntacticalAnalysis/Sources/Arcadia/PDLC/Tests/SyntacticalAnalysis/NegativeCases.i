// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
// details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

SYNTAX(UnexpectedWord, "Arcadia.PDLC.Diagnostics.Syntactical.UnexpectedWordDiagnostic", "Assets/UnexpectedWord.pdl", 0,
  "syntactical error: received <name>\n")
SYNTAX(UnexpectedInstructionSeparator, "Arcadia.PDLC.Diagnostics.Syntactical.UnexpectedWordDiagnostic", "Assets/UnexpectedInstructionSeparator.pdl", 42,
  "syntactical error: received `,`\n")
SYNTAX(UnexpectedMissingArgument, "Arcadia.PDLC.Diagnostics.Syntactical.UnexpectedWordDiagnostic", "Assets/UnexpectedMissingArgument.pdl", 51,
  "syntactical error: received <line terminator>\n")
SYNTAX(InvalidNativeHeaderEmpty, "Arcadia.PDLC.Diagnostics.InvalidPathDiagnostic", NULL, ARCADIA_PDLC_TESTS_NO_POSITION,
  "error: string `` is not a valid path\n")
SYNTAX(InvalidNativeHeaderAbsolute, "Arcadia.PDLC.Diagnostics.InvalidPathDiagnostic", NULL, ARCADIA_PDLC_TESTS_NO_POSITION,
  "error: string `/NativeResources/External.h` is not a valid path\n")
SYNTAX(InvalidNativeHeaderTraversal, "Arcadia.PDLC.Diagnostics.InvalidPathDiagnostic", NULL, ARCADIA_PDLC_TESTS_NO_POSITION,
  "error: string `NativeResources/../External.h` is not a valid path\n")
SYNTAX(InvalidNativeHeaderBackslash, "Arcadia.PDLC.Diagnostics.InvalidPathDiagnostic", NULL, ARCADIA_PDLC_TESTS_NO_POSITION,
  "error: string `NativeResources\\External.h` is not a valid path\n")
SYNTAX(InvalidNativeHeaderQuote, "Arcadia.PDLC.Diagnostics.InvalidPathDiagnostic", NULL, ARCADIA_PDLC_TESTS_NO_POSITION,
  "error: string `NativeResources/External\".h` is not a valid path\n")
SYNTAX(InvalidNonNativeHeader, "Arcadia.PDLC.Diagnostics.Syntactical.UnexpectedWordDiagnostic", "Assets/InvalidNonNativeHeader.pdl", 0,
  "syntactical error: received <string literal>\n")
SYNTAX(InvalidNativeEnumerationPath, "Arcadia.PDLC.Diagnostics.InvalidPathDiagnostic", NULL, ARCADIA_PDLC_TESTS_NO_POSITION,
  "error: string `../External.h` is not a valid path\n")
SYNTAX(NestedEnumeration, "Arcadia.PDLC.Diagnostics.Syntactical.UnexpectedWordDiagnostic", "Assets/NestedEnumeration.pdl", 26,
  "syntactical error: received `enumeration`\n")
SYNTAX(InvalidNativeImplementationEnum, "Arcadia.PDLC.Diagnostics.Syntactical.UnexpectedWordDiagnostic", "Assets/InvalidNativeImplementationEnum.pdl", 42,
  "syntactical error: received `enumeration`\n")
