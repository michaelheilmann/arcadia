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
#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/toSize.h"

#include "Arcadia/Ring1/Implementation/ImmutableUTF8String/toNatural.h"

Arcadia_SizeValue
_toSize
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* immutableUTF8StringValue
  )
{
#if Arcadia_Configuration_InstructionSetArchitecture_X64 == Arcadia_Configuration_InstructionSetArchitecture
  return _toNatural64(thread, immutableUTF8StringValue);
#elif Arcadia_Configuration_InstructionSetArchitecture_X86 == Arcadia_Configuration_InstructionSetArchitecture
  return _toNatural32(thread, immutableUTF8StringValue);
#else
  #error("environemnt not (yet) supported");
#endif
}
