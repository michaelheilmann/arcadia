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
Arcadia_SizeValue_toUTF8String
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue sizeValue,
    void* context,
    void (*function)(Arcadia_Thread* thread, void* context, const Arcadia_Natural8Value* bytes, Arcadia_SizeValue numberOfBytes)
  )
{
#if Arcadia_Configuration_InstructionSetArchitecture_X64 == Arcadia_Configuration_InstructionSetArchitecture
  Arcadia_Natural64Value_toUTF8String(thread, sizeValue, context, function);
#elif Arcadia_Configuration_InstructionSetArchitecture_X86 == Arcadia_Configuration_InstructionSetArchitecture
  Arcadia_Natural32Value_toUTF8String(thread, sizeValue, context, function);
#else
  #error("environemnt not (yet) supported");
#endif
}
