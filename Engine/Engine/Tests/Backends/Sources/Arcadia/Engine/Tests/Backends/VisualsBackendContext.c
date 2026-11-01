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

typedef struct VisualsContextProbe VisualsContextProbe;
typedef struct VisualsContextProbeDispatch VisualsContextProbeDispatch;

struct VisualsContextProbe {
  Arcadia_Engine_Visuals_BackendContext _parent;
  Arcadia_String* label;
  Arcadia_Object* results[11];
  Arcadia_SizeValue calls[12];
  Arcadia_BooleanValue fail;
  Arcadia_BooleanValue blend;
  Arcadia_Engine_Visuals_BlendFunction source, destination;
  Arcadia_Engine_Visuals_ColorSource ambient;
  // Borrowed captures: these deliberately do not root incoming arguments.
  Arcadia_Media_PixelBuffer* pixel;
  Arcadia_Engine_Visuals_TextureResource* texture;
  Arcadia_Engine_Visuals_ProgramResource* program;
  Arcadia_Engine_Visuals_ConstantBufferResource* constants;
  Arcadia_Engine_Visuals_VertexBufferResource* vertices;
  Arcadia_Engine_Visuals_MaterialResource* material;
  Arcadia_Engine_Visuals_EnterPassResource* constructingPass;
  Arcadia_WeakReference* constructionFields[3];
  Arcadia_WeakReference* constructionReceiver;
  Arcadia_BooleanValue failConstruction;
};

struct VisualsContextProbeDispatch { Arcadia_Engine_Visuals_BackendContextDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.VisualsContextProbe", VisualsContextProbe,
                          u8"Arcadia.Engine.Visuals.BackendContext");

static Arcadia_SizeValue destroyed;

static Arcadia_Object*
create
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_Object* context
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  if (context) Arcadia_ValueStack_pushObjectReferenceValue(thread, context);
  Arcadia_ValueStack_pushNatural8Value(thread, context ? 1 : 0);
  return _Arcadia_EndCreate0(thread, type, base);
}

static void
constructImpl
  (
    Arcadia_Thread* thread,
    VisualsContextProbe* self
  )
{
  Arcadia_EnterConstructor(VisualsContextProbe);
  Arcadia_ValueStack_pushNatural8Value(thread, 0);
  Arcadia_superTypeConstructor(thread, _type, self);
  for (size_t i = 0; i < 11; ++i) self->results[i] = NULL;
  for (size_t i = 0; i < 12; ++i) self->calls[i] = 0;
  self->fail = Arcadia_BooleanValue_False;
  self->constructingPass = NULL;
  self->constructionReceiver = NULL;
  self->failConstruction = false;
  for (size_t i = 0; i < 3; ++i) self->constructionFields[i] = NULL;
  self->label = Arcadia_String_createFromCxxString(thread, "visuals context payload");
  Arcadia_LeaveConstructor(VisualsContextProbe);
}

static void
visitImpl
  (
    Arcadia_Thread* thread,
    VisualsContextProbe* self
  )
{
  if (self->label) Arcadia_Object_visit(thread, (Arcadia_Object*)self->label);
  for (size_t i = 0; i < 11; ++i) if (self->results[i]) Arcadia_Object_visit(thread, self->results[i]);
  for (size_t i = 0; i < 3; ++i) if (self->constructionFields[i]) Arcadia_Object_visit(thread, (Arcadia_Object*)self->constructionFields[i]);
  if (self->constructionReceiver) Arcadia_Object_visit(thread, (Arcadia_Object*)self->constructionReceiver);
}

static void
destructImpl
  (
    Arcadia_Thread* thread,
    VisualsContextProbe* self
  )
{ destroyed++; }

static Arcadia_Object*
result
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* receiver,
    Arcadia_SizeValue operation
  )
{
  VisualsContextProbe* self = (void*)receiver;
  if (self->fail) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, self->label, "visuals context payload", sizeof("visuals context payload") - 1));
  Arcadia_Tests_assertTrue(thread, receiver->windows && Arcadia_Object_isInstanceOf(thread, (void*)receiver->windows, _Arcadia_ArrayList_getType(thread)));
  self->calls[operation]++;
  return operation < 11 ? self->results[operation] : NULL;
}

static Arcadia_Engine_Visuals_Icon*
createIcon
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* receiver,
    Arcadia_Media_PixelBuffer* pixel
  )
{
  Arcadia_Object* value = result(thread, receiver, 0);
  if (pixel) Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBuffer_getNumberOfColumns(thread, pixel) == 2);
  ((VisualsContextProbe*)receiver)->pixel = pixel;
  return (void*)value;
}

static Arcadia_Engine_Visuals_Window*
createWindow
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{ return (void*)result(thread, self, 1); }

static Arcadia_List*
getDisplayDevices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{ return (void*)result(thread, self, 2); }

static Arcadia_Engine_Visuals_ConstantBufferResource*
createConstantBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{
  VisualsContextProbe* probe = (void*)self;
  if (probe->constructingPass) {
    Arcadia_Engine_Visuals_EnterPassResource* pass = probe->constructingPass;
    Arcadia_Object* fields[] = { (Arcadia_Object*)pass->viewToProjectionMatrix,
      (Arcadia_Object*)pass->worldToViewMatrix, (Arcadia_Object*)pass->clearColor };
    probe->constructionReceiver = Arcadia_WeakReference_create(thread, Arcadia_Value_makeObjectReferenceValue((Arcadia_Object*)pass));
    for (size_t i = 0; i < 3; ++i) {
      probe->constructionFields[i] = Arcadia_WeakReference_create(thread, Arcadia_Value_makeObjectReferenceValue(fields[i]));
    }
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Value receiver = Arcadia_WeakReference_getValue(thread, probe->constructionReceiver);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isObjectReferenceValue(&receiver));
    for (size_t i = 0; i < 3; ++i) {
      Arcadia_Value value = Arcadia_WeakReference_getValue(thread, probe->constructionFields[i]);
      Arcadia_Tests_assertTrue(thread, Arcadia_Value_isObjectReferenceValue(&value));
      Arcadia_Tests_assertTrue(thread, Arcadia_Value_getObjectReferenceValue(&value) == fields[i]);
    }
    if (probe->failConstruction) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
      Arcadia_Thread_jump(thread);
    }
  }
  return (void*)result(thread, self, 3);
}

static Arcadia_Engine_Visuals_FrameBufferResource*
createFrameBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{ return (void*)result(thread, self, 4); }

static Arcadia_Engine_Visuals_EnterPassResource*
createEnterPass
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{ return (void*)result(thread, self, 5); }

static Arcadia_Engine_Visuals_MaterialResource*
createMaterial
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* receiver,
    Arcadia_BooleanValue blend,
    Arcadia_Engine_Visuals_BlendFunction source,
    Arcadia_Engine_Visuals_BlendFunction destination,
    Arcadia_Engine_Visuals_ColorSource ambient,
    Arcadia_Engine_Visuals_TextureResource* texture,
    Arcadia_Engine_Visuals_ProgramResource* program
  )
{
  Arcadia_Object* value = result(thread, receiver, 6);
  VisualsContextProbe* self = (void*)receiver;
  if (texture) Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (void*)texture, _Arcadia_Engine_Visuals_TextureResource_getType(thread)));
  if (program) Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (void*)program, _Arcadia_Engine_Visuals_ProgramResource_getType(thread)));
  self->blend = blend; self->source = source; self->destination = destination; self->ambient = ambient;
  self->texture = texture; self->program = program;
  return (void*)value;
}

static Arcadia_Engine_Visuals_ModelResource*
createModel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* receiver,
    Arcadia_Engine_Visuals_ConstantBufferResource* constants,
    Arcadia_Engine_Visuals_VertexBufferResource* vertices,
    Arcadia_Engine_Visuals_MaterialResource* material
  )
{
  Arcadia_Object* value = result(thread, receiver, 7);
  VisualsContextProbe* self = (void*)receiver;
  self->constants = constants; self->vertices = vertices; self->material = material;
  return (void*)value;
}

static Arcadia_Engine_Visuals_ProgramResource*
createProgram
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_VPL_VerifiedModule* program
  )
{
  Arcadia_Tests_assertTrue(thread, !program);
  return (void*)result(thread, self, 8);
}

static Arcadia_Engine_Visuals_TextureResource*
createTexture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{ return (void*)result(thread, self, 9); }

static Arcadia_Engine_Visuals_VertexBufferResource*
createVertexBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{ return (void*)result(thread, self, 10); }

static void
update
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_BackendContext* self
  )
{ result(thread, (void*)self, 11); }

static void
initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    VisualsContextProbeDispatch* receiver
  )
{
  Arcadia_Engine_Visuals_BackendContextDispatch* self = (void*)receiver;
  self->createIcon = &createIcon; self->createWindow = &createWindow;
  self->getDisplayDevices = &getDisplayDevices;
  self->createConstantBufferResource = &createConstantBuffer;
  self->createFrameBufferResource = &createFrameBuffer;
  self->createEnterPassResource = &createEnterPass;
  self->createMaterialResource = &createMaterial;
  self->createModelResource = &createModel;
  self->createProgramResource = &createProgram;
  self->createTextureResource = &createTexture;
  self->createVertexBufferResource = &createVertexBuffer;
  ((Arcadia_Engine_BackendContextDispatch*)self)->update = &update;
}

static const Arcadia_ObjectType_Operations objectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&initializeDispatchImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&visitImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&destructImpl,
};

static const Arcadia_Type_Operations typeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &objectOperations,
};

Arcadia_defineObjectType(u8"Tests.VisualsContextProbe", VisualsContextProbe,
                         u8"Arcadia.Engine.Visuals.BackendContext", Arcadia_Engine_Visuals_BackendContext,
                         &typeOperations);

typedef struct EnterPassConstructionProbe EnterPassConstructionProbe;
typedef struct EnterPassConstructionProbeDispatch EnterPassConstructionProbeDispatch;

struct EnterPassConstructionProbe { Arcadia_Engine_Visuals_EnterPassResource _parent; };

struct EnterPassConstructionProbeDispatch { Arcadia_Engine_Visuals_EnterPassResourceDispatch _parent; };

Arcadia_declareObjectType(u8"Tests.EnterPassConstructionProbe", EnterPassConstructionProbe,
                          u8"Arcadia.Engine.Visuals.EnterPassResource");

static void
constructPass
  (
    Arcadia_Thread* thread,
    EnterPassConstructionProbe* self
  )
{
  Arcadia_EnterConstructor(EnterPassConstructionProbe);
  VisualsContextProbe* context = (void*)Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _VisualsContextProbe_getType(thread));
  // Borrowed only: the context must not trace unfinished derived state itself.
  context->constructingPass = (void*)self;
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  Arcadia_superTypeConstructor(thread, _type, self);
  Arcadia_LeaveConstructor(EnterPassConstructionProbe);
}

static const Arcadia_ObjectType_Operations passObjectOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&constructPass,
};

static const Arcadia_Type_Operations passTypeOperations = {
  Arcadia_Type_Operations_Initializer, .objectTypeOperations = &passObjectOperations,
};

Arcadia_defineObjectType(u8"Tests.EnterPassConstructionProbe", EnterPassConstructionProbe,
                          u8"Arcadia.Engine.Visuals.EnterPassResource", Arcadia_Engine_Visuals_EnterPassResource,
                          &passTypeOperations);

static void
testEnterPassConstruction
  (
    Arcadia_Thread* thread
  )
{
  Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
  VisualsContextProbe* context = (void*)create(thread, _VisualsContextProbe_getType(thread), NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)context);
  Arcadia_Engine_Visuals_Resource* buffer = (void*)create(thread, _Arcadia_Engine_Visuals_ConstantBufferResource_getType(thread), (Arcadia_Object*)context);
  context->results[3] = (Arcadia_Object*)buffer;
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural64Value(thread, i);
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (unsigned failure = 0; failure < 3; ++failure) {
    context->failConstruction = failure == 1;
    buffer->referenceCount = failure == 2 ? Arcadia_Integer32Value_Maximum : 0;
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      Arcadia_Engine_Visuals_EnterPassResource* pass = (void*)create(thread, _EnterPassConstructionProbe_getType(thread), (Arcadia_Object*)context);
      Arcadia_Thread_popJumpTarget(thread);
      context->constructingPass = NULL;
      Arcadia_ValueStack_pushObjectReferenceValue(thread, (Arcadia_Object*)pass);
      // Base fixtures have no backend unlink slot: mirror its dependency release.
      Arcadia_BooleanValue acquired = (Arcadia_Object*)pass->viewerConstantBuffer == (Arcadia_Object*)buffer && buffer->referenceCount == 1;
      Arcadia_Engine_Visuals_Resource_unref(thread, (void*)pass->viewerConstantBuffer);
      pass->viewerConstantBuffer = NULL;
      Arcadia_Tests_assertTrue(thread, failure == 0);
      Arcadia_Tests_assertTrue(thread, acquired);
      Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (Arcadia_Object*)pass, _EnterPassConstructionProbe_getType(thread)));
      Arcadia_Tests_assertTrue(thread, buffer->referenceCount == 0);
      Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
      for (size_t i = 0; i < 3; ++i) {
        Arcadia_Value value = Arcadia_WeakReference_getValue(thread, context->constructionFields[i]);
        Arcadia_Tests_assertTrue(thread, Arcadia_Value_isObjectReferenceValue(&value));
      }
      Arcadia_ValueStack_popValues(thread, 1);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      context->constructingPass = NULL;
      Arcadia_Status status = Arcadia_Thread_getStatus(thread);
      Arcadia_Thread_setStatus(thread, Arcadia_Status_Success);
      Arcadia_Tests_assertEqualStatus(thread, failure == 1 ? Arcadia_Status_OperationFailed : Arcadia_Status_OperationInvalid, status);
      Arcadia_Tests_assertTrue(thread, failure != 0);
      Arcadia_Tests_assertTrue(thread, buffer->referenceCount == (failure == 2 ? Arcadia_Integer32Value_Maximum : 0));
    }
    Arcadia_Tests_assertEqualSize(thread, base, Arcadia_ValueStack_getSize(thread));
    Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
    Arcadia_Value receiver = Arcadia_WeakReference_getValue(thread, context->constructionReceiver);
    Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&receiver));
    for (size_t i = 0; i < 3; ++i) {
      Arcadia_Value value = Arcadia_WeakReference_getValue(thread, context->constructionFields[i]);
      Arcadia_Tests_assertTrue(thread, Arcadia_Value_isVoidValue(&value));
    }
  }
  buffer->referenceCount = 0;
  for (size_t i = 0; i < 300; ++i) Arcadia_Tests_assertEqualNatural64(thread, i, Arcadia_ValueStack_getNatural64Value(thread, 299 - i));
  Arcadia_ValueStack_popValues(thread, 301);
  Arcadia_Tests_assertEqualSize(thread, entry, Arcadia_ValueStack_getSize(thread));
}

static Arcadia_Object*
call
  (
    Arcadia_Thread* thread,
    unsigned operation,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{
  switch (operation) {
    case 0: return (void*)Arcadia_Engine_Visuals_BackendContext_createIcon(thread, self, NULL);
    case 1: return (void*)Arcadia_Engine_Visuals_BackendContext_createWindow(thread, self);
    case 2: return (void*)Arcadia_Engine_Visuals_BackendContext_getDisplayDevices(thread, self);
    case 3: return (void*)Arcadia_Engine_Visuals_BackendContext_createConstantBufferResource(thread, self);
    case 4: return (void*)Arcadia_Engine_Visuals_BackendContext_createFrameBufferResource(thread, self);
    case 5: return (void*)Arcadia_Engine_Visuals_BackendContext_createEnterPassResource(thread, self);
    case 6: return (void*)Arcadia_Engine_Visuals_BackendContext_createMaterialResource(thread, self, false, 2, 3, Arcadia_Engine_Visuals_ColorSource_Vertex, NULL, NULL);
    case 7: return (void*)Arcadia_Engine_Visuals_BackendContext_createModelResource(thread, self, NULL, NULL, NULL);
    case 8: return (void*)Arcadia_Engine_Visuals_BackendContext_createProgramResource(thread, self, NULL);
    case 9: return (void*)Arcadia_Engine_Visuals_BackendContext_createTextureResource(thread, self);
    case 10: return (void*)Arcadia_Engine_Visuals_BackendContext_createVertexBufferResource(thread, self);
    default: Arcadia_Engine_BackendContext_update(thread, (void*)self); return NULL;
  }
}

static void
expectFailure
  (
    Arcadia_Thread* thread,
    unsigned operation,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_Status status
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    call(thread, operation, self);
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
  testEnterPassConstruction(thread);
  for (size_t i = 0; i < 300; ++i) Arcadia_ValueStack_pushNatural8Value(thread, 0);
  // The qualified procedure is available through the public Engine master.
  Arcadia_ArrayList* empty = Arcadia_Engine_Visuals_createVisualsWindowsList(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, empty);
  Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (void*)empty) == 0);
  Arcadia_ValueStack_popValues(thread, 1);
  VisualsContextProbe* self = (void*)create(thread, _VisualsContextProbe_getType(thread), NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  Arcadia_List* windows = self->_parent.windows;
  Arcadia_Tests_assertTrue(thread, Arcadia_Collection_getSize(thread, (void*)windows) == 0);
  Arcadia_List_insertBackObjectReferenceValue(thread, windows, Arcadia_String_createFromCxxString(thread, "window-list payload"));
  Arcadia_Engine_Visuals_BackendContext* second = (void*)create(thread, _Arcadia_Engine_Visuals_BackendContext_getType(thread), NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, second);
  Arcadia_Tests_assertTrue(thread, second->windows != windows && Arcadia_Collection_getSize(thread, (void*)second->windows) == 0);
  Arcadia_ValueStack_popValues(thread, 1);
  self->results[0] = create(thread, _Arcadia_Engine_Visuals_Icon_getType(thread), NULL);
  self->results[1] = create(thread, _Arcadia_Engine_Visuals_Window_getType(thread), NULL);
  self->results[2] = (void*)windows;
  Arcadia_TypeValue resources[] = {
    _Arcadia_Engine_Visuals_ConstantBufferResource_getType(thread), _Arcadia_Engine_Visuals_FrameBufferResource_getType(thread),
    _Arcadia_Engine_Visuals_EnterPassResource_getType(thread), _Arcadia_Engine_Visuals_MaterialResource_getType(thread),
    _Arcadia_Engine_Visuals_ProgramResource_getType(thread), _Arcadia_Engine_Visuals_TextureResource_getType(thread),
    _Arcadia_Engine_Visuals_VertexBufferResource_getType(thread)
  };
  unsigned slots[] = { 3, 4, 5, 6, 8, 9, 10 };
  // Fixture resources have their own native construction/lifetime protocols.
  for (size_t i = 0; i < 7; ++i) {
    if (slots[i] == 6) {
      Arcadia_SizeValue entry = Arcadia_ValueStack_getSize(thread);
      Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
      Arcadia_ValueStack_pushBooleanValue(thread, false);
      Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_BlendFunction_getType(thread), Arcadia_Engine_Visuals_BlendFunction_One));
      Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_BlendFunction_getType(thread), Arcadia_Engine_Visuals_BlendFunction_Zero));
      Arcadia_ValueStack_pushEnumerationValue(thread, Arcadia_EnumerationValue_make(_Arcadia_Engine_Visuals_ColorSource_getType(thread), Arcadia_Engine_Visuals_ColorSource_Mesh));
      Arcadia_ValueStack_pushNatural8Value(thread, 5);
      self->results[slots[i]] = _Arcadia_EndCreate0(thread, resources[i], entry);
    } else {
      self->results[slots[i]] = create(thread, resources[i], (void*)self);
    }
  }
  for (size_t i = 0; i < 12; ++i) self->calls[i] = 0;
  for (unsigned op = 0; op < 12; ++op) {
    Arcadia_Object* value = call(thread, op, (void*)self);
    Arcadia_Tests_assertTrue(thread, value == (op < 11 ? self->results[op] : NULL) && self->calls[op] == 1);
  }
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, (void*)Arcadia_List_getObjectReferenceValueAt(thread, windows, 0), "window-list payload", sizeof("window-list payload") - 1));
  Arcadia_Media_PixelBuffer* pixel = Arcadia_Media_PixelBuffer_create(thread, 0, 2, 1, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, pixel);
  Arcadia_Tests_assertTrue(thread, (void*)Arcadia_Engine_Visuals_BackendContext_createIcon(thread, (void*)self, pixel) == self->results[0] && self->pixel == pixel);
  Arcadia_ValueStack_popValues(thread, 1);
  const Arcadia_Engine_Visuals_ColorSource colorSources[] = {
    Arcadia_Engine_Visuals_ColorSource_Mesh,
    Arcadia_Engine_Visuals_ColorSource_Vertex,
    Arcadia_Engine_Visuals_ColorSource_Texture,
  };
  for (Arcadia_SizeValue i = 0; i < sizeof(colorSources) / sizeof(colorSources[0]); ++i) {
    for (unsigned blend = 0; blend < 2; ++blend) {
      Arcadia_Engine_Visuals_BackendContext_createMaterialResource(thread, (void*)self, blend, 2, 3, colorSources[i], (void*)self->results[9], (void*)self->results[8]);
      Arcadia_Tests_assertTrue(thread, self->blend == blend && self->source == 2 && self->destination == 3 && self->ambient == colorSources[i] && (void*)self->texture == self->results[9] && (void*)self->program == self->results[8]);
    }
  }
  Arcadia_Engine_Visuals_BackendContext_createModelResource(thread, (void*)self, (void*)self->results[3], (void*)self->results[10], (void*)self->results[6]);
  Arcadia_Tests_assertTrue(thread, (void*)self->constants == self->results[3] && (void*)self->vertices == self->results[10] && (void*)self->material == self->results[6]);
  self->fail = true;
  for (unsigned op = 0; op < 12; ++op) expectFailure(thread, op, (void*)self, Arcadia_Status_OperationFailed);
  self->fail = false;
  // The concrete backend normally releases this retained resource when unlinking.
  Arcadia_Engine_Visuals_EnterPassResource* pass = (void*)self->results[5];
  Arcadia_Engine_Visuals_Resource_unref(thread, (void*)pass->viewerConstantBuffer);
  pass->viewerConstantBuffer = NULL;
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  destroyed = 0;
  self = (void*)create(thread, _VisualsContextProbe_getType(thread), NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  for (unsigned op = 0; op < 11; ++op) {
    Arcadia_Tests_assertTrue(thread, !call(thread, op, (void*)self));
  }
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == 1);
  Arcadia_Engine_Visuals_BackendContext* unbound = (void*)create(thread, _Arcadia_Engine_Visuals_BackendContext_getType(thread), NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, unbound);
  Arcadia_String* wrong = Arcadia_String_createFromCxxString(thread, "wrong receiver");
  Arcadia_ValueStack_pushObjectReferenceValue(thread, wrong);
  for (unsigned op = 0; op < 12; ++op) {
    expectFailure(thread, op, unbound, Arcadia_Status_OperationInvalid);
    expectFailure(thread, op, NULL, Arcadia_Status_ArgumentTypeInvalid);
    expectFailure(thread, op, (void*)wrong, Arcadia_Status_ArgumentTypeInvalid);
  }
  Arcadia_ValueStack_popValues(thread, 2);
  self = (void*)create(thread, _VisualsContextProbe_getType(thread), NULL);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, self);
  pixel = Arcadia_Media_PixelBuffer_create(thread, 0, 2, 1, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_ValueStack_popValues(thread, 1);
  Arcadia_Engine_Visuals_BackendContext_createIcon(thread, (void*)self, pixel);
  Arcadia_Tests_assertTrue(thread, self->calls[0] == 1 && self->pixel == pixel);
  Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    Arcadia_ValueStack_pushNatural64Value(thread, 1);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    _Arcadia_EndCreate0(thread, _Arcadia_Engine_Visuals_BackendContext_getType(thread), depth);
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_BooleanValue_False);
  } else {
    Arcadia_Thread_popJumpTarget(thread);
    Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_NumberOfArgumentsInvalid && Arcadia_ValueStack_getSize(thread) == depth);
    Arcadia_Thread_setStatus(thread, 0);
  }
  Arcadia_Tests_assertTrue(thread, depth == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

int
Arcadia_Engine_Tests_Backends_visualsBackendContext
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
