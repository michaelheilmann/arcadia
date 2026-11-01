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

#include "Arcadia.ADL.Tests.Definitions/ClassModels.h"
#include "Arcadia/ADL/Include.h"
#include <stddef.h>
#include <string.h>
#include <math.h>

struct LegacyColorABI { Arcadia_ADL_Definition base; Arcadia_Natural8Value red, green, blue; };
struct ColorABI { Arcadia_ADL_Definition base; Arcadia_Natural8Value red, green, blue, alpha; };
_Static_assert(sizeof(Arcadia_ADL_ColorDefinition) == sizeof(struct LegacyColorABI), "alpha fits legacy color padding");
_Static_assert(_Alignof(Arcadia_ADL_ColorDefinition) == _Alignof(struct LegacyColorABI), "legacy color alignment");
_Static_assert(sizeof(Arcadia_ADL_ColorDefinition) == sizeof(struct ColorABI), "color size");
_Static_assert(_Alignof(Arcadia_ADL_ColorDefinition) == _Alignof(struct ColorABI), "color alignment");
_Static_assert(offsetof(Arcadia_ADL_ColorDefinition, red) == offsetof(struct ColorABI, red), "red offset");
_Static_assert(offsetof(Arcadia_ADL_ColorDefinition, green) == offsetof(struct ColorABI, green), "green offset");
_Static_assert(offsetof(Arcadia_ADL_ColorDefinition, blue) == offsetof(struct ColorABI, blue), "blue offset");
_Static_assert(offsetof(Arcadia_ADL_ColorDefinition, alpha) == offsetof(struct ColorABI, alpha), "alpha follows RGB");
_Static_assert(sizeof(((Arcadia_ADL_ColorDefinition*)0)->alpha) == sizeof(Arcadia_Natural8Value), "alpha width");
_Static_assert(sizeof(Arcadia_ADL_ColorDefinitionDispatch) == sizeof(Arcadia_ADL_DefinitionDispatch), "no additional color slots");
_Static_assert(_Generic(&Arcadia_ADL_ColorDefinition_create,
  Arcadia_ADL_ColorDefinition* (*)(Arcadia_Thread*, Arcadia_ADL_Definitions*, Arcadia_String*, Arcadia_Natural8Value, Arcadia_Natural8Value, Arcadia_Natural8Value): 1, default: 0), "color factory signature");

typedef struct ADLColorModelChild ADLColorModelChild;
static Arcadia_BooleanValue failModelLink;
typedef struct ADLColorModelChildDispatch ADLColorModelChildDispatch;
struct ADLColorModelChild { Arcadia_ADL_ColorDefinition _parent; Arcadia_String* payload; };
struct ADLColorModelChildDispatch { Arcadia_ADL_ColorDefinitionDispatch _parent; };
Arcadia_declareObjectType(u8"Tests.ADLColorModelChild", ADLColorModelChild, u8"Arcadia.ADL.ColorDefinition");

static void
childConstruct
  (
    Arcadia_Thread* thread,
    ADLColorModelChild* self
  )
{
  Arcadia_EnterConstructor(ADLColorModelChild);
  if (_numberOfArguments != 5) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value arguments[5];
  for (Arcadia_SizeValue i = 0; i < 5; ++i) arguments[i] = Arcadia_ValueStack_getValue(thread, 5 - i);
  for (Arcadia_SizeValue i = 0; i < 5; ++i) Arcadia_ValueStack_pushValue(thread, &arguments[i]);
  Arcadia_ValueStack_pushNatural8Value(thread, 5);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->payload = Arcadia_String_createFromCxxString(thread, "native descendant payload");
  Arcadia_LeaveConstructor(ADLColorModelChild);
}

static void
childVisit
  (
    Arcadia_Thread* thread,
    ADLColorModelChild* self
  )
{
  if (self->payload) Arcadia_Object_visit(thread, (Arcadia_Object*)self->payload);
}

static void
childLink
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self
  )
{
  Arcadia_Tests_assertTrue(thread, self->definitions && self->name && ((ADLColorModelChild*)self)->payload);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  if (failModelLink) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed); Arcadia_Thread_jump(thread); }
}

static void
childDispatch
  (
    Arcadia_Thread* thread,
    ADLColorModelChildDispatch* self
  )
{
  ((Arcadia_ADL_DefinitionDispatch*)self)->link = childLink;
}

static const Arcadia_ObjectType_Operations childObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)childConstruct,
  .visit = (Arcadia_Object_VisitCallbackFunction*)childVisit,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)childDispatch,
};
static const Arcadia_Type_Operations childOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &childObjects };
Arcadia_defineObjectType(u8"Tests.ADLColorModelChild", ADLColorModelChild,
                         u8"Arcadia.ADL.ColorDefinition", Arcadia_ADL_ColorDefinition,
                         &childOperations);

#define DEFINE_MODEL_CHILD(Name, Parent, ParentName, Count) \
  typedef struct Name Name; \
  typedef struct Name##Dispatch Name##Dispatch; \
  struct Name { Parent _parent; Arcadia_String* payload; }; \
  struct Name##Dispatch { Parent##Dispatch _parent; }; \
  Arcadia_declareObjectType("Tests." #Name, Name, ParentName); \
  static void Name##_construct(Arcadia_Thread* thread, Name* self) { \
    Arcadia_EnterConstructor(Name); \
    if (_numberOfArguments != Count) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); } \
    Arcadia_Value arguments[Count]; \
    for (Arcadia_SizeValue i = 0; i < Count; ++i) arguments[i] = Arcadia_ValueStack_getValue(thread, Count - i); \
    for (Arcadia_SizeValue i = 0; i < Count; ++i) Arcadia_ValueStack_pushValue(thread, &arguments[i]); \
    Arcadia_ValueStack_pushNatural8Value(thread, Count); \
    Arcadia_superTypeConstructor(thread, _type, self); \
    self->payload = Arcadia_String_createFromCxxString(thread, "native descendant payload"); \
    Arcadia_LeaveConstructor(Name); \
  } \
  static void Name##_visit(Arcadia_Thread* thread, Name* self) { if (self->payload) Arcadia_Object_visit(thread, (Arcadia_Object*)self->payload); } \
  static void Name##_link(Arcadia_Thread* thread, Arcadia_ADL_Definition* self) { \
    Arcadia_Tests_assertTrue(thread, self->definitions && self->name && ((Name*)self)->payload); \
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false)); \
    if (failModelLink) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed); Arcadia_Thread_jump(thread); } \
  } \
  static void Name##_dispatch(Arcadia_Thread* thread, Name##Dispatch* self) { ((Arcadia_ADL_DefinitionDispatch*)self)->link = Name##_link; } \
  static const Arcadia_ObjectType_Operations Name##_objects = { Arcadia_ObjectType_Operations_Initializer, \
    .construct = (Arcadia_Object_ConstructCallbackFunction*)Name##_construct, \
    .visit = (Arcadia_Object_VisitCallbackFunction*)Name##_visit, \
    .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)Name##_dispatch }; \
  static const Arcadia_Type_Operations Name##_operations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &Name##_objects }; \
  Arcadia_defineObjectType("Tests." #Name, Name, ParentName, Parent, &Name##_operations);

DEFINE_MODEL_CHILD(ADLWhiteModelChild, Arcadia_ADL_WhiteNoiseDefinition, "Arcadia.ADL.WhiteNoiseDefinition", 2)
DEFINE_MODEL_CHILD(ADLConstantModelChild, Arcadia_ADL_ConstantDefinition, "Arcadia.ADL.ConstantDefinition", 3)
DEFINE_MODEL_CHILD(ADLSineModelChild, Arcadia_ADL_SineWaveDefinition, "Arcadia.ADL.SineWaveDefinition", 3)
DEFINE_MODEL_CHILD(ADLSawtoothModelChild, Arcadia_ADL_SawtoothWaveDefinition, "Arcadia.ADL.SawtoothWaveDefinition", 3)
#undef DEFINE_MODEL_CHILD

static Arcadia_String*
getChildPayload
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_ADL_Definition* object
  )
{
  if (type == _ADLColorModelChild_getType(thread)) return ((ADLColorModelChild*)object)->payload;
  if (type == _ADLWhiteModelChild_getType(thread)) return ((ADLWhiteModelChild*)object)->payload;
  if (type == _ADLConstantModelChild_getType(thread)) return ((ADLConstantModelChild*)object)->payload;
  if (type == _ADLSineModelChild_getType(thread)) return ((ADLSineModelChild*)object)->payload;
  if (type == _ADLSawtoothModelChild_getType(thread)) return ((ADLSawtoothModelChild*)object)->payload;
  return NULL;
}

static Arcadia_Object*
construct
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    const Arcadia_Value* arguments,
    Arcadia_SizeValue count
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue i = 0; i < count; ++i) Arcadia_ValueStack_pushValue(thread, &arguments[i]);
  Arcadia_ValueStack_pushNatural8Value(thread, (Arcadia_Natural8Value)count);
  return _Arcadia_EndCreate0(thread, type, base);
}

static void
expectError
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    const Arcadia_Value* arguments,
    Arcadia_SizeValue count,
    Arcadia_Status expected
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    construct(thread, type, arguments, count);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Status actual = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
    if (actual != expected) Arcadia_logf(Arcadia_LogFlags_Error, "constructor count %zu: status %u, expected %u\n", count, actual, expected);
    Arcadia_Tests_assertTrue(thread, actual == expected);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
  }
}

static void
checkConstructor
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    const char* expectedName,
    Arcadia_Value* valid,
    Arcadia_SizeValue count
  )
{
  Arcadia_Name* name = Arcadia_Type_getName(thread, type);
  Arcadia_Tests_assertTrue(thread, Arcadia_Type_getKind(thread, type) == Arcadia_TypeKind_Object);
  Arcadia_Tests_assertTrue(thread, Arcadia_Name_getNumberOfBytes(thread, name) == strlen(expectedName));
  Arcadia_Tests_assertTrue(thread, !memcmp(Arcadia_Name_getBytes(thread, name), expectedName, strlen(expectedName)));
  Arcadia_Tests_assertTrue(thread, Arcadia_getType(thread, name) == type);
  Arcadia_Value arguments[255];
  for (Arcadia_SizeValue i = 0; i < 255; ++i) arguments[i] = Arcadia_Value_makeBooleanValue(false);
  for (Arcadia_SizeValue i = 0; i <= 255; ++i) {
    if (i != count) expectError(thread, type, arguments, i, Arcadia_Status_NumberOfArgumentsInvalid);
  }
  Arcadia_Value bad[] = {
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), Arcadia_Value_makeBooleanValue(false),
    Arcadia_Value_makeInteger8Value(0), Arcadia_Value_makeInteger16Value(0),
    Arcadia_Value_makeInteger32Value(0), Arcadia_Value_makeInteger64Value(0),
    Arcadia_Value_makeNatural8Value(0), Arcadia_Value_makeNatural16Value(0),
    Arcadia_Value_makeNatural32Value(0), Arcadia_Value_makeNatural64Value(0),
    Arcadia_Value_makeSizeValue(0), Arcadia_Value_makeReal32Value(0),
    Arcadia_Value_makeReal64Value(0), Arcadia_Value_makeTypeValue(type),
    Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_ADL_MaterialType_getType(thread), 0)),
    valid[0], valid[1],
  };
  for (Arcadia_SizeValue parameter = 0; parameter < count; ++parameter) {
    for (Arcadia_SizeValue i = 0; i < sizeof(bad) / sizeof(bad[0]); ++i) {
      if ((parameter == 0 && i == 15) || (parameter == 1 && i == 16)
        || (parameter >= 2 && valid[parameter].tag == bad[i].tag)) continue;
      for (Arcadia_SizeValue j = 0; j < count; ++j) arguments[j] = valid[j];
      arguments[parameter] = bad[i];
      expectError(thread, type, arguments, count, Arcadia_Status_ArgumentTypeInvalid);
    }
  }
  for (Arcadia_SizeValue i = 0; i < sizeof(bad) / sizeof(bad[0]); ++i) {
    if (i == 6) continue;
    Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_ValueStack_pushValue(thread, &bad[i]);
      _Arcadia_EndCreate0(thread, type, base);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Status actual = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, actual == Arcadia_Status_ArgumentTypeInvalid && Arcadia_ValueStack_getSize(thread) == base);
    }
  }
}

static void
onDestroy
  (
    void* observer,
    Arcadia_Object* object
  )
{
  (*(unsigned*)observer)++;
}

static void
checkScalarState
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* object,
    Arcadia_Value* values
  )
{
  if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)object, _Arcadia_ADL_ColorDefinition_getType(thread))) {
    Arcadia_ADL_ColorDefinition* color = (Arcadia_ADL_ColorDefinition*)object;
    Arcadia_Tests_assertTrue(thread, color->red == Arcadia_Value_getNatural8Value(&values[2])
      && color->green == Arcadia_Value_getNatural8Value(&values[3]) && color->blue == Arcadia_Value_getNatural8Value(&values[4]) && color->alpha == 255);
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)object, _Arcadia_ADL_ConstantDefinition_getType(thread))) {
    Arcadia_Real32Value expected = Arcadia_Value_getReal32Value(&values[2]);
    Arcadia_Tests_assertTrue(thread, !memcmp(&((Arcadia_ADL_ConstantDefinition*)object)->value, &expected, sizeof(expected)));
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)object, _Arcadia_ADL_SineWaveDefinition_getType(thread))) {
    Arcadia_Tests_assertTrue(thread, Arcadia_ADL_SineWaveDefinition_getFrequency(thread, (Arcadia_ADL_SineWaveDefinition*)object) == Arcadia_Value_getInteger32Value(&values[2]));
  } else if (Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)object, _Arcadia_ADL_SawtoothWaveDefinition_getType(thread))) {
    Arcadia_Tests_assertTrue(thread, Arcadia_ADL_SawtoothWaveDefinition_getFrequency(thread, (Arcadia_ADL_SawtoothWaveDefinition*)object) == Arcadia_Value_getInteger32Value(&values[2]));
  }
}

static void
checkLifetime
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_Value* arguments,
    Arcadia_SizeValue count
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  static unsigned destroyed[4];
  destroyed[0] = destroyed[1] = destroyed[2] = destroyed[3] = 0;
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, "Tests.RetainedDefinition");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_Value values[5];
  for (Arcadia_SizeValue i = 0; i < count; ++i) values[i] = arguments[i];
  values[0] = Arcadia_Value_makeObjectReferenceValue(definitions);
  values[1] = Arcadia_Value_makeObjectReferenceValue(name);
  Arcadia_ADL_Definition* object = (Arcadia_ADL_Definition*)construct(thread, type, values, count);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)object);
  Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)definitions, &destroyed[0], onDestroy);
  Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)name, &destroyed[1], onDestroy);
  Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)object, &destroyed[2], onDestroy);
  Arcadia_String* payload = getChildPayload(thread, type, object);
  if (payload) Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)payload, &destroyed[3], onDestroy);
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)object);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, !destroyed[0] && !destroyed[1] && !destroyed[2]);
  if (payload) Arcadia_Tests_assertTrue(thread, !destroyed[3] && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)payload, _Arcadia_String_getType(thread)));
  Arcadia_Tests_assertTrue(thread, Arcadia_ADL_Definitions_getDefinitions(thread, object) == definitions && Arcadia_ADL_Definition_getName(thread, object) == name);
  Arcadia_ADL_Definition_link(thread, object);
  checkScalarState(thread, object, values);
  Arcadia_Tests_assertTrue(thread, object->definitions == definitions && object->name == name);
  if (payload) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    failModelLink = true;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_ADL_Definition_link(thread, object);
      Arcadia_Thread_popJumpTarget(thread);
      failModelLink = false;
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      failModelLink = false;
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, status == Arcadia_Status_OperationFailed && Arcadia_ValueStack_getSize(thread) == depth);
    }
  }
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed[0] == 1 && destroyed[1] == 1 && destroyed[2] == 1);
  if (payload) Arcadia_Tests_assertTrue(thread, destroyed[3] == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
typedLink
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definition* self,
    unsigned kind
  )
{
  switch (kind) {
    case 0: Arcadia_ADL_ColorDefinition_link(thread, (Arcadia_ADL_ColorDefinition*)self); break;
    case 1: Arcadia_ADL_WhiteNoiseDefinition_link(thread, (Arcadia_ADL_WhiteNoiseDefinition*)self); break;
    case 2: Arcadia_ADL_ConstantDefinition_link(thread, (Arcadia_ADL_ConstantDefinition*)self); break;
    case 3: Arcadia_ADL_SineWaveDefinition_link(thread, (Arcadia_ADL_SineWaveDefinition*)self); break;
    default: Arcadia_ADL_SawtoothWaveDefinition_link(thread, (Arcadia_ADL_SawtoothWaveDefinition*)self); break;
  }
}

static Arcadia_ADL_Definition*
typedCreate
  (
    Arcadia_Thread* thread,
    Arcadia_ADL_Definitions* definitions,
    Arcadia_String* name,
    Arcadia_Value* arguments,
    unsigned kind
  )
{
  switch (kind) {
    case 0: return (Arcadia_ADL_Definition*)Arcadia_ADL_ColorDefinition_create(thread, definitions, name,
      Arcadia_Value_getNatural8Value(&arguments[2]), Arcadia_Value_getNatural8Value(&arguments[3]), Arcadia_Value_getNatural8Value(&arguments[4]));
    case 1: return (Arcadia_ADL_Definition*)Arcadia_ADL_WhiteNoiseDefinition_create(thread, definitions, name);
    case 2: return (Arcadia_ADL_Definition*)Arcadia_ADL_ConstantDefinition_create(thread, definitions, name, Arcadia_Value_getReal32Value(&arguments[2]));
    case 3: return (Arcadia_ADL_Definition*)Arcadia_ADL_SineWaveDefinition_create(thread, definitions, name, Arcadia_Value_getInteger32Value(&arguments[2]));
    default: return (Arcadia_ADL_Definition*)Arcadia_ADL_SawtoothWaveDefinition_create(thread, definitions, name, Arcadia_Value_getInteger32Value(&arguments[2]));
  }
}

static void
checkGeneratedEntries
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue childType,
    Arcadia_Value* arguments,
    Arcadia_SizeValue count,
    unsigned kind
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ADL_Definitions* definitions = (Arcadia_ADL_Definitions*)Arcadia_Value_getObjectReferenceValue(&arguments[0]);
  Arcadia_String* name = (Arcadia_String*)Arcadia_Value_getObjectReferenceValue(&arguments[1]);
  for (unsigned bad = 0; bad < 6; ++bad) {
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      if (bad < 2) typedLink(thread, bad ? (Arcadia_ADL_Definition*)name : NULL, kind);
      else typedCreate(thread, bad == 2 ? NULL : bad == 4 ? (Arcadia_ADL_Definitions*)name : definitions,
        bad == 3 ? NULL : bad == 5 ? (Arcadia_String*)definitions : name, arguments, kind);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, status == Arcadia_Status_ArgumentTypeInvalid && Arcadia_ValueStack_getSize(thread) == base);
    }
  }
  static unsigned destroyed[2];
  destroyed[0] = destroyed[1] = 0;
  Arcadia_ADL_Definition* object = (Arcadia_ADL_Definition*)construct(thread, childType, arguments, count);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)object);
  Arcadia_String* payload = getChildPayload(thread, childType, object);
  Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)object, &destroyed[0], onDestroy);
  Arcadia_Object_addNotifyDestroyCallback(thread, (Arcadia_Object*)payload, &destroyed[1], onDestroy);
  Arcadia_ValueStack_popValues(thread, 1);
  // Only the generated typed wrapper now roots this otherwise unrooted receiver.
  typedLink(thread, object, kind);
  Arcadia_Tests_assertTrue(thread, !destroyed[0] && !destroyed[1] && Arcadia_ValueStack_getSize(thread) == base);
  Arcadia_JumpTarget jump;
  failModelLink = true;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    typedLink(thread, object, kind);
    Arcadia_Thread_popJumpTarget(thread);
    failModelLink = false;
    Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    failModelLink = false;
    Arcadia_Status status = Arcadia_Thread_getStatus(thread);
    Arcadia_Thread_setStatus(thread, 0);
    Arcadia_Tests_assertTrue(thread, status == Arcadia_Status_OperationFailed && Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Tests_assertTrue(thread, !destroyed[0] && !destroyed[1]);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed[0] == 1 && destroyed[1] == 1);
}

static void
colorTest
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, i);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, "");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_Value arguments[] = { Arcadia_Value_makeObjectReferenceValue(definitions), Arcadia_Value_makeObjectReferenceValue(name),
    Arcadia_Value_makeNatural8Value(0), Arcadia_Value_makeNatural8Value(73), Arcadia_Value_makeNatural8Value(255) };
  Arcadia_TypeValue type = _Arcadia_ADL_ColorDefinition_getType(thread);
  checkConstructor(thread, type, "Arcadia.ADL.ColorDefinition", arguments, 5);
  Arcadia_ADL_ColorDefinition* color = Arcadia_ADL_ColorDefinition_create(thread, definitions, name, 0, 73, 255);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)color);
  Arcadia_Tests_assertTrue(thread, color->red == 0 && color->green == 73 && color->blue == 255 && color->alpha == 255);
  Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)color);
  Arcadia_Tests_assertTrue(thread, color->red == 0 && color->green == 73 && color->blue == 255 && color->alpha == 255);
  Arcadia_ValueStack_popValues(thread, 1);
  checkLifetime(thread, type, arguments, 5);
  checkLifetime(thread, _ADLColorModelChild_getType(thread), arguments, 5);
  checkGeneratedEntries(thread, _ADLColorModelChild_getType(thread), arguments, 5, 0);
  Arcadia_ValueStack_popValues(thread, 2);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural64Value(thread, 299 - i) == i);
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_ADL_Tests_colorModel
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(colorTest) ? EXIT_SUCCESS : EXIT_FAILURE;
}

struct ConstantABI { Arcadia_ADL_Definition base; Arcadia_Real32Value value; };
struct FrequencyABI { Arcadia_ADL_Definition base; Arcadia_Integer32Value frequency; };
_Static_assert(sizeof(Arcadia_ADL_WhiteNoiseDefinition) == sizeof(Arcadia_ADL_Definition), "white noise layout");
_Static_assert(_Alignof(Arcadia_ADL_WhiteNoiseDefinition) == _Alignof(Arcadia_ADL_Definition), "white noise alignment");
_Static_assert(sizeof(Arcadia_ADL_WhiteNoiseDefinitionDispatch) == sizeof(Arcadia_ADL_DefinitionDispatch), "white noise dispatch");
_Static_assert(sizeof(Arcadia_ADL_ConstantDefinition) == sizeof(struct ConstantABI), "constant size");
_Static_assert(_Alignof(Arcadia_ADL_ConstantDefinition) == _Alignof(struct ConstantABI), "constant alignment");
_Static_assert(sizeof(Arcadia_ADL_ConstantDefinitionDispatch) == sizeof(Arcadia_ADL_DefinitionDispatch), "constant dispatch");
_Static_assert(offsetof(Arcadia_ADL_ConstantDefinition, value) == offsetof(struct ConstantABI, value), "constant field");
_Static_assert(sizeof(Arcadia_ADL_SineWaveDefinition) == sizeof(struct FrequencyABI), "sine size");
_Static_assert(sizeof(Arcadia_ADL_SawtoothWaveDefinition) == sizeof(struct FrequencyABI), "sawtooth size");
_Static_assert(_Alignof(Arcadia_ADL_SineWaveDefinition) == _Alignof(struct FrequencyABI), "sine alignment");
_Static_assert(_Alignof(Arcadia_ADL_SawtoothWaveDefinition) == _Alignof(struct FrequencyABI), "sawtooth alignment");
_Static_assert(sizeof(Arcadia_ADL_SineWaveDefinitionDispatch) == sizeof(Arcadia_ADL_DefinitionDispatch), "sine dispatch");
_Static_assert(sizeof(Arcadia_ADL_SawtoothWaveDefinitionDispatch) == sizeof(Arcadia_ADL_DefinitionDispatch), "sawtooth dispatch");
_Static_assert(offsetof(Arcadia_ADL_SineWaveDefinition, frequency) == offsetof(struct FrequencyABI, frequency), "sine field");
_Static_assert(offsetof(Arcadia_ADL_SawtoothWaveDefinition, frequency) == offsetof(struct FrequencyABI, frequency), "sawtooth field");
_Static_assert(_Generic(&Arcadia_ADL_WhiteNoiseDefinition_create,
  Arcadia_ADL_WhiteNoiseDefinition* (*)(Arcadia_Thread*, Arcadia_ADL_Definitions*, Arcadia_String*): 1, default: 0), "white noise factory");
_Static_assert(_Generic(&Arcadia_ADL_ConstantDefinition_create,
  Arcadia_ADL_ConstantDefinition* (*)(Arcadia_Thread*, Arcadia_ADL_Definitions*, Arcadia_String*, Arcadia_Real32Value): 1, default: 0), "constant factory");
_Static_assert(_Generic(&Arcadia_ADL_SineWaveDefinition_create,
  Arcadia_ADL_SineWaveDefinition* (*)(Arcadia_Thread*, Arcadia_ADL_Definitions*, Arcadia_String*, Arcadia_Integer32Value): 1, default: 0), "sine factory");
_Static_assert(_Generic(&Arcadia_ADL_SawtoothWaveDefinition_create,
  Arcadia_ADL_SawtoothWaveDefinition* (*)(Arcadia_Thread*, Arcadia_ADL_Definitions*, Arcadia_String*, Arcadia_Integer32Value): 1, default: 0), "sawtooth factory");
_Static_assert(_Generic(&Arcadia_ADL_SineWaveDefinition_getFrequency,
  Arcadia_Integer32Value (*)(Arcadia_Thread*, Arcadia_ADL_SineWaveDefinition*): 1, default: 0), "sine getter");
_Static_assert(_Generic(&Arcadia_ADL_SawtoothWaveDefinition_getFrequency,
  Arcadia_Integer32Value (*)(Arcadia_Thread*, Arcadia_ADL_SawtoothWaveDefinition*): 1, default: 0), "sawtooth getter");

static void
dspDefinitionTest
  (
    Arcadia_Thread* thread,
    unsigned kind
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, i);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)definitions);
  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, "");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)name);
  Arcadia_Value arguments[] = { Arcadia_Value_makeObjectReferenceValue(definitions), Arcadia_Value_makeObjectReferenceValue(name),
    kind == 1 ? Arcadia_Value_makeReal32Value(-0.f) : Arcadia_Value_makeInteger32Value(-1) };
  Arcadia_TypeValue type;
  const char* expected;
  switch (kind) {
    case 0: type = _Arcadia_ADL_WhiteNoiseDefinition_getType(thread); expected = "Arcadia.ADL.WhiteNoiseDefinition"; break;
    case 1: type = _Arcadia_ADL_ConstantDefinition_getType(thread); expected = "Arcadia.ADL.ConstantDefinition"; break;
    case 2: type = _Arcadia_ADL_SineWaveDefinition_getType(thread); expected = "Arcadia.ADL.SineWaveDefinition"; break;
    default: type = _Arcadia_ADL_SawtoothWaveDefinition_getType(thread); expected = "Arcadia.ADL.SawtoothWaveDefinition"; break;
  }
  checkConstructor(thread, type, expected, arguments, kind ? 3 : 2);
  if (kind == 0) {
    Arcadia_ADL_WhiteNoiseDefinition* object = Arcadia_ADL_WhiteNoiseDefinition_create(thread, definitions, name);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)object);
    Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)object);
    Arcadia_ValueStack_popValues(thread, 1);
  } else if (kind == 1) {
    const Arcadia_Real32Value values[] = { -0.f, 0.f, -.25f, 1.f, INFINITY, -INFINITY, NAN };
    for (Arcadia_SizeValue i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
      Arcadia_ADL_ConstantDefinition* object = Arcadia_ADL_ConstantDefinition_create(thread, definitions, name, values[i]);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)object);
      Arcadia_ADL_Definition_link(thread, (Arcadia_ADL_Definition*)object);
      Arcadia_Tests_assertTrue(thread, isnan(values[i]) ? isnan(object->value) : !memcmp(&object->value, &values[i], sizeof(values[i])));
      Arcadia_ValueStack_popValues(thread, 1);
    }
  } else {
    const Arcadia_Integer32Value values[] = { INT32_MIN, -1, 0, 1, INT32_MAX };
    for (Arcadia_SizeValue i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
      Arcadia_ADL_Definition* object = kind == 2
        ? (Arcadia_ADL_Definition*)Arcadia_ADL_SineWaveDefinition_create(thread, definitions, name, values[i])
        : (Arcadia_ADL_Definition*)Arcadia_ADL_SawtoothWaveDefinition_create(thread, definitions, name, values[i]);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)object);
      Arcadia_ADL_Definition_link(thread, object);
      Arcadia_Integer32Value actual = kind == 2 ? Arcadia_ADL_SineWaveDefinition_getFrequency(thread, (Arcadia_ADL_SineWaveDefinition*)object)
        : Arcadia_ADL_SawtoothWaveDefinition_getFrequency(thread, (Arcadia_ADL_SawtoothWaveDefinition*)object);
      Arcadia_Tests_assertTrue(thread, actual == values[i]);
      Arcadia_ValueStack_popValues(thread, 1);
    }
  }
  checkLifetime(thread, type, arguments, kind ? 3 : 2);
  Arcadia_TypeValue (*const childTypes[])(Arcadia_Thread*) = { _ADLWhiteModelChild_getType, _ADLConstantModelChild_getType, _ADLSineModelChild_getType, _ADLSawtoothModelChild_getType };
  checkLifetime(thread, childTypes[kind](thread), arguments, kind ? 3 : 2);
  checkGeneratedEntries(thread, childTypes[kind](thread), arguments, kind ? 3 : 2, kind + 1);
  Arcadia_ValueStack_popValues(thread, 2);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getNatural64Value(thread, 299 - i) == i);
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
whiteNoiseTest
  (
    Arcadia_Thread* thread
  )
{ dspDefinitionTest(thread, 0); }

static void
constantTest
  (
    Arcadia_Thread* thread
  )
{ dspDefinitionTest(thread, 1); }

static void
sineWaveTest
  (
    Arcadia_Thread* thread
  )
{ dspDefinitionTest(thread, 2); }

static void
sawtoothWaveTest
  (
    Arcadia_Thread* thread
  )
{ dspDefinitionTest(thread, 3); }

int
Arcadia_ADL_Tests_whiteNoiseModel
  (
    void
  )
{ return Arcadia_Tests_safeExecute(whiteNoiseTest) ? EXIT_SUCCESS : EXIT_FAILURE; }

int
Arcadia_ADL_Tests_constantModel
  (
    void
  )
{ return Arcadia_Tests_safeExecute(constantTest) ? EXIT_SUCCESS : EXIT_FAILURE; }

int
Arcadia_ADL_Tests_sineWaveModel
  (
    void
  )
{ return Arcadia_Tests_safeExecute(sineWaveTest) ? EXIT_SUCCESS : EXIT_FAILURE; }

int
Arcadia_ADL_Tests_sawtoothWaveModel
  (
    void
  )
{ return Arcadia_Tests_safeExecute(sawtoothWaveTest) ? EXIT_SUCCESS : EXIT_FAILURE; }
