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

#if !defined(ARCADIA_DDLS_TESTS_VALIDATION_DEFAULT_H_INCLUDED)
#define ARCADIA_DDLS_TESTS_VALIDATION_DEFAULT_H_INCLUDED

#include "Arcadia/DDLS/Include.h"
#include "Arcadia/DDL/Include.h"

void
Arcadia_DDLS_Tests_Validation_test1
  (
    Arcadia_Thread* thread
  );

// "$.type" not defined
void
Arcadia_DDLS_Tests_Validation_testRejectTypeNotDefined
  (
    Arcadia_Thread* thread
  );

// "$.red" not defined
void
Arcadia_DDLS_Tests_Validation_testRejectRedNotDefined
  (
    Arcadia_Thread* thread
  );

// "$.green" not defined
void
Arcadia_DDLS_Tests_Validation_testRejectGreenNotDefined
  (
    Arcadia_Thread* thread
  );

// "$.blue" not defined
void
Arcadia_DDLS_Tests_Validation_testRejectBlueNotDefined
  (
    Arcadia_Thread* thread
  );

// "$.type" is defined twice
void
Arcadia_DDLS_Tests_Validation_testRejectTypeDefinedTwice
  (
    Arcadia_Thread* thread
  );

// "$.red" is defined twice
void
Arcadia_DDLS_Tests_Validation_testRejectRedDefinedTwice
  (
    Arcadia_Thread* thread
  );

// "$.green" is defined twice
void
Arcadia_DDLS_Tests_Validation_testRejectGreenDefinedTwice
  (
    Arcadia_Thread* thread
  );

// "$.blue" is defined twice
void
Arcadia_DDLS_Tests_Validation_testRejectBlueDefinedTwice
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_DDLS_TESTS_VALIDATION_DEFAULT_H_INCLUDED
