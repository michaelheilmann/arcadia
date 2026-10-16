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

typedef struct NodeProbe NodeProbe;
typedef struct NodeProbeDispatch NodeProbeDispatch;

struct NodeProbe {
  Arcadia_Engine_Node _parent;
  Arcadia_Engine_Audials_BackendContext* audials;
  Arcadia_Engine_Visuals_BackendContext* visuals;
  Arcadia_SizeValue audialsCalls;
  Arcadia_SizeValue visualsCalls;
  Arcadia_BooleanValue fail;
};

struct NodeProbeDispatch {
  Arcadia_Engine_NodeDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.NodeProbe", NodeProbe,
                          u8"Arcadia.Engine.Node");

static void
constructImpl
  (
    Arcadia_Thread* thread,
    NodeProbe* self
  )
{
  Arcadia_EnterConstructor(NodeProbe);
  if (_numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  self->audials = NULL;
  self->visuals = NULL;
  self->audialsCalls = 0;
  self->visualsCalls = 0;
  self->fail = Arcadia_BooleanValue_False;
  Arcadia_LeaveConstructor(NodeProbe);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    NodeProbe* self
  )
{
  if (self->audials) Arcadia_Object_visit(thread, (Arcadia_Object*)self->audials);
  if (self->visuals) Arcadia_Object_visit(thread, (Arcadia_Object*)self->visuals);
}

static void
setAudials
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Node* receiver,
    Arcadia_Engine_Audials_BackendContext* context
  )
{
  NodeProbe* self = (NodeProbe*)receiver;
  if (self->fail) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->audials = context;
  self->audialsCalls++;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, !context || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)context, _Arcadia_Engine_Audials_BackendContext_getType(thread)));
}

static void
setVisuals
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Node* receiver,
    Arcadia_Engine_Visuals_BackendContext* context
  )
{
  NodeProbe* self = (NodeProbe*)receiver;
  if (self->fail) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->visuals = context;
  self->visualsCalls++;
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, !context || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)context, _Arcadia_Engine_Visuals_BackendContext_getType(thread)));
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    NodeProbeDispatch* self
  )
{
  ((Arcadia_Engine_NodeDispatch*)self)->setAudialsBackendContext = &setAudials;
  ((Arcadia_Engine_NodeDispatch*)self)->setVisualsBackendContext = &setVisuals;
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Tests.NodeProbe", NodeProbe,
                         u8"Arcadia.Engine.Node", Arcadia_Engine_Node,
                         &typeOperations);

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
test
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_Engine_NodeDispatch* root = (Arcadia_Engine_NodeDispatch*)Arcadia_ObjectType_getDispatch(_Arcadia_Engine_Node_getType(thread));
  Arcadia_Tests_assertTrue(thread, !root->setAudialsBackendContext && !root->setVisualsBackendContext);
  NodeProbe* self = (NodeProbe*)create0(thread, _NodeProbe_getType(thread));
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_Tests_assertTrue(thread, self->audialsCalls == 0 && self->visualsCalls == 0);
  Arcadia_Engine_Audials_BackendContext* audials = (Arcadia_Engine_Audials_BackendContext*)create0(thread, _Arcadia_Engine_Audials_BackendContext_getType(thread));
  Arcadia_Engine_Node_setAudialsBackendContext(thread, (Arcadia_Engine_Node*)self, audials);
  Arcadia_Tests_assertTrue(thread, self->audials == audials && self->audialsCalls == 1);
  Arcadia_Engine_Visuals_BackendContext* visuals = (Arcadia_Engine_Visuals_BackendContext*)create0(thread, _Arcadia_Engine_Visuals_BackendContext_getType(thread));
  Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self, visuals);
  Arcadia_Tests_assertTrue(thread, self->visuals == visuals && self->visualsCalls == 1);
  Arcadia_Engine_Node_setAudialsBackendContext(thread, (Arcadia_Engine_Node*)self, NULL);
  Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self, NULL);
  Arcadia_Tests_assertTrue(thread, !self->audials && !self->visuals && self->audialsCalls == 2 && self->visualsCalls == 2);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 1);
  self->fail = Arcadia_BooleanValue_True;
  for (Arcadia_SizeValue i = 0; i < 2; ++i) {
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      if (i == 0) Arcadia_Engine_Node_setAudialsBackendContext(thread, (Arcadia_Engine_Node*)self, NULL);
      else Arcadia_Engine_Node_setVisualsBackendContext(thread, (Arcadia_Engine_Node*)self, NULL);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationInvalid);
      Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base + 1);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  Arcadia_Tests_assertTrue(thread, self->audialsCalls == 2 && self->visualsCalls == 2);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_JumpTarget arity;
  Arcadia_Thread_pushJumpTarget(thread, &arity);
  if (Arcadia_JumpTarget_save(&arity)) {
    Arcadia_ValueStack_pushNatural64Value(thread, 123);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    _Arcadia_EndCreate0(thread, _Arcadia_Engine_Node_getType(thread), base);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
    Arcadia_Thread_setStatus(thread, 0);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

int
Arcadia_Engine_Tests_Nodes_node
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
