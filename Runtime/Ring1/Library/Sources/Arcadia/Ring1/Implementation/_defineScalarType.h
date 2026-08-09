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

#if !defined(ARCADIA_RING1_IMPLEMENTATION__DEFINESCALARTYPE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION__DEFINESCALARTYPE_H_INCLUDED

#include "Arcadia/Ring1/Include.h"

#define Arcadia_defineScalarType(cName, cilName, typeOperations) \
  static Arcadia_Type* g_##cName##_type = NULL; \
\
  static void \
  _##cName##_typeDestructing \
    ( \
      void* context \
    ) \
  { g_##cName##_type = NULL; } \
\
  Arcadia_TypeValue \
  _##cName##Value_getType \
    ( \
      Arcadia_Thread* thread \
    ) \
  { \
    if (!g_##cName##_type) { \
      g_##cName##_type = Arcadia_registerScalarType(thread, Arcadia_Names_getOrCreateName(thread, cilName, sizeof(cilName) - 1), typeOperations, &_##cName##_typeDestructing); \
    } \
    return g_##cName##_type; \
  }

#endif // ARCADIA_RING1_IMPLEMENTATION__DEFINESCALARTYPE_H_INCLUDED
