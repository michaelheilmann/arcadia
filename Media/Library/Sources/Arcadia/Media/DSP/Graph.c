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

#define ARCADIA_MEDIA_PRIVATE (1)
#include "Arcadia/Media/DSP/Graph.h"

static Arcadia_SizeValue
getNumberOfOutputPins(Arcadia_Thread* thread, Arcadia_Media_DSP_Graph* self);

static Arcadia_SizeValue
getNumberOfInputPins(Arcadia_Thread* thread, Arcadia_Media_DSP_Graph* self);

static void
Arcadia_Media_DSP_Graph_constructImpl(Arcadia_Thread* thread, Arcadia_Media_DSP_Graph* self);

static void
Arcadia_Media_DSP_Graph_initializeDispatchImpl(Arcadia_Thread* thread, Arcadia_Media_DSP_GraphDispatch* self);

static void
Arcadia_Media_DSP_Graph_visitImpl(Arcadia_Thread* thread, Arcadia_Media_DSP_Graph* self);

static void
Arcadia_Media_DSP_Graph_render(Arcadia_Thread* thread, Arcadia_Media_DSP_Graph* self, Arcadia_Media_DSP_Buffer* target);

static void
Arcadia_Media_DSP_Graph_resetImpl(Arcadia_Thread* thread, Arcadia_Media_DSP_Graph* self);

static void
Arcadia_Media_DSP_Graph_reseedImpl(Arcadia_Thread* thread, Arcadia_Media_DSP_Graph* self, Arcadia_Natural32Value seed);

static Arcadia_Natural32Value
deriveNodeSeed(Arcadia_Natural32Value seed, Arcadia_SizeValue index);

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Media_DSP_Graph_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Media_DSP_Graph_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Media_DSP_Graph_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Media.DSP.Graph", Arcadia_Media_DSP_Graph,
                         u8"Arcadia.Media.DSP", Arcadia_Media_DSP,
                         &_typeOperations);

static Arcadia_SizeValue
getNumberOfOutputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self
  )
{ return Arcadia_SizeValue_Literal(1); }

static Arcadia_SizeValue
getNumberOfInputPins
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self
  )
{ return Arcadia_SizeValue_Literal(0); }

static void
Arcadia_Media_DSP_Graph_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Media_DSP_Graph);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->nodes = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  self->output = NULL;
  Arcadia_LeaveConstructor(Arcadia_Media_DSP_Graph);
}

static void
Arcadia_Media_DSP_Graph_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_GraphDispatch* self
  )
{
  ((Arcadia_Media_DSPDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Media_DSP_Buffer*)) & Arcadia_Media_DSP_Graph_render;
  ((Arcadia_Media_DSPDispatch*)self)->reset = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & Arcadia_Media_DSP_Graph_resetImpl;
  ((Arcadia_Media_DSPDispatch*)self)->reseed = (void (*)(Arcadia_Thread*, Arcadia_Media_DSP*, Arcadia_Natural32Value)) & Arcadia_Media_DSP_Graph_reseedImpl;
  ((Arcadia_Media_DSPDispatch*)self)->getNumberOfOutputPins = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & getNumberOfOutputPins;
  ((Arcadia_Media_DSPDispatch*)self)->getNumberOfInputPins = (Arcadia_SizeValue (*)(Arcadia_Thread*, Arcadia_Media_DSP*)) & getNumberOfInputPins;
}

static void
Arcadia_Media_DSP_Graph_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self
  )
{
  if (self->nodes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->nodes);
  }
  if (self->output) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->output);
  }
}

static void
Arcadia_Media_DSP_Graph_render
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self,
    Arcadia_Media_DSP_Buffer* target
  )
{
  if (!self->output) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Media_DSP_render(thread, self->output, target);
}

static void
Arcadia_Media_DSP_Graph_resetImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self
  )
{ Arcadia_Media_DSP_Graph_reset(thread, self); }

static void
Arcadia_Media_DSP_Graph_reseedImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self,
    Arcadia_Natural32Value seed
  )
{ Arcadia_Media_DSP_Graph_reseed(thread, self, seed); }

static Arcadia_Natural32Value
deriveNodeSeed
  (
    Arcadia_Natural32Value seed,
    Arcadia_SizeValue index
  )
{
  Arcadia_Natural32Value value = seed + Arcadia_Natural32Value_Literal(0x9e3779b9) + (Arcadia_Natural32Value)index;
  value ^= value >> 16;
  value *= Arcadia_Natural32Value_Literal(0x7feb352d);
  value ^= value >> 15;
  value *= Arcadia_Natural32Value_Literal(0x846ca68b);
  value ^= value >> 16;
  return value ? value : Arcadia_Natural32Value_Literal(0x6d2b79f5);
}

Arcadia_Media_DSP_Graph*
Arcadia_Media_DSP_Graph_create
  (
    Arcadia_Thread* thread
  )
{
  _Arcadia_BeginCreate(Arcadia_Media_DSP_Graph);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  _Arcadia_EndCreate(Arcadia_Media_DSP_Graph);
}

void
Arcadia_Media_DSP_Graph_addNode
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self,
    Arcadia_Media_DSP* node
  )
{
  if (!node) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if ((Arcadia_Object*)node == (Arcadia_Object*)self || node->ownerGraph || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)node, _Arcadia_Media_DSP_Graph_getType(thread))) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Object_lock(thread, (Arcadia_Object*)node);
  node->ownerGraph = (Arcadia_Object*)self;
  Arcadia_JumpTarget jumpTarget;
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
  if (Arcadia_JumpTarget_save(&jumpTarget)) {
    Arcadia_List_insertBackObjectReferenceValue(thread, self->nodes, (Arcadia_Object*)node);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Object_unlock(thread, (Arcadia_Object*)node);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    node->ownerGraph = NULL;
    Arcadia_Object_unlock(thread, (Arcadia_Object*)node);
    Arcadia_Thread_jump(thread);
  }
}

void
Arcadia_Media_DSP_Graph_setOutput
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self,
    Arcadia_Media_DSP* output
  )
{
  if (!output) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  if (output->ownerGraph != (Arcadia_Object*)self) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_ArgumentValueInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Object_lock(thread, (Arcadia_Object*)output);
  self->output = output;
  Arcadia_Object_unlock(thread, (Arcadia_Object*)output);
}

void
Arcadia_Media_DSP_Graph_reset
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->nodes); i < n; ++i) {
    Arcadia_Media_DSP* node = (Arcadia_Media_DSP*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, self->nodes, i, _Arcadia_Media_DSP_getType(thread));
    _Arcadia_Media_DSP_reset(thread, node);
  }
}

void
Arcadia_Media_DSP_Graph_reseed
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self,
    Arcadia_Natural32Value seed
  )
{
  for (Arcadia_SizeValue i = 0, n = Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->nodes); i < n; ++i) {
    Arcadia_Media_DSP* node = (Arcadia_Media_DSP*)Arcadia_List_getObjectReferenceValueCheckedAt(thread, self->nodes, i, _Arcadia_Media_DSP_getType(thread));
    _Arcadia_Media_DSP_reseed(thread, node, deriveNodeSeed(seed, i));
    _Arcadia_Media_DSP_reset(thread, node);
  }
}
