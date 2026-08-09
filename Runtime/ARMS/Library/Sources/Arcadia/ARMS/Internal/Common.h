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

#if !defined(ARCADIA_ARMS_INTERNAL_COMMON_H_INCLUDED)
#define ARCADIA_ARMS_INTERNAL_COMMON_H_INCLUDED

#include "Arcadia/ARMS/Configure.h"

#include <stdlib.h> // exit, EXIT_FAILURE
#include <stdio.h> // fprintf, stderr

// Runtime fatal error.
#define Cxx_fatalError() { \
  fprintf(stderr, "%s:%d: fatal error\n", __FILE__, __LINE__); \
  exit(EXIT_FAILURE); \
}

// Compile time assertion.
#if __STDC_VERSION__ < 202311L
  #define Cxx_staticAssert(expression, message) _Static_assert(expression, message)
#else
  #define Cxx_staticAssert(expression, message) static_assert(expression, message)
#endif

#endif // ARCADIA_ARMS_INTERNAL_COMMON_H_INCLUDED
