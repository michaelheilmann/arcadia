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
#include "Arcadia/Engine/Visuals/DisplayMode.h"

static void
Arcadia_Engine_Visuals_DisplayMode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  );

static void
Arcadia_Engine_Visuals_DisplayMode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayModeDispatch* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*) & Arcadia_Engine_Visuals_DisplayMode_constructImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_DisplayMode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Engine.Visuals.DisplayMode", Arcadia_Engine_Visuals_DisplayMode,
                         u8"Arcadia.Object", Arcadia_Object,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_DisplayMode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_DisplayMode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (0 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_DisplayMode);
}

static void
Arcadia_Engine_Visuals_DisplayMode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayModeDispatch* self
  )
{/*Intentionally empty.*/}

Arcadia_Integer32Value
Arcadia_Engine_Visuals_DisplayMode_getHorizontalResolution
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_DisplayMode, getHorizontalResolution, self); }

Arcadia_Integer32Value
Arcadia_Engine_Visuals_DisplayMode_getVerticalResolution
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_DisplayMode, getVerticalResolution, self); }

Arcadia_Integer32Value
Arcadia_Engine_Visuals_DisplayMode_getColorDepth
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_DisplayMode, getColorDepth, self); }

Arcadia_Integer32Value
Arcadia_Engine_Visuals_DisplayMode_getFrequency
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  )
{ Arcadia_VirtualCallWithReturn(Arcadia_Engine_Visuals_DisplayMode, getFrequency, self); }

void
Arcadia_Engine_Visuals_DisplayMode_apply
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_DisplayMode* self
  )
{ Arcadia_VirtualCall(Arcadia_Engine_Visuals_DisplayMode, apply, self); }
