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
#include "Arcadia/Engine/Visuals/Resources/FrameBufferResource.h"

#include "Arcadia/Engine/Visuals/BackendContext.h"
#include <assert.h>

static void
Arcadia_Engine_Visuals_FrameBufferResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  );

static void
Arcadia_Engine_Visuals_FrameBufferResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResourceDispatch* self
  );

static void
Arcadia_Engine_Visuals_FrameBufferResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  );

static void
Arcadia_Engine_Visuals_FrameBufferResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_FrameBufferResource_constructImpl,
  .destruct = (Arcadia_Object_DestructCallbackFunction*)&Arcadia_Engine_Visuals_FrameBufferResource_destructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_FrameBufferResource_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_FrameBufferResource_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.FrameBufferResource", Arcadia_Engine_Visuals_FrameBufferResource,
                         u8"Arcadia.Visuals.Resource", Arcadia_Engine_Visuals_Resource,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_FrameBufferResource_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_FrameBufferResource);
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
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_FrameBufferResource);
}

static void
Arcadia_Engine_Visuals_FrameBufferResource_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResourceDispatch* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_FrameBufferResource_destructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  )
{/*Intentionally empty.*/}

static void
Arcadia_Engine_Visuals_FrameBufferResource_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  )
{/*Intentionally empty.*/}

void
Arcadia_Engine_Visuals_FrameBufferResource_activate
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_FrameBufferResource, activate, self); }

void
Arcadia_Engine_Visuals_FrameBufferResource_deactivate
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_FrameBufferResource, activate, self); }

void
Arcadia_Engine_Visuals_FrameBufferResource_setSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self,
    Arcadia_Integer32Value width,
    Arcadia_Integer32Value height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_FrameBufferResource, setSize, self, width, height); }

void
Arcadia_Engine_Visuals_FrameBufferResource_getSize
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_FrameBufferResource* self,
    Arcadia_Integer32Value* width,
    Arcadia_Integer32Value* height
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_FrameBufferResource, getSize, self, width, height); }
