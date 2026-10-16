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

#include "Arcadia/Engine/Tests/Backends/Cases.h"
#include "Arcadia/Engine/Include.h"

typedef struct CoreProbe CoreProbe;
typedef struct CoreProbeDispatch CoreProbeDispatch;
typedef struct AudialsProbe AudialsProbe;
typedef struct AudialsProbeDispatch AudialsProbeDispatch;
typedef struct VisualsProbe VisualsProbe;
typedef struct VisualsProbeDispatch VisualsProbeDispatch;
typedef struct ContextProbe ContextProbe;
typedef struct ContextProbeDispatch ContextProbeDispatch;
typedef struct DisplayProbe DisplayProbe;
typedef struct DisplayProbeDispatch DisplayProbeDispatch;

struct CoreProbe { Arcadia_Engine_Backend _parent; };

struct CoreProbeDispatch { Arcadia_Engine_BackendDispatch _parent; };

struct AudialsProbe { Arcadia_Engine_Audials_Backend _parent; };

struct AudialsProbeDispatch { Arcadia_Engine_Audials_BackendDispatch _parent; };

struct VisualsProbe { Arcadia_Engine_Visuals_Backend _parent; };

struct VisualsProbeDispatch { Arcadia_Engine_Visuals_BackendDispatch _parent; };

struct ContextProbe {
  Arcadia_Engine_BackendContext _parent;
  Arcadia_String* label;
  Arcadia_SizeValue calls;
};

struct ContextProbeDispatch { Arcadia_Engine_BackendContextDispatch _parent; };

struct DisplayProbe {
  Arcadia_Engine_Visuals_DisplayMode _parent;
  Arcadia_String* label;
  Arcadia_Integer32Value values[4];
  Arcadia_SizeValue calls;
};

struct DisplayProbeDispatch { Arcadia_Engine_Visuals_DisplayModeDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.CoreProbe", CoreProbe,
                          u8"Arcadia.Engine.Backend");
Arcadia_declareObjectType(u8"Tests.AudialsProbe", AudialsProbe,
                          u8"Arcadia.Engine.Audials.Backend");
Arcadia_declareObjectType(u8"Tests.VisualsProbe", VisualsProbe,
                          u8"Arcadia.Engine.Visuals.Backend");
Arcadia_declareObjectType(u8"Tests.ContextProbe", ContextProbe,
                          u8"Arcadia.Engine.BackendContext");
Arcadia_declareObjectType(u8"Tests.DisplayProbe", DisplayProbe,
                          u8"Arcadia.Engine.Visuals.DisplayMode");

static Arcadia_BooleanValue failCallbacks;
static Arcadia_SizeValue destroyed;

static Arcadia_Object*
create0
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  return _Arcadia_EndCreate0(thread, type, base);
}

static void
checkCallback
  (
    Arcadia_Thread* thread
  )
{
  if (failCallbacks) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
}

static Arcadia_String*
getName
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Backend* self
  )
{
  checkCallback(thread);
  return Arcadia_String_createFromCxxString(thread, "probe backend");
}

static Arcadia_Engine_BackendContext*
createContext
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Backend* self
  )
{
  checkCallback(thread);
  return (void*)create0(thread, _ContextProbe_getType(thread));
}

static void
initializeBackendDispatch
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_BackendDispatch* self
  )
{
  self->getName = &getName;
  self->createBackendContext = &createContext;
}

static void
constructCore
  (
    Arcadia_Thread* thread,
    CoreProbe* self
  )
{
  Arcadia_EnterConstructor(CoreProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_LeaveConstructor(CoreProbe);
}

static void
constructAudials
  (
    Arcadia_Thread* thread,
    AudialsProbe* self
  )
{
  Arcadia_EnterConstructor(AudialsProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_LeaveConstructor(AudialsProbe);
}

static void
constructVisuals
  (
    Arcadia_Thread* thread,
    VisualsProbe* self
  )
{
  Arcadia_EnterConstructor(VisualsProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_LeaveConstructor(VisualsProbe);
}

static void
constructContext
  (
    Arcadia_Thread* thread,
    ContextProbe* self
  )
{
  Arcadia_EnterConstructor(ContextProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->calls = 0;
  self->label = Arcadia_String_createFromCxxString(thread, "context payload");
  Arcadia_LeaveConstructor(ContextProbe);
}

static void
visitContext
  (
    Arcadia_Thread* thread,
    ContextProbe* self
  )
{
  if (self->label) Arcadia_Object_visit(thread, (Arcadia_Object*)self->label);
}

static void
update
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_BackendContext* receiver
  )
{
  ContextProbe* self = (void*)receiver;
  checkCallback(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, self->label, "context payload", sizeof("context payload") - 1));
  self->calls++;
}

static void
initializeContextDispatch
  (
    Arcadia_Thread* thread,
    ContextProbeDispatch* self
  )
{ ((Arcadia_Engine_BackendContextDispatch*)self)->update = &update; }

static void
constructDisplay
  (
    Arcadia_Thread* thread,
    DisplayProbe* self
  )
{
  Arcadia_EnterConstructor(DisplayProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->calls = 0;
  self->values[0] = Arcadia_Integer32Value_Maximum;
  self->values[1] = Arcadia_Integer32Value_Minimum;
  self->values[2] = 32;
  self->values[3] = 144;
  self->label = Arcadia_String_createFromCxxString(thread, "display payload");
  Arcadia_LeaveConstructor(DisplayProbe);
}

static void
visitDisplay
  (
    Arcadia_Thread* thread,
    DisplayProbe* self
  )
{
  if (self->label) Arcadia_Object_visit(thread, (Arcadia_Object*)self->label);
}

static Arcadia_Integer32Value
getDisplayValue
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* receiver,
    Arcadia_SizeValue index
  )
{
  DisplayProbe* self = (void*)receiver;
  checkCallback(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, self->label, "display payload", sizeof("display payload") - 1));
  return self->values[index];
}

static Arcadia_Integer32Value
getHorizontal
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  )
{ return getDisplayValue(thread, self, 0); }

static Arcadia_Integer32Value
getVertical
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  )
{ return getDisplayValue(thread, self, 1); }

static Arcadia_Integer32Value
getDepth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  )
{ return getDisplayValue(thread, self, 2); }

static Arcadia_Integer32Value
getFrequency
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  )
{ return getDisplayValue(thread, self, 3); }

static void
apply
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* receiver
  )
{
  DisplayProbe* self = (void*)receiver;
  checkCallback(thread);
  self->calls++;
}

static void
initializeDisplayDispatch
  (
    Arcadia_Thread* thread,
    DisplayProbeDispatch* receiver
  )
{
  Arcadia_Engine_Visuals_DisplayModeDispatch* self = (void*)receiver;
  self->getHorizontalResolution = &getHorizontal;
  self->getVerticalResolution = &getVertical;
  self->getColorDepth = &getDepth;
  self->getFrequency = &getFrequency;
  self->apply = &apply;
}

static void
destructProbe
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ destroyed++; }

static const Arcadia_ObjectType_Operations coreObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructCore,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeBackendDispatch,
};

static const Arcadia_Type_Operations coreTypeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &coreObjectOperations,
};

static const Arcadia_ObjectType_Operations audialsObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructAudials,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeBackendDispatch,
};

static const Arcadia_Type_Operations audialsTypeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &audialsObjectOperations,
};

static const Arcadia_ObjectType_Operations visualsObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructVisuals,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeBackendDispatch,
};

static const Arcadia_Type_Operations visualsTypeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &visualsObjectOperations,
};

static const Arcadia_ObjectType_Operations contextObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructContext,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeContextDispatch,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitContext,
  .destruct = &destructProbe,
};

static const Arcadia_Type_Operations contextTypeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &contextObjectOperations,
};

static const Arcadia_ObjectType_Operations displayObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructDisplay,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDisplayDispatch,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitDisplay,
  .destruct = &destructProbe,
};

static const Arcadia_Type_Operations displayTypeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &displayObjectOperations,
};

Arcadia_defineObjectType(u8"Tests.CoreProbe", CoreProbe,
                         u8"Arcadia.Engine.Backend", Arcadia_Engine_Backend,
                         &coreTypeOperations);
Arcadia_defineObjectType(u8"Tests.AudialsProbe", AudialsProbe,
                         u8"Arcadia.Engine.Audials.Backend", Arcadia_Engine_Audials_Backend,
                         &audialsTypeOperations);
Arcadia_defineObjectType(u8"Tests.VisualsProbe", VisualsProbe,
                         u8"Arcadia.Engine.Visuals.Backend", Arcadia_Engine_Visuals_Backend,
                         &visualsTypeOperations);
Arcadia_defineObjectType(u8"Tests.ContextProbe", ContextProbe,
                         u8"Arcadia.Engine.BackendContext", Arcadia_Engine_BackendContext,
                         &contextTypeOperations);
Arcadia_defineObjectType(u8"Tests.DisplayProbe", DisplayProbe,
                         u8"Arcadia.Engine.Visuals.DisplayMode", Arcadia_Engine_Visuals_DisplayMode,
                         &displayTypeOperations);

static void
callOperation
  (
    Arcadia_Thread* thread,
    unsigned operation,
    Arcadia_Object* self
  )
{
  switch (operation) {
    case 0: Arcadia_Engine_Backend_getName(thread, (void*)self); break;
    case 1: Arcadia_Engine_Backend_createBackendContext(thread, (void*)self); break;
    case 2: Arcadia_Engine_BackendContext_update(thread, (void*)self); break;
    case 3: Arcadia_Engine_Visuals_DisplayMode_getHorizontalResolution(thread, (void*)self); break;
    case 4: Arcadia_Engine_Visuals_DisplayMode_getVerticalResolution(thread, (void*)self); break;
    case 5: Arcadia_Engine_Visuals_DisplayMode_getColorDepth(thread, (void*)self); break;
    case 6: Arcadia_Engine_Visuals_DisplayMode_getFrequency(thread, (void*)self); break;
    case 7: Arcadia_Engine_Visuals_DisplayMode_apply(thread, (void*)self); break;
  }
}

static void
expectCallFailure
  (
    Arcadia_Thread* thread,
    unsigned operation,
    Arcadia_Object* self,
    Arcadia_Status status
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    callOperation(thread, operation, self);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == status);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Thread_setStatus(thread, 0);
  }
}

static void
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_TypeValue types[] = {
    _Arcadia_Engine_Backend_getType(thread), _Arcadia_Engine_BackendContext_getType(thread),
    _Arcadia_Engine_Audials_Backend_getType(thread), _Arcadia_Engine_Visuals_Backend_getType(thread),
    _Arcadia_Engine_Visuals_DisplayMode_getType(thread)
  };
  for (size_t i = 0; i < 5; ++i) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_ValueStack_pushNatural64Value(thread, 1);
      Arcadia_ValueStack_pushNatural8Value(thread, 1);
      _Arcadia_EndCreate0(thread, types[i], depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_TypeValue probeTypes[] = { _CoreProbe_getType(thread), _AudialsProbe_getType(thread), _VisualsProbe_getType(thread) };
  for (size_t i = 0; i < 3; ++i) {
    Arcadia_Engine_BackendDispatch* root = (void*)Arcadia_ObjectType_getDispatch(types[i == 0 ? 0 : i + 1]);
    Arcadia_Tests_assertTrue(thread, !root->getName && !root->createBackendContext);
    Arcadia_Engine_Backend* backend = (void*)create0(thread, probeTypes[i]);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, backend);
    Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)backend, types[0]));
    Arcadia_String* name = Arcadia_Engine_Backend_getName(thread, backend);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
    Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, name, "probe backend", sizeof("probe backend") - 1));
    ContextProbe* context = (void*)Arcadia_Engine_Backend_createBackendContext(thread, backend);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, context);
    Arcadia_Engine_BackendContext_update(thread, (void*)context);
    Arcadia_Tests_assertTrue(thread, context->calls == 1);
    failCallbacks = Arcadia_BooleanValue_True;
    expectCallFailure(thread, 0, (void*)backend, Arcadia_Status_OperationFailed);
    expectCallFailure(thread, 1, (void*)backend, Arcadia_Status_OperationFailed);
    expectCallFailure(thread, 2, (void*)context, Arcadia_Status_OperationFailed);
    failCallbacks = Arcadia_BooleanValue_False;
    Arcadia_ValueStack_popValues(thread, 3);
  }
  Arcadia_Engine_BackendContextDispatch* contextRoot = (void*)Arcadia_ObjectType_getDispatch(types[1]);
  Arcadia_Engine_Visuals_DisplayModeDispatch* displayRoot = (void*)Arcadia_ObjectType_getDispatch(types[4]);
  Arcadia_Tests_assertTrue(thread, !contextRoot->update);
  Arcadia_Tests_assertTrue(thread, !displayRoot->getHorizontalResolution && !displayRoot->getVerticalResolution && !displayRoot->getColorDepth && !displayRoot->getFrequency && !displayRoot->apply);
  DisplayProbe* display = (void*)create0(thread, _DisplayProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, display);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Visuals_DisplayMode_getHorizontalResolution(thread, (void*)display) == Arcadia_Integer32Value_Maximum);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Visuals_DisplayMode_getVerticalResolution(thread, (void*)display) == Arcadia_Integer32Value_Minimum);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Visuals_DisplayMode_getColorDepth(thread, (void*)display) == 32);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Visuals_DisplayMode_getFrequency(thread, (void*)display) == 144);
  Arcadia_Engine_Visuals_DisplayMode_apply(thread, (void*)display);
  Arcadia_Tests_assertTrue(thread, display->calls == 1);
  failCallbacks = Arcadia_BooleanValue_True;
  for (unsigned op = 3; op < 8; ++op) expectCallFailure(thread, op, (void*)display, Arcadia_Status_OperationFailed);
  failCallbacks = Arcadia_BooleanValue_False;
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  destroyed = 0;
  ContextProbe* context = (void*)create0(thread, _ContextProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, context);
  display = (void*)create0(thread, _DisplayProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, display);
  Arcadia_Engine_BackendContext_update(thread, (void*)context);
  Arcadia_Engine_Visuals_DisplayMode_apply(thread, (void*)display);
  Arcadia_Tests_assertTrue(thread, destroyed == 0 && context->calls == 1 && display->calls == 1);
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 2);
  // Only the generated wrapper roots these receivers during callback collection.
  context = (void*)create0(thread, _ContextProbe_getType(thread));
  Arcadia_Engine_BackendContext_update(thread, (void*)context);
  Arcadia_Tests_assertTrue(thread, context->calls == 1);
  for (unsigned op = 3; op < 8; ++op) {
    display = (void*)create0(thread, _DisplayProbe_getType(thread));
    callOperation(thread, op, (void*)display);
  }
  // Generated wrappers make unbound abstract slots and invalid receivers safe.
  for (unsigned op = 0; op < 8; ++op) {
    Arcadia_Object* unbound = create0(thread, types[op < 2 ? 0 : op == 2 ? 1 : 4]);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, unbound);
    expectCallFailure(thread, op, unbound, Arcadia_Status_OperationInvalid);
    expectCallFailure(thread, op, NULL, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_String* wrong = Arcadia_String_createFromCxxString(thread, "wrong receiver");
    Arcadia_ValueStack_pushObjectReferenceValue(thread, wrong);
    expectCallFailure(thread, op, (void*)wrong, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_ValueStack_popValues(thread, 2);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

int
Arcadia_Engine_Tests_Backends_backendBases
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
