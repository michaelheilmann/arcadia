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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_ENUMERATION_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_ENUMERATION_H_INCLUDED

#include "Arcadia/Ring1/Implementation/Configure.h"
#include "Arcadia/Ring1/Implementation/Integer32.h"
typedef struct Arcadia_Thread Arcadia_Thread;
typedef void Arcadia_Type;

/// R(untime) ex(tension) macro.
/// @param _cilName UTF-8 string literal for the Machine Interface Language type name of the type.
/// @param _cName C name of the type.
#define Arcadia_declareEnumerationType(_cilName, _cName) \
  typedef enum _cName _cName; \
  Arcadia_TypeValue \
  _##_cName##_getType \
    ( \
      Arcadia_Thread* thread \
    );

/// R(untime) ex(tension) macro.
/// @param _cilName UTF-8 string literal for the Machine Interface Language type name of the type.
/// @param _cName C name of the type.
#define Arcadia_defineEnumerationType(_cilName, _cName, _cTypeOperations) \
  static Arcadia_TypeValue g_##_cName##_type = NULL; \
  \
  static void \
  _##_cName##_typeDestructing \
    ( \
      void *context \
    ) \
  { \
    g_##_cName##_type = NULL; \
  } \
  \
  Arcadia_TypeValue \
  _##_cName##_getType \
    ( \
      Arcadia_Thread* thread \
    ) \
  { \
    if (!g_##_cName##_type) { \
      g_##_cName##_type = Arcadia_registerEnumerationType(thread, Arcadia_Names_getOrCreateName(thread, _cilName, sizeof(_cilName) - 1), sizeof(_cName), _cTypeOperations, &_##_cName##_typeDestructing); \
    } \
    return g_##_cName##_type; \
  }

typedef struct Arcadia_EnumerationValue {
  Arcadia_Type* type;
  Arcadia_Integer32Value value;
} Arcadia_EnumerationValue;

static inline Arcadia_EnumerationValue
Arcadia_EnumerationValue_make
  (
    Arcadia_Type* type,
    Arcadia_Integer32Value value
  )
{
  Arcadia_EnumerationValue temporary = { .type = type, .value = value };
  return temporary;
}

#endif // ARCADIA_RING1_IMPLEMENTATION_ENUMERATION_H_INCLUDED
