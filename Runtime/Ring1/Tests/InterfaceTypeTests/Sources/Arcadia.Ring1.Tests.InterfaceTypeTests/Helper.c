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

#include "Arcadia.Ring1.Tests.InterfaceTypeTests/Helper.h"

void
Helper_typeDestructing
  (
    void* context
  )
{ }

// The operations of the object dispatch. These are the same implementations as those of
// "Arcadia.Object": identity for "isIdenticalTo" and "isEqualTo", and the address of the object
// for "getHash". They are defined here because the operations of "Arcadia.Object" are private.
void
Helper_objectIsIdenticalTo
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value numberOfArguments = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (2 != numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value self = Arcadia_ValueStack_getValue(thread, 2);
  Arcadia_Value other = Arcadia_ValueStack_getValue(thread, 1);
  Arcadia_BooleanValue result = Arcadia_BooleanValue_False;
  if (Arcadia_Value_isObjectReferenceValue(&self) && Arcadia_Value_isObjectReferenceValue(&other)) {
    result = Arcadia_Value_getObjectReferenceValue(&self) == Arcadia_Value_getObjectReferenceValue(&other)
      ? Arcadia_BooleanValue_True
      : Arcadia_BooleanValue_False;
  }
  Arcadia_ValueStack_popValues(thread, numberOfArguments + 1);
  Arcadia_ValueStack_pushBooleanValue(thread, result);
}

void
Helper_objectGetHash
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Natural8Value numberOfArguments = Arcadia_ValueStack_getNatural8Value(thread, 0);
  if (1 != numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value self = Arcadia_ValueStack_getValue(thread, 1);
  Arcadia_Object* object = NULL;
  if (Arcadia_Value_isObjectReferenceValue(&self)) {
    object = (Arcadia_Object*)Arcadia_Value_getObjectReferenceValue(&self);
  }
  Arcadia_ValueStack_popValues(thread, numberOfArguments + 1);
  Arcadia_ValueStack_pushSizeValue(thread, (Arcadia_SizeValue)(uintptr_t)object);
}
