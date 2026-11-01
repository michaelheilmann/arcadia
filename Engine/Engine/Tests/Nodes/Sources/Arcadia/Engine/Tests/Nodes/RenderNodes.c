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

#include "Arcadia/Engine/Tests/Nodes/Cases.h"
#include "Arcadia/Engine/Include.h"

typedef struct AudialsProbe AudialsProbe;
typedef struct AudialsProbeDispatch AudialsProbeDispatch;
typedef struct VisualsProbe VisualsProbe;
typedef struct VisualsProbeDispatch VisualsProbeDispatch;

struct AudialsProbe {
  Arcadia_Engine_Audials_Node _parent;
  Arcadia_String* label;
  Arcadia_SizeValue calls;
};

struct AudialsProbeDispatch { Arcadia_Engine_Audials_NodeDispatch _parent; };

struct VisualsProbe {
  Arcadia_Engine_Visuals_Node _parent;
  Arcadia_String* label;
  Arcadia_SizeValue calls;
  Arcadia_Engine_Visuals_EnterPassNode* pass;
};

struct VisualsProbeDispatch { Arcadia_Engine_Visuals_NodeDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.RenderAudialsProbe", AudialsProbe,
                          u8"Arcadia.Engine.Audials.Node");
Arcadia_declareObjectType(u8"Tests.RenderVisualsProbe", VisualsProbe,
                          u8"Arcadia.Engine.Visuals.Node");

static Arcadia_SizeValue destroyed;
static Arcadia_SizeValue attachments;
static Arcadia_BooleanValue fail;

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
constructAudials
  (
    Arcadia_Thread* thread,
    AudialsProbe* self
  )
{
  Arcadia_EnterConstructor(AudialsProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->calls = 0;
  self->label = Arcadia_String_createFromCxxString(thread, "audials render payload");
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
  self->calls = 0;
  self->pass = NULL;
  self->label = Arcadia_String_createFromCxxString(thread, "visuals render payload");
  Arcadia_LeaveConstructor(VisualsProbe);
}

static void
collect
  (
    Arcadia_Thread* thread
  )
{
  if (fail) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
}

static void
renderAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_Node* receiver
  )
{
  AudialsProbe* self = (void*)receiver;
  collect(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, self->label, "audials render payload", sizeof("audials render payload") - 1));
  self->calls++;
}

static void
renderVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Node* receiver,
    Arcadia_Engine_Visuals_EnterPassNode* pass
  )
{
  VisualsProbe* self = (void*)receiver;
  collect(thread);
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, self->label, "visuals render payload", sizeof("visuals render payload") - 1));
  if (pass) Arcadia_Tests_assertTrue(thread, pass->worldToViewMatrix && pass->viewToProjectionMatrix);
  self->pass = pass;
  self->calls++;
}

static void
setAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Node* self,
    Arcadia_Engine_Audials_BackendContext* context
  )
{
  collect(thread);
  if (context) Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (void*)context, _Arcadia_Engine_Audials_BackendContext_getType(thread)));
  attachments++;
}

static void
setVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Node* self,
    Arcadia_Engine_Visuals_BackendContext* context
  )
{
  collect(thread);
  if (context) Arcadia_Tests_assertTrue(thread, context->windows != NULL);
  attachments++;
}

static void
initializeAudials
  (
    Arcadia_Thread* thread,
    AudialsProbeDispatch* self
  )
{
  ((Arcadia_Engine_Audials_NodeDispatch*)self)->render = &renderAudials;
  ((Arcadia_Engine_NodeDispatch*)self)->setAudialsBackendContext = &setAudials;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = &setVisuals;
}

static void
initializeVisuals
  (
    Arcadia_Thread* thread,
    VisualsProbeDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_NodeDispatch*)self)->render = &renderVisuals;
  ((Arcadia_Engine_NodeDispatch*)self)->setAudialsBackendContext = &setAudials;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = &setVisuals;
}

static void
visitAudials
  (
    Arcadia_Thread* thread,
    AudialsProbe* self
  )
{ if (self->label) Arcadia_Object_visit(thread, (void*)self->label); }

static void
visitVisuals
  (
    Arcadia_Thread* thread,
    VisualsProbe* self
  )
{
  if (self->label) Arcadia_Object_visit(thread, (void*)self->label);
  if (self->pass) Arcadia_Object_visit(thread, (void*)self->pass);
}

static void
destructProbe
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self
  )
{ destroyed++; }

static const Arcadia_ObjectType_Operations audialsOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructAudials,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeAudials,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitAudials,
  .destruct = &destructProbe,
};

static const Arcadia_Type_Operations audialsTypeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &audialsOperations,
};

static const Arcadia_ObjectType_Operations visualsOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructVisuals,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeVisuals,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitVisuals,
  .destruct = &destructProbe,
};

static const Arcadia_Type_Operations visualsTypeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &visualsOperations,
};

Arcadia_defineObjectType(u8"Tests.RenderAudialsProbe", AudialsProbe,
                         u8"Arcadia.Engine.Audials.Node", Arcadia_Engine_Audials_Node,
                         &audialsTypeOperations);
Arcadia_defineObjectType(u8"Tests.RenderVisualsProbe", VisualsProbe,
                         u8"Arcadia.Engine.Visuals.Node", Arcadia_Engine_Visuals_Node,
                         &visualsTypeOperations);

static void
expectRenderFailure
  (
    Arcadia_Thread* thread,
    Arcadia_BooleanValue visuals,
    Arcadia_Object* self,
    Arcadia_Status status
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    if (visuals) Arcadia_Engine_Visuals_Node_render(thread, (void*)self, NULL);
    else Arcadia_Engine_Audials_Node_render(thread, (void*)self);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == status && Arcadia_ValueStack_getSize(thread) == base);
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
  Arcadia_TypeValue types[] = { _Arcadia_Engine_Audials_Node_getType(thread), _Arcadia_Engine_Visuals_Node_getType(thread) };
  for (size_t k = 0; k < 2; ++k) {
    Arcadia_Engine_NodeDispatch* dispatch = (void*)Arcadia_ObjectType_getDispatch(types[k]);
    Arcadia_Tests_assertTrue(thread, !dispatch->setAudialsBackendContext && !dispatch->setVisualsBackendContext);
    if (k) {
      Arcadia_Tests_assertTrue(thread, !((Arcadia_Engine_Visuals_NodeDispatch*)dispatch)->render);
    } else {
      Arcadia_Tests_assertTrue(thread, !((Arcadia_Engine_Audials_NodeDispatch*)dispatch)->render);
    }
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_ValueStack_pushNatural64Value(thread, 1);
      Arcadia_ValueStack_pushNatural8Value(thread, 1);
      _Arcadia_EndCreate0(thread, types[k], depth);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid && Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  destroyed = 0;
  AudialsProbe* audials = (void*)create0(thread, _AudialsProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, audials);
  VisualsProbe* visuals = (void*)create0(thread, _VisualsProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, visuals);
  Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (void*)audials, _Arcadia_Engine_Node_getType(thread)) && Arcadia_Object_isInstanceOf(thread, (void*)visuals, _Arcadia_Engine_Node_getType(thread)));
  Arcadia_Engine_Audials_Node_render(thread, (void*)audials);
  Arcadia_Engine_Visuals_Node_render(thread, (void*)visuals, NULL);
  Arcadia_Engine_Visuals_EnterPassNode* pass = Arcadia_Engine_Visuals_EnterPassNode_create(thread, NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, pass);
  Arcadia_Engine_Visuals_Node_render(thread, (void*)visuals, pass);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, audials->calls == 1 && visuals->calls == 2 && visuals->pass == pass);
  Arcadia_Engine_Audials_BackendContext* audioContext = (void*)create0(thread, _Arcadia_Engine_Audials_BackendContext_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, audioContext);
  Arcadia_Engine_Visuals_BackendContext* videoContext = (void*)create0(thread, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, videoContext);
  attachments = 0;
  Arcadia_Object* nodes[] = { (void*)audials, (void*)visuals };
  for (size_t k = 0; k < 2; ++k) {
    Arcadia_Engine_Node_setAudialsBackendContext(thread, (void*)nodes[k], audioContext);
    Arcadia_Engine_Node_setVisualsBackendContext(thread, (void*)nodes[k], videoContext);
    Arcadia_Engine_Node_setAudialsBackendContext(thread, (void*)nodes[k], NULL);
    Arcadia_Engine_Node_setVisualsBackendContext(thread, (void*)nodes[k], NULL);
  }
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_Tests_assertTrue(thread, attachments == 8 && destroyed == 0);
  fail = Arcadia_BooleanValue_True;
  expectRenderFailure(thread, false, (void*)audials, Arcadia_Status_OperationFailed);
  expectRenderFailure(thread, true, (void*)visuals, Arcadia_Status_OperationFailed);
  fail = Arcadia_BooleanValue_False;
  Arcadia_ValueStack_popValues(thread, 2);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 2);
  for (size_t k = 0; k < 2; ++k) {
    Arcadia_Object* root = create0(thread, types[k]);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, root);
    expectRenderFailure(thread, k, root, Arcadia_Status_OperationInvalid);
    expectRenderFailure(thread, k, NULL, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_String* wrong = Arcadia_String_createFromCxxString(thread, "wrong receiver");
    Arcadia_ValueStack_pushObjectReferenceValue(thread, wrong);
    expectRenderFailure(thread, k, (void*)wrong, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_ValueStack_popValues(thread, 2);
  }
  audials = (void*)create0(thread, _AudialsProbe_getType(thread));
  Arcadia_Engine_Audials_Node_render(thread, (void*)audials);
  Arcadia_Tests_assertTrue(thread, audials->calls == 1);
  visuals = (void*)create0(thread, _VisualsProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, visuals);
  pass = Arcadia_Engine_Visuals_EnterPassNode_create(thread, NULL);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Engine_Visuals_Node_render(thread, (void*)visuals, pass);
  Arcadia_Tests_assertTrue(thread, visuals->calls == 1 && visuals->pass == pass);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

int
Arcadia_Engine_Tests_Nodes_renderNodes
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
