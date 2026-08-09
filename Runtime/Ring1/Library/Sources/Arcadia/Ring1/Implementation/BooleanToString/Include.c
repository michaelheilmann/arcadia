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

#define ARCADIA_RING1_MODULE (1)
#include "Arcadia/Ring1/Implementation/NaturalToString/Include.h"

#include "Arcadia/Ring1/Include.h"

void
Arcadia_BooleanValue_toUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue booleanValue,
    void* context,
    void (*function)(Arcadia_Thread* thread, void* context, const Arcadia_Natural8Value* bytes, Arcadia_SizeValue numberOfBytes)
  )
{
  booleanValue ? (*function)(thread, context, u8"true", sizeof(u8"true") - 1)
               : (*function)(thread, context, u8"false", sizeof(u8"false") - 1);
}
