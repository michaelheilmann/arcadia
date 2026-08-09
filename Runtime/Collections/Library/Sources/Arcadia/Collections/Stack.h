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

#if !defined(ARCADIA_COLLECTIONS_STACK_H_INCLUDED)
#define ARCADIA_COLLECTIONS_STACK_H_INCLUDED

#if !defined(ARCADIA_COLLECTIONS_MODULE)
  #error("do not include directly, include `Arcadia/Collections/Include.h` instead")
#endif

#include "Arcadia/Collections/Collection.h"

Arcadia_declareObjectType(u8"Arcadia.Stack", Arcadia_Stack,
                          u8"Arcadia.Collection");

struct Arcadia_StackDispatch {
  Arcadia_CollectionDispatch _parent;

  Arcadia_Value(*peek)(Arcadia_Thread*, Arcadia_Stack*);
  Arcadia_Value(*peekAt)(Arcadia_Thread*, Arcadia_Stack*, Arcadia_SizeValue);
  void (*pop)(Arcadia_Thread*, Arcadia_Stack*);
  void (*push)(Arcadia_Thread*, Arcadia_Stack*, Arcadia_Value);
};

struct Arcadia_Stack {
  Arcadia_Collection parent;
};

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Stack_push
void
Arcadia_Stack_push
  (
    Arcadia_Thread* thread,
    Arcadia_Stack* self,
    Arcadia_Value value
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Stack_pop
void
Arcadia_Stack_pop
  (
    Arcadia_Thread* thread,
    Arcadia_Stack* self
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Stack_peek
Arcadia_Value
Arcadia_Stack_peek
  (
    Arcadia_Thread* thread,
    Arcadia_Stack* self
  );

// https://michaelheilmann.com/Arcadia/Collections/#Arcadia_Stack_peekAt
Arcadia_Value
Arcadia_Stack_peekAt
  (
    Arcadia_Thread* thread,
    Arcadia_Stack* self,
    Arcadia_SizeValue index
  );

#define Define(Type, Suffix, Variable) \
  void \
  Arcadia_Stack_push##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_Stack* self, \
      Type##Value variable##Value \
    ); \
\
  Arcadia_BooleanValue \
  Arcadia_Stack_is##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_Stack* self, \
      Arcadia_SizeValue index \
    ); \
\
  Type##Value \
  Arcadia_Stack_get##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_Stack* self, \
      Arcadia_SizeValue index \
    ); \
\
  Type##Value \
  Arcadia_Stack_peek##Suffix##Value \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_Stack* self \
    );

Define(Arcadia_Boolean, Boolean, boolean)
Define(Arcadia_ForeignProcedure, ForeignProcedure, foreignProcedure)
Define(Arcadia_Integer8, Integer8, integer8)
Define(Arcadia_Integer16, Integer16, integer16)
Define(Arcadia_Integer32, Integer32, integer32)
Define(Arcadia_Integer64, Intege64, integer64)
Define(Arcadia_Natural8, Natural8, natural8)
Define(Arcadia_Natural16, Natural16, natural16)
Define(Arcadia_Natural32, Natural32, natural32)
Define(Arcadia_Natural64, Natural64, natural64)
Define(Arcadia_ObjectReference, ObjectReference, objectReference)
Define(Arcadia_Size, Size, size)
Define(Arcadia_Void, Void, void)

#undef Define

static inline Arcadia_ObjectReferenceValue
Arcadia_Stack_popObjectReferenceValueChecked
  (
    Arcadia_Thread* thread,
    Arcadia_Stack* self,
    Arcadia_Type* type
  )
{
  Arcadia_ObjectReferenceValue v = Arcadia_Stack_peekObjectReferenceValue(thread, self);
  if (type) {
    if (!Arcadia_Object_isInstanceOf(thread, v, type)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_jump(thread);
    }
  }
  Arcadia_Stack_pop(thread, self);
  return v;
}

#endif // ARCADIA_COLLECTIONS_STACK_H_INCLUDED
