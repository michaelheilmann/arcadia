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

typedef struct ProbeData ProbeData;

struct ProbeData {
  Arcadia_String* label;
  Arcadia_Media_PixelBuffer* pixels;
  Arcadia_Integer32Value values[6];
  Arcadia_SizeValue calls[13];
  const void* last;
  Arcadia_SizeValue size;
};

static Arcadia_BooleanValue failCallbacks;
static Arcadia_SizeValue destroyed;
static Arcadia_Math_Color4Real32* colorArgument;
static Arcadia_Math_Matrix4Real32* matrixArgument;
static Arcadia_Media_PixelBuffer* pixelArgument;

static void
initBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResourceDispatch* self
  );

static void
initFrame
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResourceDispatch* self
  );

static void
initTexture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResourceDispatch* self
  );

static void
initProgram
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ProgramResourceDispatch* self
  );

// Share only test-subclass boilerplate. Every parent and dispatch remains its
// real C type, and the migrated parent supplies the actual initialization.
#define DEFINE_PROBE(Name, Parent, RuntimeName, ParentName, Initialize) \
  typedef struct Name Name; \
  typedef struct Name##Dispatch Name##Dispatch; \
  struct Name { Parent _parent; ProbeData data; }; \
  struct Name##Dispatch { Parent##Dispatch _parent; }; \
  Arcadia_declareObjectType(RuntimeName, Name, ParentName); \
  static void Name##_construct(Arcadia_Thread* thread, Name* self) { \
    Arcadia_EnterConstructor(Name); \
    if (_numberOfArguments != 1) { Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid); Arcadia_Thread_jump(thread); } \
    Arcadia_Value argument = Arcadia_ValueStack_getValue(thread, 1); \
    Arcadia_ValueStack_pushValue(thread, &argument); Arcadia_ValueStack_pushNatural8Value(thread, 1); \
    Arcadia_superTypeConstructor(thread, _type, self); \
    self->data = (ProbeData){0}; \
    self->data.label = Arcadia_String_createFromCxxString(thread, "thin resource payload"); \
    Arcadia_LeaveConstructor(Name); \
  } \
  static void Name##_visit(Arcadia_Thread* thread, Name* self) { \
    if (self->data.label) Arcadia_Object_visit(thread, (void*)self->data.label); \
    if (self->data.pixels) Arcadia_Object_visit(thread, (void*)self->data.pixels); \
  } \
  static void Name##_destruct(Arcadia_Thread* thread, Name* self) { destroyed++; } \
  static const Arcadia_ObjectType_Operations Name##_objectOperations = { \
    Arcadia_ObjectType_Operations_Initializer, \
    .construct = (Arcadia_Object_ConstructCallbackFunction*)&Name##_construct, \
    .visit = (Arcadia_Object_VisitCallbackFunction*)&Name##_visit, \
    .destruct = (Arcadia_Object_DestructCallbackFunction*)&Name##_destruct, \
    .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)Initialize, \
  }; \
  static const Arcadia_Type_Operations Name##_operations = { Arcadia_Type_Operations_Initializer, .objectTypeOperations = &Name##_objectOperations }; \
  Arcadia_defineObjectType(RuntimeName, Name, ParentName, Parent, &Name##_operations);

DEFINE_PROBE(BufferProbe, Arcadia_Engine_Visuals_ConstantBufferResource,
             u8"Tests.ThinBuffer", u8"Arcadia.Engine.Visuals.ConstantBufferResource", initBuffer)
DEFINE_PROBE(FrameProbe, Arcadia_Engine_Visuals_FrameBufferResource,
             u8"Tests.ThinFrame", u8"Arcadia.Engine.Visuals.FrameBufferResource", initFrame)
DEFINE_PROBE(TextureProbe, Arcadia_Engine_Visuals_TextureResource,
             u8"Tests.ThinTexture", u8"Arcadia.Engine.Visuals.TextureResource", initTexture)
DEFINE_PROBE(ProgramProbe, Arcadia_Engine_Visuals_ProgramResource,
             u8"Tests.ThinProgram", u8"Arcadia.Engine.Visuals.ProgramResource", initProgram)
#undef DEFINE_PROBE

static Arcadia_Object*
create
  (
    Arcadia_Thread* thread,
    Arcadia_TypeValue type,
    Arcadia_Value* args,
    Arcadia_Natural8Value count
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  for (Arcadia_Natural8Value i = 0; i < count; ++i) Arcadia_ValueStack_pushValue(thread, &args[i]);
  Arcadia_ValueStack_pushNatural8Value(thread, count);
  return _Arcadia_EndCreate0(thread, type, base);
}

static void
record
  (
    Arcadia_Thread* thread,
    ProbeData* data,
    unsigned slot
  )
{
  if (failCallbacks) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_OperationFailed);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, Arcadia_String_isEqualTo_pn(thread, data->label, "thin resource payload", sizeof("thin resource payload") - 1));
  data->calls[slot]++;
}

static void
setData
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self,
    const void* bytes,
    Arcadia_SizeValue size
  )
{ ProbeData* d = &((BufferProbe*)self)->data; record(thread, d, 0); d->last = bytes; d->size = size; }

static void
clear
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self
  )
{ record(thread, &((BufferProbe*)self)->data, 1); }

static void
writeColor
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self,
    const Arcadia_Math_Color4Real32* value
  )
{
  record(thread, &((BufferProbe*)self)->data, 2);
  if (value) Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (void*)value, _Arcadia_Math_Color4Real32_getType(thread)));
  ((BufferProbe*)self)->data.last = value;
}

static void
writeMatrix
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResource* self,
    Arcadia_BooleanValue transpose,
    const Arcadia_Math_Matrix4Real32* value
  )
{
  record(thread, &((BufferProbe*)self)->data, 3);
  if (value) Arcadia_Tests_assertTrue(thread, Arcadia_Object_isInstanceOf(thread, (void*)value, _Arcadia_Math_Matrix4Real32_getType(thread)));
  ((BufferProbe*)self)->data.last = value; ((BufferProbe*)self)->data.values[0] = transpose;
}

static void
initBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ConstantBufferResourceDispatch* self
  )
{
  self->setData = &setData; self->clear = &clear;
  self->writeColor4Real32 = &writeColor; self->writeMatrix4x4Real32 = &writeMatrix;
}

static void
activate
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  )
{ record(thread, &((FrameProbe*)self)->data, 0); }

static void
deactivate
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  )
{ record(thread, &((FrameProbe*)self)->data, 1); }

static void
setSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{
  record(thread, &((FrameProbe*)self)->data, 2);
  ((FrameProbe*)self)->data.values[0] = width; ((FrameProbe*)self)->data.values[1] = height;
}

static void
getSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{
  record(thread, &((FrameProbe*)self)->data, 3);
  if (width) *width = ((FrameProbe*)self)->data.values[0];
  if (height) *height = ((FrameProbe*)self)->data.values[1];
}

static Arcadia_Media_PixelBuffer*
capture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  )
{ record(thread, &((FrameProbe*)self)->data, 4); return ((FrameProbe*)self)->data.pixels; }

static void
initFrame
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResourceDispatch* self
  )
{
  self->activate = &activate; self->deactivate = &deactivate; self->setSize = &setSize;
  self->getSize = &getSize; self->capturePixels = &capture;
}

#define GETTER(Name, Type, Index) \
  static Type Name(Arcadia_Thread* thread, Arcadia_Engine_Visuals_TextureResource* self) { \
    record(thread, &((TextureProbe*)self)->data, Index); return (Type)((TextureProbe*)self)->data.values[Index]; \
  }
#define SETTER(Name, Type, Index) \
  static void Name(Arcadia_Thread* thread, Arcadia_Engine_Visuals_TextureResource* self, Type value) { \
    record(thread, &((TextureProbe*)self)->data, Index + 6); ((TextureProbe*)self)->data.values[Index] = value; \
  }
GETTER(getU, Arcadia_Engine_Visuals_TextureAddressMode, 0)
GETTER(getV, Arcadia_Engine_Visuals_TextureAddressMode, 1)
GETTER(getHeight, Arcadia_Integer32Value, 2)
GETTER(getMag, Arcadia_Engine_Visuals_TextureFilter, 3)
GETTER(getMin, Arcadia_Engine_Visuals_TextureFilter, 4)
GETTER(getWidth, Arcadia_Integer32Value, 5)
SETTER(setU, Arcadia_Engine_Visuals_TextureAddressMode, 0)
SETTER(setV, Arcadia_Engine_Visuals_TextureAddressMode, 1)
SETTER(setHeight, Arcadia_Integer32Value, 2)
SETTER(setMag, Arcadia_Engine_Visuals_TextureFilter, 3)
SETTER(setMin, Arcadia_Engine_Visuals_TextureFilter, 4)
SETTER(setWidth, Arcadia_Integer32Value, 5)
#undef GETTER
#undef SETTER

static void
setPixel
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Media_PixelBuffer* value
  )
{
  record(thread, &((TextureProbe*)self)->data, 12);
  if (value) Arcadia_Tests_assertTrue(thread, Arcadia_Media_PixelBuffer_getWidth(thread, value) == 2);
  ((TextureProbe*)self)->data.last = value;
}

static void
initTexture
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResourceDispatch* self
  )
{
  self->getAddressModeU = &getU; self->getAddressModeV = &getV; self->getHeight = &getHeight;
  self->getMagnificationFilter = &getMag; self->getMinificationFilter = &getMin; self->getWidth = &getWidth;
  self->setAddressModeU = &setU; self->setAddressModeV = &setV; self->setHeight = &setHeight;
  self->setMagnificationFilter = &setMag; self->setMinificationFilter = &setMin; self->setWidth = &setWidth;
  self->setPixelBuffer = &setPixel;
}

static void
programLoad
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  )
{ record(thread, &((ProgramProbe*)self)->data, 0); }

static void
programUnload
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  )
{ record(thread, &((ProgramProbe*)self)->data, 1); }

static void
programUnlink
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self
  )
{ record(thread, &((ProgramProbe*)self)->data, 2); }

static void
programRender
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Resource* self,
    Arcadia_Engine_Visuals_EnterPassResource* pass
  )
{
  record(thread, &((ProgramProbe*)self)->data, 3);
  ((ProgramProbe*)self)->data.last = pass;
}

static void
initProgram
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_ProgramResourceDispatch* self
  )
{
  Arcadia_Engine_Visuals_ResourceDispatch* base = (void*)self;
  base->load = &programLoad; base->unload = &programUnload;
  base->unlink = &programUnlink; base->render = &programRender;
}

static void
callProgram
  (
    Arcadia_Thread* thread,
    ProgramProbe* self,
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
call
  (
    Arcadia_Thread* thread,
    unsigned operation,
    void* self
  )
{
  Arcadia_Integer32Value width, height;
  switch (operation) {
    case 0: Arcadia_Engine_Visuals_ConstantBufferResource_setData(thread, self, NULL, 0); break;
    case 1: Arcadia_Engine_Visuals_ConstantBufferResource_clear(thread, self); break;
    case 2: Arcadia_Engine_Visuals_ConstantBufferResource_writeColor4Real32(thread, self, colorArgument); break;
    case 3: Arcadia_Engine_Visuals_ConstantBufferResource_writeMatrix4x4Real32(thread, self, false, matrixArgument); break;
    case 4: Arcadia_Engine_Visuals_FrameBufferResource_activate(thread, self); break;
    case 5: Arcadia_Engine_Visuals_FrameBufferResource_deactivate(thread, self); break;
    case 6: Arcadia_Engine_Visuals_FrameBufferResource_setSize(thread, self, 3, 17); break;
    case 7: Arcadia_Engine_Visuals_FrameBufferResource_getSize(thread, self, &width, &height); break;
    case 8: Arcadia_Engine_Visuals_FrameBufferResource_capturePixels(thread, self); break;
    case 9: Arcadia_Engine_Visuals_TextureResource_getAddressModeU(thread, self); break;
    case 10: Arcadia_Engine_Visuals_TextureResource_getAddressModeV(thread, self); break;
    case 11: Arcadia_Engine_Visuals_TextureResource_getHeight(thread, self); break;
    case 12: Arcadia_Engine_Visuals_TextureResource_getMagnificationFilter(thread, self); break;
    case 13: Arcadia_Engine_Visuals_TextureResource_getMinificationFilter(thread, self); break;
    case 14: Arcadia_Engine_Visuals_TextureResource_getWidth(thread, self); break;
    case 15: Arcadia_Engine_Visuals_TextureResource_setAddressModeU(thread, self, 1); break;
    case 16: Arcadia_Engine_Visuals_TextureResource_setAddressModeV(thread, self, 2); break;
    case 17: Arcadia_Engine_Visuals_TextureResource_setHeight(thread, self, 17); break;
    case 18: Arcadia_Engine_Visuals_TextureResource_setMagnificationFilter(thread, self, 1); break;
    case 19: Arcadia_Engine_Visuals_TextureResource_setMinificationFilter(thread, self, 2); break;
    case 20: Arcadia_Engine_Visuals_TextureResource_setWidth(thread, self, 3); break;
    case 21: Arcadia_Engine_Visuals_TextureResource_setPixelBuffer(thread, self, pixelArgument); break;
  }
}

static void
expectCallFailure
  (
    Arcadia_Thread* thread,
    unsigned operation,
    void* self,
    Arcadia_Status status
  )
{
  Arcadia_SizeValue base = Arcadia_ValueStack_getSize(thread);
  Arcadia_JumpTarget jump;
  Arcadia_Thread_pushJumpTarget(thread, &jump);
  if (Arcadia_JumpTarget_save(&jump)) {
    call(thread, operation, self);
    Arcadia_Thread_popJumpTarget(thread); Arcadia_Tests_assertTrue(thread, false);
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
  Arcadia_Engine_Visuals_BackendContext* context = (void*)create(thread, _Arcadia_Engine_Visuals_BackendContext_getType(thread), NULL, 0);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, context);
  Arcadia_Value args[] = { Arcadia_Value_makeObjectReferenceValue(context), Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void) };
  BufferProbe* buffer = (void*)create(thread, _BufferProbe_getType(thread), args, 1);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, buffer);
  FrameProbe* frame = (void*)create(thread, _FrameProbe_getType(thread), args, 1);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, frame);
  TextureProbe* texture = (void*)create(thread, _TextureProbe_getType(thread), args, 1);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, texture);
  ProgramProbe* program = (void*)create(thread, _ProgramProbe_getType(thread), args, 1);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, program);
  colorArgument = Arcadia_Math_Color4Real32_create4(thread, 1.f, 2.f, 3.f, 4.f);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, colorArgument);
  matrixArgument = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, matrixArgument);
  pixelArgument = Arcadia_Media_PixelBuffer_create(thread, 0, 2, 3, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, pixelArgument);
  void* probes[] = { buffer, frame, texture, program };
  Arcadia_TypeValue types[] = { _Arcadia_Engine_Visuals_ConstantBufferResource_getType(thread), _Arcadia_Engine_Visuals_FrameBufferResource_getType(thread), _Arcadia_Engine_Visuals_TextureResource_getType(thread), _Arcadia_Engine_Visuals_ProgramResource_getType(thread) };
  Arcadia_Engine_Visuals_ResourceDispatch* programBase = (void*)Arcadia_ObjectType_getDispatch(types[3]);
  Arcadia_Tests_assertTrue(thread, !programBase->load && !programBase->unload && !programBase->unlink && !programBase->render);
  for (size_t i = 0; i < sizeof(probes) / sizeof(probes[0]); ++i) {
    Arcadia_Engine_Visuals_Resource* resource = probes[i];
    Arcadia_Tests_assertTrue(thread, resource->referenceCount == 0 && resource->context == context && Arcadia_Object_isInstanceOf(thread, (void*)resource, _Arcadia_Engine_Visuals_Resource_getType(thread)));
    Arcadia_Engine_Visuals_Resource_ref(thread, resource); Arcadia_Engine_Visuals_Resource_unref(thread, resource);
    Arcadia_Tests_assertTrue(thread, resource->referenceCount == 0);
    for (unsigned variant = 0; variant < 6; ++variant) {
      Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
      Arcadia_Value invalid[] = { args[0], args[1] };
      Arcadia_Natural8Value count = 1;
      if (variant == 0) count = 0;
      else if (variant == 1) count = 2;
      else if (variant == 2) invalid[0] = Arcadia_Value_makeVoidValue(Arcadia_VoidValue_Void);
      else if (variant == 3) invalid[0] = Arcadia_Value_makeNatural64Value(1);
      else if (variant == 4) invalid[0] = Arcadia_Value_makeObjectReferenceValue(colorArgument);
      else invalid[0] = Arcadia_Value_makeInteger32Value(1);
      Arcadia_JumpTarget jump;
      Arcadia_Thread_pushJumpTarget(thread, &jump);
      if (Arcadia_JumpTarget_save(&jump)) {
        create(thread, types[i], invalid, count);
        Arcadia_Thread_popJumpTarget(thread); Arcadia_Tests_assertTrue(thread, false);
      } else {
        Arcadia_Thread_popJumpTarget(thread);
        Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == (variant < 2 ? Arcadia_Status_NumberOfArgumentsInvalid : Arcadia_Status_ArgumentTypeInvalid) && Arcadia_ValueStack_getSize(thread) == depth);
        Arcadia_Thread_setStatus(thread, 0);
      }
    }
  }
  for (unsigned operation = 0; operation < 4; ++operation) {
    callProgram(thread, program, operation);
    Arcadia_Tests_assertTrue(thread, program->data.calls[operation] == 1);
  }
  Arcadia_Tests_assertTrue(thread, program->data.last == NULL);
  failCallbacks = true;
  for (unsigned operation = 0; operation < 4; ++operation) {
    Arcadia_SizeValue depth = Arcadia_ValueStack_getSize(thread);
    Arcadia_JumpTarget jump;
    Arcadia_Thread_pushJumpTarget(thread, &jump);
    if (Arcadia_JumpTarget_save(&jump)) {
      callProgram(thread, program, operation);
      Arcadia_Thread_popJumpTarget(thread); Arcadia_Tests_assertTrue(thread, false);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      Arcadia_Tests_assertTrue(thread, Arcadia_Thread_getStatus(thread) == Arcadia_Status_OperationFailed && Arcadia_ValueStack_getSize(thread) == depth);
      Arcadia_Thread_setStatus(thread, 0);
    }
  }
  failCallbacks = false;
  for (unsigned operation = 0; operation < 22; ++operation) call(thread, operation, probes[operation < 4 ? 0 : operation < 9 ? 1 : 2]);
  for (unsigned i = 0; i < 4; ++i) Arcadia_Tests_assertTrue(thread, buffer->data.calls[i] == 1);
  for (unsigned i = 0; i < 5; ++i) Arcadia_Tests_assertTrue(thread, frame->data.calls[i] == 1);
  for (unsigned i = 0; i < 13; ++i) Arcadia_Tests_assertTrue(thread, texture->data.calls[i] == 1);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Visuals_TextureResource_getAddressModeU(thread, (void*)texture) == 1 && Arcadia_Engine_Visuals_TextureResource_getAddressModeV(thread, (void*)texture) == 2);
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Visuals_TextureResource_getMagnificationFilter(thread, (void*)texture) == 1 && Arcadia_Engine_Visuals_TextureResource_getMinificationFilter(thread, (void*)texture) == 2);
  unsigned char bytes[] = { 3, 0, 255, 17 };
  Arcadia_Engine_Visuals_ConstantBufferResource_setData(thread, (void*)buffer, bytes, sizeof(bytes));
  Arcadia_Tests_assertTrue(thread, buffer->data.last == bytes && buffer->data.size == sizeof(bytes) && bytes[2] == 255);
  Arcadia_Engine_Visuals_ConstantBufferResource_setData(thread, (void*)buffer, NULL, Arcadia_SizeValue_Maximum);
  Arcadia_Tests_assertTrue(thread, !buffer->data.last && buffer->data.size == Arcadia_SizeValue_Maximum);
  for (unsigned transpose = 0; transpose < 2; ++transpose) {
    Arcadia_Engine_Visuals_ConstantBufferResource_writeMatrix4x4Real32(thread, (void*)buffer, transpose, matrixArgument);
    Arcadia_Tests_assertTrue(thread, buffer->data.last == matrixArgument && buffer->data.values[0] == transpose);
  }
  Arcadia_Integer32Value pairs[][2] = { { 0, 0 }, { -3, 17 }, { INT32_MIN, INT32_MAX }, { INT32_MAX, INT32_MIN } };
  for (size_t i = 0; i < 4; ++i) {
    Arcadia_Integer32Value width = 0, height = 0;
    Arcadia_Engine_Visuals_FrameBufferResource_setSize(thread, (void*)frame, pairs[i][0], pairs[i][1]);
    Arcadia_Engine_Visuals_FrameBufferResource_getSize(thread, (void*)frame, &width, &height);
    Arcadia_Tests_assertTrue(thread, width == pairs[i][0] && height == pairs[i][1]);
    Arcadia_Engine_Visuals_FrameBufferResource_getSize(thread, (void*)frame, &width, &width);
    Arcadia_Tests_assertTrue(thread, width == pairs[i][1]);
    Arcadia_Engine_Visuals_FrameBufferResource_getSize(thread, (void*)frame, NULL, &height);
    Arcadia_Engine_Visuals_FrameBufferResource_getSize(thread, (void*)frame, &width, NULL);
    Arcadia_Engine_Visuals_TextureResource_setWidth(thread, (void*)texture, pairs[i][0]);
    Arcadia_Engine_Visuals_TextureResource_setHeight(thread, (void*)texture, pairs[i][1]);
    Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Visuals_TextureResource_getWidth(thread, (void*)texture) == pairs[i][0] && Arcadia_Engine_Visuals_TextureResource_getHeight(thread, (void*)texture) == pairs[i][1]);
  }
  Arcadia_Tests_assertTrue(thread, !Arcadia_Engine_Visuals_FrameBufferResource_capturePixels(thread, (void*)frame));
  frame->data.pixels = pixelArgument;
  Arcadia_Tests_assertTrue(thread, Arcadia_Engine_Visuals_FrameBufferResource_capturePixels(thread, (void*)frame) == pixelArgument);
  failCallbacks = true;
  for (unsigned operation = 0; operation < 22; ++operation) expectCallFailure(thread, operation, probes[operation < 4 ? 0 : operation < 9 ? 1 : 2], Arcadia_Status_OperationFailed);
  failCallbacks = false;
  for (unsigned operation = 0; operation < 22; ++operation) {
    unsigned kind = operation < 4 ? 0 : operation < 9 ? 1 : 2;
    Arcadia_Object* unbound = create(thread, types[kind], args, 1);
    Arcadia_ValueStack_pushObjectReferenceValue(thread, unbound);
    expectCallFailure(thread, operation, unbound, Arcadia_Status_OperationInvalid);
    expectCallFailure(thread, operation, NULL, Arcadia_Status_ArgumentTypeInvalid);
    expectCallFailure(thread, operation, buffer->data.label, Arcadia_Status_ArgumentTypeInvalid);
    Arcadia_ValueStack_popValues(thread, 1);
  }
  BufferProbe* temporary = (void*)create(thread, _BufferProbe_getType(thread), args, 1);
  Arcadia_Engine_Visuals_ConstantBufferResource_clear(thread, (void*)temporary);
  Arcadia_Tests_assertTrue(thread, temporary->data.calls[1] == 1);
  Arcadia_Math_Color4Real32* temporaryColor = Arcadia_Math_Color4Real32_create4(thread, 1.f, 2.f, 3.f, 4.f);
  Arcadia_Engine_Visuals_ConstantBufferResource_writeColor4Real32(thread, (void*)buffer, temporaryColor);
  Arcadia_Tests_assertTrue(thread, buffer->data.last == temporaryColor);
  Arcadia_Math_Matrix4Real32* temporaryMatrix = Arcadia_Math_Matrix4Real32_create(thread);
  Arcadia_Engine_Visuals_ConstantBufferResource_writeMatrix4x4Real32(thread, (void*)buffer, true, temporaryMatrix);
  Arcadia_Tests_assertTrue(thread, buffer->data.last == temporaryMatrix);
  Arcadia_Media_PixelBuffer* temporaryPixel = Arcadia_Media_PixelBuffer_create(thread, 0, 2, 1, Arcadia_Media_PixelFormat_RedGreenBlueAlphaNatural8);
  Arcadia_Engine_Visuals_TextureResource_setPixelBuffer(thread, (void*)texture, temporaryPixel);
  Arcadia_Tests_assertTrue(thread, texture->data.last == temporaryPixel);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_SizeValue before = destroyed;
  Arcadia_ValueStack_popValues(thread, 8);
  Arcadia_Tests_assertTrue(thread, !Arcadia_Process_runARMS(Arcadia_Thread_getProcess(thread), false));
  Arcadia_Tests_assertTrue(thread, destroyed == before + 4 && Arcadia_ValueStack_getSize(thread) == base + 300);
  Arcadia_ValueStack_popValues(thread, 300);
}

int
Arcadia_Engine_Tests_VisualResources_thinResources
  (
    void
  )
{
  return Arcadia_Tests_safeExecute(test) ? EXIT_SUCCESS : EXIT_FAILURE;
}
