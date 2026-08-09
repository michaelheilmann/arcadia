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

#if !defined(ARCADIA_RING1_IMPLEMENTATION_OBJECTREFERENCE_H_INCLUDED)
#define ARCADIA_RING1_IMPLEMENTATION_OBJECTREFERENCE_H_INCLUDED

#if !defined(ARCADIA_RING1_MODULE)
  #error("do not include directly, include `Arcadia/Ring1/Include.h` instead")
#endif
#include "Arcadia/Ring1/Implementation/Configure.h"
#include "Arcadia/Ring1/Implementation/StaticAssert.h"

typedef void* Arcadia_ObjectReferenceValue;

#define Arcadia_ObjectReferenceValue_Null (NULL)

#define Arcadia_ObjectReferenceValue_NumberOfBytes (sizeof(void*))

#if Arcadia_Configuration_InstructionSetArchitecture == Arcadia_Configuration_InstructionSetArchitecture_X64
  Arcadia_StaticAssert(Arcadia_ObjectReferenceValue_NumberOfBytes == 8, "Arcadia.ObjectReferenceValue.NumberOfBytes must be 8");
#elif Arcadia_Configuration_InstructionSetArchitecture == Arcadia_Configuration_InstructionSetArchitecture_X86
  Arcadia_StaticAssert(Arcadia_ObjectReferenceValue_NumberOfBytes == 4, "Arcadia.ObjectReferenceValue.NumberOfBytes must be 4");
#else
  #error("invalid/unknown value for Arcadia_Configuration_InstructionSetArchitecture")
#endif

#define Arcadia_ObjectReferenceValue_NumberOfBits (Arcadia_ObjectReferenceValue_NumberOfBytes * 8)

#endif // ARCADIA_RING1_IMPLEMENTATION_OBJECTREFERENCE_H_INCLUDED
