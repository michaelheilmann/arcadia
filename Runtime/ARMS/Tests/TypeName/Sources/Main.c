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

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// strlen
#include <string.h>

#include "Arcadia/ARMS/Include.h"

static Arcadia_ARMS_Status
check1
  (
  )
{
  Arcadia_ARMS_TypeName* a, * b;
  if (Arcadia_ARMS_TypeName_getOrCreate(&a, u8"a", strlen(u8"a"))) {
    return Arcadia_ARMS_Status_EnvironmentFailed;
  }
  if (Arcadia_ARMS_TypeName_getOrCreate(&b, u8"a", strlen(u8"a"))) {
    Arcadia_ARMS_shutdown();
    return Arcadia_ARMS_Status_EnvironmentFailed;
  }
  if (a != b) {
    return Arcadia_ARMS_Status_EnvironmentFailed;
  }
  return Arcadia_ARMS_Status_Success;
}

static Arcadia_ARMS_Status
check2
  (
  )
{
  Arcadia_ARMS_TypeName* a, * b;
  if (Arcadia_ARMS_TypeName_getOrCreate(&a, u8"a", strlen(u8"a"))) {
    return Arcadia_ARMS_Status_EnvironmentFailed;
  }
  if (Arcadia_ARMS_TypeName_getOrCreate(&b, u8"b", strlen(u8"b"))) {
    Arcadia_ARMS_shutdown();
    return Arcadia_ARMS_Status_EnvironmentFailed;
  }
  if (a == b) {
    return Arcadia_ARMS_Status_EnvironmentFailed;
  }
  return Arcadia_ARMS_Status_Success;
}

int
main
  (
    int argc,
    char **argv
  )
{
  if (Arcadia_ARMS_startup()) {
    return EXIT_FAILURE;
  }
  if (check1()) {
    Arcadia_ARMS_shutdown();
    return EXIT_FAILURE;
  }
  if (check2()) {
    Arcadia_ARMS_shutdown();
    return EXIT_FAILURE;
  }
  Arcadia_ARMS_shutdown();
  return EXIT_SUCCESS;
}
