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

#include "Arcadia/Engine/Text/Include.h"

void
Arcadia_Engine_Text_requireRunStringsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_String* text,
    Arcadia_String* glyphSource,
    Arcadia_String* colorName
  )
{
  // Generated typed callers check non-null argument types and own all roots.
  // Required references are not yet expressible in PDL parameter metadata.
  if (!text || !glyphSource || !colorName) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_jump(thread);
  }
}
