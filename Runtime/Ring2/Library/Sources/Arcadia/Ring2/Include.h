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

#if !defined(ARCADIA_RING2_INCLUDE_H_INCLUDED)
#define ARCADIA_RING2_INCLUDE_H_INCLUDED

// If a file x belongs to a module a and ARCADIA_a_MODULE is not defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_RING2_MODULE")
#define ARCADIA_RING2_MODULE (1)

// If a file x of a module a is not an export file of that module and ARCADIA_a_EXPORT is defined, then that file shall raise a compile-time error.
#pragma push_macro("ARCADIA_RING2_EXPORT")
#define ARCADIA_RING2_EXPORT (1)

#include "Arcadia/Ring2/CommandLine/Include.h"

#include "Arcadia/Ring2/Exceptions/Include.h"

#include "Arcadia/Ring2/Implementation/ArgumentsValidation.h"

#include "Arcadia/Ring2/Logging/Include.h"

#include "Arcadia/Ring2/Strings/Include.h"

#include "Arcadia/Ring2/Time/Include.h"

#include "Arcadia/Ring2/Unicode/Include.h"

#if defined(_DEBUG)

/* Diagnostics for a value stack. */
static inline void
Arcadia_Diagnostics_logValueStack
  (
    Arcadia_Thread* thread,
    Arcadia_LogFlags logFlags
  )
{
  Arcadia_SizeValue size = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue index = 0; index < size; ++index) {
    Arcadia_Value value = Arcadia_ValueStack_getValue(thread, index);
    Arcadia_TypeValue type = Arcadia_Value_getType(thread, &value);
    Arcadia_Name* name = Arcadia_Type_getName(thread, type);
    Arcadia_logf(Arcadia_LogFlags_Debug,
                 "%zu) %.*s\n",
                 index,
                 Arcadia_Name_getNumberOfBytes(thread, name) > INT_MAX ? INT_MAX : Arcadia_Name_getNumberOfBytes(thread, name),
                 Arcadia_Name_getBytes(thread, name));
  }
}

/* Diagnostics for a value stack element. */
static inline void
Arcadia_Diagnostics_logValueStackElement
  (
    Arcadia_Thread* thread,
    Arcadia_LogFlags logFlags,
    Arcadia_SizeValue index
  )
{
  Arcadia_Value value = Arcadia_ValueStack_getValue(thread, index);
  Arcadia_TypeValue type = Arcadia_Value_getType(thread, &value);
  Arcadia_Name* name = Arcadia_Type_getName(thread, type);
  Arcadia_logf(logFlags,
               "%zu) %.*s\n",
               index,
               Arcadia_Name_getNumberOfBytes(thread, name) > INT_MAX ? INT_MAX : Arcadia_Name_getNumberOfBytes(thread, name),
               Arcadia_Name_getBytes(thread, name));
}

/* Diagnostics for a value. */
static inline void
Arcadia_Diagnostics_logValue
  (
    Arcadia_Thread* thread,
    Arcadia_LogFlags logFlags,
    Arcadia_Value value
  )
{
  Arcadia_Type* type = Arcadia_Value_getType(thread, &value);
  Arcadia_Name* name = Arcadia_Type_getName(thread, type);
  Arcadia_logf(logFlags, u8"%.*s\n", Arcadia_Name_getNumberOfBytes(thread, name) > INT_MAX ? INT_MAX : Arcadia_Name_getNumberOfBytes(thread, name),
                                     Arcadia_Name_getBytes(thread, name));
}

/* Diagnostics for an object value. */
static inline void
Arcadia_Diagnostics_logObjectType
  (
    Arcadia_Thread* thread,
    Arcadia_LogFlags logFlags,
    Arcadia_Object* object
  )
{
  Arcadia_Type* type = Arcadia_Object_getType(thread, object);
  Arcadia_Name* name = Arcadia_Type_getName(thread, type);
  Arcadia_logf(logFlags, u8"%.*s\n", Arcadia_Name_getNumberOfBytes(thread, name) > INT_MAX ? INT_MAX : Arcadia_Name_getNumberOfBytes(thread, name),
                                     Arcadia_Name_getBytes(thread, name));
}

#endif

#undef ARCADIA_RING2_EXPORT
#pragma pop_macro("ARCADIA_RING2_EXPORT")

#undef ARCADIA_RING2_MODULE
#pragma pop_macro("ARCADIA_RING2_MODULE")

#endif // ARCADIA_RING2_INCLUDE_H_INCLUDED
