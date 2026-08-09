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

#if !defined(ARCADIA_LOGGING_CONSOLELOG_H_INCLUDED)
#define ARCADIA_LOGGING_CONSOLELOG_H_INCLUDED

#if !defined(ARCADIA_LOGGING_MODULE)
  #error("do not include directly, include `Arcadia/Logging/Include.h` instead")
#endif

#include "Arcadia/Ring2/Include.h"
#include "Arcadia/FileSystem/Include.h"

Arcadia_declareObjectType(u8"Arcadia.ConsoleLog", Arcadia_ConsoleLog,
                          u8"Arcadia.Log");

struct Arcadia_ConsoleLogDispatch {
  Arcadia_LogDispatch _parent;
};

struct Arcadia_ConsoleLog {
  Arcadia_Log _parent;
  Arcadia_BooleanValue colorEnabled;
  Arcadia_FileHandle* fileHandle;
};

Arcadia_ConsoleLog*
Arcadia_ConsoleLog_create
  (
    Arcadia_Thread* thread
  );

void
Arcadia_ConsoleLog_setColorEnabled
  (
    Arcadia_Thread* thread,
    Arcadia_ConsoleLog* self,
    Arcadia_BooleanValue colorEnabled
  );

Arcadia_BooleanValue
Arcadia_ConsoleLog_getColorEnabled
  (
    Arcadia_Thread* thread,
    Arcadia_ConsoleLog* self
  );

#endif // ARCADIA_LOGGING_CONSOLELOG_H_INCLUDED
