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

#include "Arcadia.Tools.GenerateRyuLookupTables/Common.h"

#include "Arcadia/Ring2/Include.h"

#include <stdio.h>

void
Arcadia_printInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value integer32
  )
{
  Arcadia_printRuntimeUTF8String(thread, Arcadia_RuntimeUTF8String_createFromInteger32(thread, integer32));
}

void
Arcadia_printRuntimeUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* string
  )
{
  fwrite(Arcadia_RuntimeUTF8String_getBytes(thread, string), 1, Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, string), stdout);
}

void
Arcadia_printNewline
  (
    Arcadia_Thread* thread
  )
{
  static const uint8_t v[] = u8"\n";
  fwrite(v, 1, sizeof(v) - 1, stdout);
}

void
Arcadia_printStringBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* stringBuffer
  )
{
  fwrite(Arcadia_StringBuilder_getBytes(thread, stringBuffer), 1, Arcadia_StringBuilder_getNumberOfBytes(thread, stringBuffer), stdout);
}
