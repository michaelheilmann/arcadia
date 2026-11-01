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

#define ARCADIA_ENGINE_PRIVATE (1)
#include "Arcadia/Ring1/Include.h"
#include "Arcadia/Engine/Audials/NodeFactory.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Engine/Audials/BackendContext.h"
#include "Arcadia/Engine/Audials/Nodes/SoundSourceNode.h"
#include "Arcadia/Engine/Audials/Nodes/StreamingSoundSourceNode.h"
#include "Arcadia/Engine/NodeFactory.h"

static Arcadia_Value
pdlGet
  (
    Arcadia_Thread* t,
    Arcadia_SizeValue b,
    Arcadia_SizeValue i
  )
{
  return Arcadia_ValueStack_getValue(t, Arcadia_ValueStack_getSize(t)-b-i-1);
}

static void
pdlPut
  (
    Arcadia_Thread* t,
    Arcadia_SizeValue b,
    Arcadia_SizeValue i,
    Arcadia_Value v
  )
{
  Arcadia_ValueStack_setValue(t, Arcadia_ValueStack_getSize(t)-b-i-1, &v);
}

static void
pdlRequire
  (
    Arcadia_Thread* t,
    Arcadia_BooleanValue b
  )
{
  if (!b) { Arcadia_Thread_setStatus(t, Arcadia_Status_ArgumentTypeInvalid); Arcadia_Thread_jump(t); }
}

static Arcadia_Value
pdlBoxObjectReference
  (
    Arcadia_ObjectReferenceValue object
  )
{
  return object ? Arcadia_Value_makeObjectReferenceValue(object) : Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
}

static Arcadia_Object*
pdlAsObjectReference
  (
    Arcadia_Thread* thread,
    Arcadia_Value v,
    Arcadia_Type* type
  )
{
  if (Arcadia_Value_isVoidValue(&v)) return NULL;
  pdlRequire(thread, Arcadia_Value_isObjectReferenceValue(&v) && Arcadia_Value_getObjectReferenceValue(&v));
  return (Arcadia_Object*)Arcadia_Value_getObjectReferenceValueChecked(thread, v, type);
}

static void
Arcadia_Engine_Audials_NodeFactory_construct
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < 5; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 4, pdlBoxObjectReference(self));
pdlReturn:;
  pdlPut(thread, base, 0, Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void));
  { Arcadia_Value v = pdlGet(thread, base, 0); pdlRequire(thread, Arcadia_Value_isVoidValue(&v)); }
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return;
}

static Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createSoundSourceNode_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* p0,
    Arcadia_ADL_SampleBufferDefinition* p1
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < 7; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 6, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  pdlPut(thread, base, 5, pdlBoxObjectReference(p1));
  (void)((Arcadia_Engine_Audials_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Audials_BackendContext_getType(thread)));
  (void)((Arcadia_ADL_SampleBufferDefinition*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_ADL_SampleBufferDefinition_getType(thread)));
  extern Arcadia_Engine_Audials_SoundSourceNode* Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeNative(Arcadia_Thread* thread, Arcadia_Engine_Audials_NodeFactory* self, Arcadia_Engine_Audials_BackendContext* p0, Arcadia_ADL_SampleBufferDefinition* p1);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeNative(thread, (Arcadia_Engine_Audials_NodeFactory*)self, (Arcadia_Engine_Audials_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Audials_BackendContext_getType(thread)), (Arcadia_ADL_SampleBufferDefinition*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_ADL_SampleBufferDefinition_getType(thread)))));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)((Arcadia_Engine_Audials_SoundSourceNode*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Engine_Audials_SoundSourceNode_getType(thread)));
  Arcadia_Engine_Audials_SoundSourceNode* result = (Arcadia_Engine_Audials_SoundSourceNode*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Engine_Audials_SoundSourceNode_getType(thread));
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createSoundSourceNode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* p0,
    Arcadia_ADL_SampleBufferDefinition* p1
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value selfValue = pdlBoxObjectReference(self);
  Arcadia_ValueStack_pushValue(thread, &selfValue);
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p1); Arcadia_ValueStack_pushValue(thread, &value); }
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Audials_NodeFactory_getType(thread)));
  Arcadia_Engine_Audials_NodeFactoryDispatch* dispatch = (Arcadia_Engine_Audials_NodeFactoryDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->createSoundSourceNode) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_Engine_Audials_BackendContext_getType(thread)));
  pdlRequire(thread, !p1 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p1, _Arcadia_ADL_SampleBufferDefinition_getType(thread)));
  Arcadia_Engine_Audials_SoundSourceNode* result = dispatch->createSoundSourceNode(thread, (Arcadia_Engine_Audials_NodeFactory*)self, p0, p1);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeFromDefinition_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* p0,
    Arcadia_ADL_SoundSourceNodeDefinition* p1
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < 7; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 6, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  pdlPut(thread, base, 5, pdlBoxObjectReference(p1));
  (void)((Arcadia_Engine_Audials_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Audials_BackendContext_getType(thread)));
  (void)((Arcadia_ADL_SoundSourceNodeDefinition*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_ADL_SoundSourceNodeDefinition_getType(thread)));
  extern Arcadia_Engine_Audials_SoundSourceNode* Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeFromDefinitionNative(Arcadia_Thread* thread, Arcadia_Engine_Audials_NodeFactory* self, Arcadia_Engine_Audials_BackendContext* p0, Arcadia_ADL_SoundSourceNodeDefinition* p1);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeFromDefinitionNative(thread, (Arcadia_Engine_Audials_NodeFactory*)self, (Arcadia_Engine_Audials_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Audials_BackendContext_getType(thread)), (Arcadia_ADL_SoundSourceNodeDefinition*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_ADL_SoundSourceNodeDefinition_getType(thread)))));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)((Arcadia_Engine_Audials_SoundSourceNode*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Engine_Audials_SoundSourceNode_getType(thread)));
  Arcadia_Engine_Audials_SoundSourceNode* result = (Arcadia_Engine_Audials_SoundSourceNode*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Engine_Audials_SoundSourceNode_getType(thread));
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

Arcadia_Engine_Audials_SoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* p0,
    Arcadia_ADL_SoundSourceNodeDefinition* p1
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value selfValue = pdlBoxObjectReference(self);
  Arcadia_ValueStack_pushValue(thread, &selfValue);
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p1); Arcadia_ValueStack_pushValue(thread, &value); }
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Audials_NodeFactory_getType(thread)));
  Arcadia_Engine_Audials_NodeFactoryDispatch* dispatch = (Arcadia_Engine_Audials_NodeFactoryDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->createSoundSourceNodeFromDefinition) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_Engine_Audials_BackendContext_getType(thread)));
  pdlRequire(thread, !p1 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p1, _Arcadia_ADL_SoundSourceNodeDefinition_getType(thread)));
  Arcadia_Engine_Audials_SoundSourceNode* result = dispatch->createSoundSourceNodeFromDefinition(thread, (Arcadia_Engine_Audials_NodeFactory*)self, p0, p1);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static Arcadia_Engine_Audials_StreamingSoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinition_impl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* p0,
    Arcadia_ADL_StreamingSoundSourceNodeDefinition* p1
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
    Arcadia_Thread_jump(thread);
  }
  for (Arcadia_SizeValue i = 0; i < 7; ++i) Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  pdlPut(thread, base, 6, pdlBoxObjectReference(self));
  pdlPut(thread, base, 4, pdlBoxObjectReference(p0));
  pdlPut(thread, base, 5, pdlBoxObjectReference(p1));
  (void)((Arcadia_Engine_Audials_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Audials_BackendContext_getType(thread)));
  (void)((Arcadia_ADL_StreamingSoundSourceNodeDefinition*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_ADL_StreamingSoundSourceNodeDefinition_getType(thread)));
  extern Arcadia_Engine_Audials_StreamingSoundSourceNode* Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinitionNative(Arcadia_Thread* thread, Arcadia_Engine_Audials_NodeFactory* self, Arcadia_Engine_Audials_BackendContext* p0, Arcadia_ADL_StreamingSoundSourceNodeDefinition* p1);
  pdlPut(thread, base, 0, pdlBoxObjectReference(Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinitionNative(thread, (Arcadia_Engine_Audials_NodeFactory*)self, (Arcadia_Engine_Audials_BackendContext*)pdlAsObjectReference(thread, pdlGet(thread, base, 4), _Arcadia_Engine_Audials_BackendContext_getType(thread)), (Arcadia_ADL_StreamingSoundSourceNodeDefinition*)pdlAsObjectReference(thread, pdlGet(thread, base, 5), _Arcadia_ADL_StreamingSoundSourceNodeDefinition_getType(thread)))));
  goto pdlReturn;
  Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread);
pdlReturn:;
  (void)((Arcadia_Engine_Audials_StreamingSoundSourceNode*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Engine_Audials_StreamingSoundSourceNode_getType(thread)));
  Arcadia_Engine_Audials_StreamingSoundSourceNode* result = (Arcadia_Engine_Audials_StreamingSoundSourceNode*)pdlAsObjectReference(thread, pdlGet(thread, base, 0), _Arcadia_Engine_Audials_StreamingSoundSourceNode_getType(thread));
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread)-base);
  return result;
}

Arcadia_Engine_Audials_StreamingSoundSourceNode*
Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinition
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self,
    Arcadia_Engine_Audials_BackendContext* p0,
    Arcadia_ADL_StreamingSoundSourceNodeDefinition* p1
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Value selfValue = pdlBoxObjectReference(self);
  Arcadia_ValueStack_pushValue(thread, &selfValue);
  { Arcadia_Value value = pdlBoxObjectReference(p0); Arcadia_ValueStack_pushValue(thread, &value); }
  { Arcadia_Value value = pdlBoxObjectReference(p1); Arcadia_ValueStack_pushValue(thread, &value); }
  pdlRequire(thread, self && Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)self, _Arcadia_Engine_Audials_NodeFactory_getType(thread)));
  Arcadia_Engine_Audials_NodeFactoryDispatch* dispatch = (Arcadia_Engine_Audials_NodeFactoryDispatch*)Arcadia_ObjectType_getDispatch(Arcadia_Object_getType(thread, (Arcadia_Object*)self));
  if (!dispatch->createStreamingSoundSourceNodeFromDefinition) { Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationInvalid); Arcadia_Thread_jump(thread); }
  pdlRequire(thread, !p0 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p0, _Arcadia_Engine_Audials_BackendContext_getType(thread)));
  pdlRequire(thread, !p1 || Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)p1, _Arcadia_ADL_StreamingSoundSourceNodeDefinition_getType(thread)));
  Arcadia_Engine_Audials_StreamingSoundSourceNode* result = dispatch->createStreamingSoundSourceNodeFromDefinition(thread, (Arcadia_Engine_Audials_NodeFactory*)self, p0, p1);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Audials_NodeFactory);
  if (_numberOfArguments != 0) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); }
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_Object_setType(thread, (Arcadia_Object*)self, _type);
  { Arcadia_Value args[1];
    Arcadia_Engine_Audials_NodeFactory_construct(thread, self);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Audials_NodeFactory);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactory* self
  )
{
}

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Audials_NodeFactoryDispatch* self
  )
{
  ((Arcadia_Engine_Audials_NodeFactoryDispatch*)self)->createSoundSourceNode = Arcadia_Engine_Audials_NodeFactory_createSoundSourceNode_impl;
  ((Arcadia_Engine_Audials_NodeFactoryDispatch*)self)->createSoundSourceNodeFromDefinition = Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeFromDefinition_impl;
  ((Arcadia_Engine_Audials_NodeFactoryDispatch*)self)->createStreamingSoundSourceNodeFromDefinition = Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinition_impl;
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)initializeDispatchImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Audials.NodeFactory", Arcadia_Engine_Audials_NodeFactory,
                         u8"Arcadia.Engine.NodeFactory", Arcadia_Engine_NodeFactory,
                         &typeOperations);

Arcadia_Engine_Audials_NodeFactory*
Arcadia_Engine_Audials_NodeFactory_create
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (!Arcadia_JumpTarget_save(&jump)) {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_SizeValue argumentsBase = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_Engine_Audials_NodeFactory* result = _Arcadia_EndCreate0(thread, _Arcadia_Engine_Audials_NodeFactory_getType(thread), argumentsBase);
  Arcadia_Thread_popJumpTarget(thread);
  Arcadia_ValueStack_popValues(thread, Arcadia_ValueStack_getSize(thread) - base);
  return result;
}
