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

#if !defined(ARCADIA_TESTING_INCLUDE_H_INCLUDED)
#define ARCADIA_TESTING_INCLUDE_H_INCLUDED

#include <stddef.h>

// Reserved process/callback result for an unavailable optional prerequisite.
// CTest test products register SKIP_RETURN_CODE 77. Never use it for an assertion,
// corrupt input, or an unexpected initialization failure after preflight.
#define Arcadia_Testing_Skip (77)

typedef struct Arcadia_Testing_Case Arcadia_Testing_Case;

struct Arcadia_Testing_Case {
  const char* name;
  int (*run)(void);
};

// Test-only library: production libraries must not depend on Testing.
// Each case keeps its own safeExecute/process boundary. With no selector,
// execute every case even if an earlier case reports failure.
// --case NAME selects exactly one case (case-sensitive). --list writes one name
// per line in declaration order without executing callbacks or progress logs.
// The whole table must be nonempty and valid in every mode: names are nonempty,
// unique (case-sensitive), contain no CR/LF, and callbacks are non-null. Tables
// and names must remain valid throughout the call. Invalid suites/arguments and
// callback failures return EXIT_FAILURE. Code Arcadia_Testing_Skip reports SKIPPED
// and continues; all selected cases skipped returns 77, a mixture of passes/skips
// returns EXIT_SUCCESS, and any failure takes precedence over both. Successful
// listing returns EXIT_SUCCESS even if execution would skip every case.
// Initialization performed by a caller before dispatch is outside
// the runner's side-effect-free discovery contract.
// For CTest discovery, callers must bypass backend/font/fixture setup in --list
// mode too. Normal callbacks own setup/cleanup and must catch runtime jumps
// before returning across this boundary; the runner does not catch exceptions.
// During execution the runner scopes Ring1's borrowed OS-thread-local case name
// around each callback and restores the previous name, including for nested
// suites and ordinary failures. This name survives callback-owned process
// teardown; listing does not change it. Case discovery does not imply parallel
// safety or automatic isolation of direct executable invocation.
int
Arcadia_Testing_runSuite
  (
    int argc,
    char** argv,
    const Arcadia_Testing_Case* cases,
    size_t count
  );

#endif // ARCADIA_TESTING_INCLUDE_H_INCLUDED
