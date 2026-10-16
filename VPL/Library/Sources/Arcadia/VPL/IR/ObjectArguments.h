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

#if !defined(ARCADIA_VPL_IR_OBJECTARGUMENTS_H_INCLUDED)
#define ARCADIA_VPL_IR_OBJECTARGUMENTS_H_INCLUDED

#if !defined(ARCADIA_VPL_PRIVATE) || 1 != ARCADIA_VPL_PRIVATE
  #error("do not include directly, include `Arcadia/VPL/Include.h` instead")
#endif
#include "Arcadia/Ring2/Include.h"

// The runtime checked getter requires a non-NULL object before checking its type.
static inline Arcadia_Object*
Arcadia_VPL_IR_getRequiredObjectArgument
  (
    Arcadia_Thread* thread,
    Arcadia_SizeValue index,
    Arcadia_Type* type
  )
{
  Arcadia_Object* object = Arcadia_ValueStack_getObjectReferenceValue(thread, index);
  if (!object) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  return Arcadia_ValueStack_getObjectReferenceValueChecked(thread, index, type);
}

#endif // ARCADIA_VPL_IR_OBJECTARGUMENTS_H_INCLUDED
