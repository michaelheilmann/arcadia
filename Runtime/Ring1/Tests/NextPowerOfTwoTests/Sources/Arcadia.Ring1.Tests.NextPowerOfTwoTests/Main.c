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

#include <stdlib.h>

#include "Arcadia/Ring1/Include.h"

#define EXPECT_SUCCESS(TYPE, SUFFIX, FLAVOR, INPUT, OUTPUT) \
  { \
    TYPE##Value x = Arcadia_nextPowerOfTwo##FLAVOR##SUFFIX##Value(thread, INPUT); \
    TYPE##Value y = OUTPUT; \
    if (x != y) { \
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed); \
      Arcadia_Thread_jump(thread); \
    } \
  }

#define EXPECT_FAILURE(TYPE, SUFFIX, FLAVOR, INPUT) \
  { \
    Arcadia_JumpTarget jumpTarget; \
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget); \
    if (Arcadia_JumpTarget_save(&jumpTarget)) { \
      Arcadia_nextPowerOfTwo##FLAVOR##SUFFIX##Value(thread, INPUT); \
      Arcadia_Thread_popJumpTarget(thread); \
      Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed); \
      Arcadia_Thread_jump(thread); \
    } else { \
      if (Arcadia_Thread_getStatus(thread) != Arcadia_Status_NotExists) { \
        Arcadia_Thread_popJumpTarget(thread); \
        Arcadia_Thread_setStatus(thread, Arcadia_Status_TestFailed); \
        Arcadia_Thread_jump(thread); \
      } else { \
        Arcadia_Thread_setStatus(thread, Arcadia_Status_Success); \
      } \
    } \
  }

static void
nextPowerOfTwoNatural64ValueTests
  (
    Arcadia_Thread* thread
  )
{
  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThan, 0, 1);
  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThanOrEqualTo, 0, 1);

  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThan, 1, 2);
  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThanOrEqualTo, 1, 1);

  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThanOrEqualTo, 2, 2);
  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThan, 2, 4);

  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThanOrEqualTo, 3, 4);
  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThan, 3, 4);

  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThanOrEqualTo, 4, 4);
  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThan, 4, 8);

  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThanOrEqualTo, 5, 8);
  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThan, 5, 8);

  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThanOrEqualTo, 6, 8);
  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThan, 6, 8);

  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThanOrEqualTo, 7, 8);
  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThan, 7, 8);

  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThanOrEqualTo, 8, 8);
  EXPECT_SUCCESS(Arcadia_Natural64, Natural64, GreaterThan, 8, 16);

  // ...

  EXPECT_FAILURE(Arcadia_Natural64, Natural64, GreaterThanOrEqualTo, Arcadia_Natural64Value_Maximum - 1);
  EXPECT_FAILURE(Arcadia_Natural64, Natural64, GreaterThan, Arcadia_Natural64Value_Maximum - 1);

  EXPECT_FAILURE(Arcadia_Natural64, Natural64, GreaterThanOrEqualTo, Arcadia_Natural64Value_Maximum - 0);
  EXPECT_FAILURE(Arcadia_Natural64, Natural64, GreaterThan, Arcadia_Natural64Value_Maximum - 0);
}

static void
nextPowerOfTwoNatural32ValueTests
  (
    Arcadia_Thread* thread
  )
{
  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThanOrEqualTo, 0, 1);
  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThan, 0, 1);

  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThanOrEqualTo, 1, 1);
  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThan, 1, 2);

  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThanOrEqualTo, 2, 2);
  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThan, 2, 4);

  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThanOrEqualTo, 3, 4);
  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThan, 3, 4);

  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThanOrEqualTo, 4, 4);
  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThan, 4, 8);

  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThanOrEqualTo, 5, 8);
  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThan, 5, 8);

  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThanOrEqualTo, 6, 8);
  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThan, 6, 8);

  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThanOrEqualTo, 7, 8);
  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThan, 7, 8);

  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThanOrEqualTo, 8, 8);
  EXPECT_SUCCESS(Arcadia_Natural32, Natural32, GreaterThan, 8, 16);

  // ...

  EXPECT_FAILURE(Arcadia_Natural32, Natural32, GreaterThanOrEqualTo, Arcadia_Natural32Value_Maximum - 1);
  EXPECT_FAILURE(Arcadia_Natural32, Natural32, GreaterThan, Arcadia_Natural32Value_Maximum - 1);

  EXPECT_FAILURE(Arcadia_Natural32, Natural32, GreaterThanOrEqualTo, Arcadia_Natural32Value_Maximum - 0);
  EXPECT_FAILURE(Arcadia_Natural32, Natural32, GreaterThan, Arcadia_Natural32Value_Maximum - 0);
}

static void
nextPowerOfTwoNatural16ValueTests
  (
    Arcadia_Thread* thread
  )
{
  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThanOrEqualTo, 0, 1);
  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThan, 0, 1);

  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThanOrEqualTo, 1, 1);
  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThan, 1, 2);

  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThanOrEqualTo, 2, 2);
  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThan, 2, 4);

  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThanOrEqualTo, 3, 4);
  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThan, 3, 4);

  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThanOrEqualTo, 4, 4);
  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThan, 4, 8);

  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThanOrEqualTo, 5, 8);
  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThan, 5, 8);

  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThanOrEqualTo, 6, 8);
  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThan, 6, 8);

  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThanOrEqualTo, 7, 8);
  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThan, 7, 8);

  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThanOrEqualTo, 8, 8);
  EXPECT_SUCCESS(Arcadia_Natural16, Natural16, GreaterThan, 8, 16);

  // ...

  EXPECT_FAILURE(Arcadia_Natural16, Natural16, GreaterThanOrEqualTo, Arcadia_Natural16Value_Maximum - 1);
  EXPECT_FAILURE(Arcadia_Natural16, Natural16, GreaterThan, Arcadia_Natural16Value_Maximum - 1);

  EXPECT_FAILURE(Arcadia_Natural16, Natural16, GreaterThanOrEqualTo, Arcadia_Natural16Value_Maximum - 0);
  EXPECT_FAILURE(Arcadia_Natural16, Natural16, GreaterThan, Arcadia_Natural16Value_Maximum - 0);
}

static void
nextPowerOfTwoNatural8ValueTests
  (
    Arcadia_Thread* thread
  )
{
  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThanOrEqualTo, 0, 1);
  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThan, 0, 1);

  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThanOrEqualTo, 1, 1);
  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThan, 1, 2);

  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThanOrEqualTo, 2, 2);
  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThan, 2, 4);

  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThanOrEqualTo, 3, 4);
  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThan, 3, 4);

  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThanOrEqualTo, 4, 4);
  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThan, 4, 8);

  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThanOrEqualTo, 5, 8);
  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThan, 5, 8);

  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThanOrEqualTo, 6, 8);
  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThan, 6, 8);

  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThanOrEqualTo, 7, 8);
  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThan, 7, 8);

  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThanOrEqualTo, 8, 8);
  EXPECT_SUCCESS(Arcadia_Natural8, Natural8, GreaterThan, 8, 16);

  // ...

  EXPECT_FAILURE(Arcadia_Natural8, Natural8, GreaterThanOrEqualTo, Arcadia_Natural8Value_Maximum - 1);
  EXPECT_FAILURE(Arcadia_Natural8, Natural8, GreaterThan, Arcadia_Natural8Value_Maximum - 1);

  EXPECT_FAILURE(Arcadia_Natural8, Natural8, GreaterThanOrEqualTo, Arcadia_Natural8Value_Maximum - 0);
  EXPECT_FAILURE(Arcadia_Natural8, Natural8, GreaterThan, Arcadia_Natural8Value_Maximum - 0);
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (!Arcadia_Tests_safeExecute(&nextPowerOfTwoNatural64ValueTests)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&nextPowerOfTwoNatural32ValueTests)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&nextPowerOfTwoNatural16ValueTests)) {
    return EXIT_FAILURE;
  }
  if (!Arcadia_Tests_safeExecute(&nextPowerOfTwoNatural8ValueTests)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
