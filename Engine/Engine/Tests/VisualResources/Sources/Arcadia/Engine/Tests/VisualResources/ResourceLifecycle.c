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

#include "Arcadia/Engine/Tests/VisualResources/Cases.h"
#include "Arcadia/Engine/Include.h"
#include "Arcadia/Math/Include.h"

typedef struct ResourceLifecycleContext ResourceLifecycleContext;
typedef struct ResourceLifecycleContextDispatch ResourceLifecycleContextDispatch;

struct ResourceLifecycleContext {
  Arcadia_Engine_Visuals_BackendContext _parent;
  Arcadia_Engine_Visuals_Resource* resources[4];
  Arcadia_BooleanValue failCreation;
  Arcadia_BooleanValue observeConstruction;
  Arcadia_BooleanValue guardReceiver;
  // Borrowed capture: never visited by this fixture.
  Arcadia_Engine_Visuals_ModelResource* constructingModel;
  Arcadia_WeakReference* constructionObjects[6];
};

struct ResourceLifecycleContextDispatch {
  Arcadia_Engine_Visuals_BackendContextDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.ResourceLifecycleContext", ResourceLifecycleContext,
                          u8"Arcadia.Engine.Visuals.BackendContext");

static Arcadia_Object*
create
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_Value* arguments,
    Arcadia_Natural8Value count
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_Natural8Value i = 0; i < count; ++i) Arcadia_ValueStack_pushValue(thread, &arguments[i]);
  Arcadia_ValueStack_pushNatural8Value(thread, count);
  return _Arcadia_EndCreate0(thread, type, base);
}

static void
constructContext
  (
    Arcadia_Thread* thread,
    ResourceLifecycleContext* self
  )
{
  Arcadia_EnterConstructor(ResourceLifecycleContext);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  for (size_t i = 0; i < 4; ++i) self->resources[i] = NULL;
  self->failCreation = false;
  self->observeConstruction = false;
  self->guardReceiver = false;
  self->constructingModel = NULL;
  for (size_t i = 0; i < 6; ++i) self->constructionObjects[i] = NULL;
  Arcadia_LeaveConstructor(ResourceLifecycleContext);
}

static void
visitContext
  (
    Arcadia_Thread* thread,
    ResourceLifecycleContext* self
  )
{
  for (size_t i = 0; i < 4; ++i) {
    if (self->resources[i]) Arcadia_Object_visit(thread, (Arcadia_Object*)self->resources[i]);
  }
  for (size_t i = 0; i < 6; ++i) {
    if (self->constructionObjects[i]) Arcadia_Object_visit(thread, (Arcadia_Object*)self->constructionObjects[i]);
  }
}

static Arcadia_Engine_Visuals_ConstantBufferResource*
createBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* receiver
  )
{
  ResourceLifecycleContext* self = (void*)receiver;
  if (self->constructingModel) {
    Arcadia_Engine_Visuals_ModelResource* model = self->constructingModel;
    Arcadia_Object* objects[] = { (Arcadia_Object*)model, (Arcadia_Object*)model->localToWorldMatrix,
      (Arcadia_Object*)self->resources[0], (Arcadia_Object*)self->resources[1],
      (Arcadia_Object*)self->resources[2], (Arcadia_Object*)self->resources[3] };
    // Only weak observers are retained. In particular neither this capture nor
    // the local objects array is traced. Dependencies follow the real registry.
    for (size_t i = 0; i < 6; ++i) {
      self->constructionObjects[i] = Arcadia_WeakReference_create(thread, Arcadia_Value_makeObjectReferenceValue(objects[i]));
    }
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    for (size_t i = 0; i < 6; ++i) {
      Arcadia_Value retained = Arcadia_WeakReference_getValue(thread, self->constructionObjects[i]);
      Arcadia_Tests_assertTrue(thread, Arcadia_Value_isObjectReferenceValue(&retained));
      Arcadia_Tests_assertTrue(thread, Arcadia_Value_getObjectReferenceValue(&retained) == objects[i]);
    }
  }
  if (self->failCreation) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  return (void*)self->resources[0];
}

static void
initializeContext
  (
    Arcadia_Thread* thread,
    ResourceLifecycleContextDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_BackendContextDispatch*)self)->createConstantBufferResource = &createBuffer;
}

static const Arcadia_ObjectType_Operations contextObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructContext,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitContext,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeContext,
};

static const Arcadia_Type_Operations contextOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &contextObjectOperations,
};

Arcadia_defineObjectType(u8"Tests.ResourceLifecycleContext", ResourceLifecycleContext,
                         u8"Arcadia.Engine.Visuals.BackendContext", Arcadia_Engine_Visuals_BackendContext,
                         &contextOperations);

typedef struct ModelConstructionProbe ModelConstructionProbe;
typedef struct ModelConstructionProbeDispatch ModelConstructionProbeDispatch;

struct ModelConstructionProbe { Arcadia_Engine_Visuals_ModelResource _parent; };

struct ModelConstructionProbeDispatch { Arcadia_Engine_Visuals_ModelResourceDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.ModelConstructionProbe", ModelConstructionProbe,
                          u8"Arcadia.Engine.Visuals.ModelResource");

static void
constructModelProbe
  (
    Arcadia_Thread* thread,
    ModelConstructionProbe* self
  )
{
  Arcadia_EnterConstructor(ModelConstructionProbe);
  ResourceLifecycleContext* context = (void*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 4, _ResourceLifecycleContext_getType(thread));
  context->constructingModel = (void*)self;
  // First isolate derived-field visitation while keeping the receiver safe on
  // the unrepaired implementation. The second pass supplies no receiver root.
  Arcadia_Value arguments[4];
  for (size_t i = 0; i < 4; ++i) arguments[i] = Arcadia_ValueStack_getValue(thread, 4 - i);
  if (context->guardReceiver) Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)self);
  for (size_t i = 0; i < 4; ++i) Arcadia_ValueStack_pushValue(thread, &arguments[i]);
  Arcadia_ValueStack_pushNatural8Value(thread, 4);
  Arcadia_superTypeConstructor(thread, _type, self);
  if (context->guardReceiver) Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_LeaveConstructor(ModelConstructionProbe);
}

static const Arcadia_ObjectType_Operations modelProbeObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructModelProbe,
};

static const Arcadia_Type_Operations modelProbeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &modelProbeObjectOperations,
};

Arcadia_defineObjectType(u8"Tests.ModelConstructionProbe", ModelConstructionProbe,
                         u8"Arcadia.Engine.Visuals.ModelResource", Arcadia_Engine_Visuals_ModelResource,
                         &modelProbeOperations);

static void
checkCounts
  (
    Arcadia_Thread* thread,
    ResourceLifecycleContext* context,
    Arcadia_Integer32Value const* expected
  )
{
  for (size_t i = 0; i < 4; ++i) {
    Arcadia_Tests_assertEqualInteger64(thread, expected[i], context->resources[i]->referenceCount);
    Arcadia_Tests_assertTrue(thread, context->resources[i]->context == (Arcadia_Engine_Visuals_BackendContext*)context);
  }
}

static void
expectCountFailure
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* resource,
    Arcadia_BooleanValue increment
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Integer32Value before = resource->referenceCount;
  Arcadia_Engine_Visuals_BackendContext* context = resource->context;
  volatile Arcadia_BooleanValue raised = false;
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    if (increment) Arcadia_Engine_Visuals_Resource_ref(thread, resource);
    else Arcadia_Engine_Visuals_Resource_unref(thread, resource);
  } else {
    raised = true;
  }
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  Arcadia_Tests_assertTrue(thread, raised);
  Arcadia_Tests_assertEqualStatus(thread, Arcadia_Status_OperationInvalid, status);
  Arcadia_Tests_assertEqualInteger64(thread, before, resource->referenceCount);
  Arcadia_Tests_assertTrue(thread, resource->context == context);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
}

static Arcadia_Engine_Visuals_ModelResource*
createModel
  (
    Arcadia_Thread* thread,
    ResourceLifecycleContext* context,
    Arcadia_BooleanValue alias,
    unsigned badArgument
  )
{
  Arcadia_Value arguments[] = {
    Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)context),
    Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)context->resources[alias ? 0 : 1]),
    Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)context->resources[2]),
    Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)context->resources[3]),
  };
  if (badArgument) arguments[badArgument] = Arcadia_Value_makeInteger32Value(0);
  return (void*)create(thread, context->observeConstruction ? _ModelConstructionProbe_getType(thread)
    : _Arcadia_Engine_Visuals_ModelResource_getType(thread), arguments, 4);
}

static void
expectModelFailure
  (
    Arcadia_Thread* thread,
    ResourceLifecycleContext* context,
    Arcadia_BooleanValue alias,
    unsigned badArgument,
    Arcadia_Status expectedStatus
  )
{
  Arcadia_Integer32Value before[4];
  for (size_t i = 0; i < 4; ++i) before[i] = context->resources[i]->referenceCount;
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  volatile Arcadia_BooleanValue raised = false;
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Engine_Visuals_ModelResource* model = createModel(thread, context, alias, badArgument);
    // If rejection is broken, still honor the successful object's unlink protocol.
    Arcadia_Engine_Visuals_Resource_unload(thread, (void*)model);
    Arcadia_Engine_Visuals_Resource_unlink(thread, (void*)model);
  } else {
    raised = true;
  }
  Arcadia_Status status = Arcadia_Thread_getStatus(thread);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
  Arcadia_Tests_assertTrue(thread, raised);
  Arcadia_Tests_assertEqualStatus(thread, expectedStatus, status);
  Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
  checkCounts(thread, context, before);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
}

static ResourceLifecycleContext*
createFixture
  (
    Arcadia_Thread* thread
  )
{
  ResourceLifecycleContext* context = (void*)create(thread, _ResourceLifecycleContext_getType(thread), NULL, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_Value argument = Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)context);
  Arcadia_TypeValue types[] = { _Arcadia_Engine_Visuals_ConstantBufferResource_getType(thread),
    _Arcadia_Engine_Visuals_ConstantBufferResource_getType(thread), _Arcadia_Engine_Visuals_VertexBufferResource_getType(thread) };
  for (size_t i = 0; i < 3; ++i) context->resources[i] = (void*)create(thread, types[i], &argument, 1);
  Arcadia_Value materialArguments[] = { argument, Arcadia_Value_makeBooleanValue(false),
    Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_BlendFunction_getType(thread), Arcadia_Engine_Visuals_BlendFunction_One)),
    Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_BlendFunction_getType(thread), Arcadia_Engine_Visuals_BlendFunction_Zero)),
    Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_ColorSource_getType(thread), Arcadia_Engine_Visuals_ColorSource_Mesh)) };
  context->resources[3] = (void*)create(thread, _Arcadia_Engine_Visuals_MaterialResource_getType(thread), materialArguments, 5);
  return context;
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  ResourceLifecycleContext* context = createFixture(thread);
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, i);
  Arcadia_Integer32Value zero[] = { 0, 0, 0, 0 };
  checkCounts(thread, context, zero);
  for (size_t i = 0; i < 4; ++i) {
    Arcadia_Engine_Visuals_Resource* resource = context->resources[i];
    expectCountFailure(thread, resource, false);
    for (unsigned n = 0; n < 256; ++n) Arcadia_Engine_Visuals_Resource_ref(thread, resource);
    Arcadia_Tests_assertEqualInteger64(thread, 256, resource->referenceCount);
    for (unsigned n = 0; n < 256; ++n) Arcadia_Engine_Visuals_Resource_unref(thread, resource);
    Arcadia_Tests_assertEqualInteger64(thread, 0, resource->referenceCount);
    resource->referenceCount = Arcadia_Integer32Value_Maximum - 1;
    Arcadia_Engine_Visuals_Resource_ref(thread, resource);
    expectCountFailure(thread, resource, true);
    Arcadia_Engine_Visuals_Resource_unref(thread, resource);
    Arcadia_Tests_assertEqualInteger64(thread, Arcadia_Integer32Value_Maximum - 1, resource->referenceCount);
    resource->referenceCount = 0;
  }
  Arcadia_Integer32Value initial[] = { 3, 5, 7, 11 };
  for (size_t i = 0; i < 4; ++i) context->resources[i]->referenceCount = initial[i];
  for (size_t i = 0; i < 4; ++i) {
    context->resources[i]->referenceCount = Arcadia_Integer32Value_Maximum;
    expectModelFailure(thread, context, false, 0, Arcadia_Status_OperationInvalid);
    context->resources[i]->referenceCount = initial[i];
  }
  for (unsigned i = 1; i <= 3; ++i) expectModelFailure(thread, context, false, i, Arcadia_Status_ArgumentTypeInvalid);
  context->failCreation = true;
  expectModelFailure(thread, context, false, 0, Arcadia_Status_OperationFailed);
  context->failCreation = false;
  context->resources[0]->referenceCount = Arcadia_Integer32Value_Maximum - 1;
  expectModelFailure(thread, context, true, 0, Arcadia_Status_OperationInvalid);
  context->resources[0]->referenceCount = initial[0];

  Arcadia_Engine_Visuals_ModelResource* first = createModel(thread, context, false, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)first);
  Arcadia_Engine_Visuals_ModelResource* second = createModel(thread, context, false, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)second);
  Arcadia_Integer32Value shared[] = { 5, 7, 9, 13 }, single[] = { 4, 6, 8, 12 };
  checkCounts(thread, context, shared);
  Arcadia_Tests_assertTrue(thread, first->constantBuffer == (void*)context->resources[0] && first->meshConstantBuffer == (void*)context->resources[1]);
  Arcadia_Engine_Visuals_Resource_unload(thread, (void*)first);
  checkCounts(thread, context, shared);
  Arcadia_Engine_Visuals_Resource_unlink(thread, (void*)first);
  checkCounts(thread, context, single);
  Arcadia_Tests_assertTrue(thread, !first->constantBuffer && !first->meshConstantBuffer && !first->meshVertexBuffer && !first->material);
  Arcadia_Tests_assertTrue(thread, !((Arcadia_Engine_Visuals_Resource*)first)->context);
  Arcadia_Engine_Visuals_Resource_unload(thread, (void*)second);
  Arcadia_Engine_Visuals_Resource_unlink(thread, (void*)second);
  checkCounts(thread, context, initial);
  Arcadia_ValueStack_popValues(thread, 2);

  Arcadia_Engine_Visuals_ModelResource* aliased = createModel(thread, context, true, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)aliased);
  Arcadia_Integer32Value aliasCounts[] = { 5, 5, 8, 12 };
  checkCounts(thread, context, aliasCounts);
  Arcadia_Engine_Visuals_Resource_unload(thread, (void*)aliased);
  checkCounts(thread, context, aliasCounts);
  Arcadia_Engine_Visuals_Resource_unlink(thread, (void*)aliased);
  checkCounts(thread, context, initial);
  Arcadia_ValueStack_popValues(thread, 1);
  for (size_t i = 0; i < 4; ++i) context->resources[i]->referenceCount = 0;
  // Logical unref does not synchronously delete a context-retained object.
  Arcadia_WeakReference* observer = Arcadia_WeakReference_create(thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)context->resources[0]));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)observer);
  Arcadia_Engine_Visuals_Resource_ref(thread, context->resources[0]);
  Arcadia_Engine_Visuals_Resource_unref(thread, context->resources[0]);
  Arcadia_Value retained = Arcadia_WeakReference_getValue(thread, observer);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isObjectReferenceValue(&retained));
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Value value = Arcadia_WeakReference_getValue(thread, observer);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isObjectReferenceValue(&value));
  context->resources[0] = NULL;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  value = Arcadia_WeakReference_getValue(thread, observer);
  Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&value));
  Arcadia_ValueStack_popValues(thread, 1);
  for (size_t i = 0; i < 300; ++i) Arcadia_Tests_assertEqualNatural64(thread, i, Arcadia_ValueStack_getNatural64Value(thread, 299 - i));
  Arcadia_ValueStack_popValues(thread, 301);
  Arcadia_Tests_assertEqualSize(thread, entry, Arcadia_ValueStack_getSize(thread));
}

int
Arcadia_Engine_Tests_VisualResources_resourceLifecycle
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void
testModelConstruction
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  ResourceLifecycleContext* context = createFixture(thread);
  context->observeConstruction = true;
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, i);
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned guarded = 0; guarded < 2; ++guarded) {
    context->guardReceiver = guarded == 0;
    for (unsigned failure = 0; failure < 7; ++failure) {
      context->failCreation = failure == 1;
      Arcadia_Integer32Value expected[] = { 3, 5, 7, 11 };
      if (failure >= 2 && failure <= 5) expected[failure - 2] = Arcadia_Integer32Value_Maximum;
      if (failure == 6) expected[0] = Arcadia_Integer32Value_Maximum - 1;
      for (size_t i = 0; i < 4; ++i) context->resources[i]->referenceCount = expected[i];
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        Arcadia_Engine_Visuals_ModelResource* model = createModel(thread, context, failure == 6, 0);
        Arcadia_Thread_popJumpTarget(thread);
        context->constructingModel = NULL;
        Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)model);
        Arcadia_Tests_assertTrue(thread, failure == 0);
        Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)model, _ModelConstructionProbe_getType(thread)));
        Arcadia_Tests_assertTrue(thread, model->localToWorldMatrix != NULL);
        Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
        for (size_t i = 0; i < 2; ++i) {
          Arcadia_Value retained = Arcadia_WeakReference_getValue(thread, context->constructionObjects[i]);
          Arcadia_Tests_assertTrue(thread, Arcadia_Value_isObjectReferenceValue(&retained));
        }
        // Real composite lifecycle, not destructor-driven count cleanup.
        Arcadia_Engine_Visuals_Resource_unload(thread, (void*)model);
        Arcadia_Engine_Visuals_Resource_unlink(thread, (void*)model);
        Arcadia_ValueStack_popValues(thread, 1);
      } else {
        Arcadia_Status status = Arcadia_Thread_getStatus(thread);
        Arcadia_Thread_popJumpTarget(thread);
        context->constructingModel = NULL;
        Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
        Arcadia_Tests_assertTrue(thread, failure != 0);
        Arcadia_Tests_assertEqualStatus(thread, failure == 1 ? Arcadia_Status_OperationFailed : Arcadia_Status_OperationInvalid, status);
      }
      Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
      checkCounts(thread, context, expected);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      for (size_t i = 0; i < 6; ++i) {
        Arcadia_Tests_assertTrue(thread, context->constructionObjects[i] != NULL);
        Arcadia_Value value = Arcadia_WeakReference_getValue(thread, context->constructionObjects[i]);
        // Receiver/matrix roots must be released; dependencies stay in registry.
        Arcadia_Tests_assertTrue(thread, i < 2 ? Arcadia_Value_isVoidValue(&value) : Arcadia_Value_isObjectReferenceValue(&value));
      }
      for (size_t i = 0; i < 300; ++i) Arcadia_Tests_assertEqualNatural64(thread, i, Arcadia_ValueStack_getNatural64Value(thread, 299 - i));
    }
  }
  for (size_t i = 0; i < 4; ++i) {
    context->resources[i]->referenceCount = 0;
    context->resources[i] = NULL;
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  for (size_t i = 2; i < 6; ++i) {
    Arcadia_Value value = Arcadia_WeakReference_getValue(thread, context->constructionObjects[i]);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&value));
  }
  Arcadia_ValueStack_popValues(thread, 301);
  Arcadia_Tests_assertEqualSize(thread, entry, Arcadia_ValueStack_getSize(thread));
}

int
Arcadia_Engine_Tests_VisualResources_modelConstruction
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(testModelConstruction) ? EXIT_SUCCESS : EXIT_FAILURE;
}
