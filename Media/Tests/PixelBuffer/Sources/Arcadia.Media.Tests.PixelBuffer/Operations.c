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

#include "Operations.h"
#include "Arcadia/Media/Include.h"
#include "Arcadia/ADL/Include.h"
#include <stddef.h>
#include <stdio.h>

struct PixelOperationABI { Arcadia_Object _parent; };

struct PixelOperationDispatchABI {
  Arcadia_ObjectDispatch _parent;
  void (*apply)(Arcadia_Thread*, Arcadia_Media_PixelBufferOperation*, Arcadia_Media_PixelBuffer*);
};

_Static_assert(sizeof(Arcadia_Media_PixelBufferOperation) == sizeof(struct PixelOperationABI), "operation size");
_Static_assert(_Alignof(Arcadia_Media_PixelBufferOperation) == _Alignof(struct PixelOperationABI), "operation alignment");
_Static_assert(sizeof(Arcadia_Media_PixelBufferOperationDispatch) == sizeof(struct PixelOperationDispatchABI), "dispatch size");
_Static_assert(_Alignof(Arcadia_Media_PixelBufferOperationDispatch) == _Alignof(struct PixelOperationDispatchABI), "dispatch alignment");
_Static_assert(offsetof(Arcadia_Media_PixelBufferOperationDispatch, apply) == offsetof(struct PixelOperationDispatchABI, apply), "apply offset");
_Static_assert(_Generic(&Arcadia_Media_PixelBufferOperation_apply, void (*)(Arcadia_Thread*, Arcadia_Media_PixelBufferOperation*, Arcadia_Media_PixelBuffer*): 1, default: 0), "public signature");
_Static_assert(_Generic(((Arcadia_Media_PixelBufferOperationDispatch*)0)->apply, void (*)(Arcadia_Thread*, Arcadia_Media_PixelBufferOperation*, Arcadia_Media_PixelBuffer*): 1, default: 0), "slot signature");

typedef struct MediaPixelOperationProbe MediaPixelOperationProbe;
typedef struct MediaPixelOperationProbeDispatch MediaPixelOperationProbeDispatch;

struct MediaPixelOperationProbe {
  Arcadia_Media_PixelBufferOperation _parent;
  Arcadia_String* payload;
};

struct MediaPixelOperationProbeDispatch {
  Arcadia_Media_PixelBufferOperationDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.Media.PixelOperationProbe", MediaPixelOperationProbe,
                          u8"Arcadia.Media.PixelBufferOperation");

static Arcadia_SizeValue calls;
static Arcadia_BooleanValue throwApply;
static Arcadia_Media_PixelBuffer* expectedTarget;
static Arcadia_SizeValue destroyed[3];

static void
onDestroy
  (
    void* observer,
    Arcadia_Object* object
  )
{ (*(Arcadia_SizeValue*)observer)++; }

static void
constructProbe
  (
    Arcadia_Thread* thread,
    MediaPixelOperationProbe* self
  )
{
  Arcadia_EnterConstructor(MediaPixelOperationProbe);
  if (_numberOfArguments != 0) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->payload = Arcadia_String_createFromCxxString(thread, "pixel operation payload");
  Arcadia_LeaveConstructor(MediaPixelOperationProbe);
}

static void
visitProbe
  (
    Arcadia_Thread* thread,
    MediaPixelOperationProbe* self
  )
{ if (self->payload) Arcadia_Object_visit(thread, (void*)self->payload); }

static void
applyProbe
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperation* operation,
    Arcadia_Media_PixelBuffer* target
  )
{
  MediaPixelOperationProbe* self = (void*)operation;
  calls++;
  Arcadia_Tests_assertTrue(thread, target == expectedTarget);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, self->payload, "pixel operation payload", sizeof("pixel operation payload") - 1));
  if (target) Arcadia_Media_PixelBuffer_setPixelRGBA(thread, target, 1, 2, 7, 31, 199, 241);
  if (throwApply) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
initializeProbe
  (
    Arcadia_Thread* thread,
    MediaPixelOperationProbeDispatch* self
  )
{ ((Arcadia_Media_PixelBufferOperationDispatch*)self)->apply = &applyProbe; }

static const Arcadia_ObjectType_Operations probeObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructProbe,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitProbe,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeProbe,
};

static const Arcadia_Type_Operations probeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &probeObjects,
};

Arcadia_defineObjectType(u8"Tests.Media.PixelOperationProbe", MediaPixelOperationProbe,
                         u8"Arcadia.Media.PixelBufferOperation", Arcadia_Media_PixelBufferOperation,
                         &probeOperations);

static MediaPixelOperationProbe*
createProbe
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  return (void*)_Arcadia_EndCreate0(thread, _MediaPixelOperationProbe_getType(thread), base);
}

static void
checkPixel
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBuffer* buffer,
    Arcadia_Integer32Value x,
    Arcadia_Integer32Value y,
    Arcadia_Natural8Value r,
    Arcadia_Natural8Value g,
    Arcadia_Natural8Value b,
    Arcadia_Natural8Value a
  )
{
  Arcadia_Natural8Value rr, gg, bb, aa;
  Arcadia_Media_PixelBuffer_getPixelRGBA(thread, buffer, x, y, &rr, &gg, &bb, &aa);
  Arcadia_Tests_assertTrue(thread, rr == r && gg == g && bb == b && aa == a);
}

static void
testDispatch
  (
    Arcadia_Thread* thread
  )
{
  calls = 0; throwApply = false;
  for (unsigned i = 0; i < 3; ++i) destroyed[i] = 0;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_TypeValue type = _Arcadia_Media_PixelBufferOperation_getType(thread);
  Arcadia_Tests_assertTrue(thread, !((Arcadia_Media_PixelBufferOperationDispatch*)Arcadia_ObjectType_getDispatch(type))->apply);
  MediaPixelOperationProbe* self = createProbe(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Media_PixelBuffer* target = Arcadia_Media_PixelBuffer_create(thread, 3, 4, 5, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, target);
  Arcadia_Object* tracked[] = { (void*)self, (void*)self->payload, (void*)target };
  for (unsigned i = 0; i < 3; ++i) Arcadia_Object_addNotifyDestroyCallback(thread, tracked[i], &destroyed[i], &onDestroy);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (void*)self, type));
  expectedTarget = target;
  Arcadia_Media_PixelBufferOperation_apply(thread, (void*)self, target);
  checkPixel(thread, target, 1, 2, 7, 31, 199, 241);
  expectedTarget = NULL;
  Arcadia_Media_PixelBufferOperation_apply(thread, (void*)self, NULL);
  expectedTarget = target; throwApply = true;
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Media_PixelBufferOperation_apply(thread, (void*)self, target);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 302);
    Arcadia_Thread_setStatus(thread, 0);
  }
  throwApply = false;
  Arcadia_Tests_assertTrue(thread, calls == 3);
  for (unsigned i = 0; i < 3; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 0);
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (unsigned i = 0; i < 3; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 1);
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testConstructorErrors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  for (unsigned count = 1; count <= 255; ++count) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      for (unsigned i = 0; i < count; ++i) Arcadia_ValueStack_pushBooleanValue(thread, false);
      Arcadia_ValueStack_pushNatural8Value(thread, (Arcadia_Natural8Value)count);
      _Arcadia_EndCreate0(thread, _Arcadia_Media_PixelBufferOperation_getType(thread), depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  const Arcadia_Value badCounts[] = {
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), Arcadia_Value_makeBooleanValue(false),
    Arcadia_Value_makeInteger8Value(0), Arcadia_Value_makeInteger16Value(0), Arcadia_Value_makeInteger32Value(0), Arcadia_Value_makeInteger64Value(0),
    Arcadia_Value_makeNatural16Value(0), Arcadia_Value_makeNatural32Value(0), Arcadia_Value_makeNatural64Value(0),
    Arcadia_Value_makeSizeValue(0), Arcadia_Value_makeReal32Value(0.f), Arcadia_Value_makeReal64Value(0.),
  };
  for (unsigned i = 0; i < sizeof(badCounts) / sizeof(badCounts[0]); ++i) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_ValueStack_pushValue(thread, &badCounts[i]);
      _Arcadia_EndCreate0(thread, _Arcadia_Media_PixelBufferOperation_getType(thread), depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
    }
  }
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testNativeConsumers
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, definitions);
  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, "fixture");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
  Arcadia_ADL_ColorDefinition* first = Arcadia_ADL_ColorDefinition_create(thread, definitions, name, 13, 79, 227);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, first);
  Arcadia_ADL_ColorDefinition* second = Arcadia_ADL_ColorDefinition_create(thread, definitions, name, 211, 43, 5);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, second);
  Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* fillDefinition = Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_create(thread, definitions, name, name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, fillDefinition);
  fillDefinition->color->definition = (void*)first;
  Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* checkerDefinition = Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_create(thread, definitions, name, 2, 3, name, name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, checkerDefinition);
  checkerDefinition->firstCheckerColor->definition = (void*)first;
  checkerDefinition->secondCheckerColor->definition = (void*)second;
  Arcadia_Media_PixelBufferOperations_Fill* fill = Arcadia_Media_PixelBufferOperations_Fill_create(thread, fillDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, fill);
  Arcadia_Media_PixelBufferOperations_CheckerboardFill* checker = Arcadia_Media_PixelBufferOperations_CheckerboardFill_create(thread, checkerDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, checker);
  Arcadia_Media_PixelBuffer* buffer = Arcadia_Media_PixelBuffer_create(thread, 3, 5, 7, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, buffer);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Media_PixelBufferOperation_apply(thread, (void*)fill, buffer);
  for (Arcadia_Integer32Value y = 0; y < 7; ++y) {
    for (Arcadia_Integer32Value x = 0; x < 5; ++x) checkPixel(thread, buffer, x, y, 13, 79, 227, 255);
  }
  Arcadia_Media_PixelBufferOperation_apply(thread, (void*)checker, buffer);
  for (Arcadia_Integer32Value y = 0; y < 7; ++y) {
    for (Arcadia_Integer32Value x = 0; x < 5; ++x) {
      Arcadia_ADL_ColorDefinition* expected = ((x / 2 + y / 3) % 2 == 0) ? first : second;
      checkPixel(thread, buffer, x, y, expected->red, expected->green, expected->blue, 255);
    }
  }
  // Unresolved definitions still fail in the native implementation, without
  // consuming any caller slots. The generated base must not swallow errors.
  fillDefinition->color->definition = NULL;
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Media_PixelBufferOperation_apply(thread, (void*)fill, buffer);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationInvalid);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 9);
  }
  Arcadia_ValueStack_popValues(thread, 9);
}

static void
testWrapperSafety
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  MediaPixelOperationProbe* self = createProbe(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_String* wrong = Arcadia_String_createFromCxxString(thread, "wrong type");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, wrong);
  Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  // Runtime construction is available to native subclasses even though this
  // abstract class has no public generated factory.
  Arcadia_Media_PixelBufferOperation* unbound = (void*)_Arcadia_EndCreate0(thread, _Arcadia_Media_PixelBufferOperation_getType(thread), argumentsBase);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, unbound);
  calls = 0; throwApply = false;
  for (unsigned operation = 0; operation < 4; ++operation) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      switch (operation) {
        case 0: Arcadia_Media_PixelBufferOperation_apply(thread, NULL, NULL); break;
        case 1: Arcadia_Media_PixelBufferOperation_apply(thread, (void*)wrong, NULL); break;
        case 2: Arcadia_Media_PixelBufferOperation_apply(thread, (void*)self, (void*)wrong); break;
        case 3: Arcadia_Media_PixelBufferOperation_apply(thread, unbound, NULL); break;
      }
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (operation == 3 ? Arcadia_Status_OperationInvalid : Arcadia_Status_ArgumentTypeInvalid));
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth && calls == 0);
    }
  }
  Arcadia_ValueStack_popValues(thread, 3);
  for (unsigned fail = 0; fail < 2; ++fail) {
    for (unsigned i = 0; i < 3; ++i) destroyed[i] = 0;
    self = createProbe(thread);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
    Arcadia_Media_PixelBuffer* target = Arcadia_Media_PixelBuffer_create(thread, 1, 3, 4, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, target);
    Arcadia_Object* tracked[] = { (void*)self, (void*)self->payload, (void*)target };
    for (unsigned i = 0; i < 3; ++i) Arcadia_Object_addNotifyDestroyCallback(thread, tracked[i], &destroyed[i], &onDestroy);
    expectedTarget = target; throwApply = fail != 0;
    Arcadia_ValueStack_popValues(thread, 2);
    // Neither the receiver nor the buffer has a caller root. applyProbe forces
    // collection and then uses both. The wrapper must root them and unwind on
    // both ordinary returns and callback failures.
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Media_PixelBufferOperation_apply(thread, (void*)self, target);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, !fail);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, fail && Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
      Arcadia_Thread_setStatus(thread, 0);
    }
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
    for (unsigned i = 0; i < 3; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 0);
    checkPixel(thread, target, 1, 2, 7, 31, 199, 241);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    for (unsigned i = 0; i < 3; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 1);
  }
  throwApply = false;
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Media_Tests_PixelBuffer_operations
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testDispatch)) { fprintf(stderr, "pixel operation dispatch failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testConstructorErrors)) { fprintf(stderr, "pixel operation constructor errors failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testNativeConsumers)) { fprintf(stderr, "pixel operation native consumers failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testWrapperSafety)) { fprintf(stderr, "pixel operation wrapper safety failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}
