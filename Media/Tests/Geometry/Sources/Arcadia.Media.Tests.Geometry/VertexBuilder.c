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

#include "VertexBuilder.h"
#include "Arcadia/Media/Include.h"
#include "Arcadia/Collections/Include.h"
#include <stddef.h>
#include <stdio.h>

struct VertexBuilderABI {
  Arcadia_Object _parent;
  Arcadia_Natural8Value flags;
  Arcadia_SizeValue stride;
  Arcadia_List* vertexElementDescriptors;
};

struct VertexDescriptorABI {
  Arcadia_Object parent;
  Arcadia_SizeValue stride;
  Arcadia_ImmutableList* vertexElementDescriptors;
  Arcadia_Natural32Value vertexSize;
};

_Static_assert(sizeof(Arcadia_Media_VertexDescriptor) == sizeof(struct VertexDescriptorABI), "descriptor size");
_Static_assert(_Alignof(Arcadia_Media_VertexDescriptor) == _Alignof(struct VertexDescriptorABI), "descriptor alignment");
_Static_assert(sizeof(Arcadia_Media_VertexDescriptorDispatch) == sizeof(Arcadia_ObjectDispatch), "descriptor has no own slots");
#define DESCRIPTOR_FIELD(Name, Type) \
  _Static_assert(offsetof(Arcadia_Media_VertexDescriptor, Name) == offsetof(struct VertexDescriptorABI, Name), #Name " offset"); \
  _Static_assert(_Generic(((Arcadia_Media_VertexDescriptor*)0)->Name, Type: 1, default: 0), #Name " type");
DESCRIPTOR_FIELD(stride, Arcadia_SizeValue)
DESCRIPTOR_FIELD(vertexElementDescriptors, Arcadia_ImmutableList*)
DESCRIPTOR_FIELD(vertexSize, Arcadia_Natural32Value)
#undef DESCRIPTOR_FIELD
_Static_assert(_Generic(&Arcadia_Media_VertexDescriptor_getVertexSize, Arcadia_Natural32Value (*)(Arcadia_Thread*, Arcadia_Media_VertexDescriptor*): 1, default: 0), "vertex size signature");
_Static_assert(_Generic(&Arcadia_Media_VertexDescriptor_getStride, Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_VertexDescriptor*): 1, default: 0), "stride signature");
_Static_assert(_Generic(&Arcadia_Media_VertexDescriptor_getVertexElementDescriptors, Arcadia_ImmutableList* (*)(Arcadia_Thread*, Arcadia_Media_VertexDescriptor*): 1, default: 0), "elements signature");

_Static_assert(sizeof(Arcadia_Media_VertexDescriptorBuilder) == sizeof(struct VertexBuilderABI), "builder size");
_Static_assert(_Alignof(Arcadia_Media_VertexDescriptorBuilder) == _Alignof(struct VertexBuilderABI), "builder alignment");
_Static_assert(sizeof(Arcadia_Media_VertexDescriptorBuilderDispatch) == sizeof(Arcadia_ObjectDispatch), "no own slots");
#define FIELD(Name, Type) \
  _Static_assert(offsetof(Arcadia_Media_VertexDescriptorBuilder, Name) == offsetof(struct VertexBuilderABI, Name), #Name " offset"); \
  _Static_assert(_Generic(((Arcadia_Media_VertexDescriptorBuilder*)0)->Name, Type: 1, default: 0), #Name " type");
FIELD(flags, Arcadia_Natural8Value)
FIELD(stride, Arcadia_SizeValue)
FIELD(vertexElementDescriptors, Arcadia_List*)
#undef FIELD
_Static_assert(_Generic(&Arcadia_Media_VertexDescriptorBuilder_create, Arcadia_Media_VertexDescriptorBuilder* (*)(Arcadia_Thread*): 1, default: 0), "factory signature");
_Static_assert(_Generic(&Arcadia_Media_VertexDescriptorBuilder_build, Arcadia_Media_VertexDescriptor* (*)(Arcadia_Thread*, Arcadia_Media_VertexDescriptorBuilder*): 1, default: 0), "build signature");

typedef struct VertexBuilderListProbe VertexBuilderListProbe;
typedef struct VertexBuilderListProbeDispatch VertexBuilderListProbeDispatch;

struct VertexBuilderListProbe { Arcadia_ArrayList _parent; };

struct VertexBuilderListProbeDispatch { Arcadia_ArrayListDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.VertexBuilderListProbe", VertexBuilderListProbe,
                          u8"Arcadia.ArrayList");

static unsigned countCalls, failOnCount;
static Arcadia_SizeValue destroyed[6];

typedef struct NativeVertexBuilderProbe NativeVertexBuilderProbe;
typedef struct NativeVertexBuilderProbeDispatch NativeVertexBuilderProbeDispatch;

struct NativeVertexBuilderProbe {
  Arcadia_Media_VertexDescriptorBuilder _parent;
  Arcadia_String* payload;
};

struct NativeVertexBuilderProbeDispatch { Arcadia_Media_VertexDescriptorBuilderDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.NativeVertexBuilderProbe", NativeVertexBuilderProbe,
                          u8"Arcadia.Media.VertexDescriptorBuilder");

static void
constructBuilderProbe
  (
    Arcadia_Thread* thread,
    NativeVertexBuilderProbe* self
  )
{
  Arcadia_EnterConstructor(NativeVertexBuilderProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->payload = Arcadia_String_createFromCxxString(thread, "builder payload");
  Arcadia_LeaveConstructor(NativeVertexBuilderProbe);
}

static void
visitBuilderProbe
  (
    Arcadia_Thread* thread,
    NativeVertexBuilderProbe* self
  )
{ if (self->payload) Arcadia_Object_visit(thread, (void*)self->payload); }

static const Arcadia_ObjectType_Operations builderProbeObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructBuilderProbe,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitBuilderProbe,
};
static const Arcadia_Type_Operations builderProbeOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &builderProbeObjects };

Arcadia_defineObjectType(u8"Tests.NativeVertexBuilderProbe", NativeVertexBuilderProbe,
                         u8"Arcadia.Media.VertexDescriptorBuilder", Arcadia_Media_VertexDescriptorBuilder,
                         &builderProbeOperations);

static void
constructList
  (
    Arcadia_Thread* thread,
    VertexBuilderListProbe* self
  )
{
  Arcadia_EnterConstructor(VertexBuilderListProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_LeaveConstructor(VertexBuilderListProbe);
}

static Arcadia_SizeValue
countList
  (
    Arcadia_Thread* thread,
    Arcadia_Collection* self
  )
{
  ++countCalls;
  if (countCalls == 1) Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  if (countCalls == failOnCount) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_CollectionDispatch* parent = (void*)Arcadia_ObjectType_getDispatch(_Arcadia_ArrayList_getType(thread));
  return parent->getSize(thread, self);
}

static void
initializeList
  (
    Arcadia_Thread* thread,
    VertexBuilderListProbeDispatch* self
  )
{ ((Arcadia_CollectionDispatch*)self)->getSize = &countList; }

static const Arcadia_ObjectType_Operations listObjects = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructList,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeList,
};
static const Arcadia_Type_Operations listOperations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &listObjects };

Arcadia_defineObjectType(u8"Tests.VertexBuilderListProbe", VertexBuilderListProbe,
                         u8"Arcadia.ArrayList", Arcadia_ArrayList,
                         &listOperations);

static Arcadia_Media_VertexElementDescriptor*
createElement
  (
    Arcadia_Thread* thread,
    Arcadia_Natural32Value offset,
    Arcadia_Integer32Value syntactics
  )
{
  // The legacy C factory narrows offsets to Natural8; its runtime constructor
  // accepts the full Natural32 range. Preserve and test that existing boundary.
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural32Value(thread, offset);
  Arcadia_ValueStack_pushInteger32Value(thread, 0);
  Arcadia_ValueStack_pushInteger32Value(thread, syntactics);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  return (void*)_Arcadia_EndCreate0(thread, _Arcadia_Media_VertexElementDescriptor_getType(thread), base);
}

static void
appendElement
  (
    Arcadia_Thread* thread,
    Arcadia_List* list,
    Arcadia_Media_VertexElementDescriptor* element
  )
{
  Arcadia_ValueStack_pushObjectReferenceValue(thread, element);
  Arcadia_List_insertBackObjectReferenceValue(thread, list, (void*)element);
  Arcadia_ValueStack_popValues(thread, 1);
}

static void
onDestroy
  (
    void* observer,
    Arcadia_Object* object
  )
{ (*(Arcadia_SizeValue*)observer)++; }

static void
testBuild
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Media_VertexDescriptorBuilder* self = Arcadia_Media_VertexDescriptorBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Media_VertexDescriptorBuilder* other = Arcadia_Media_VertexDescriptorBuilder_create(thread);
  Arcadia_Tests_assertTrue(thread, self->stride == 0 && self->vertexElementDescriptors != other->vertexElementDescriptors);
  Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (void*)self->vertexElementDescriptors) == 0);
  Arcadia_Media_VertexDescriptor* empty = Arcadia_Media_VertexDescriptorBuilder_build(thread, self);
  Arcadia_Tests_assertTrue(thread, empty->stride == 0 && empty->vertexSize == 0 && Arcadia_Collection_getSize(thread, (void*)empty->vertexElementDescriptors) == 0);
  self->flags = 0xa5; self->stride = 1234;
  const Arcadia_Integer32Value syntax[] = {
    Arcadia_Media_VertexElementSyntactics_Real32, Arcadia_Media_VertexElementSyntactics_Real32Real32,
    Arcadia_Media_VertexElementSyntactics_Real32Real32Real32, Arcadia_Media_VertexElementSyntactics_Real32Real32Real32Real32,
  };
  for (unsigned width = 1; width <= 4; ++width) {
    const Arcadia_Natural32Value offsets[] = { 0, 255, UINT32_MAX };
    for (unsigned j = 0; j < sizeof(offsets) / sizeof(offsets[0]); ++j) {
      Arcadia_Collection_clear(thread, (void*)self->vertexElementDescriptors);
      Arcadia_Media_VertexElementDescriptor* e = createElement(thread, offsets[j], syntax[width - 1]);
      e->source = 77;
      appendElement(thread, self->vertexElementDescriptors, e);
      Arcadia_Media_VertexDescriptor* result = Arcadia_Media_VertexDescriptorBuilder_build(thread, self);
      Arcadia_SizeValue expected = (Arcadia_SizeValue)offsets[j]; expected += width * sizeof(Arcadia_Real32Value);
      Arcadia_Tests_assertTrue(thread, result->stride == expected && result->vertexSize == (Arcadia_Natural32Value)expected);
      Arcadia_Tests_assertTrue(thread, self->flags == 0xa5 && self->stride == 1234 && e->source == 77);
    }
  }
  Arcadia_Collection_clear(thread, (void*)self->vertexElementDescriptors);
  Arcadia_Media_VertexElementDescriptor* a = createElement(thread, 40, syntax[0]);
  appendElement(thread, self->vertexElementDescriptors, a);
  Arcadia_Media_VertexElementDescriptor* b = createElement(thread, 0, syntax[3]);
  appendElement(thread, self->vertexElementDescriptors, b);
  Arcadia_Media_VertexElementDescriptor* c = createElement(thread, 39, syntax[1]);
  appendElement(thread, self->vertexElementDescriptors, c);
  appendElement(thread, self->vertexElementDescriptors, b);
  Arcadia_Media_VertexDescriptor* result = Arcadia_Media_VertexDescriptorBuilder_build(thread, self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, result);
  Arcadia_Tests_assertTrue(thread, result->stride == 47 && result->vertexSize == 47);
  Arcadia_Object* expected[] = { (void*)a, (void*)b, (void*)c, (void*)b };
  for (unsigned i = 0; i < 4; ++i) Arcadia_Tests_assertTrue(thread, Arcadia_List_getObjectReferenceValueAt(thread, (void*)result->vertexElementDescriptors, i) == expected[i]);
  c->offset = 100;
  Arcadia_Collection_clear(thread, (void*)self->vertexElementDescriptors);
  Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (void*)result->vertexElementDescriptors) == 4 && result->stride == 47);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Media_VertexElementDescriptor*)Arcadia_List_getObjectReferenceValueAt(thread, (void*)result->vertexElementDescriptors, 2))->offset == 100);
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testErrors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_Media_VertexDescriptorBuilder* self = Arcadia_Media_VertexDescriptorBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  self->vertexElementDescriptors = (void*)_Arcadia_EndCreate0(thread, _VertexBuilderListProbe_getType(thread), argumentsBase);
  Arcadia_Media_VertexElementDescriptor* element = createElement(thread, 3, Arcadia_Media_VertexElementSyntactics_Real32);
  appendElement(thread, self->vertexElementDescriptors, element);
  for (unsigned stage = 0; stage < 3; ++stage) {
    countCalls = 0; failOnCount = stage;
    element->syntactics = stage == 0 ? (Arcadia_Media_VertexElementSyntactics)-1 : Arcadia_Media_VertexElementSyntactics_Real32;
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Media_VertexDescriptorBuilder_build(thread, self);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (stage == 0 ? Arcadia_Status_ArgumentValueInvalid : Arcadia_Status_OperationFailed));
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 301);
    }
  }
  failOnCount = 0; countCalls = 0;
  Arcadia_Media_VertexDescriptor* recovered = Arcadia_Media_VertexDescriptorBuilder_build(thread, self);
  Arcadia_Tests_assertTrue(thread, recovered->stride == 7);
  Arcadia_ValueStack_popValues(thread, 1);
  for (unsigned count = 1; count <= 255; ++count) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      for (unsigned i = 0; i < count; ++i) Arcadia_ValueStack_pushBooleanValue(thread, false);
      Arcadia_ValueStack_pushNatural8Value(thread, (Arcadia_Natural8Value)count);
      _Arcadia_EndCreate0(thread, _Arcadia_Media_VertexDescriptorBuilder_getType(thread), depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
    }
  }
  Arcadia_ValueStack_popValues(thread, 300);
}

static void
testLifetime
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  NativeVertexBuilderProbe* probe = (void*)_Arcadia_EndCreate0(thread, _NativeVertexBuilderProbe_getType(thread), base);
  Arcadia_Media_VertexDescriptorBuilder* self = (void*)probe;
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Media_VertexElementDescriptor* e = createElement(thread, 7, Arcadia_Media_VertexElementSyntactics_Real32);
  appendElement(thread, self->vertexElementDescriptors, e);
  Arcadia_Media_VertexDescriptor* result = Arcadia_Media_VertexDescriptorBuilder_build(thread, self);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, result);
  Arcadia_Object* objects[] = { (void*)self, (void*)self->vertexElementDescriptors, (void*)e, (void*)result, (void*)result->vertexElementDescriptors, (void*)probe->payload };
  for (unsigned i = 0; i < 6; ++i) {
    destroyed[i] = 0;
    Arcadia_Object_addNotifyDestroyCallback(thread, objects[i], &destroyed[i], &onDestroy);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (unsigned i = 0; i < 6; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 0);
  Arcadia_Value value = Arcadia_Value_makeObjectReferenceValue(result);
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_ValueStack_pushValue(thread, &value);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed[0] == 1 && destroyed[1] == 1 && destroyed[2] == 0 && destroyed[3] == 0 && destroyed[4] == 0 && destroyed[5] == 1);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (unsigned i = 0; i < 6; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testGeneratedSafety
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 77);
  Arcadia_Media_VertexDescriptorBuilder* self = Arcadia_Media_VertexDescriptorBuilder_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_String* wrong = Arcadia_String_createFromCxxString(thread, "not a list or descriptor");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, wrong);
  Arcadia_List* original = self->vertexElementDescriptors;
  Arcadia_ValueStack_pushObjectReferenceValue(thread, original);
  for (unsigned error = 0; error < 5; ++error) {
    self->vertexElementDescriptors = error == 2 ? NULL : error == 3 ? (void*)wrong : original;
    if (error == 4) Arcadia_List_insertBackObjectReferenceValue(thread, original, (void*)wrong);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Media_VertexDescriptorBuilder_build(thread, error == 0 ? NULL : error == 1 ? (void*)wrong : self);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_setStatus(thread, 0);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 303);
    }
  }
  self->vertexElementDescriptors = original;
  Arcadia_ValueStack_popValues(thread, 3);
  for (unsigned failure = 0; failure < 3; ++failure) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    NativeVertexBuilderProbe* probe = (void*)_Arcadia_EndCreate0(thread, _NativeVertexBuilderProbe_getType(thread), depth);
    self = (void*)probe;
    Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
    Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    self->vertexElementDescriptors = (void*)_Arcadia_EndCreate0(thread, _VertexBuilderListProbe_getType(thread), argumentsBase);
    Arcadia_Media_VertexElementDescriptor* element = createElement(thread, 9, Arcadia_Media_VertexElementSyntactics_Real32);
    appendElement(thread, self->vertexElementDescriptors, element);
    Arcadia_Object* objects[] = { (void*)self, (void*)self->vertexElementDescriptors, (void*)element, (void*)probe->payload };
    for (unsigned i = 0; i < 4; ++i) {
      destroyed[i] = 0;
      Arcadia_Object_addNotifyDestroyCallback(thread, objects[i], &destroyed[i], &onDestroy);
    }
    countCalls = 0; failOnCount = failure;
    Arcadia_ValueStack_popValues(thread, 1);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Media_VertexDescriptor* result = Arcadia_Media_VertexDescriptorBuilder_build(thread, self);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, failure == 0 && result->stride == 13);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, result);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, failure != 0 && Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
      Arcadia_Thread_setStatus(thread, 0);
    }
    for (unsigned i = 0; i < 4; ++i) Arcadia_Tests_assertTrue(thread, destroyed[i] == 0);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth + (failure == 0 ? 1 : 0));
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, destroyed[0] == 1 && destroyed[1] == 1 && destroyed[3] == 1 && destroyed[2] == (failure == 0 ? 0 : 1));
    if (failure == 0) Arcadia_ValueStack_popValues(thread, 1);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, destroyed[2] == 1);
  }
  failOnCount = 0;
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testDescriptorDeclaration
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_List* legacy = (void*)Arcadia_ArrayList_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, legacy);
  Arcadia_Media_VertexElementDescriptor* element = createElement(thread, UINT32_MAX, Arcadia_Media_VertexElementSyntactics_Real32);
  appendElement(thread, legacy, element);
  // The public field/getter remains ImmutableList*, but the existing native
  // runtime constructor accepts List implementations. Declaration generation
  // must not replace that executable contract with a stricter constructor.
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushSizeValue(thread, SIZE_MAX);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, legacy);
  Arcadia_ValueStack_pushNatural8Value(thread, 2);
  Arcadia_Media_VertexDescriptor* descriptor = (void*)_Arcadia_EndCreate0(thread, _Arcadia_Media_VertexDescriptor_getType(thread), depth);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, descriptor);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_VertexDescriptor_getStride(thread, descriptor) == SIZE_MAX);
  Arcadia_Tests_assertTrue(thread, Arcadia_Media_VertexDescriptor_getVertexSize(thread, descriptor) == 3);
  Arcadia_Tests_assertTrue(thread, (void*)Arcadia_Media_VertexDescriptor_getVertexElementDescriptors(thread, descriptor) == (void*)legacy);
  destroyed[0] = destroyed[1] = destroyed[2] = 0;
  Arcadia_Object_addNotifyDestroyCallback(thread, (void*)descriptor, &destroyed[0], &onDestroy);
  Arcadia_Object_addNotifyDestroyCallback(thread, (void*)legacy, &destroyed[1], &onDestroy);
  Arcadia_Object_addNotifyDestroyCallback(thread, (void*)element, &destroyed[2], &onDestroy);
  Arcadia_Value value = Arcadia_Value_makeObjectReferenceValue(descriptor);
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_ValueStack_pushValue(thread, &value);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, !destroyed[0] && !destroyed[1] && !destroyed[2]);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed[0] == 1 && destroyed[1] == 1 && destroyed[2] == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Media_Tests_Geometry_vertexBuilder
  (
    void
  )
{
  if (!Arcadia_Tests_safeExecute(testBuild)) { fprintf(stderr, "vertex builder values failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testErrors)) { fprintf(stderr, "vertex builder errors failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testLifetime)) { fprintf(stderr, "vertex builder lifetime failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testGeneratedSafety)) { fprintf(stderr, "vertex builder generated safety failed\n"); return EXIT_FAILURE; }
  if (!Arcadia_Tests_safeExecute(testDescriptorDeclaration)) { fprintf(stderr, "vertex descriptor declaration failed\n"); return EXIT_FAILURE; }
  return EXIT_SUCCESS;
}
