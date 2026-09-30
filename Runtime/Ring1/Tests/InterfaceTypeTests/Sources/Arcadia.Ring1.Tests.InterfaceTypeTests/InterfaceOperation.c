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

#include <string.h>
#include "Arcadia/Ring1/Include.h"
#include "Arcadia.Ring1.Tests.InterfaceTypeTests/Helper.h"
#include "Arcadia.Ring1.Tests.InterfaceTypeTests/InterfaceOperation.h"

/// "Arcadia.MyInterfaceA" is an interface type which extends no interface type, and which declares
/// a single operation, "getMessage".
Arcadia_declareInterfaceType(u8"Arcadia.MyInterfaceA", Arcadia_MyInterfaceA)

/// The leading "Arcadia.InterfaceDispatch" is mandatory, and is the leading part of the dispatch of
/// every interface type. The operations of the interface type follow it. A value of an interface type
/// is represented by an "Arcadia.Object" value, so the receiver of an operation is an
/// "Arcadia.Object" value, too.
struct Arcadia_MyInterfaceADispatch {
  Arcadia_InterfaceDispatch parent;
  /// Get the message of the object.
  Arcadia_RuntimeUTF8String* (*getMessage)(Arcadia_Thread* thread, Arcadia_Object* self);
};

Arcadia_defineInterfaceType(u8"Arcadia.MyInterfaceA", Arcadia_MyInterfaceA, NULL)

/// "Arcadia.MyObjectA" derives from "Arcadia.Object" and implements "Arcadia.MyInterfaceA".
Arcadia_declareObjectType(u8"Arcadia.MyObjectA", Arcadia_MyObjectA, u8"Arcadia.Object")

struct Arcadia_MyObjectADispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_MyObjectA {
  Arcadia_Object parent;
};

/// The implementation of "Arcadia.MyInterfaceA.getMessage" in "Arcadia.MyObjectA", which returns
/// "Hello, World!\n".
static Arcadia_RuntimeUTF8String*
Arcadia_MyObjectA_myInterfaceA_getMessage
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  static Arcadia_Natural8Value const message[] = {
    'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\n'
  };
  return Arcadia_RuntimeUTF8String_create(thread, message, sizeof(message));
}

static void
Arcadia_MyObjectA_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectA* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MyObjectA);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MyObjectA);
}

static void
Arcadia_MyObjectA_initializeObjectDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectADispatch* self
  )
{
  self->parent.isIdenticalTo = &Helper_objectIsIdenticalTo;
  self->parent.isEqualTo = &Helper_objectIsIdenticalTo;
  self->parent.getHash = &Helper_objectGetHash;
}

/// The dispatch of "Arcadia.MyObjectA" as a "Arcadia.MyInterfaceA". The leading
/// "Arcadia.InterfaceDispatch" is filled in by the type system.
static void
Arcadia_MyObjectA_initializeMyInterfaceADispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyInterfaceADispatch* self
  )
{
  self->getMessage = (Arcadia_RuntimeUTF8String* (*)(Arcadia_Thread*, Arcadia_Object*))
    &Arcadia_MyObjectA_myInterfaceA_getMessage;
}

static const Arcadia_ObjectType_Operations _Arcadia_MyObjectA_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MyObjectA_constructImpl,
  .initializeDispatch =
    (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MyObjectA_initializeObjectDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_MyObjectA_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_MyObjectA_objectTypeOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.MyObjectA",
    Arcadia_MyObjectA,
    u8"Arcadia.Object",
    Arcadia_Object,
    &_Arcadia_MyObjectA_typeOperations,
    {
      .interfaceType = _Arcadia_MyInterfaceA_getType(thread),
      .dispatchSize = sizeof(Arcadia_MyInterfaceADispatch),
      .initializeDispatch =
        (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_MyObjectA_initializeMyInterfaceADispatchImpl
    }
  )

static Arcadia_MyObjectA*
Arcadia_MyObjectA_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_MyObjectA);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MyObjectA);
}

/// "Arcadia.MyInterfaceA.getMessage", as it is invoked on a value of the interface type. This is the
/// shape of the operation a caller of the interface type uses.
Arcadia_RuntimeUTF8String*
Arcadia_MyInterfaceA_getMessage
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ Arcadia_InterfaceCallWithReturn(Arcadia_MyInterfaceA, getMessage, self); }

/// Exercise the interface call macro without return value. "Arcadia.MyInterfaceA.getMessage" does
/// return a value, so the result of the operation is discarded here.
static void
Arcadia_MyInterfaceA_discardMessage
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ Arcadia_InterfaceCall(Arcadia_MyInterfaceA, getMessage, self); }

/// Define the interface type "Arcadia.MyInterfaceA", which declares the single operation
/// "getMessage" returning an "Arcadia.RuntimeUTF8String", and the object type "Arcadia.MyObjectA",
/// which derives from "Arcadia.Object" and implements "Arcadia.MyInterfaceA", where "getMessage"
/// returns "Hello, World!\n". Then verify that a value of "Arcadia.MyObjectA" is an instance of
/// the interface type and that its interface dispatch is present, belongs to the object type and
/// the interface type, and is populated. Invoke the operation through
/// "Arcadia_InterfaceCallWithReturn" and through "Arcadia_InterfaceCall": both must reach the
/// implementation and return "Hello, World!\n", and both must leave the value stack untouched,
/// because the receiver and the result are passed in C, not on the value stack.
void
testInterfaceOperation
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_TypeValue interfaceType = _Arcadia_MyInterfaceA_getType(thread);
  Arcadia_TypeValue objectType = _Arcadia_MyObjectA_getType(thread);

  // The object type derives from "Arcadia.Object" and implements the interface type.
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isInterfacelKind(thread, interfaceType));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isObjectKind(thread, objectType));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, objectType, interfaceType));

  Arcadia_Object* object = (Arcadia_Object*)Arcadia_MyObjectA_create(thread);
  Arcadia_Tests_assertTrue(thread, NULL != object);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_getType(thread, object) == objectType);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, object, interfaceType));

  // The object type provides a dispatch for the interface type.
  Arcadia_MyInterfaceADispatch* dispatch =
    (Arcadia_MyInterfaceADispatch*)Arcadia_ObjectType_getInterfaceDispatch
      (
        thread,
        (Arcadia_ObjectType*)objectType,
        (Arcadia_InterfaceType*)interfaceType
      );
  Arcadia_Tests_assertTrue(thread, NULL != dispatch);
  if (!dispatch) {
    return;
  }
  Arcadia_Tests_assertTrue(thread, dispatch->parent.objectType == objectType);
  Arcadia_Tests_assertTrue(thread, dispatch->parent.interfaceType == interfaceType);
  Arcadia_Tests_assertTrue(thread, NULL != dispatch->getMessage);

  // Invoke the operation through the interface call macro, as a caller of the interface type does.
  // The value stack is untouched by an interface call, as the receiver and the result are passed in
  // C, not on the value stack.
  Arcadia_SizeValue n = Arcadia_ValueStack_getSize(thread);
  Arcadia_RuntimeUTF8String* message = Arcadia_MyInterfaceA_getMessage(thread, object);
  Arcadia_Tests_assertTrue(thread, n == Arcadia_ValueStack_getSize(thread));
  Arcadia_Tests_assertTrue(thread, NULL != message);

  // The interface call macro without return value invokes the same operation.
  Arcadia_MyInterfaceA_discardMessage(thread, object);
  Arcadia_Tests_assertTrue(thread, n == Arcadia_ValueStack_getSize(thread));

  // The result of the operation is the message.
  static char const expectedMessage[] = "Hello, World!\n";
  if (message) {
    Arcadia_Natural8Value const* bytes = Arcadia_RuntimeUTF8String_getBytes(thread, message);
    Arcadia_SizeValue numberOfBytes = Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, message);
    Arcadia_Tests_assertTrue(thread, numberOfBytes == (sizeof(expectedMessage) - 1));
    if (numberOfBytes == (sizeof(expectedMessage) - 1)) {
      Arcadia_Tests_assertTrue(thread, 0 == memcmp(bytes, expectedMessage, numberOfBytes));
    }
  }
}
