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

#include "FillModels.h"
#include "Arcadia/Media/Include.h"
#include "Arcadia/ADL/Include.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct FillABI { Arcadia_Media_PixelBufferOperation _parent; Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* definition; };

struct CheckerABI { Arcadia_Media_PixelBufferOperation _parent; Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* definition; };

#define ABI(Type, Model, Definition) \
  _Static_assert(sizeof(Type) == sizeof(struct Model), #Type " size"); \
  _Static_assert(_Alignof(Type) == _Alignof(struct Model), #Type " alignment"); \
  _Static_assert(offsetof(Type, definition) == offsetof(struct Model, definition), #Type " field offset"); \
  _Static_assert(_Generic(((Type*)0)->definition, Definition*: 1, default: 0), #Type " field type"); \
  _Static_assert(sizeof(Type##Dispatch) == sizeof(Arcadia_Media_PixelBufferOperationDispatch), #Type " adds no slots"); \
  _Static_assert(_Alignof(Type##Dispatch) == _Alignof(Arcadia_Media_PixelBufferOperationDispatch), #Type " dispatch alignment"); \
  _Static_assert(_Generic(&Type##_create, Type* (*)(Arcadia_Thread*, Definition*): 1, default: 0), #Type " factory signature"); \
  _Static_assert(_Generic(&Type##_apply, void (*)(Arcadia_Thread*, Type*, Arcadia_Media_PixelBuffer*): 1, default: 0), #Type " typed wrapper signature");
ABI(Arcadia_Media_PixelBufferOperations_Fill, FillABI, Arcadia_ADL_PixelBufferOperations_FillOperationDefinition)
ABI(Arcadia_Media_PixelBufferOperations_CheckerboardFill, CheckerABI, Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition)
#undef ABI

typedef struct NativeFillProbe NativeFillProbe;
typedef struct NativeFillProbeDispatch NativeFillProbeDispatch;
typedef struct NativeCheckerProbe NativeCheckerProbe;
typedef struct NativeCheckerProbeDispatch NativeCheckerProbeDispatch;

struct NativeFillProbe { Arcadia_Media_PixelBufferOperations_Fill _parent; Arcadia_String* payload; };

struct NativeFillProbeDispatch { Arcadia_Media_PixelBufferOperations_FillDispatch _parent; };

struct NativeCheckerProbe { Arcadia_Media_PixelBufferOperations_CheckerboardFill _parent; Arcadia_String* payload; };

struct NativeCheckerProbeDispatch { Arcadia_Media_PixelBufferOperations_CheckerboardFillDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.NativeFillProbe", NativeFillProbe,
                          u8"Arcadia.Media.PixelBufferOperations.Fill");
Arcadia_declareObjectType(u8"Tests.NativeCheckerProbe", NativeCheckerProbe,
                          u8"Arcadia.Media.PixelBufferOperations.CheckerboardFill");

#define PROBE(Name, Parent) \
  static void \
  construct##Name \
    ( \
      Arcadia_Thread* thread, \
      Name* self \
    ) \
  { \
    Arcadia_EnterConstructor(Name); \
    if (_numberOfArguments != 1) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); } \
    Arcadia_Value definition = Arcadia_ValueStack_getValue(thread, 1); \
    Arcadia_ValueStack_pushValue(thread, &definition); \
    Arcadia_ValueStack_pushNatural8Value(thread, 1); \
    Arcadia_superTypeConstructor(thread, _type, self); \
    self->payload = Arcadia_String_createFromCxxString(thread, "native fill payload"); \
    Arcadia_LeaveConstructor(Name); \
  } \
  static void \
  visit##Name \
    ( \
      Arcadia_Thread* thread, \
      Name* self \
    ) \
  { if (self->payload) Arcadia_Object_visit(thread, (void*)self->payload); } \
  static const Arcadia_ObjectType_Operations objects##Name = { Arcadia_ObjectType_Operations_Initializer, \
    .construct = (Arcadia_Object_ConstructCallbackFunction*)&construct##Name, .visit = (Arcadia_Object_VisitCallbackFunction*)&visit##Name }; \
  static const Arcadia_Type_Operations operations##Name = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &objects##Name }; \
  Arcadia_defineObjectType(u8"Tests." #Name, Name, u8"Arcadia.Media.PixelBufferOperations." #Parent, Arcadia_Media_PixelBufferOperations_##Parent, &operations##Name);
PROBE(NativeFillProbe, Fill)
PROBE(NativeCheckerProbe, CheckerboardFill)
#undef PROBE

typedef struct FillFixture {
  Arcadia_ADL_Definitions* definitions;
  Arcadia_String* name;
  Arcadia_ADL_ColorDefinition* first;
  Arcadia_ADL_ColorDefinition* second;
  Arcadia_ADL_PixelBufferOperations_FillOperationDefinition* fillDefinition;
  Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition* checkerDefinition;
  Arcadia_Media_PixelBufferOperations_Fill* fill;
  Arcadia_Media_PixelBufferOperations_CheckerboardFill* checker;
  Arcadia_Media_PixelBuffer* buffer;
} FillFixture;

// Leave all nine fixture objects rooted until the caller establishes the
// narrower ownership graph it wants to verify.
static FillFixture
createFixture
  (
    Arcadia_Thread* thread
  )
{
  FillFixture f;
  f.definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, f.definitions);
  f.name = Arcadia_String_createFromCxxString(thread, "fixture");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, f.name);
  f.first = Arcadia_ADL_ColorDefinition_create(thread, f.definitions, f.name, 13, 79, 227);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, f.first);
  f.second = Arcadia_ADL_ColorDefinition_create(thread, f.definitions, f.name, 211, 43, 5);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, f.second);
  // Fill algorithms intentionally retain opaque output despite stored ADL alpha.
  f.first->alpha = 0;
  f.second->alpha = 128;
  f.fillDefinition = Arcadia_ADL_PixelBufferOperations_FillOperationDefinition_create(thread, f.definitions, f.name, f.name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, f.fillDefinition);
  f.fillDefinition->color->definition = (void*)f.first;
  f.checkerDefinition = Arcadia_ADL_PixelBufferOperations_CheckerboardFillOperationDefinition_create(thread, f.definitions, f.name, 2, 3, f.name, f.name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, f.checkerDefinition);
  f.checkerDefinition->firstCheckerColor->definition = (void*)f.first;
  f.checkerDefinition->secondCheckerColor->definition = (void*)f.second;
  f.fill = Arcadia_Media_PixelBufferOperations_Fill_create(thread, f.fillDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, f.fill);
  f.checker = Arcadia_Media_PixelBufferOperations_CheckerboardFill_create(thread, f.checkerDefinition);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, f.checker);
  f.buffer = Arcadia_Media_PixelBuffer_create(thread, 3, 5, 7, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, f.buffer);
  return f;
}

static void
checkPixels
  (
    Arcadia_Thread* thread,
    FillFixture* f,
    Arcadia_Media_PixelBuffer* buffer,
    unsigned checker,
    Arcadia_BooleanValue sentinel
  )
{
  for (Arcadia_Integer32Value y = 0; y < 7; ++y) {
    for (Arcadia_Integer32Value x = 0; x < 5; ++x) {
      Arcadia_Natural8Value r, g, b, a;
      Arcadia_Media_PixelBuffer_getPixelRGBA(thread, buffer, x, y, &r, &g, &b, &a);
      if (sentinel) {
        Arcadia_Tests_assertTrue(thread, r == 1 && g == 2 && b == 3 && a == 4);
      } else {
        Arcadia_ADL_ColorDefinition* expected = checker && (x / 2 + y / 3) % 2 ? f->second : f->first;
        Arcadia_Tests_assertTrue(thread, r == expected->red && g == expected->green && b == expected->blue && a == 255);
      }
    }
  }
}

static void
testPixels
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  FillFixture f = createFixture(thread);
  Arcadia_Tests_assertTrue(thread, f.fill->definition == f.fillDefinition && f.checker->definition == f.checkerDefinition);
  Arcadia_Object* operations[] = { (void*)f.fill, (void*)f.checker };
  const char* names[] = { "Arcadia.Media.PixelBufferOperations.Fill", "Arcadia.Media.PixelBufferOperations.CheckerboardFill" };
  for (unsigned i = 0; i < 2; ++i) {
    Arcadia_TypeValue type = Arcadia_Object_getType(thread, operations[i]);
    Arcadia_Name* name = Arcadia_Type_getName(thread, type);
    Arcadia_Tests_assertTrue(thread, Arcadia_Name_getNumberOfBytes(thread, name) == strlen(names[i]) && !memcmp(Arcadia_Name_getBytes(thread, name), names[i], strlen(names[i])));
    Arcadia_Tests_assertTrue(thread, Arcadia_Type_isDescendantType(thread, type, _Arcadia_Media_PixelBufferOperation_getType(thread)));
    Arcadia_Tests_assertTrue(thread, ((Arcadia_Media_PixelBufferOperationDispatch*)Arcadia_ObjectType_getDispatch(type))->apply != NULL);
  }
  const Arcadia_Natural8Value formats[] = {
    Arcadia_Media_PixelFormat_AlphaBlueGreenRedNatural8, Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8,
    Arcadia_Media_PixelFormat_BlueGreenRedNatural8, Arcadia_Media_PixelFormat_BlueGreenRedAlphaNatural8,
    Arcadia_Media_PixelFormat_RedGreenBlueNatural8, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8,
  };
  for (unsigned format = 0; format < sizeof(formats) / sizeof(formats[0]); ++format) {
    Arcadia_Media_PixelBuffer* buffer = Arcadia_Media_PixelBuffer_create(thread, 3, 5, 7, formats[format]);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, buffer);
    for (unsigned checker = 0; checker < 2; ++checker) {
      Arcadia_Media_PixelBufferOperation_apply(thread, checker ? (void*)f.checker : (void*)f.fill, buffer);
      checkPixels(thread, &f, buffer, checker, false);
    }
    Arcadia_ValueStack_popValues(thread, 1);
  }
  // The definition and its color are shared, not copied at construction.
  f.first->red = 97;
  Arcadia_Media_PixelBufferOperation_apply(thread, (void*)f.fill, f.buffer);
  checkPixels(thread, &f, f.buffer, 0, false);
  Arcadia_Media_PixelBufferOperation_apply(thread, (void*)f.checker, f.buffer);
  checkPixels(thread, &f, f.buffer, 1, false);
  // Negative -1 dimensions historically produce no cells, not a new error.
  for (unsigned axis = 0; axis < 2; ++axis) {
    f.checkerDefinition->checkerWidth = axis == 0 ? -1 : 2;
    f.checkerDefinition->checkerHeight = axis == 1 ? -1 : 3;
    Arcadia_Media_PixelBuffer_fillRectangle(thread, f.buffer, 0, 0, 5, 7, 1, 2, 3, 4);
    Arcadia_Media_PixelBufferOperation_apply(thread, (void*)f.checker, f.buffer);
    checkPixels(thread, &f, f.buffer, 1, true);
  }
  f.checkerDefinition->checkerWidth = INT32_MAX;
  f.checkerDefinition->checkerHeight = INT32_MAX;
  Arcadia_Media_PixelBufferOperation_apply(thread, (void*)f.checker, f.buffer);
  checkPixels(thread, &f, f.buffer, 0, false);
  for (unsigned error = 0; error < 5; ++error) {
    f.fillDefinition->color->definition = error == 0 ? NULL : (void*)f.first;
    f.checkerDefinition->checkerWidth = error == 1 ? 0 : 2;
    f.checkerDefinition->checkerHeight = error == 2 ? 0 : 3;
    f.checkerDefinition->firstCheckerColor->definition = error == 3 ? NULL : (void*)f.first;
    f.checkerDefinition->secondCheckerColor->definition = error == 4 ? NULL : (void*)f.second;
    Arcadia_Media_PixelBuffer_fillRectangle(thread, f.buffer, 0, 0, 5, 7, 1, 2, 3, 4);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Media_PixelBufferOperation_apply(thread, error == 0 ? (void*)f.fill : (void*)f.checker, f.buffer);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationInvalid);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 9);
      checkPixels(thread, &f, f.buffer, 0, true);
    }
  }
  Arcadia_ValueStack_popValues(thread, 9);
}

static void
testConstructors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  FillFixture f = createFixture(thread);
  const Arcadia_Value wrong[] = {
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), Arcadia_Value_makeBooleanValue(false),
    Arcadia_Value_makeInteger8Value(0), Arcadia_Value_makeInteger16Value(0), Arcadia_Value_makeInteger64Value(0),
    Arcadia_Value_makeNatural8Value(0), Arcadia_Value_makeNatural16Value(0), Arcadia_Value_makeReal32Value(0.f),
    Arcadia_Value_makeInteger32Value(0), Arcadia_Value_makeNatural32Value(0), Arcadia_Value_makeNatural64Value(0),
    Arcadia_Value_makeSizeValue(0), Arcadia_Value_makeReal64Value(0.), Arcadia_Value_makeObjectReferenceValue(f.name),
    Arcadia_Value_makeObjectReferenceValue(f.first),
  };
  for (unsigned checker = 0; checker < 2; ++checker) {
    Arcadia_TypeValue type = checker ? _Arcadia_Media_PixelBufferOperations_CheckerboardFill_getType(thread) : _Arcadia_Media_PixelBufferOperations_Fill_getType(thread);
    for (unsigned failure = 0; failure < sizeof(wrong) / sizeof(wrong[0]) + 2; ++failure) {
      Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        if (failure == sizeof(wrong) / sizeof(wrong[0]) + 1) {
          if (checker) Arcadia_Media_PixelBufferOperations_CheckerboardFill_create(thread, NULL);
          else Arcadia_Media_PixelBufferOperations_Fill_create(thread, NULL);
        } else {
          Arcadia_Value argument = failure < sizeof(wrong) / sizeof(wrong[0]) ? wrong[failure]
            : Arcadia_Value_makeObjectReferenceValue(checker ? (void*)f.fillDefinition : (void*)f.checkerDefinition);
          Arcadia_ValueStack_pushValue(thread, &argument);
          Arcadia_ValueStack_pushNatural8Value(thread, 1);
          _Arcadia_EndCreate0(thread, type, depth);
        }
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
        Arcadia_Thread_setStatus(thread, 0);
        Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      }
    }
    for (unsigned count = 0; count <= 255; ++count) {
      if (count == 1) continue;
      Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        for (unsigned i = 0; i < count; ++i) Arcadia_ValueStack_pushBooleanValue(thread, false);
        Arcadia_ValueStack_pushNatural8Value(thread, (Arcadia_Natural8Value)count);
        _Arcadia_EndCreate0(thread, type, depth);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid);
        Arcadia_Thread_setStatus(thread, 0);
        Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      }
    }
  }
  Arcadia_ValueStack_popValues(thread, 309);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static Arcadia_SizeValue destroyed[10];

static void
onDestroy
  (
    void* observer,
    Arcadia_Object* object
  )
{ (*(Arcadia_SizeValue*)observer)++; }

static void
testLifetime
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned checker = 0; checker < 2; ++checker) {
    for (unsigned derived = 0; derived < 2; ++derived) {
      FillFixture f = createFixture(thread);
      Arcadia_Media_PixelBufferOperation* operation = checker ? (void*)f.checker : (void*)f.fill;
      Arcadia_String* payload = NULL;
      if (derived) {
        Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
        Arcadia_ValueStack_pushObjectReferenceValue(thread, checker ? (void*)f.checkerDefinition : (void*)f.fillDefinition);
        Arcadia_ValueStack_pushNatural8Value(thread, 1);
        operation = (void*)_Arcadia_EndCreate0(thread, checker ? _NativeCheckerProbe_getType(thread) : _NativeFillProbe_getType(thread), argumentsBase);
        Arcadia_ValueStack_pushObjectReferenceValue(thread, operation);
        payload = checker ? ((NativeCheckerProbe*)operation)->payload : ((NativeFillProbe*)operation)->payload;
      }
      Arcadia_Object* graph[] = {
        (void*)operation, checker ? (void*)f.checkerDefinition : (void*)f.fillDefinition,
        checker ? (void*)f.checkerDefinition->firstCheckerColor : (void*)f.fillDefinition->color,
        (void*)f.first, (void*)f.definitions, (void*)f.name,
        checker ? (void*)f.checkerDefinition->secondCheckerColor : (void*)f.buffer,
        checker ? (void*)f.second : (void*)payload,
        checker ? (void*)f.buffer : NULL,
        checker ? (void*)payload : NULL,
      };
      unsigned count = checker ? (derived ? 10 : 9) : (derived ? 8 : 7);
      unsigned bufferIndex = checker ? 8 : 6;
      for (unsigned i = 0; i < count; ++i) {
        destroyed[i] = 0;
        Arcadia_Object_addNotifyDestroyCallback(thread, graph[i], &destroyed[i], &onDestroy);
      }
      Arcadia_ValueStack_popValues(thread, 9 + derived);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, operation);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, f.buffer);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      for (unsigned i = 0; i < count; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 0);
      Arcadia_Media_PixelBufferOperation_apply(thread, operation, f.buffer);
      checkPixels(thread, &f, f.buffer, checker, false);
      Arcadia_Value value = Arcadia_Value_makeObjectReferenceValue(f.buffer);
      Arcadia_ValueStack_popValues(thread, 2);
      Arcadia_ValueStack_pushValue(thread, &value);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      for (unsigned i = 0; i < count; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == (i == bufferIndex ? 0 : 1));
      Arcadia_ValueStack_popValues(thread, 1);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      Arcadia_Tests_assertTrue(thread, destroyed[bufferIndex] == 1);
    }
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
typedApply
  (
    Arcadia_Thread* thread,
    unsigned checker,
    Arcadia_Media_PixelBufferOperation* self,
    Arcadia_Media_PixelBuffer* target
  )
{
  if (checker) Arcadia_Media_PixelBufferOperations_CheckerboardFill_apply(thread, (void*)self, target);
  else Arcadia_Media_PixelBufferOperations_Fill_apply(thread, (void*)self, target);
}

static void
testTypedWrappers
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  FillFixture f = createFixture(thread);
  for (unsigned checker = 0; checker < 2; ++checker) {
    Arcadia_Media_PixelBufferOperation* operation = checker ? (void*)f.checker : (void*)f.fill;
    typedApply(thread, checker, operation, f.buffer);
    checkPixels(thread, &f, f.buffer, checker, false);
    Arcadia_Media_PixelBufferOperationDispatch* dispatch = (void*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (void*)operation));
    void (*saved)(Arcadia_Thread*, Arcadia_Media_PixelBufferOperation*, Arcadia_Media_PixelBuffer*) = dispatch->apply;
    for (unsigned error = 0; error < 4; ++error) {
      if (error == 3) dispatch->apply = NULL;
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        typedApply(thread, checker, error == 0 ? NULL : error == 1 ? (checker ? (void*)f.fill : (void*)f.checker) : operation,
          error == 2 ? (void*)f.name : f.buffer);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (error == 3 ? Arcadia_Status_OperationInvalid : Arcadia_Status_ArgumentTypeInvalid));
        Arcadia_Thread_setStatus(thread, 0);
        Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 309);
      }
      dispatch->apply = saved;
    }
  }
  Arcadia_ValueStack_popValues(thread, 309);
}

static unsigned nativeCalls;
static Arcadia_BooleanValue throwOverride;

static void
collectingOverride
  (
    Arcadia_Thread* thread,
    Arcadia_Media_PixelBufferOperation* self,
    Arcadia_Media_PixelBuffer* target
  )
{
  ++nativeCalls;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (unsigned i = 0; i < 4; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 0);
  Arcadia_String* payload = Arcadia_Object_isInstanceOf(thread, (void*)self, _NativeCheckerProbe_getType(thread))
    ? ((NativeCheckerProbe*)self)->payload : ((NativeFillProbe*)self)->payload;
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, payload, "native fill payload", sizeof("native fill payload") - 1));
  Arcadia_Media_PixelBuffer_setPixelRGBA(thread, target, 1, 2, 9, 71, 201, 123);
  if (throwOverride) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
}

static void
testTypedWrapperLifetime
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  for (unsigned checker = 0; checker < 2; ++checker) {
    for (unsigned failure = 0; failure < 2; ++failure) {
      FillFixture f = createFixture(thread);
      Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, checker ? (void*)f.checkerDefinition : (void*)f.fillDefinition);
      Arcadia_ValueStack_pushNatural8Value(thread, 1);
      Arcadia_Media_PixelBufferOperation* operation = (void*)_Arcadia_EndCreate0(thread, checker ? _NativeCheckerProbe_getType(thread) : _NativeFillProbe_getType(thread), argumentsBase);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, operation);
      Arcadia_String* payload = checker ? ((NativeCheckerProbe*)operation)->payload : ((NativeFillProbe*)operation)->payload;
      Arcadia_Object* tracked[] = { (void*)operation, (void*)payload, checker ? (void*)f.checkerDefinition : (void*)f.fillDefinition, (void*)f.buffer };
      for (unsigned i = 0; i < 4; ++i) {
        destroyed[i] = 0;
        Arcadia_Object_addNotifyDestroyCallback(thread, tracked[i], &destroyed[i], &onDestroy);
      }
      Arcadia_Media_PixelBufferOperationDispatch* dispatch = (void*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (void*)operation));
      dispatch->apply = &collectingOverride;
      nativeCalls = 0; throwOverride = failure != 0;
      Arcadia_ValueStack_popValues(thread, 10);
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        typedApply(thread, checker, operation, f.buffer);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, !failure);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, failure && Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
        Arcadia_Thread_setStatus(thread, 0);
      }
      Arcadia_Tests_assertTrue(thread, nativeCalls == 1 && Arcadia_ValueStack_getSize(thread) == base + 300);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, f.buffer);
      Arcadia_Natural8Value r, g, b, a;
      Arcadia_Media_PixelBuffer_getPixelRGBA(thread, f.buffer, 1, 2, &r, &g, &b, &a);
      Arcadia_Tests_assertTrue(thread, r == 9 && g == 71 && b == 201 && a == 123);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      Arcadia_Tests_assertTrue(thread, destroyed[0] == 1 && destroyed[1] == 1 && destroyed[2] == 1 && destroyed[3] == 0);
      Arcadia_ValueStack_popValues(thread, 1);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      Arcadia_Tests_assertTrue(thread, destroyed[3] == 1);
    }
  }
  throwOverride = false;
  Arcadia_ValueStack_popValues(thread, 300);
}

int
Arcadia_Media_Tests_PixelBuffer_fills
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testPixels)) { fprintf(stderr, "fill pixel results failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testConstructors)) { fprintf(stderr, "fill constructors failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testLifetime)) { fprintf(stderr, "fill managed lifetime failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testTypedWrappers)) { fprintf(stderr, "fill typed wrappers failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testTypedWrapperLifetime)) { fprintf(stderr, "fill typed wrapper lifetime failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}
