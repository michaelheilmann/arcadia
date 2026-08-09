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
#include "Arcadia/Engine/Visuals/BackendContext.h"

static void
Arcadia_Engine_Visuals_BackendContext_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

static void
Arcadia_Engine_Visuals_BackendContext_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContextDispatch* self
  );

static void
Arcadia_Engine_Visuals_BackendContext_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_BackendContext_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_BackendContext_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_BackendContext_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.BackendContext", Arcadia_Engine_Visuals_BackendContext,
                         u8"Arcadia.Engine.BackendContext", Arcadia_Engine_BackendContext,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_BackendContext_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_BackendContext);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->windows = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_BackendContext);
}

static void
Arcadia_Engine_Visuals_BackendContext_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContextDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_BackendContext_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{
  if (self->windows) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->windows);
  }
}

Arcadia_Engine_Visuals_Icon*
Arcadia_Engine_Visuals_BackendContext_createIcon
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_Media_PixelBuffer* pixelBuffer
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_BackendContext, createIcon, self, pixelBuffer); }

Arcadia_Engine_Visuals_Window*
Arcadia_Engine_Visuals_BackendContext_createWindow
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_BackendContext, createWindow, self); }

Arcadia_List*
Arcadia_Engine_Visuals_BackendContext_getDisplayDevices
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_BackendContext, getDisplayDevices, self); }

Arcadia_Engine_Visuals_ConstantBufferResource*
Arcadia_Engine_Visuals_BackendContext_createConstantBufferResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_BackendContext, createConstantBufferResource, self); }

Arcadia_Engine_Visuals_FrameBufferResource*
Arcadia_Engine_Visuals_BackendContext_createFrameBufferResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_BackendContext, createFrameBufferResource, self); }

Arcadia_Engine_Visuals_EnterPassResource*
Arcadia_Engine_Visuals_BackendContext_createEnterPassResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_BackendContext, createEnterPassResource, self); }

Arcadia_Engine_Visuals_MaterialResource*
Arcadia_Engine_Visuals_BackendContext_createMaterialResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_Engine_Visuals_MaterialResource_AmbientColorSource ambientColorSource,
    Arcadia_Engine_Visuals_TextureResource* ambientTexture,
    Arcadia_Engine_Visuals_ProgramResource* program
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_BackendContext, createMaterialResource, self, ambientColorSource, ambientTexture, program); }

Arcadia_Engine_Visuals_ModelResource*
Arcadia_Engine_Visuals_BackendContext_createModelResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_Engine_Visuals_ConstantBufferResource* meshConstantBufferResource,
    Arcadia_Engine_Visuals_VertexBufferResource* vertexBufferResource,
    Arcadia_Engine_Visuals_MaterialResource* material
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_BackendContext, createModelResource, self, meshConstantBufferResource, vertexBufferResource, material); }

Arcadia_Engine_Visuals_ProgramResource*
Arcadia_Engine_Visuals_BackendContext_createProgramResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self,
    Arcadia_VPL_Symbols_Program* program
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_BackendContext, createProgramResource, self, program); }

Arcadia_Engine_Visuals_TextureResource*
Arcadia_Engine_Visuals_BackendContext_createTextureResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_BackendContext, createTextureResource, self); }

Arcadia_Engine_Visuals_VertexBufferResource*
Arcadia_Engine_Visuals_BackendContext_createVertexBufferResource
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_BackendContext* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_BackendContext, createVertexBufferResource, self); }
