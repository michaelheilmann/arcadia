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

#if !defined(ARCADIA_PDLC_TESTS_INCLUDE_H_INCLUDED)
#define ARCADIA_PDLC_TESTS_INCLUDE_H_INCLUDED

#include "Arcadia/PDLC/Include.h"
#include <stdint.h>

#define ARCADIA_PDLC_TESTS_NO_POSITION SIZE_MAX

typedef struct Arcadia_PDLC_Tests_ExpectedDiagnostic {
  const char* fixture;
  const char* typeName;
  // Component-boundary suffix of the normalized generic path, or NULL for no file.
  const char* fileSuffix;
  // NO_POSITION requires an absent position, not a wildcard.
  Arcadia_SizeValue offset;
  // Exact message; FileSystemDiagnostic uses its reason field (path checked separately).
  const char* message;
  Arcadia_Status status;
} Arcadia_PDLC_Tests_ExpectedDiagnostic;

// Test-only exact contract: one Error diagnostic with the expected runtime class,
// message/reason, file suffix/presence and byte position/presence, plus status.
// ValueRaised also requires an actual CompilationFailedException raised value.
// Capture status at the original jump boundary before clearing thread status for
// inspection; keep the raised value and diagnostic/context roots alive until
// validation. NO_POSITION/NULL file expectations require absence, not wildcards.
// Ordinary mismatches return false without loading source files or line maps.
Arcadia_BooleanValue
Arcadia_PDLC_Tests_matchesDiagnostic
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics,
    Arcadia_Status status,
    const Arcadia_PDLC_Tests_ExpectedDiagnostic* expected
  );

// On mismatch, render expected/actual metadata without source-line I/O, restore
// the helper's stack frame, then raise TestFailed. Diagnostic reporting must not
// replace the mismatch with an unrelated file-loading error. See the reviewed
// NegativeCases.i fixtures and Diagnostics/ExpectedDiagnostics checker tests.
void
Arcadia_PDLC_Tests_assertDiagnostic
  (
    Arcadia_Thread* thread,
    Arcadia_Languages_Diagnostics* diagnostics,
    Arcadia_Status status,
    const Arcadia_PDLC_Tests_ExpectedDiagnostic* expected
  );

#endif // ARCADIA_PDLC_TESTS_INCLUDE_H_INCLUDED
