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
#include "Arcadia/Engine/Visuals/Resources/TextureResource.h"

static void
Arcadia_Engine_Visuals_TextureResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  );

static void
Arcadia_Engine_Visuals_TextureResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResourceDispatch* self
  );

static void
Arcadia_Engine_Visuals_TextureResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  );

static void
Arcadia_Engine_Visuals_TextureResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_TextureResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_TextureResource_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_TextureResource_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_TextureResource_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.TextureResource", Arcadia_Engine_Visuals_TextureResource,
                         u8"Arcadia.Visuals.Resource", Arcadia_Engine_Visuals_Resource,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_TextureResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_TextureResource);
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
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_TextureResource);
}

static void
Arcadia_Engine_Visuals_TextureResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResourceDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_TextureResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_TextureResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  )
{/*Intentionally empty.*/}

Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureResource_getAddressModeU
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_TextureResource, getAddressModeU, self); }

Arcadia_Engine_Visuals_TextureAddressMode
Arcadia_Engine_Visuals_TextureResource_getAddressModeV
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_TextureResource, getAddressModeV, self); }

Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureResource_getHeight
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_TextureResource, getHeight, self); }

Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureResource_getMagnificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_TextureResource, getMagnificationFilter, self); }

Arcadia_Engine_Visuals_TextureFilter
Arcadia_Engine_Visuals_TextureResource_getMinificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_TextureResource, getMinificationFilter, self); }

Arcadia_Integer32Value
Arcadia_Engine_Visuals_TextureResource_getWidth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_TextureResource, getWidth, self); }

void
Arcadia_Engine_Visuals_TextureResource_setAddressModeU
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Engine_Visuals_TextureAddressMode addressMode
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_TextureResource, setAddressModeU, self, addressMode); }

void
Arcadia_Engine_Visuals_TextureResource_setAddressModeV
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Engine_Visuals_TextureAddressMode addressMode
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_TextureResource, setAddressModeU, self, addressMode); }

void
Arcadia_Engine_Visuals_TextureResource_setHeight
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Integer32Value height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_TextureResource, setHeight, self, height); }

void
Arcadia_Engine_Visuals_TextureResource_setMagnificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Engine_Visuals_TextureFilter magnificationFilter
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_TextureResource, setMagnificationFilter, self, magnificationFilter); }

void
Arcadia_Engine_Visuals_TextureResource_setMinificationFilter
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Engine_Visuals_TextureFilter minificationFilter
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_TextureResource, setMinificationFilter, self, minificationFilter); }

void
Arcadia_Engine_Visuals_TextureResource_setWidth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Integer32Value width
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_TextureResource, setWidth, self, width); }

void
Arcadia_Engine_Visuals_TextureResource_setPixelBuffer
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_TextureResource* self,
    Arcadia_Media_PixelBuffer* pixelBuffer
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_TextureResource, setPixelBuffer, self, pixelBuffer); }
