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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_ANNOTATIONS_THREADLOCAL_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_ANNOTATIONS_THREADLOCAL_H_INCLUDED

#include "Arcadia/Ring1/Implementation/Configure.h"

// https://michaelheilmann.com/Arcadia/Ring1/#Arcadia_ThreadLocal
#if (Arcadia_Configuration_CompilerC_Gcc == Arcadia_Configuration_CompilerC || Arcadia_Configuration_CompilerC_Clang == Arcadia_Configuration_CompilerC)
  #define Arcadia_ThreadLocal() __thread
#elif (Core_Compiler_C == Core_Compiler_C_Msvc)
  #define Arcadia_ThreadLocal() __declspec(thread)
#else
  #error("environment not (yet) supported")
#endif

#endif // ARCADIA_RING1_IMPLEMENTATION_ANNOTATIONS_THREADLOCAL_H_INCLUDED
