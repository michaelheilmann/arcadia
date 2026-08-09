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

#if !defined(ARCADIA_TOOLS_RYULOOKUPTABLEGENERATOR_COMMON_H_INCLUDED)
#define ARCADIA_TOOLS_RYULOOKUPTABLEGENERATOR_COMMON_H_INCLUDED

#include "Arcadia/Ring2/Include.h"

void
Arcadia_printInteger32
  (
    Arcadia_Thread* thread,
    Arcadia_Integer32Value integer32
  );

void
Arcadia_printRuntimeUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* string
  );

void
Arcadia_printRuntimeUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* string
  );

void
Arcadia_printStringBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_StringBuilder* stringBuffer
  );

void
Arcadia_printNewline
  (
    Arcadia_Thread* thread
  );

#endif // ARCADIA_TOOLS_RYULOOKUPTABLEGENERATOR_COMMON_H_INCLUDED
