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

typedef struct ScalarMaterialProbe ScalarMaterialProbe;
typedef struct ScalarMaterialProbeDispatch ScalarMaterialProbeDispatch;

struct ScalarMaterialProbe {
  Arcadia_Engine_Visuals_MaterialResource _parent;
  Arcadia_String* label;
  Arcadia_SizeValue calls[4];
};

struct ScalarMaterialProbeDispatch { Arcadia_Engine_Visuals_MaterialResourceDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.ScalarMaterialProbe", ScalarMaterialProbe,
                          u8"Arcadia.Engine.Visuals.MaterialResource");

static Arcadia_SizeValue destroyed;
static Arcadia_BooleanValue failCallbacks;

static void
constructImpl
  (
    Arcadia_Thread* thread,
    ScalarMaterialProbe* self
  )
{
  Arcadia_EnterConstructor(ScalarMaterialProbe);
  if (_numberOfArguments != 5) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value arguments[5];
  for (Arcadia_SizeValue i = 0; i < 5; ++i) arguments[i] = Arcadia_ValueStack_getValue(thread, 5 - i);
  for (Arcadia_SizeValue i = 0; i < 5; ++i) Arcadia_ValueStack_pushValue(thread, &arguments[i]);
  Arcadia_ValueStack_pushNatural8Value(thread, 5);
  Arcadia_superTypeConstructor(thread, _type, self);
  for (Arcadia_SizeValue i = 0; i < 4; ++i) self->calls[i] = 0;
  self->label = Arcadia_String_createFromCxxString(thread, "scalar material payload");
  Arcadia_LeaveConstructor(ScalarMaterialProbe);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    ScalarMaterialProbe* self
  )
{ if (self->label) Arcadia_Object_visit(thread, (void*)self->label); }

static void
destructImpl
  (
    Arcadia_Thread* thread,
    ScalarMaterialProbe* self
  )
{ destroyed++; }

static void
record
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* receiver,
    unsigned slot
  )
{
  ScalarMaterialProbe* self = (void*)receiver;
  if (failCallbacks) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, self->label, "scalar material payload", sizeof("scalar material payload") - 1));
  self->calls[slot]++;
}

static void
load
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  )
{ record(thread, self, 0); }

static void
unload
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  )
{ record(thread, self, 1); }

static void
unlinkResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  )
{ record(thread, self, 2); }

static void
render
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self,
    Arcadia_Engine_Visuals_EnterPassResource* pass
  )
{
  Arcadia_Tests_assertTrue(thread, pass == NULL);
  record(thread, self, 3);
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    ScalarMaterialProbeDispatch* self
  )
{
  Arcadia_Engine_Visuals_ResourceDispatch* base = (Arcadia_Engine_Visuals_ResourceDispatch*)self;
  base->load = &load; base->unload = &unload;
  base->unlink = &unlinkResource; base->render = &render;
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Tests.ScalarMaterialProbe", ScalarMaterialProbe,
                         u8"Arcadia.Engine.Visuals.MaterialResource", Arcadia_Engine_Visuals_MaterialResource,
                         &typeOperations);

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
expectFailure
  (
    Arcadia_Thread* thread,
    Arcadia_Value* arguments,
    Arcadia_Natural8Value count,
    Arcadia_Status status
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    create(thread, _Arcadia_Engine_Visuals_MaterialResource_getType(thread), arguments, count);
    Arcadia_Thread_popJumpTarget(thread); Arcadia_Tests_assertTrue(thread, false);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == status && Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Thread_setStatus(thread, 0);
  }
}

static void
call
  (
    Arcadia_Thread* thread,
    ScalarMaterialProbe* self,
    unsigned operation
  )
{
  switch (operation) {
    case 0: Arcadia_Engine_Visuals_Resource_load(thread, (void*)self); break;
    case 1: Arcadia_Engine_Visuals_Resource_unload(thread, (void*)self); break;
    case 2: Arcadia_Engine_Visuals_Resource_unlink(thread, (void*)self); break;
    case 3: Arcadia_Engine_Visuals_Resource_render(thread, (void*)self, NULL); break;
  }
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_SizeValue i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Engine_Visuals_BackendContext* context = (void*)create(thread, _Arcadia_Engine_Visuals_BackendContext_getType(thread), NULL, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, context);
  Arcadia_TypeValue materialType = _Arcadia_Engine_Visuals_MaterialResource_getType(thread);
  Arcadia_Engine_Visuals_ResourceDispatch* dispatch = (void*)Arcadia_ObjectType_getDispatch(materialType);
  Arcadia_Tests_assertTrue(thread, !dispatch->load && !dispatch->unload && !dispatch->unlink && !dispatch->render);
  Arcadia_TypeValue blendType = _Arcadia_Engine_Visuals_BlendFunction_getType(thread);
  Arcadia_TypeValue colorType = _Arcadia_Engine_Visuals_ColorSource_getType(thread);
  Arcadia_Value arguments[] = {
    Arcadia_Value_makeObjectReferenceValue(context), Arcadia_Value_makeBooleanValue(false),
    Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(blendType, Arcadia_Engine_Visuals_BlendFunction_SourceAlpha)),
    Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(blendType, Arcadia_Engine_Visuals_BlendFunction_OneMinusSourceAlpha)),
    Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(colorType, Arcadia_Engine_Visuals_ColorSource_Mesh)),
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void)
  };
  const Arcadia_Engine_Visuals_ColorSource sources[] = { Arcadia_Engine_Visuals_ColorSource_Mesh, Arcadia_Engine_Visuals_ColorSource_Vertex, Arcadia_Engine_Visuals_ColorSource_Texture };
  for (unsigned blend = 0; blend < 2; ++blend) {
    arguments[1] = Arcadia_Value_makeBooleanValue(blend);
    for (Arcadia_SizeValue i = 0; i < sizeof(sources) / sizeof(sources[0]); ++i) {
      arguments[4] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(colorType, sources[i]));
      Arcadia_Engine_Visuals_MaterialResource* self = (void*)create(thread, materialType, arguments, 5);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Visuals_Resource*)self)->context == context && ((Arcadia_Engine_Visuals_Resource*)self)->referenceCount == 0 && self->blendEnabled == blend);
      Arcadia_Tests_assertTrue(thread, self->blendSourceFunction == Arcadia_Engine_Visuals_BlendFunction_SourceAlpha && self->blendDestinationFunction == Arcadia_Engine_Visuals_BlendFunction_OneMinusSourceAlpha && self->ambientColorSource == sources[i]);
      Arcadia_ValueStack_popValues(thread, 1);
    }
  }
  // All combinations of exact enum values and legacy Integer32 callback input.
  for (unsigned mask = 0; mask < 8; ++mask) {
    const Arcadia_Integer32Value values[] = { Arcadia_Engine_Visuals_BlendFunction_SourceAlpha, Arcadia_Engine_Visuals_BlendFunction_OneMinusSourceAlpha, Arcadia_Engine_Visuals_ColorSource_Texture };
    for (unsigned i = 0; i < 3; ++i) arguments[i + 2] = mask & (1u << i) ? Arcadia_Value_makeInteger32Value(values[i]) : Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(i < 2 ? blendType : colorType, values[i]));
    Arcadia_Engine_Visuals_MaterialResource* self = (void*)create(thread, materialType, arguments, 5);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
    Arcadia_Tests_assertTrue(thread, self->blendSourceFunction == values[0] && self->blendDestinationFunction == values[1] && self->ambientColorSource == values[2]);
    Arcadia_ValueStack_popValues(thread, 1);
  }
  arguments[2] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(blendType, Arcadia_Engine_Visuals_BlendFunction_SourceAlpha));
  arguments[3] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(blendType, Arcadia_Engine_Visuals_BlendFunction_OneMinusSourceAlpha));
  arguments[4] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(colorType, Arcadia_Engine_Visuals_ColorSource_Texture));
  expectFailure(thread, arguments, 0, Arcadia_Status_NumberOfArgumentsInvalid);
  expectFailure(thread, arguments, 4, Arcadia_Status_NumberOfArgumentsInvalid);
  expectFailure(thread, arguments, 6, Arcadia_Status_NumberOfArgumentsInvalid);
  for (unsigned i = 0; i < 5; ++i) {
    Arcadia_Value saved = arguments[i];
    arguments[i] = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
    expectFailure(thread, arguments, 5, Arcadia_Status_ArgumentTypeInvalid);
    arguments[i] = Arcadia_Value_makeNatural32Value(1);
    expectFailure(thread, arguments, 5, Arcadia_Status_ArgumentTypeInvalid);
    arguments[i] = saved;
  }
  for (unsigned i = 2; i < 5; ++i) {
    Arcadia_Value saved = arguments[i];
    arguments[i] = Arcadia_Value_makeEnumerationValue(Arcadia_EnumerationValue_make(i < 4 ? colorType : blendType, 1));
    expectFailure(thread, arguments, 5, Arcadia_Status_ArgumentTypeInvalid);
    arguments[i] = saved;
  }
  ScalarMaterialProbe* probe = (void*)create(thread, _ScalarMaterialProbe_getType(thread), arguments, 5);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, probe);
  for (unsigned operation = 0; operation < 4; ++operation) {
    call(thread, probe, operation);
    Arcadia_Tests_assertTrue(thread, probe->calls[operation] == 1);
  }
  failCallbacks = true;
  for (unsigned operation = 0; operation < 4; ++operation) {
    Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      call(thread, probe, operation);
      Arcadia_Thread_popJumpTarget(thread); Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed && Arcadia_ValueStack_getSize(thread) == entry);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  failCallbacks = false;
  Arcadia_Engine_Visuals_Resource_ref(thread, (void*)probe);
  Arcadia_Engine_Visuals_Resource_unref(thread, (void*)probe);
  Arcadia_Tests_assertTrue(thread, ((Arcadia_Engine_Visuals_Resource*)probe)->referenceCount == 0);
  Arcadia_Tests_assertTrue(thread, destroyed == 0);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 1);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

int
Arcadia_Engine_Tests_VisualResources_materialResource
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
