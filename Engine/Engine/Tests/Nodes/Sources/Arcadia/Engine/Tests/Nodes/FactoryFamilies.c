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

#include "Arcadia/Engine/Include.h"
#include "Arcadia/ADL/Include.h"
#include "Arcadia/Media/Include.h"
#include <stddef.h>

#define A Arcadia_Engine_Audials_NodeFactory
#define V Arcadia_Engine_Visuals_NodeFactory
#define AC Arcadia_Engine_Audials_BackendContext
#define VC Arcadia_Engine_Visuals_BackendContext
#define AP(Name) Arcadia_Engine_Audials_##Name
#define VP(Name) Arcadia_Engine_Visuals_##Name

struct AudialsFactoryABI {
  Arcadia_Engine_NodeFactoryDispatch parent;
  AP(SoundSourceNode)* (*createSoundSourceNode)(Arcadia_Thread*, A*, AC*, Arcadia_ADL_SampleBufferDefinition*);
  AP(SoundSourceNode)* (*createSoundSourceNodeFromDefinition)(Arcadia_Thread*, A*, AC*, Arcadia_ADL_SoundSourceNodeDefinition*);
  AP(StreamingSoundSourceNode)* (*createStreamingSoundSourceNodeFromDefinition)(Arcadia_Thread*, A*, AC*, Arcadia_ADL_StreamingSoundSourceNodeDefinition*);
};

struct VisualsFactoryABI {
  Arcadia_Engine_NodeFactoryDispatch parent;
  VP(CameraNode)* (*createCameraNode)(Arcadia_Thread*, V*, VC*);
  VP(FrameBufferNode)* (*createFrameBufferNode)(Arcadia_Thread*, V*, VC*);
  VP(EnterPassNode)* (*createEnterPassNode)(Arcadia_Thread*, V*, VC*);
  VP(MaterialNode)* (*createMaterialNode)(Arcadia_Thread*, V*, VC*, Arcadia_ADL_MaterialDefinition*);
  VP(MeshNode)* (*createMeshNode)(Arcadia_Thread*, V*, VC*, Arcadia_Media_Geometry*);
  VP(ModelNode)* (*createModelNode)(Arcadia_Thread*, V*, VC*, Arcadia_ADL_ModelDefinition*);
  VP(PixelBufferNode)* (*createPixelBufferNode)(Arcadia_Thread*, V*, VC*, Arcadia_ADL_PixelBufferDefinition*);
  VP(TextureNode)* (*createTextureNode)(Arcadia_Thread*, V*, VC*, Arcadia_ADL_TextureDefinition*);
  VP(ViewportNode)* (*createViewportNode)(Arcadia_Thread*, V*, VC*);
};

_Static_assert(sizeof(A) == sizeof(Arcadia_Engine_NodeFactory), "stateless audials factory");
_Static_assert(sizeof(V) == sizeof(Arcadia_Engine_NodeFactory), "stateless visuals factory");
_Static_assert(_Alignof(A) == _Alignof(Arcadia_Engine_NodeFactory), "audials alignment");
_Static_assert(_Alignof(V) == _Alignof(Arcadia_Engine_NodeFactory), "visuals alignment");
_Static_assert(sizeof(Arcadia_Engine_Audials_NodeFactoryDispatch) == sizeof(struct AudialsFactoryABI), "three slots");
_Static_assert(sizeof(Arcadia_Engine_Visuals_NodeFactoryDispatch) == sizeof(struct VisualsFactoryABI), "nine slots");
#define SLOT(Family, ABI, Name, ...) \
  _Static_assert(offsetof(Family##Dispatch, Name) == offsetof(struct ABI, Name), #Name " offset"); \
  _Static_assert(_Generic(((Family##Dispatch*)0)->Name, __VA_ARGS__: 1, default: 0), #Name " slot signature"); \
  _Static_assert(_Generic(&Family##_##Name, __VA_ARGS__: 1, default: 0), #Name " public signature");
SLOT(Arcadia_Engine_Audials_NodeFactory, AudialsFactoryABI, createSoundSourceNode, AP(SoundSourceNode)* (*)(Arcadia_Thread*, A*, AC*, Arcadia_ADL_SampleBufferDefinition*))
SLOT(Arcadia_Engine_Audials_NodeFactory, AudialsFactoryABI, createSoundSourceNodeFromDefinition, AP(SoundSourceNode)* (*)(Arcadia_Thread*, A*, AC*, Arcadia_ADL_SoundSourceNodeDefinition*))
SLOT(Arcadia_Engine_Audials_NodeFactory, AudialsFactoryABI, createStreamingSoundSourceNodeFromDefinition, AP(StreamingSoundSourceNode)* (*)(Arcadia_Thread*, A*, AC*, Arcadia_ADL_StreamingSoundSourceNodeDefinition*))
SLOT(Arcadia_Engine_Visuals_NodeFactory, VisualsFactoryABI, createCameraNode, VP(CameraNode)* (*)(Arcadia_Thread*, V*, VC*))
SLOT(Arcadia_Engine_Visuals_NodeFactory, VisualsFactoryABI, createFrameBufferNode, VP(FrameBufferNode)* (*)(Arcadia_Thread*, V*, VC*))
SLOT(Arcadia_Engine_Visuals_NodeFactory, VisualsFactoryABI, createEnterPassNode, VP(EnterPassNode)* (*)(Arcadia_Thread*, V*, VC*))
SLOT(Arcadia_Engine_Visuals_NodeFactory, VisualsFactoryABI, createMaterialNode, VP(MaterialNode)* (*)(Arcadia_Thread*, V*, VC*, Arcadia_ADL_MaterialDefinition*))
SLOT(Arcadia_Engine_Visuals_NodeFactory, VisualsFactoryABI, createMeshNode, VP(MeshNode)* (*)(Arcadia_Thread*, V*, VC*, Arcadia_Media_Geometry*))
SLOT(Arcadia_Engine_Visuals_NodeFactory, VisualsFactoryABI, createModelNode, VP(ModelNode)* (*)(Arcadia_Thread*, V*, VC*, Arcadia_ADL_ModelDefinition*))
SLOT(Arcadia_Engine_Visuals_NodeFactory, VisualsFactoryABI, createPixelBufferNode, VP(PixelBufferNode)* (*)(Arcadia_Thread*, V*, VC*, Arcadia_ADL_PixelBufferDefinition*))
SLOT(Arcadia_Engine_Visuals_NodeFactory, VisualsFactoryABI, createTextureNode, VP(TextureNode)* (*)(Arcadia_Thread*, V*, VC*, Arcadia_ADL_TextureDefinition*))
SLOT(Arcadia_Engine_Visuals_NodeFactory, VisualsFactoryABI, createViewportNode, VP(ViewportNode)* (*)(Arcadia_Thread*, V*, VC*))
#undef SLOT

static Arcadia_SizeValue finalized[3];
static Arcadia_Object* expectedSelf;
static Arcadia_Object* expectedSource;
static unsigned calls;
static unsigned lastSlot;
static Arcadia_BooleanValue failCall;

static void
onDestroy
  (
    void* context,
    Arcadia_Object* object
  )
{ ++*(Arcadia_SizeValue*)context; }

static void
checkCall
  (
    Arcadia_Thread* thread,
    Arcadia_Object* self,
    Arcadia_Object* context,
    Arcadia_Object* source,
    unsigned slot
  )
{
  ++calls;
  lastSlot = slot;
  Arcadia_Tests_assertTrue(thread, self == expectedSelf && !context && source == expectedSource);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, !finalized[0] && !finalized[1]);
  if (failCall) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
}

// Native subclasses override every slot with the exact declaring-class ABI.
#define OVERRIDE1(Name, Result, Id) \
  static VP(Result)* \
  Name \
    ( \
      Arcadia_Thread* t, \
      V* s, \
      VC* c \
    ) \
  { \
    checkCall(t, (void*)s, (void*)c, NULL, Id); \
    return NULL; \
  }
#define OVERRIDE2(Prefix, Factory, Context, Name, Result, Source, Id) \
  static Prefix##Result* \
  Name \
    ( \
      Arcadia_Thread* t, \
      Factory* s, \
      Context* c, \
      Source* d \
    ) \
  { \
    checkCall(t, (void*)s, (void*)c, (void*)d, Id); \
    return NULL; \
  }
OVERRIDE2(Arcadia_Engine_Audials_, A, AC, a0, SoundSourceNode, Arcadia_ADL_SampleBufferDefinition, 0)
OVERRIDE2(Arcadia_Engine_Audials_, A, AC, a1, SoundSourceNode, Arcadia_ADL_SoundSourceNodeDefinition, 1)
OVERRIDE2(Arcadia_Engine_Audials_, A, AC, a2, StreamingSoundSourceNode, Arcadia_ADL_StreamingSoundSourceNodeDefinition, 2)
OVERRIDE1(v0, CameraNode, 3)
OVERRIDE1(v1, FrameBufferNode, 4)
OVERRIDE1(v2, EnterPassNode, 5)
OVERRIDE2(Arcadia_Engine_Visuals_, V, VC, v3, MaterialNode, Arcadia_ADL_MaterialDefinition, 6)
OVERRIDE2(Arcadia_Engine_Visuals_, V, VC, v4, MeshNode, Arcadia_Media_Geometry, 7)
OVERRIDE2(Arcadia_Engine_Visuals_, V, VC, v5, ModelNode, Arcadia_ADL_ModelDefinition, 8)
OVERRIDE2(Arcadia_Engine_Visuals_, V, VC, v6, PixelBufferNode, Arcadia_ADL_PixelBufferDefinition, 9)
OVERRIDE2(Arcadia_Engine_Visuals_, V, VC, v7, TextureNode, Arcadia_ADL_TextureDefinition, 10)
OVERRIDE1(v8, ViewportNode, 11)
#undef OVERRIDE1
#undef OVERRIDE2

typedef struct AudialsFactoryChild AudialsFactoryChild;
typedef struct AudialsFactoryChildDispatch AudialsFactoryChildDispatch;
typedef struct VisualsFactoryChild VisualsFactoryChild;
typedef struct VisualsFactoryChildDispatch VisualsFactoryChildDispatch;

struct AudialsFactoryChild {
  A _parent;
};

struct AudialsFactoryChildDispatch {
  Arcadia_Engine_Audials_NodeFactoryDispatch _parent;
};

struct VisualsFactoryChild {
  V _parent;
};

struct VisualsFactoryChildDispatch {
  Arcadia_Engine_Visuals_NodeFactoryDispatch _parent;
};

Arcadia_declareObjectType(u8"Tests.AudialsFactoryChild", AudialsFactoryChild,
                         u8"Arcadia.Engine.Audials.NodeFactory");
Arcadia_declareObjectType(u8"Tests.VisualsFactoryChild", VisualsFactoryChild,
                         u8"Arcadia.Engine.Visuals.NodeFactory");

#define CONSTRUCTOR(Type) \
  static void \
  Type##_construct \
    ( \
      Arcadia_Thread* thread, \
      Type* self \
    ) \
  { \
    Arcadia_EnterConstructor(Type); \
    Arcadia_ValueStack_pushNatural8Value(thread, 0); \
    Arcadia_superTypeConstructor(thread, _type, self); \
    Arcadia_LeaveConstructor(Type); \
  }
CONSTRUCTOR(AudialsFactoryChild)
CONSTRUCTOR(VisualsFactoryChild)
#undef CONSTRUCTOR

static void
audialsDispatch
  (
    Arcadia_Thread* thread,
    AudialsFactoryChildDispatch* dispatch
  )
{
  Arcadia_Engine_Audials_NodeFactoryDispatch* d = (void*)dispatch;
  d->createSoundSourceNode = &a0;
  d->createSoundSourceNodeFromDefinition = &a1;
  d->createStreamingSoundSourceNodeFromDefinition = &a2;
}

static void
visualsDispatch
  (
    Arcadia_Thread* thread,
    VisualsFactoryChildDispatch* dispatch
  )
{
  Arcadia_Engine_Visuals_NodeFactoryDispatch* d = (void*)dispatch;
  d->createCameraNode = &v0;
  d->createFrameBufferNode = &v1;
  d->createEnterPassNode = &v2;
  d->createMaterialNode = &v3;
  d->createMeshNode = &v4;
  d->createModelNode = &v5;
  d->createPixelBufferNode = &v6;
  d->createTextureNode = &v7;
  d->createViewportNode = &v8;
}

#define CHILD(Type, Parent, Dispatch) \
  static const Arcadia_ObjectType_Operations Type##_ops = { \
    Arcadia_ObjectType_Operations_Initializer, \
    .construct = (Arcadia_Object_ConstructCallbackFunction*)&Type##_construct, \
    .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Dispatch, \
  }; \
  static const Arcadia_Type_Operations Type##_typeOps = { \
    Arcadia_Type_Operations_Initializer, .objectTypeOperations = &Type##_ops, \
  }; \
  Arcadia_defineObjectType(u8"Tests." #Type, Type, u8"Arcadia.Engine." #Parent ".NodeFactory", Arcadia_Engine_##Parent##_NodeFactory, &Type##_typeOps);
CHILD(AudialsFactoryChild, Audials, audialsDispatch)
CHILD(VisualsFactoryChild, Visuals, visualsDispatch)
#undef CHILD

static Arcadia_Object*
invokeSlot
  (
    Arcadia_Thread* thread,
    unsigned slot,
    Arcadia_Object* self,
    Arcadia_Object* source
  )
{
  switch (slot) {
    case 0: return (void*)Arcadia_Engine_Audials_NodeFactory_createSoundSourceNode(thread, (void*)self, NULL, (void*)source);
    case 1: return (void*)Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeFromDefinition(thread, (void*)self, NULL, (void*)source);
    case 2: return (void*)Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinition(thread, (void*)self, NULL, (void*)source);
    case 3: return (void*)Arcadia_Engine_Visuals_NodeFactory_createCameraNode(thread, (void*)self, NULL);
    case 4: return (void*)Arcadia_Engine_Visuals_NodeFactory_createFrameBufferNode(thread, (void*)self, NULL);
    case 5: return (void*)Arcadia_Engine_Visuals_NodeFactory_createEnterPassNode(thread, (void*)self, NULL);
    case 6: return (void*)Arcadia_Engine_Visuals_NodeFactory_createMaterialNode(thread, (void*)self, NULL, (void*)source);
    case 7: return (void*)Arcadia_Engine_Visuals_NodeFactory_createMeshNode(thread, (void*)self, NULL, (void*)source);
    case 8: return (void*)Arcadia_Engine_Visuals_NodeFactory_createModelNode(thread, (void*)self, NULL, (void*)source);
    case 9: return (void*)Arcadia_Engine_Visuals_NodeFactory_createPixelBufferNode(thread, (void*)self, NULL, (void*)source);
    case 10: return (void*)Arcadia_Engine_Visuals_NodeFactory_createTextureNode(thread, (void*)self, NULL, (void*)source);
    default: return (void*)Arcadia_Engine_Visuals_NodeFactory_createViewportNode(thread, (void*)self, NULL);
  }
}

static void
testOverrides
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 73);
  for (unsigned slot = 0; slot < 12; ++slot) {
    for (unsigned failure = 0; failure < 2; ++failure) {
      Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
      Arcadia_ValueStack_pushNatural8Value(thread, 0);
      expectedSelf = _Arcadia_EndCreate0(thread, slot < 3 ? _AudialsFactoryChild_getType(thread) : _VisualsFactoryChild_getType(thread), depth);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, expectedSelf);
      // Every managed signature also permits NULL; actual definitions are tested below.
      expectedSource = NULL;
      finalized[0] = finalized[1] = 0;
      Arcadia_Object_addNotifyDestroyCallback(thread, expectedSelf, &finalized[0], &onDestroy);
      calls = 0; failCall = failure != 0;
      Arcadia_ValueStack_popValues(thread, 1);
      Arcadia_SizeValue callDepth = Arcadia_ValueStack_getSize(thread);
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        Arcadia_Object* result = invokeSlot(thread, slot, expectedSelf, NULL);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, !failure && !result);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, failure && Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed);
        Arcadia_Thread_setStatus(thread, 0);
      }
      Arcadia_Tests_assertTrue(thread, calls == 1 && lastSlot == slot && Arcadia_ValueStack_getSize(thread) == callDepth);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      Arcadia_Tests_assertTrue(thread, finalized[0] == 1);
    }
  }
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testDefaults
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  A* a = Arcadia_Engine_Audials_NodeFactory_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, a);
  V* v = Arcadia_Engine_Visuals_NodeFactory_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, v);
  Arcadia_Engine_Audials_NodeFactoryDispatch* ad = (void*)Arcadia_ObjectType_getDispatch(_Arcadia_Engine_Audials_NodeFactory_getType(thread));
  Arcadia_Engine_Visuals_NodeFactoryDispatch* vd = (void*)Arcadia_ObjectType_getDispatch(_Arcadia_Engine_Visuals_NodeFactory_getType(thread));
  Arcadia_Tests_assertTrue(thread, ad->createSoundSourceNode && ad->createSoundSourceNodeFromDefinition && ad->createStreamingSoundSourceNodeFromDefinition);
  Arcadia_Tests_assertTrue(thread, vd->createCameraNode && vd->createFrameBufferNode && vd->createEnterPassNode && vd->createMaterialNode && vd->createMeshNode && vd->createModelNode && vd->createPixelBufferNode && vd->createTextureNode && vd->createViewportNode);
  Arcadia_String* wrong = Arcadia_String_createFromCxxString(thread, "not a definition");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, wrong);
  for (unsigned slot = 0; slot < 12; ++slot) {
    if (slot == 1 || slot == 2) continue; // These native algorithms dereference their required definition.
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Object* result = invokeSlot(thread, slot, slot < 3 ? (void*)a : (void*)v, (void*)wrong);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, slot == 3 || slot == 4 || slot == 5 || slot == 11);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, result);
      finalized[2] = 0;
      Arcadia_Object_addNotifyDestroyCallback(thread, result, &finalized[2], &onDestroy);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false) && !finalized[2]);
      Arcadia_ValueStack_popValues(thread, 1);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false) && finalized[2] == 1);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, slot != 3 && slot != 4 && slot != 5 && slot != 11);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid);
      Arcadia_Thread_setStatus(thread, 0);
    }
    Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
  }
  Arcadia_ValueStack_popValues(thread, 3);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testConstructors
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 73);
  Arcadia_Value bad[] = {
    Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void), Arcadia_Value_makeBooleanValue(false),
    Arcadia_Value_makeInteger8Value(0), Arcadia_Value_makeInteger16Value(0),
    Arcadia_Value_makeInteger32Value(0), Arcadia_Value_makeInteger64Value(0),
    Arcadia_Value_makeNatural16Value(0), Arcadia_Value_makeNatural32Value(0),
    Arcadia_Value_makeNatural64Value(0), Arcadia_Value_makeSizeValue(0),
    Arcadia_Value_makeReal32Value(0.), Arcadia_Value_makeReal64Value(0.),
    Arcadia_Value_makeTypeValue(_Arcadia_Engine_NodeFactory_getType(thread)),
  };
  for (unsigned family = 0; family < 2; ++family) {
    Arcadia_TypeValue type = family ? _Arcadia_Engine_Visuals_NodeFactory_getType(thread) : _Arcadia_Engine_Audials_NodeFactory_getType(thread);
    for (unsigned i = 0; i < 255 + sizeof(bad) / sizeof(bad[0]); ++i) {
      Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        if (i < 255) {
          for (unsigned j = 0; j <= i; ++j) Arcadia_ValueStack_pushBooleanValue(thread, false);
          Arcadia_ValueStack_pushNatural8Value(thread, (Arcadia_Natural8Value)(i + 1));
        } else {
          Arcadia_ValueStack_pushValue(thread, &bad[i - 255]);
        }
        _Arcadia_EndCreate0(thread, type, depth);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (i < 255 ? Arcadia_Status_NumberOfArgumentsInvalid : Arcadia_Status_ArgumentTypeInvalid));
        Arcadia_Thread_setStatus(thread, 0);
        Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      }
    }
  }
  Arcadia_ValueStack_popValues(thread, 300);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testAudialsDefinitions
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, definitions);
  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, "Tests.Factory");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
  Arcadia_ADL_Reference* reference = Arcadia_ADL_Reference_create(thread, definitions, name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, reference);
  Arcadia_ADL_SampleBufferDefinition* sample = Arcadia_ADL_SampleBufferDefinition_create(thread, definitions, name, 0, reference);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, sample);
  reference->definition = (void*)sample;
  Arcadia_ADL_SoundSourceNodeDefinition* source = Arcadia_ADL_SoundSourceNodeDefinition_create(thread, definitions, name, reference, true, .25f);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, source);
  A* factory = Arcadia_Engine_Audials_NodeFactory_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, factory);
  AP(SoundSourceNode)* plain = Arcadia_Engine_Audials_NodeFactory_createSoundSourceNode(thread, factory, NULL, sample);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, plain);
  Arcadia_Tests_assertTrue(thread, plain->source == sample && !plain->backendContext && !plain->isLooping && plain->volume == 1.f);
  AP(SoundSourceNode)* fromDefinition = Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeFromDefinition(thread, factory, NULL, source);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, fromDefinition);
  Arcadia_Tests_assertTrue(thread, fromDefinition->source == sample && !fromDefinition->backendContext && fromDefinition->isLooping && fromDefinition->volume == .25f);
  source->isLooping = false; source->volume = .75f;
  AP(SoundSourceNode)* second = Arcadia_Engine_Audials_NodeFactory_createSoundSourceNodeFromDefinition(thread, factory, NULL, source);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, second);
  Arcadia_Tests_assertTrue(thread, !second->isLooping && second->volume == .75f && fromDefinition->isLooping && fromDefinition->volume == .25f);
  Arcadia_ADL_StreamingSoundSourceNodeDefinition* streaming = Arcadia_ADL_StreamingSoundSourceNodeDefinition_create(thread, definitions, name, reference, .5f);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, streaming);
  for (unsigned slot = 1; slot <= 2; ++slot) {
    // Both creation algorithms retain their explicit rejection of a resolved
    // reference with the wrong definition kind, before allocating a child.
    reference->definition = slot == 1 ? (void*)streaming : (void*)sample;
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      invokeSlot(thread, slot, (void*)factory, slot == 1 ? (void*)source : (void*)streaming);
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_ArgumentTypeInvalid && Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  finalized[0] = finalized[1] = 0;
  Arcadia_Object_addNotifyDestroyCallback(thread, (void*)sample, &finalized[0], &onDestroy);
  Arcadia_Object_addNotifyDestroyCallback(thread, (void*)plain, &finalized[1], &onDestroy);
  Arcadia_Value value = Arcadia_Value_makeObjectReferenceValue(plain);
  Arcadia_ValueStack_popValues(thread, 10);
  Arcadia_ValueStack_pushValue(thread, &value);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false) && !finalized[0] && !finalized[1]);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false) && finalized[0] == 1 && finalized[1] == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

static void
testWrapperSafety
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 73);
  A* a = Arcadia_Engine_Audials_NodeFactory_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, a);
  V* v = Arcadia_Engine_Visuals_NodeFactory_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, v);
  Arcadia_String* wrong = Arcadia_String_createFromCxxString(thread, "wrong factory or argument");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, wrong);
  Arcadia_Engine_Audials_NodeFactoryDispatch* ad = (void*)Arcadia_ObjectType_getDispatch(_Arcadia_Engine_Audials_NodeFactory_getType(thread));
  Arcadia_Engine_Visuals_NodeFactoryDispatch* vd = (void*)Arcadia_ObjectType_getDispatch(_Arcadia_Engine_Visuals_NodeFactory_getType(thread));
  Arcadia_Engine_Audials_NodeFactoryDispatch savedA = *ad;
  Arcadia_Engine_Visuals_NodeFactoryDispatch savedV = *vd;
  ad->createSoundSourceNode = NULL;
  ad->createSoundSourceNodeFromDefinition = NULL;
  ad->createStreamingSoundSourceNodeFromDefinition = NULL;
  vd->createCameraNode = NULL;
  vd->createFrameBufferNode = NULL;
  vd->createEnterPassNode = NULL;
  vd->createMaterialNode = NULL;
  vd->createMeshNode = NULL;
  vd->createModelNode = NULL;
  vd->createPixelBufferNode = NULL;
  vd->createTextureNode = NULL;
  vd->createViewportNode = NULL;
  for (unsigned slot = 0; slot < 12; ++slot) {
    for (unsigned error = 0; error < 3; ++error) {
      Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        invokeSlot(thread, slot, error == 0 ? NULL : error == 1 ? (void*)wrong : slot < 3 ? (void*)a : (void*)v, NULL);
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (error < 2 ? Arcadia_Status_ArgumentTypeInvalid : Arcadia_Status_OperationInvalid));
        Arcadia_Thread_setStatus(thread, 0);
        Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == depth);
      }
    }
  }
  *ad = savedA;
  *vd = savedV;
  Arcadia_ValueStack_popValues(thread, 303);
  // The receiver and definition have no caller roots while a raw native
  // override collects. The definition retains its native ADL dependency graph.
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, definitions);
  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, "Tests.RootedFactoryArgument");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
  Arcadia_ADL_Reference* ref = Arcadia_ADL_Reference_create(thread, definitions, name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, ref);
  Arcadia_ADL_SampleBufferDefinition* source = Arcadia_ADL_SampleBufferDefinition_create(thread, definitions, name, 0, ref);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, source);
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  expectedSelf = _Arcadia_EndCreate0(thread, _AudialsFactoryChild_getType(thread), depth);
  expectedSource = (void*)source;
  finalized[0] = finalized[1] = 0;
  Arcadia_Object_addNotifyDestroyCallback(thread, expectedSelf, &finalized[0], &onDestroy);
  Arcadia_Object_addNotifyDestroyCallback(thread, expectedSource, &finalized[1], &onDestroy);
  Arcadia_ValueStack_popValues(thread, 4);
  calls = 0; failCall = false;
  Arcadia_Tests_assertTrue(thread, !invokeSlot(thread, 0, expectedSelf, expectedSource) && calls == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false) && finalized[0] == 1 && finalized[1] == 1);
}

static void
testStreamingCreation
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_ADL_Definitions* definitions = Arcadia_ADL_Definitions_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, definitions);
  Arcadia_String* name = Arcadia_String_createFromCxxString(thread, "Tests.Factory.Stream");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, name);
  Arcadia_ADL_SineWaveDefinition* sine = Arcadia_ADL_SineWaveDefinition_create(thread, definitions, name, 220);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, sine);
  Arcadia_ADL_Reference* dsp = Arcadia_ADL_Reference_create(thread, definitions, name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, dsp);
  dsp->definition = (void*)sine;
  Arcadia_ADL_DSPStreamProviderDefinition* providerDefinition = Arcadia_ADL_DSPStreamProviderDefinition_create(thread, definitions, name, dsp, 8000, true, 2);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, providerDefinition);
  Arcadia_ADL_Reference* providerReference = Arcadia_ADL_Reference_create(thread, definitions, name);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, providerReference);
  providerReference->definition = (void*)providerDefinition;
  Arcadia_ADL_StreamingSoundSourceNodeDefinition* definition = Arcadia_ADL_StreamingSoundSourceNodeDefinition_create(thread, definitions, name, providerReference, .375f);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, definition);
  A* factory = Arcadia_Engine_Audials_NodeFactory_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, factory);
  for (unsigned infinite = 0; infinite < 2; ++infinite) {
    providerDefinition->hasDuration = !infinite;
    AP(StreamingSoundSourceNode)* node = Arcadia_Engine_Audials_NodeFactory_createStreamingSoundSourceNodeFromDefinition(thread, factory, NULL, definition);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, node);
    AP(DSPStreamProvider)* provider = (void*)node->provider;
    Arcadia_Tests_assertTrue(thread, !node->backendContext && !node->resource && node->volume == .375f);
    Arcadia_Tests_assertTrue(thread, provider->isInfinite == (infinite != 0));
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Audials_AudioStreamProvider_getSampleRate(thread, (void*)provider) == 8000);
    if (!infinite) Arcadia_Tests_assertTrue(thread, provider->framesRemaining == 16000);
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (void*)provider->dsp, _Arcadia_Media_DSP_SineWave_getType(thread)));
    Arcadia_ValueStack_popValues(thread, 1);
  }
  Arcadia_ValueStack_popValues(thread, 8);
  Arcadia_Tests_assertTrue(thread, Arcadia_ValueStack_getSize(thread) == base);
}

void
Arcadia_Engine_Tests_Nodes_factoryFamilies
  (
    Arcadia_Thread* thread
  )
{
  testDefaults(thread);
  testOverrides(thread);
  testConstructors(thread);
  testAudialsDefinitions(thread);
  testStreamingCreation(thread);
  testWrapperSafety(thread);
}
