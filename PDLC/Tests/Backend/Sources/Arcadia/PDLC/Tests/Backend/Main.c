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

#include "Arcadia/Testing/Include.h"

#define TEST_PRIVATE (1)

#include "compute.h"
#include "factorial.h"
#include "exercise.h"
#include "fail.h"
#include "emptyCall.h"
#include "builtin.h"
#include "holderRun.h"
#include "Test/Native.h"
#include "Test/Colored.h"
#include "colorRun.h"
#include "Test/ColoredChild.h"
#include "Test/OtherColor.h"
#include "enumConstantRun.h"
#include "bareReturn.h"
#include "nativeEnumConstant.h"
#include "wrongEnumConstant.h"
#if !defined(TEST_PALETTE_COLOR_H_INCLUDED) || defined(TEST_COLOR_H_INCLUDED)
#error "Mapped enum guards must follow the header path, not the type name"
#endif
#include "strictEnumConstruct.h"
#include "abstractRun.h"
#include "escapedRun.h"
#include "relay.h"
#include "makeBase.h"
#include "voidArgument.h"
#include "renamed.h"
#include "Test/Base.h"
#include "Test/Derived.h"
#include "Test/DerivedBox.h"
#include "Test/DerivedContext.h"
#include "nullableIdentity.h"
#include "nullableOptional.h"
#include "nullableCall.h"
#include "nullableRun.h"
#include "nullableNullReceiver.h"
#include "nullableNullField.h"
#include "externalIdentity.h"
#include "externalRun.h"
#include "Test/ExternalDerived.h"
#include "Test/Sized.h"
#include "Test/NativeEnumHolder.h"
#include "Test/NativeEnumBox.h"
#include "Test/NativeEnumChild.h"
#include "nativeEnumFromAny.h"
#include "nativeEnumConstruct.h"
#include "nativeUnsignedEnumIdentity.h"
#include "Test/Nonvirtual.h"
#include "readOnlyValue.h"
#include "NativeGenerated/Type.h"
#include "nativeHeaderRun.h"
#include "Test/AbstractBase.h"
#include "Test/AbstractConcrete.h"
#include "Test/AbstractInherited.h"
#include "Test/AbstractAgain.h"
#include "Test/AbstractFinal.h"
#include "Test/AbstractNativeBase.h"
#include "Test/AbstractNativeConcrete.h"
#include "abstractMethodRun.h"
#include "Test/Namespaced/nativeIdentity.h"
#include "Test/Namespaced/roundTrip.h"

_Static_assert(sizeof(Test_NonvirtualDispatch) == sizeof(Arcadia_ObjectDispatch), "nonvirtual methods must not add dispatch slots");
#include "sizeIdentity.h"
#include "sizeFromAny.h"
#include <stddef.h>
#include <string.h>

void
nativeCollect
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
}

Arcadia_Natural64Value
nativeRegistryPing
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{
  nativeCollect(thread);
  return 7;
}

Arcadia_Natural64Value
nativeTwice
  (
    Arcadia_Thread* thread,
    Arcadia_Natural64Value n
  )
{ return n * 2; }

Arcadia_Value
nativeIdentity
  (
    Arcadia_Thread* thread,
    Arcadia_Value value
  )
{
  nativeCollect(thread);
  return value;
}

void
nativeConstruct
  (
    Arcadia_Thread* thread,
    Test_Native* self,
    Arcadia_Natural64Value n
  )
{ self->value = n; }

Arcadia_String*
nativeName
  (
    Arcadia_Thread* thread,
    Test_ValueBox* self
  )
{
  nativeCollect(thread);
  return Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread, "value-box", sizeof("value-box") - 1));
}

Arcadia_Natural64Value
nativeGet
  (
    Arcadia_Thread* thread,
    Test_Native* self
  )
{ return self->value; }

Test_Base*
nativeNullableContext
  (
    Arcadia_Thread* thread,
    Test_OptionalContext* self,
    Test_Base* value
  )
{
  nativeCollect(thread);
  return value;
}

Arcadia_Object*
nativeNullableOptional
  (
    Arcadia_Thread* thread,
    Test_OptionalContext* self,
    Arcadia_Object* value
  )
{
  nativeCollect(thread);
  return value;
}

static void
testNullable
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Value absent = Test_nullableRun(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&absent));
  Arcadia_Tests_assertTrue(thread, Test_nullableIdentity(thread, NULL) == NULL);
  Arcadia_Tests_assertTrue(thread, Test_nullableOptional(thread, NULL) == NULL);
  Arcadia_Tests_assertTrue(thread, Test_nullableCall(thread, absent) == NULL);

  Test_DerivedContext* derived = Test_DerivedContext_create(thread, NULL, NULL);
  Test_OptionalContext* context = (Test_OptionalContext*)derived;
  Arcadia_ValueStack_pushObjectReferenceValue(thread, context);
  Arcadia_SizeValue rooted = Arcadia_ValueStack_getSize(thread);
  Arcadia_Tests_assertTrue(thread, Test_OptionalContext_getContext(thread, context) == NULL);
  Arcadia_Tests_assertTrue(thread, Test_OptionalContext_getOptional(thread, context) == NULL);
  // The setter roots its incoming reference before collection; the field then keeps it live.
  Test_Base* child = (Test_Base*)Test_Derived_create(thread, 37);
  Test_OptionalContext_setContext(thread, context, child);
  nativeCollect(thread);
  Arcadia_Tests_assertTrue(thread, Test_OptionalContext_getContext(thread, context) == child);
  Arcadia_Tests_assertTrue(thread, Test_nullableIdentity(thread, child) == child);
  Arcadia_Tests_assertTrue(thread, Test_OptionalContext_nativeContext(thread, context, child) == child);
  Arcadia_Tests_assertTrue(thread, Test_Base_get(thread, child) == 137);
  Test_OptionalContext_setContext(thread, context, NULL);
  nativeCollect(thread);
  Arcadia_Tests_assertTrue(thread, Test_OptionalContext_getContext(thread, context) == NULL);
  Arcadia_Tests_assertTrue(thread, Test_OptionalContext_nativeContext(thread, context, NULL) == NULL);
  Test_OptionalContext_setOptional(thread, context, NULL);
  Arcadia_Tests_assertTrue(thread, Test_OptionalContext_nativeOptional(thread, context, NULL) == NULL);
  Test_OptionalContext_storeAny(thread, context, absent);
  Arcadia_Tests_assertTrue(thread, Test_OptionalContext_returnAny(thread, context, absent) == NULL);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == rooted);

  enum {
    BadScalarArgument, BadObjectArgument, BadInterfaceArgument, NullTypedReceiver,
    NullPdlReceiver, NullFieldReceiver, BadFieldValue, BadScalarResult,
    BadTypedArgument, BadTypedInterfaceArgument, BadConstructorArgument, BadObjectResult,
    FailureCaseCount
  };
  for (unsigned which = 0; which < FailureCaseCount; ++which) {
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      switch (which) {
        case BadScalarArgument: Test_nullableCall(thread, Arcadia_Value_makeBooleanValue(false)); break;
        case BadObjectArgument: Test_nullableCall(thread, Arcadia_Value_makeObjectReferenceValue(context)); break;
        case BadInterfaceArgument: Test_nullableOptional(thread, (Arcadia_Object*)context); break;
        case NullTypedReceiver: Test_OptionalContext_setContext(thread, NULL, NULL); break;
        case NullPdlReceiver: Test_nullableNullReceiver(thread); break;
        case NullFieldReceiver: Test_nullableNullField(thread); break;
        case BadFieldValue: Test_OptionalContext_storeAny(thread, context, Arcadia_Value_makeBooleanValue(false)); break;
        case BadScalarResult: Test_OptionalContext_returnAny(thread, context, Arcadia_Value_makeBooleanValue(false)); break;
        case BadTypedArgument: Test_OptionalContext_setContext(thread, context, (Test_Base*)context); break;
        case BadTypedInterfaceArgument: Test_OptionalContext_setOptional(thread, context, (Arcadia_Object*)context); break;
        case BadConstructorArgument: Test_OptionalContext_create(thread, (Test_Base*)context, NULL); break;
        case BadObjectResult: Test_OptionalContext_returnAny(thread, context, Arcadia_Value_makeObjectReferenceValue(context)); break;
      }
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == rooted);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Natural64Value (*compute)(Arcadia_Thread*, Arcadia_Natural64Value) = Test_compute;
  void (*voidArgument)(Arcadia_Thread*, Arcadia_VoidValue) = Test_voidArgument;
  Arcadia_Tests_assertTrue(thread, compute(thread, 5) == 210);
  voidArgument(thread, Arcadia_VoidValue_Void);
  Arcadia_Tests_assertTrue(thread, Test_renamed(thread, 3, 4) == 7);
  Test_Base* child = Test_makeBase(thread, 9);
  Arcadia_Value value = Arcadia_Value_makeObjectReferenceValue(child);
  // The typed callee must root a general-value argument before collecting.
  Arcadia_Value relayed = Test_relay(thread, value);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_getObjectReferenceValue(&relayed) == (Arcadia_Object*)child);
  Test_DerivedBox* box = Test_DerivedBox_create(thread, relayed);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, box);
  nativeCollect(thread);
  Arcadia_Value stored = Test_ValueBox_readValue(thread, (Test_ValueBox*)box);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_getObjectReferenceValue(&stored) == (Arcadia_Object*)child);
  Test_Base* returned = Test_ValueBox_take(thread, (Test_ValueBox*)box, child, Test_Color_Blue);
  Arcadia_Tests_assertTrue(thread, returned == child && Test_Base_get(thread, returned) == 9);
  Test_ValueBox_replaceValue(thread, (Test_ValueBox*)box, Arcadia_Value_makeNatural64Value(23));
  stored = Test_ValueBox_readValue(thread, (Test_ValueBox*)box);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_getNatural64Value(&stored) == 23);
  Arcadia_String* name = Test_ValueBox_readName(thread, (Test_ValueBox*)box);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_getNumberOfBytes(thread, name) == sizeof("value-box") - 1);
  Arcadia_Tests_assertTrue(thread, !memcmp(Arcadia_String_getBytes(thread, name), "value-box", sizeof("value-box") - 1));
  Arcadia_ValueStack_popValues(thread, 1);
  // These accesses also verify the generated object and dispatch base-member names.
  Arcadia_Tests_assertTrue(thread, offsetof(Test_Colored, _parent) == 0);
  Arcadia_Tests_assertTrue(thread, offsetof(Test_ColoredDispatch, _parent) == 0);
  Test_Color colored = Test_colorRun(thread, Test_Color_Blue);
  Arcadia_Tests_assertTrue(thread, colored == Test_Color_Blue);
  Arcadia_Tests_assertTrue(thread, Test_abstractRun(thread, 5) == 5);
  Arcadia_Tests_assertTrue(thread, Test_escapedRun(thread, 5) == 57);
  Arcadia_Tests_assertTrue(thread, Test_emptyCall(thread) == 42);
  Arcadia_Tests_assertTrue(thread, Test_compute(thread, 5) == 210);
  Arcadia_Tests_assertTrue(thread, Test_factorial(thread, 5) == 120);
  Arcadia_Tests_assertTrue(thread, Test_exercise(thread, 5) == 15);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
  Arcadia_Tests_assertTrue(thread, Test_builtin(thread, 5) == 7);
  Arcadia_Tests_assertTrue(thread, Test_holderRun(thread, 5) == 5);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Test_fail(thread, 5);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Thread_setStatus(thread, 0);
  }
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_ValueStack_pushBooleanValue(thread, Arcadia_BooleanValue_False);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    _Arcadia_EndCreate0(thread, _Test_Native_getType(thread), base);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_setStatus(thread, 0);
  }
}

static void
testNativeHeaderMapping
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Test_External* external = Test_External_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, external);
  Arcadia_Tests_assertTrue(thread, Test_externalIdentity(thread, external) == external);
  Arcadia_Tests_assertTrue(thread, Test_externalIdentity(thread, NULL) == NULL);
  Arcadia_Tests_assertTrue(thread, Test_externalRun(thread, external) == 37);
  Test_ExternalDerived* derived = Test_ExternalDerived_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, derived);
  Arcadia_Tests_assertTrue(thread, Test_externalRun(thread, (Test_External*)derived) == 37);
  Arcadia_ValueStack_popValues(thread, 2);
  Test_NativeEnumHolder* holder = Test_NativeEnumHolder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, holder);
  Arcadia_Tests_assertTrue(thread, holder->choice == Test_External_Kind_First);
  holder->choice = Test_External_Kind_Second;
  nativeCollect(thread);
  Arcadia_Tests_assertTrue(thread, holder->choice == Test_External_Kind_Second && holder->choice == 7);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

Arcadia_Natural64Value
nativeReadOnlyValue
  (
    Arcadia_Thread* thread,
    Test_Base const* value
  )
{
  nativeCollect(thread);
  return value ? value->value : 0;
}

static void
testSemanticBindings
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Natural64Value (*read)(Arcadia_Thread*, Test_Base const*) = &Test_readOnlyValue;
  Arcadia_Tests_assertTrue(thread, read(thread, Test_Base_create(thread, 37)) == 37);
  Arcadia_Tests_assertTrue(thread, read(thread, NULL) == 0);
  Arcadia_Tests_assertTrue(thread, Test_Nonvirtual_direct(thread, Test_Nonvirtual_create(thread)) == 29);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Test_NativeDeclared* declared = _Arcadia_EndCreate0(thread, _Test_NativeDeclared_getType(thread), base);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, declared);
  Arcadia_Tests_assertTrue(thread, Test_nativeHeaderRun(thread, declared) == 48);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

Arcadia_SizeValue
nativeSizeEcho
  (
    Arcadia_Thread* thread,
    Test_Sized* self,
    Arcadia_SizeValue value
  )
{
  nativeCollect(thread);
  Arcadia_Tests_assertTrue(thread, self->length == value);
  return value;
}

static void
testSizes
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_SizeValue maximum = Arcadia_SizeValue_Maximum;
  Arcadia_Tests_assertTrue(thread, Test_sizeIdentity(thread, maximum) == maximum);
  Arcadia_Tests_assertTrue(thread, Test_sizeFromAny(thread, Arcadia_Value_makeSizeValue(maximum)) == maximum);
  Test_Sized* self = Test_Sized_create(thread, maximum);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Tests_assertTrue(thread, Test_Sized_getLength(thread, self) == maximum);
  Arcadia_Tests_assertTrue(thread, Test_Sized_echo(thread, self, maximum) == maximum);
  Test_Sized_setLength(thread, self, 0);
  Arcadia_Tests_assertTrue(thread, Test_Sized_getLength(thread, self) == 0);
  Arcadia_Tests_assertTrue(thread, Test_Sized_echo(thread, self, 0) == 0);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Test_sizeFromAny(thread, Arcadia_Value_makeNatural64Value(0));
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 1);
    Arcadia_Thread_setStatus(thread, 0);
  }
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

Arcadia_Natural64Value
nativeAbstractRead
  (
    Arcadia_Thread* thread,
    Test_AbstractNativeConcrete* self
  )
{
  nativeCollect(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Test_AbstractNativeConcrete_getType(thread)));
  return 77;
}

static unsigned rawAcceptCalls;

static void
rawAccept
  (
    Arcadia_Thread* thread,
    Test_AbstractNativeBase* self,
    Test_Base* value,
    Arcadia_Object* optional
  )
{
  rawAcceptCalls++;
  nativeCollect(thread);
}

static void
testAbstractArguments
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Test_AbstractNativeConcrete* self = Test_AbstractNativeConcrete_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Test_Base* value = Test_Base_create(thread, 5);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, value);
  // Install a raw native override, bypassing generated method-body checks.
  Test_AbstractNativeBaseDispatch* dispatch = (void*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (void*)self));
  dispatch->accept = &rawAccept;
  rawAcceptCalls = 0;
  Test_AbstractNativeBase_accept(thread, (void*)self, value, NULL);
  Test_AbstractNativeBase_accept(thread, (void*)self, NULL, NULL);
  Arcadia_Tests_assertTrue(thread, rawAcceptCalls == 2);
  for (unsigned argument = 0; argument < 2; ++argument) {
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Test_AbstractNativeBase_accept(thread, (void*)self, argument == 0 ? (void*)self : value,
        argument == 1 ? (void*)value : NULL);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, rawAcceptCalls == 2 && Arcadia_ValueStack_getSize(thread) == base + 302);
    }
  }
  Arcadia_ValueStack_popValues(thread, 302);
}

static void
testAbstractMethods
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Test_AbstractInherited* inherited = Test_AbstractInherited_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, inherited);
  Arcadia_Tests_assertTrue(thread, Test_AbstractBase_read(thread, (Test_AbstractBase*)inherited) == 73);
  Arcadia_Tests_assertTrue(thread, Test_AbstractBase_optional(thread, (Test_AbstractBase*)inherited) == 5);
  Test_AbstractBase_touch(thread, (Test_AbstractBase*)inherited);
  Arcadia_Tests_assertTrue(thread, Test_abstractMethodRun(thread) == 91);
  Test_AbstractFinal* final = Test_AbstractFinal_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, final);
  Arcadia_Tests_assertTrue(thread, Test_AbstractBase_read(thread, (Test_AbstractBase*)final) == 91);
  Arcadia_Tests_assertTrue(thread, Test_AbstractAgain_read(thread, (Test_AbstractAgain*)final) == 91);
  Test_AbstractNativeConcrete* native = Test_AbstractNativeConcrete_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, native);
  Arcadia_Tests_assertTrue(thread, Test_AbstractNativeBase_read(thread, (Test_AbstractNativeBase*)native) == 77);
  Test_AbstractBaseDispatch* abstractDispatch = (Test_AbstractBaseDispatch*)Arcadia_ObjectType_getDispatch(_Test_AbstractAgain_getType(thread));
  Arcadia_Tests_assertTrue(thread, abstractDispatch->read == NULL);
  Arcadia_ValueStack_popValues(thread, 3);

  // Native code can bypass the PDL allocation restriction. The generated
  // wrapper must reject its unbound abstract slot and restore stack depth.
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Test_AbstractBase* unbound = _Arcadia_EndCreate0(thread, _Test_AbstractBase_getType(thread), base);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, unbound);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Test_AbstractBase_read(thread, unbound);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationInvalid);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 1);
    Arcadia_Thread_setStatus(thread, 0);
  }
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testEnumConstants
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Test_bareReturn(thread, true);
  Test_bareReturn(thread, false);
  Arcadia_Tests_assertTrue(thread, Test_enumConstantRun(thread) == Test_Color_Blue);
  Arcadia_Tests_assertTrue(thread, Test_nativeEnumConstant(thread, false) == Test_External_Kind_Second);
  Arcadia_Tests_assertTrue(thread, Test_nativeEnumConstant(thread, true) == Test_External_Kind_Maximum);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Test_wrongEnumConstant(thread);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_Thread_setStatus(thread, 0);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

static void
testEnumConstructorBoundary
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushInteger32Value(thread, Test_Color_Blue);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  Test_Colored* legacy = _Arcadia_EndCreate0(thread, _Test_Colored_getType(thread), base);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, legacy);
  Arcadia_Tests_assertTrue(thread, legacy->color == Test_Color_Blue);
  Arcadia_Value valid = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Test_Color_getType(thread), Test_Color_Red));
  Test_ColoredChild* child = Test_ColoredChild_create(thread, valid);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, child);
  Arcadia_Tests_assertTrue(thread, ((Test_Colored*)child)->color == Test_Color_Red);
  Test_Colored* direct = Test_strictEnumConstruct(thread, valid);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, direct);
  Arcadia_Tests_assertTrue(thread, direct->color == Test_Color_Red);
  Arcadia_Value invalid[] = {
    Arcadia_Value_makeInteger32Value(Test_Color_Red),
    Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Test_OtherColor_getType(thread), Test_OtherColor_Red)),
  };
  for (size_t i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
    for (unsigned super = 0; super < 2; ++super) {
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        if (super) Test_ColoredChild_create(thread, invalid[i]);
        else Test_strictEnumConstruct(thread, invalid[i]);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
        Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 3);
        Arcadia_Thread_setStatus(thread, 0);
      }
    }
  }
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testNativeEnumerationValues
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Test_External_Kind values[] = {
    Test_External_Kind_Minimum, Test_External_Kind_Negative, Test_External_Kind_First,
    Test_External_Kind_Second, Test_External_Kind_Maximum
  };
  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
    Test_NativeEnumBox* self = Test_NativeEnumBox_create(thread, values[i]);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
    Arcadia_Tests_assertTrue(thread, self->choice == values[i] && Test_NativeEnumBox_getChoice(thread, self) == values[i]);
    Test_NativeEnumBox_setChoice(thread, self, values[sizeof(values) / sizeof(values[0]) - 1 - i]);
    Arcadia_Tests_assertTrue(thread, Test_NativeEnumBox_getChoice(thread, self) == values[sizeof(values) / sizeof(values[0]) - 1 - i]);
    Arcadia_Tests_assertTrue(thread, Test_NativeEnumBox_echo(thread, self, values[i]) == values[i]);
    Arcadia_Value typed = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Test_External_Kind_getType(thread), values[i]));
    Arcadia_Tests_assertTrue(thread, Test_nativeEnumFromAny(thread, typed) == values[i]);
    Test_NativeEnumBox* allocated = Test_nativeEnumConstruct(thread, typed);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, allocated);
    Test_NativeEnumChild* child = Test_NativeEnumChild_create(thread, typed);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, child);
    Arcadia_Tests_assertTrue(thread, allocated->choice == values[i] && ((Test_NativeEnumBox*)child)->choice == values[i]);
    Arcadia_ValueStack_popValues(thread, 3);
  }
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushInteger32Value(thread, Test_External_Kind_Second);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  Test_NativeEnumBox* legacy = _Arcadia_EndCreate0(thread, _Test_NativeEnumBox_getType(thread), depth);
  Arcadia_Tests_assertTrue(thread, legacy->choice == Test_External_Kind_Second && Arcadia_ValueStack_getSize(thread) == depth);
  Arcadia_Value invalid[] = {
    Arcadia_Value_makeInteger32Value(7),
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void),
    Arcadia_Value_makeBooleanValue(Arcadia_BooleanValue_True),
    Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Test_Color_getType(thread), 7))
  };
  for (size_t i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
    for (unsigned operation = 0; operation < 4; ++operation) {
      if (!i && operation == 3) continue; // Legacy runtime C callbacks accept Integer32.
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        if (operation == 0) Test_nativeEnumFromAny(thread, invalid[i]);
        else if (operation == 1) Test_nativeEnumConstruct(thread, invalid[i]);
        else if (operation == 2) Test_NativeEnumChild_create(thread, invalid[i]);
        else {
          Arcadia_ValueStack_pushValue(thread, &invalid[i]);
          Arcadia_ValueStack_pushNatural8Value(thread, 1);
          _Arcadia_EndCreate0(thread, _Test_NativeEnumBox_getType(thread), depth);
        }
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid && Arcadia_ValueStack_getSize(thread) == depth);
        Arcadia_Thread_setStatus(thread, 0);
      }
    }
  }
  Arcadia_Tests_assertTrue(thread, Test_nativeUnsignedEnumIdentity(thread, Test_External_UnsignedKind_First) == Test_External_UnsignedKind_First);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Test_nativeUnsignedEnumIdentity(thread, Test_External_UnsignedKind_OutsideValueRange);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid && Arcadia_ValueStack_getSize(thread) == depth);
    Arcadia_Thread_setStatus(thread, 0);
  }
  Test_NativeEnumBox* self = Test_NativeEnumBox_create(thread, Test_External_Kind_First);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Value zero = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Test_External_UnsignedKind_getType(thread), 0));
  Arcadia_Tests_assertTrue(thread, Test_NativeEnumBox_unsignedFromAny(thread, self, zero) == Test_External_UnsignedKind_First);
  Arcadia_Value negative = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Test_External_UnsignedKind_getType(thread), -1));
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Test_NativeEnumBox_unsignedFromAny(thread, self, negative);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid && Arcadia_ValueStack_getSize(thread) == depth + 1);
    Arcadia_Thread_setStatus(thread, 0);
  }
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testQualifiedProcedures
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Value input = Arcadia_Value_makeNatural64Value(Arcadia_Natural64Value_Maximum);
  Arcadia_Value output = Test_Namespaced_roundTrip(thread, input);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isNatural64Value(&output) && Arcadia_Value_getNatural64Value(&output) == Arcadia_Natural64Value_Maximum);
  output = Test_Namespaced_roundTrip(thread, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&output));
  Arcadia_String* text = Arcadia_String_create_pn(thread, Arcadia_RuntimeByteArray_create(thread,
    "qualified procedure payload", sizeof("qualified procedure payload") - 1));
  output = Test_Namespaced_roundTrip(thread, Arcadia_Value_makeObjectReferenceValue(text));
  Arcadia_ValueStack_pushValue(thread, &output);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isObjectReferenceValue(&output) && Arcadia_Value_getObjectReferenceValue(&output) == (Arcadia_Object*)text);
  output = Test_Namespaced_nativeIdentity(thread, output);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_getObjectReferenceValue(&output) == (Arcadia_Object*)text);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, text, "qualified procedure payload", sizeof("qualified procedure payload") - 1));
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static int
runExecution
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&test) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runNullable
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNullable) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runNativeHeaderMapping
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNativeHeaderMapping) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runSemanticBindings
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testSemanticBindings) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAbstractMethods
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testAbstractMethods) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runAbstractArguments
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testAbstractArguments) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runEnumConstants
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testEnumConstants) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runEnumConstructorBoundary
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testEnumConstructorBoundary) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runNativeEnumerationValues
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testNativeEnumerationValues) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runQualifiedProcedures
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testQualifiedProcedures) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int
runSizes
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(&testSizes) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int
main
  (
    int argc,
    char** argv
  )
{
  static const Arcadia_Testing_Case cases[] = {
    { "Execution", &runExecution },
    { "Nullable", &runNullable },
    { "NativeHeaderMapping", &runNativeHeaderMapping },
    { "SemanticBindings", &runSemanticBindings },
    { "AbstractMethods", &runAbstractMethods },
    { "AbstractArguments", &runAbstractArguments },
    { "EnumConstants", &runEnumConstants },
    { "EnumConstructorBoundary", &runEnumConstructorBoundary },
    { "NativeEnumerationValues", &runNativeEnumerationValues },
    { "QualifiedProcedures", &runQualifiedProcedures },
    { "Sizes", &runSizes },
  };
  return Arcadia_Testing_runSuite(argc, argv, cases, sizeof(cases) / sizeof(cases[0]));
}
