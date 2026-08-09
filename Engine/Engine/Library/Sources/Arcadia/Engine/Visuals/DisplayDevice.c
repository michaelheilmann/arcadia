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
#include "Arcadia/Engine/Visuals/DisplayDevice.h"

#include "Arcadia/Collections/Include.h"

static void
Arcadia_Engine_Visuals_DisplayDevice_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  );

static void
Arcadia_Engine_Visuals_DisplayDevice_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDeviceDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*) & Arcadia_Engine_Visuals_DisplayDevice_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_DisplayDevice_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.DisplayDevice", Arcadia_Engine_Visuals_DisplayDevice,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_DisplayDevice_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_DisplayDevice);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_DisplayDevice);
}

static void
Arcadia_Engine_Visuals_DisplayDevice_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDeviceDispatch* self
  )
{/*Intetionally empty.*/}

void
Arcadia_Engine_Visuals_DisplayDevice_getBounds
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top,
    Arcadia_Integer32Value* right,
    Arcadia_Integer32Value* bottom
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_DisplayDevice, getBounds, self, left, top, right, bottom); }

Arcadia_Engine_Visuals_DisplayMode*
Arcadia_Engine_Visuals_DisplayDevice_getCurrentDisplayMode
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_DisplayDevice, getCurrentDisplayMode, self); }

Arcadia_List*
Arcadia_Engine_Visuals_DisplayDevice_getAvailableDisplayModes
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_DisplayDevice, getAvailableDisplayModes, self); }

Arcadia_String*
Arcadia_Engine_Visuals_DisplayDevice_getName
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_DisplayDevice, getName, self); }

Arcadia_String*
Arcadia_Engine_Visuals_DisplayDevice_getId
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayDevice* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_DisplayDevice, getId, self); }
