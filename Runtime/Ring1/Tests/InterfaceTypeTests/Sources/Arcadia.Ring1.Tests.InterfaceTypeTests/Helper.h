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

#if !defined(ARCADIA_RING1_TESTS_INTERFACETYPETESTS_HELPER_H_INCLUDED)
#define ARCADIA_RING1_TESTS_INTERFACETYPETESTS_HELPER_H_INCLUDED

#include "Arcadia/Ring1/Include.h"

/// A type destructing callback for a type which lives for the duration of a test.
void
Helper_typeDestructing
  (
    void* context
  );

/// The "isIdenticalTo" and "isEqualTo" operations of the object dispatch. These are the same
/// implementations as those of "Arcadia.Object": identity for both. They are defined here
/// because the operations of "Arcadia.Object" are private.
void
Helper_objectIsIdenticalTo
  (
    Arcadia_Thread* thread
  );

/// The "getHash" operation of the object dispatch, which returns the address of the object, as
/// that is what the implementation of "Arcadia.Object" does.
void
Helper_objectGetHash
  (
    Arcadia_Thread* thread
  );

/// Define the operations of an object type "_cName" which has no fields, and which is
/// constructible with any number of arguments. The definitions are "static", so every including
/// translation unit gets its own set of definitions.
// The constructor of an object type is responsible for consuming its arguments: the number of
// arguments is the topmost value on the value stack. It is also responsible for assigning the type
// of the object. `Arcadia_EnterConstructor`/`Arcadia_LeaveConstructor` do both, but require a
// `_##_cName##_getType` function, which these types do not have, so the body is spelled out.
#define Helper_defineObjectTypeOperations(_cName) \
  static Arcadia_TypeValue g_##_cName##_type = NULL; \
  \
  static void \
  _##_cName##_construct \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_Object* self \
    ) \
  { \
    if (Arcadia_ValueStack_getSize(thread) < 1) { \
      Arcadia_Thread_setStatus(thread, Arcadia_Status_StackCorruption); \
      Arcadia_Thread_jump(thread); \
    } \
    Arcadia_Natural8Value numberOfArguments = Arcadia_ValueStack_getNatural8Value(thread, 0); \
    Arcadia_Object_setType(thread, self, g_##_cName##_type); \
    Arcadia_ValueStack_popValues(thread, numberOfArguments + 1); \
  } \
  \
  static void \
  _##_cName##_initializeDispatch \
    ( \
      Arcadia_Thread* thread, \
      Arcadia_ObjectDispatch* self \
    ) \
  { \
    self->isIdenticalTo = &Helper_objectIsIdenticalTo; \
    self->isEqualTo = &Helper_objectIsIdenticalTo; \
    self->getHash = &Helper_objectGetHash; \
  } \
  \
  static const Arcadia_ObjectType_Operations _##_cName##_objectTypeOperations = { \
    Arcadia_ObjectType_Operations_Initializer, \
    .construct = &_##_cName##_construct, \
    .initializeDispatch = &_##_cName##_initializeDispatch, \
  }; \
  \
  static const Arcadia_Type_Operations _##_cName##_typeOperations = { \
    Arcadia_Type_Operations_Initializer, \
    .objectTypeOperations = &_##_cName##_objectTypeOperations, \
  };

#endif // ARCADIA_RING1_TESTS_INTERFACETYPETESTS_HELPER_H_INCLUDED
