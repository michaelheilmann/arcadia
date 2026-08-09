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

#include "Arcadia/Visuals/Implementation/OpenGL4/Resources/TextureResource.h"

#include "Arcadia/Visuals/Implementation/OpenGL4/BackendContext.h"
#include <assert.h>

#define AddressModeUDirty (1)
#define AddressModeVDirty (2)
#define MagnificationFilterDirty (8)
#define MinificationFilterDirty (16)
#define PixelsDirty (64)

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResourceDispatch* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  );

static Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getAddressModeUImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  );

static Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getAddressModeVImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  );

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getHeightImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  );

static Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getMagnificationFilterImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  );

static Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getMinificationFilterImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  );

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getWidthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setAddressModeUImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Engine_Visuals_TextureAddressMode addressMode
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setAddressModeVImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Engine_Visuals_TextureAddressMode addressMode
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setHeightImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Integer32Value height
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setMagnificationFilterImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Engine_Visuals_TextureFilter magnificationFilter
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setMinificationFilterImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Engine_Visuals_TextureFilter minificationFilter
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setWidthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Integer32Value width
  );

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setPixelBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Media_PixelBuffer* pixelBuffer
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Implementation.OpenGL4.TextureResource", Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource,
                         u8"Arcadia.Visuals.TextureResource", Arcadia_Engine_Visuals_TextureResource,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource);
  if (1 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  {
    Arcadia_Value t;
    t = Arcadia_ValueStack_getValue(thread, 1);
    Arcadia_ValueStack_pushValue(thread, &t);
    Arcadia_ValueStack_pushNatural8Value(thread, 1);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  //
  self->addressModeU = Arcadia_Engine_Visuals_TextureAddressMode_Repeat;
  self->addressModeV = Arcadia_Engine_Visuals_TextureAddressMode_Repeat;
  self->dirtyBits = AddressModeUDirty | AddressModeVDirty | MagnificationFilterDirty | MinificationFilterDirty | PixelsDirty;
  self->id = 0;
  self->magnificationFilter = Arcadia_Engine_Visuals_TextureFilter_Linear;
  self->minificationFilter = Arcadia_Engine_Visuals_TextureFilter_Linear;
  //
  self->pixelBuffer = Arcadia_Media_PixelBuffer_create(thread, 0, 320, 240, Arcadia_Media_PixelFormat_AlphaRedGreenBlueNatural8);
  //
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResourceDispatch* self
  )
{
  //
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->getAddressModeU = (Arcadia_Engine_Visuals_TextureAddressMode(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getAddressModeUImpl;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->getAddressModeV = (Arcadia_Engine_Visuals_TextureAddressMode(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getAddressModeVImpl;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->getHeight = (Arcadia_Integer32Value(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getHeightImpl;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->getMagnificationFilter = (Arcadia_Engine_Visuals_TextureFilter(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getMagnificationFilterImpl;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->getMinificationFilter = (Arcadia_Engine_Visuals_TextureFilter(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getMinificationFilterImpl;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->getWidth = (Arcadia_Integer32Value(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getWidthImpl;
  //
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setAddressModeU = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Engine_Visuals_TextureAddressMode)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setAddressModeUImpl;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setAddressModeV = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Engine_Visuals_TextureAddressMode)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setAddressModeVImpl;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setHeight = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Integer32Value)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setHeightImpl;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setMagnificationFilter = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Engine_Visuals_TextureFilter)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setMagnificationFilterImpl;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setMinificationFilter = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Engine_Visuals_TextureFilter)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setMinificationFilterImpl;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setWidth = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Integer32Value)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setWidthImpl;
  ((Arcadia_Engine_Visuals_TextureResourceDispatch*)self)->setPixelBuffer = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_TextureResource*, Arcadia_Media_PixelBuffer*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setPixelBufferImpl;
  //
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->load = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_loadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unload = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_unloadImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->unlink = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_unlinkImpl;
  ((Arcadia_Engine_Visuals_ResourceDispatch*)self)->render = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_Resource*, Arcadia_Engine_Visuals_EnterPassResource*)) & Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_renderImpl;
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  )
{
  assert(0 == self->id);
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  )
{
  if (self->pixelBuffer) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->pixelBuffer);
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_loadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  )
{
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
  if (!self->id) {
    gl->glGenTextures(1, &self->id);
    if (gl->glGetError()) {
      return;
    }
  }

  if (self->dirtyBits & (AddressModeUDirty | AddressModeVDirty | MagnificationFilterDirty | MinificationFilterDirty | PixelsDirty)) {
    Arcadia_JumpTarget jumpTarget;
    Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);
    if (Arcadia_JumpTarget_save(&jumpTarget)) {
      gl->glBindTexture(GL_TEXTURE_2D, self->id);
      if (gl->glGetError()) {
        Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
        Arcadia_Thread_jump(thread);
      }
      if (self->dirtyBits & AddressModeUDirty) {
        GLint addressMode;
        switch (self->addressModeU) {
          case Arcadia_Engine_Visuals_TextureAddressMode_ClampToBorder: {
            addressMode = GL_CLAMP_TO_BORDER;
          } break;
          case Arcadia_Engine_Visuals_TextureAddressMode_ClampToEdge: {
            addressMode = GL_CLAMP_TO_EDGE;
          } break;
          case Arcadia_Engine_Visuals_TextureAddressMode_MirroredRepeat: {
            addressMode = GL_MIRRORED_REPEAT;
          } break;
          case Arcadia_Engine_Visuals_TextureAddressMode_Repeat: {
            addressMode = GL_REPEAT;
          } break;
          default: {
            Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
            Arcadia_Thread_jump(thread);
          } break;
        };
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, addressMode);
        if (gl->glGetError()) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        }
        self->dirtyBits &= ~AddressModeUDirty;
      }
      if (self->dirtyBits & AddressModeVDirty) {
        GLint addressMode;
        switch (self->addressModeV) {
          case Arcadia_Engine_Visuals_TextureAddressMode_ClampToBorder: {
            addressMode = GL_CLAMP_TO_BORDER;
          } break;
          case Arcadia_Engine_Visuals_TextureAddressMode_ClampToEdge: {
            addressMode = GL_CLAMP_TO_EDGE;
          } break;
          case Arcadia_Engine_Visuals_TextureAddressMode_MirroredRepeat: {
            addressMode = GL_MIRRORED_REPEAT;
          } break;
          case Arcadia_Engine_Visuals_TextureAddressMode_Repeat: {
            addressMode = GL_REPEAT;
          } break;
          default: {
            Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
            Arcadia_Thread_jump(thread);
          } break;
        };
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, addressMode);
        if (gl->glGetError()) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        }
        self->dirtyBits &= ~AddressModeVDirty;
      }
      if (self->dirtyBits & MagnificationFilterDirty) {
        GLint magnificationFilter;
        switch (self->magnificationFilter) {
          case Arcadia_Engine_Visuals_TextureFilter_Linear: {
            magnificationFilter = GL_LINEAR;
          } break;
          case Arcadia_Engine_Visuals_TextureFilter_Nearest: {
            magnificationFilter = GL_NEAREST;
          } break;
          default: {
            Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
            Arcadia_Thread_jump(thread);
          } break;
        }
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter);
        if (gl->glGetError()) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        }
        self->dirtyBits &= ~MagnificationFilterDirty;
      }
      if (self->dirtyBits & MinificationFilterDirty) {
        GLint minificationFilter;
        switch (self->minificationFilter) {
          case Arcadia_Engine_Visuals_TextureFilter_Linear: {
            minificationFilter = GL_LINEAR;
          } break;
          case Arcadia_Engine_Visuals_TextureFilter_Nearest: {
            minificationFilter = GL_NEAREST;
          } break;
          default: {
            Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
            Arcadia_Thread_jump(thread);
          } break;
        }
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter);
        if (gl->glGetError()) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        }
        self->dirtyBits &= ~MinificationFilterDirty;
      }
      if (self->dirtyBits & PixelsDirty) {
        gl->glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         Arcadia_Media_PixelBuffer_getWidth(thread, self->pixelBuffer),
                         Arcadia_Media_PixelBuffer_getHeight(thread, self->pixelBuffer),
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         self->pixelBuffer->bytes); // @todo Proper encapsulation.
        if (gl->glGetError()) {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        }
        self->dirtyBits &= ~PixelsDirty;
      }
      Arcadia_Thread_popJumpTarget(thread);
    } else {
      Arcadia_Thread_popJumpTarget(thread);
      gl->glDeleteTextures(1, &self->id);
      self->id = 0;
      self->dirtyBits = AddressModeUDirty | AddressModeVDirty | MagnificationFilterDirty | MinificationFilterDirty | PixelsDirty;
      Arcadia_Thread_jump(thread);
    }
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_unloadImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  )
{
  Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* context = (Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext*)((Arcadia_Engine_Visuals_Resource*)self)->context;
  _Arcadia_Engine_Visuals_Implementation_OpenGL4_Functions* gl = Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext_getFunctions(thread, context);
  if (self->id) {
    gl->glDeleteTextures(1, &self->id);
    self->id = 0;
    self->dirtyBits = AddressModeUDirty | AddressModeVDirty | MagnificationFilterDirty | MinificationFilterDirty | PixelsDirty;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_unlinkImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  )
{ }

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_renderImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Engine_Visuals_EnterPassResource* enterPassResource
  )
{ }

static Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getAddressModeUImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  )
{ return self->addressModeU; }

static Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getAddressModeVImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  )
{ return self->addressModeV; }

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getHeightImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  )
{ return Arcadia_Media_PixelBuffer_getHeight(thread, self->pixelBuffer); }

static Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getMagnificationFilterImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  )
{ return self->magnificationFilter; }

static Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getMinificationFilterImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  )
{ return self->minificationFilter; }

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_getWidthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self
  )
{ return Arcadia_Media_PixelBuffer_getWidth(thread, self->pixelBuffer); }

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setAddressModeUImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Engine_Visuals_TextureAddressMode addressModeU
  )
{
  if (self->addressModeU != addressModeU) {
    self->addressModeU = addressModeU;
    self->dirtyBits |= AddressModeUDirty;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setAddressModeVImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Engine_Visuals_TextureAddressMode addressModeV
  )
{
  if (self->addressModeV != addressModeV) {
    self->addressModeV = addressModeV;
    self->dirtyBits |= AddressModeVDirty;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setHeightImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Integer32Value height
  )
{
  if (height != Arcadia_Media_PixelBuffer_getHeight(thread, self->pixelBuffer)) {
    Arcadia_Media_PixelBuffer_setHeight(thread, self->pixelBuffer, height);
    self->dirtyBits |= PixelsDirty;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setMagnificationFilterImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Engine_Visuals_TextureFilter magnificationFilter
  )
{
  if (self->magnificationFilter != magnificationFilter) {
    self->magnificationFilter = magnificationFilter;
    self->dirtyBits |= MagnificationFilterDirty;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setMinificationFilterImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Engine_Visuals_TextureFilter minificationFilter
  )
{
  if (self->minificationFilter != minificationFilter) {
    self->minificationFilter = minificationFilter;
    self->dirtyBits|= MinificationFilterDirty;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setWidthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Integer32Value width
  )
{
  if (width != Arcadia_Media_PixelBuffer_getHeight(thread, self->pixelBuffer)) {
    Arcadia_Media_PixelBuffer_setWidth(thread, self->pixelBuffer, width);
    self->dirtyBits |= PixelsDirty;
  }
}

static void
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_setPixelBufferImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource* self,
    Arcadia_Media_PixelBuffer* pixelBuffer
  )
{
  self->dirtyBits |= PixelsDirty;
  Arcadia_Media_PixelBuffer_assign(thread, self->pixelBuffer, pixelBuffer);
}

Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource*
Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Implementation_OpenGL4_BackendContext* backendContext
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource);
  if (backendContext) Arcadia_ValueStack_pushObjectReferenceValue(thread, backendContext); else Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void);
  Arcadia_ValueStack_pushNatural8Value(thread, 1);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Implementation_OpenGL4_TextureResource);
}
