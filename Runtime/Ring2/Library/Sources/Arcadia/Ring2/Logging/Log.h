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

#if !defined(ARCADIA_RING2_LOGGING_LOG_H_INCLUDED)
#define ARCADIA_RING2_LOGGING_LOG_H_INCLUDED

#if !defined(ARCADIA_RING2_MODULE)
  #error("do not include directly, include `Arcadia/Ring2/Include.h` instead")
#endif

#include "Arcadia/Ring1/Include.h"
typedef struct Arcadia_String Arcadia_String;

Arcadia_declareObjectType(u8"Arcadia.Log", Arcadia_Log,
                          u8"Arcadia.Object");

struct Arcadia_LogDispatch {
  Arcadia_ObjectDispatch _parent;
  void (*information)(Arcadia_Thread*, Arcadia_Log*, Arcadia_String*);
  void (*warning)(Arcadia_Thread*, Arcadia_Log*, Arcadia_String*);
  void (*error)(Arcadia_Thread*, Arcadia_Log*, Arcadia_String*);
};

struct Arcadia_Log {
  Arcadia_Object _parent;
};

void
Arcadia_Log_information
  (
    Arcadia_Thread* thread,
    Arcadia_Log* self,
    Arcadia_String* message
  );

void
Arcadia_Log_warning
  (
    Arcadia_Thread* thread,
    Arcadia_Log* self,
    Arcadia_String* message
  );

void
Arcadia_Log_error
  (
    Arcadia_Thread* thread,
    Arcadia_Log* self,
    Arcadia_String* message
  );

#endif // ARCADIA_RING2_LOGGING_LOG_H_INCLUDED
