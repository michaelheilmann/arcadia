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
#include "Arcadia.Ring1.Tests.InterfaceTypeTests/InterfaceOperationOverride.h"

/// "Arcadia.MyInterfaceB" is an interface type which extends no interface type, and which declares
/// two operations, "f" and "g".
Arcadia_declareInterfaceType(u8"Arcadia.MyInterfaceB", Arcadia_MyInterfaceB)

/// The leading "Arcadia.InterfaceDispatch" is mandatory, and is the leading part of the dispatch of
/// every interface type. The operations of the interface type follow it. A value of an interface type
/// is represented by an "Arcadia.Object" value, so the receiver of an operation is an
/// "Arcadia.Object" value, too.
struct Arcadia_MyInterfaceBDispatch {
  Arcadia_InterfaceDispatch parent;
  /// The operation "f", which is overridden by "Arcadia.MyObjectX".
  Arcadia_RuntimeUTF8String* (*f)(Arcadia_Thread* thread, Arcadia_Object* self);
  /// The operation "g", which is inherited by "Arcadia.MyObjectX" from "Arcadia.MyObjectY".
  Arcadia_RuntimeUTF8String* (*g)(Arcadia_Thread* thread, Arcadia_Object* self);
};

Arcadia_defineInterfaceType(u8"Arcadia.MyInterfaceB", Arcadia_MyInterfaceB, NULL)

/// "Arcadia.MyObjectY" derives from "Arcadia.Object" and implements "Arcadia.MyInterfaceB".
Arcadia_declareObjectType(u8"Arcadia.MyObjectY", Arcadia_MyObjectY, u8"Arcadia.Object")

struct Arcadia_MyObjectYDispatch {
  Arcadia_ObjectDispatch parent;
};

struct Arcadia_MyObjectY {
  Arcadia_Object parent;
};

/// The implementation of "Arcadia.MyInterfaceB.f" in "Arcadia.MyObjectY", which returns "B::f".
static Arcadia_RuntimeUTF8String*
Arcadia_MyObjectY_myInterfaceB_f
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  static Arcadia_Natural8Value const message[] = { 'B', ':', ':', 'f' };
  return Arcadia_RuntimeUTF8String_create(thread, message, sizeof(message));
}

/// The implementation of "Arcadia.MyInterfaceB.g" in "Arcadia.MyObjectY", which returns "B::g".
static Arcadia_RuntimeUTF8String*
Arcadia_MyObjectY_myInterfaceB_g
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  static Arcadia_Natural8Value const message[] = { 'B', ':', ':', 'g' };
  return Arcadia_RuntimeUTF8String_create(thread, message, sizeof(message));
}

static void
Arcadia_MyObjectY_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectY* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MyObjectY);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MyObjectY);
}

static void
Arcadia_MyObjectY_initializeObjectDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectDispatch* self
  )
{
  self->isIdenticalTo = &Helper_objectIsIdenticalTo;
  self->isEqualTo = &Helper_objectIsIdenticalTo;
  self->getHash = &Helper_objectGetHash;
}

/// The dispatch of "Arcadia.MyObjectY" as a "Arcadia.MyInterfaceB". The leading
/// "Arcadia.InterfaceDispatch" is filled in by the type system.
static void
Arcadia_MyObjectY_initializeMyInterfaceBDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyInterfaceBDispatch* self
  )
{
  self->f = (Arcadia_RuntimeUTF8String* (*)(Arcadia_Thread*, Arcadia_Object*))&Arcadia_MyObjectY_myInterfaceB_f;
  self->g = (Arcadia_RuntimeUTF8String* (*)(Arcadia_Thread*, Arcadia_Object*))&Arcadia_MyObjectY_myInterfaceB_g;
}

static const Arcadia_ObjectType_Operations _Arcadia_MyObjectY_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MyObjectY_constructImpl,
  .initializeDispatch =
    (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MyObjectY_initializeObjectDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_MyObjectY_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_MyObjectY_objectTypeOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.MyObjectY",
    Arcadia_MyObjectY,
    u8"Arcadia.Object",
    Arcadia_Object,
    &_Arcadia_MyObjectY_typeOperations,
    {
      .interfaceType = _Arcadia_MyInterfaceB_getType(thread),
      .dispatchSize = sizeof(Arcadia_MyInterfaceBDispatch),
      .initializeDispatch =
        (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_MyObjectY_initializeMyInterfaceBDispatchImpl
    }
  )

static Arcadia_MyObjectY*
Arcadia_MyObjectY_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_MyObjectY);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MyObjectY);
}

/// "Arcadia.MyObjectX" derives from "Arcadia.MyObjectY" and implements "Arcadia.MyInterfaceB",
/// overriding only the operation "f".
Arcadia_declareObjectType(u8"Arcadia.MyObjectX", Arcadia_MyObjectX, u8"Arcadia.MyObjectY")

struct Arcadia_MyObjectXDispatch {
  Arcadia_MyObjectYDispatch parent;
};

struct Arcadia_MyObjectX {
  Arcadia_MyObjectY parent;
};

/// The implementation of "Arcadia.MyInterfaceB.f" in "Arcadia.MyObjectX", which returns "A::f".
static Arcadia_RuntimeUTF8String*
Arcadia_MyObjectX_myInterfaceB_f
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  static Arcadia_Natural8Value const message[] = { 'A', ':', ':', 'f' };
  return Arcadia_RuntimeUTF8String_create(thread, message, sizeof(message));
}

static void
Arcadia_MyObjectX_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyObjectX* self
  )
{
  Arcadia_EnterConstructor(Arcadia_MyObjectX);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_MyObjectX);
}

static void
Arcadia_MyObjectX_initializeObjectDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_ObjectDispatch* self
  )
{
  self->isIdenticalTo = &Helper_objectIsIdenticalTo;
  self->isEqualTo = &Helper_objectIsIdenticalTo;
  self->getHash = &Helper_objectGetHash;
}

/// The dispatch of "Arcadia.MyObjectX" as a "Arcadia.MyInterfaceB". The dispatch of the interface
/// type on the parent object type "Arcadia.MyObjectY" is copied into it, so the operation "g" is
/// inherited. The initializer only sets the overridden operation "f".
static void
Arcadia_MyObjectX_initializeMyInterfaceBDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_MyInterfaceBDispatch* self
  )
{
  self->f = (Arcadia_RuntimeUTF8String* (*)(Arcadia_Thread*, Arcadia_Object*))&Arcadia_MyObjectX_myInterfaceB_f;
}

static const Arcadia_ObjectType_Operations _Arcadia_MyObjectX_objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_MyObjectX_constructImpl,
  .initializeDispatch =
    (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_MyObjectX_initializeObjectDispatchImpl,
};

static const Arcadia_Type_Operations _Arcadia_MyObjectX_typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_Arcadia_MyObjectX_objectTypeOperations,
};

Arcadia_defineObjectTypeWithInterfaces
  (
    u8"Arcadia.MyObjectX",
    Arcadia_MyObjectX,
    u8"Arcadia.MyObjectY",
    Arcadia_MyObjectY,
    &_Arcadia_MyObjectX_typeOperations,
    {
      .interfaceType = _Arcadia_MyInterfaceB_getType(thread),
      .dispatchSize = sizeof(Arcadia_MyInterfaceBDispatch),
      .initializeDispatch =
        (Arcadia_InterfaceDispatch_InitializeCallbackFunction*)&Arcadia_MyObjectX_initializeMyInterfaceBDispatchImpl
    }
  )

static Arcadia_MyObjectX*
Arcadia_MyObjectX_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_MyObjectX);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_MyObjectX);
}

/// "Arcadia.MyInterfaceB.f", as it is invoked on a value of the interface type.
static Arcadia_RuntimeUTF8String*
Arcadia_MyInterfaceB_f
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ Arcadia_InterfaceCallWithReturn(Arcadia_MyInterfaceB, f, self); }

/// "Arcadia.MyInterfaceB.g", as it is invoked on a value of the interface type.
static Arcadia_RuntimeUTF8String*
Arcadia_MyInterfaceB_g
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ Arcadia_InterfaceCallWithReturn(Arcadia_MyInterfaceB, g, self); }

/// The message returned by "Arcadia.MyInterfaceB.f" on a value of "Arcadia.MyObjectY".
static Arcadia_Natural8Value const _fMessageOfY[] = { 'B', ':', ':', 'f' };

/// The message returned by "Arcadia.MyInterfaceB.g" on a value of "Arcadia.MyObjectY".
static Arcadia_Natural8Value const _gMessageOfY[] = { 'B', ':', ':', 'g' };

/// The message returned by "Arcadia.MyInterfaceB.f" on a value of "Arcadia.MyObjectX", which
/// overrides "f". The three messages are distinct, so a swap of the "f" and "g" operation pointers,
/// or of the "f" pointer between the two object types, is detected by comparing the messages.
static Arcadia_Natural8Value const _fMessageOfX[] = { 'A', ':', ':', 'f' };

static void
checkMessage
  (
    Arcadia_Thread* thread,
    Arcadia_RuntimeUTF8String* message,
    Arcadia_Natural8Value const* expectedMessage,
    Arcadia_SizeValue numberOfBytes
  )
{
  Arcadia_Tests_assertTrue(thread, NULL != message);
  if (NULL != message) {
    Arcadia_Natural8Value const* bytes = Arcadia_RuntimeUTF8String_getBytes(thread, message);
    Arcadia_SizeValue numberOfMessageBytes = Arcadia_RuntimeUTF8String_getNumberOfBytes(thread, message);
    Arcadia_Tests_assertTrue(thread, numberOfBytes == numberOfMessageBytes);
    if (numberOfBytes == numberOfMessageBytes) {
      Arcadia_Tests_assertTrue(thread, 0 == memcmp(bytes, expectedMessage, numberOfBytes));
    }
  }
}

/// Define the interface type "Arcadia.MyInterfaceB", which declares the operations "f" and "g",
/// each returning an "Arcadia.RuntimeUTF8String". Define the object type "Arcadia.MyObjectY",
/// which derives from "Arcadia.Object" and implements "Arcadia.MyInterfaceB": "f" returns "B::f",
/// "g" returns "B::g". Define the object type "Arcadia.MyObjectX", which derives from
/// "Arcadia.MyObjectY" and overrides only "f", which returns "A::f". Then verify that the two
/// dispatches are distinct allocations, that both carry the object type and the interface type
/// they belong to, that the "f" operation pointers differ while the "g" operation pointers are
/// equal (so "g" is genuinely inherited, not copied by value or reimplemented), and that invoking
/// the operations through the interface call wrappers returns "B::f" and "B::g" on a value of
/// "Arcadia.MyObjectY" and "A::f" and "B::g" on a value of "Arcadia.MyObjectX". The three messages
/// are distinct, so a swap of the "f" and "g" operation pointers, or of the "f" pointer between
/// the two object types, is detected by comparing the returned messages.
void
testInterfaceOperationOverride
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_TypeValue interfaceType = _Arcadia_MyInterfaceB_getType(thread);
  Arcadia_TypeValue objectYType = _Arcadia_MyObjectY_getType(thread);
  Arcadia_TypeValue objectXType = _Arcadia_MyObjectX_getType(thread);

  // "Arcadia.MyObjectY" is the parent object type of "Arcadia.MyObjectX", and both object types are
  // descendants of the interface type.
  Arcadia_Tests_assertTrue(thread, objectYType == Arcadia_ObjectType_getParentObjectType(thread, (Arcadia_ObjectType*)objectXType));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, objectYType, interfaceType));
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, objectXType, interfaceType));

  Arcadia_Object* objectY = (Arcadia_Object*)Arcadia_MyObjectY_create(thread);
  Arcadia_Object* objectX = (Arcadia_Object*)Arcadia_MyObjectX_create(thread);
  Arcadia_Tests_assertTrue(thread, NULL != objectY);
  Arcadia_Tests_assertTrue(thread, NULL != objectX);

  Arcadia_MyInterfaceBDispatch* dispatchOfY =
    (Arcadia_MyInterfaceBDispatch*)Arcadia_ObjectType_getInterfaceDispatch
      (
        thread,
        (Arcadia_ObjectType*)objectYType,
        (Arcadia_InterfaceType*)interfaceType
      );
  Arcadia_MyInterfaceBDispatch* dispatchOfX =
    (Arcadia_MyInterfaceBDispatch*)Arcadia_ObjectType_getInterfaceDispatch
      (
        thread,
        (Arcadia_ObjectType*)objectXType,
        (Arcadia_InterfaceType*)interfaceType
      );
  Arcadia_Tests_assertTrue(thread, NULL != dispatchOfY);
  Arcadia_Tests_assertTrue(thread, NULL != dispatchOfX);
  if (dispatchOfY && dispatchOfX) {
    // "Arcadia.MyObjectX" has an implementation of its own, so its dispatch is not the dispatch of
    // its parent object type.
    Arcadia_Tests_assertTrue(thread, dispatchOfX != dispatchOfY);
    Arcadia_Tests_assertTrue(thread, dispatchOfY->parent.objectType == objectYType);
    Arcadia_Tests_assertTrue(thread, dispatchOfX->parent.objectType == objectXType);
    Arcadia_Tests_assertTrue(thread, dispatchOfY->parent.interfaceType == interfaceType);
    Arcadia_Tests_assertTrue(thread, dispatchOfX->parent.interfaceType == interfaceType);
    // The operation "f" is overridden, so the dispatch of "Arcadia.MyObjectX" does not use the
    // implementation of "Arcadia.MyObjectY".
    Arcadia_Tests_assertTrue(thread, dispatchOfY->f != dispatchOfX->f);
    Arcadia_Tests_assertTrue(thread, NULL != dispatchOfY->f);
    Arcadia_Tests_assertTrue(thread, NULL != dispatchOfX->f);
    // The operation "g" is not overridden, so the dispatch of "Arcadia.MyObjectX" uses the
    // implementation of "Arcadia.MyObjectY".
    Arcadia_Tests_assertTrue(thread, dispatchOfY->g == dispatchOfX->g);
    Arcadia_Tests_assertTrue(thread, NULL != dispatchOfY->g);
    Arcadia_Tests_assertTrue(thread, NULL != dispatchOfX->g);

    // On a value of "Arcadia.MyObjectY", "f" returns "B::f" and "g" returns "B::g". On a value of
    // "Arcadia.MyObjectX", "f" is overridden to return the distinct "A::f", and "g" is inherited
    // from "Arcadia.MyObjectY", returning "B::g".
    checkMessage(thread, Arcadia_MyInterfaceB_f(thread, objectY), _fMessageOfY, sizeof(_fMessageOfY));
    checkMessage(thread, Arcadia_MyInterfaceB_g(thread, objectY), _gMessageOfY, sizeof(_gMessageOfY));
    checkMessage(thread, Arcadia_MyInterfaceB_f(thread, objectX), _fMessageOfX, sizeof(_fMessageOfX));
    checkMessage(thread, Arcadia_MyInterfaceB_g(thread, objectX), _gMessageOfY, sizeof(_gMessageOfY));
  }
}