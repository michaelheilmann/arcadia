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

#include "Arcadia/Visuals/Implementation/Windows/DisplayMode.h"

#include "Arcadia/Engine/Include.h"
#include "Arcadia/Visuals/Implementation/Windows/DisplayDevice.h"

static void
Arcadia_Engine_Visuals_Windows_DisplayMode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  );

static void
Arcadia_Engine_Visuals_Windows_DisplayMode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayModeDispatch* self
  );

static void
Arcadia_Engine_Visuals_Windows_DisplayMode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  );

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_Windows_DisplayMode_getHorizontalResolutionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  );

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_Windows_DisplayMode_getVerticalResolutionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  );

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_Windows_DisplayMode_getColorDepthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  );

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_Windows_DisplayMode_getFrequencyImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  );

static void
Arcadia_Engine_Visuals_Windows_DisplayMode_applyImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Windows_DisplayMode_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Windows_DisplayMode_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Windows_DisplayMode_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Windows.DisplayMode", Arcadia_Engine_Visuals_Windows_DisplayMode,
                         u8"Arcadia.Engine.Visuals.DisplayMode", Arcadia_Engine_Visuals_DisplayMode,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_Windows_DisplayMode_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Windows_DisplayMode);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (5 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->device = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 5, _Arcadia_Engine_Visuals_Windows_DisplayDevice_getType(thread));
  self->horizontalResolution = Arcadia_ValueStack_getInteger32Value(thread, 4);
  self->verticalResolution = Arcadia_ValueStack_getInteger32Value(thread, 3);
  self->colorDepth = Arcadia_ValueStack_getInteger32Value(thread, 2);
  self->frequency = Arcadia_ValueStack_getInteger32Value(thread, 1);
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Windows_DisplayMode);
}

static void
Arcadia_Engine_Visuals_Windows_DisplayMode_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayModeDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_DisplayModeDispatch*)self)->getHorizontalResolution = (Arcadia_Integer32Value(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayMode*)) & Arcadia_Engine_Visuals_Windows_DisplayMode_getHorizontalResolutionImpl;
  ((Arcadia_Engine_Visuals_DisplayModeDispatch*)self)->getVerticalResolution = (Arcadia_Integer32Value(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayMode*)) & Arcadia_Engine_Visuals_Windows_DisplayMode_getVerticalResolutionImpl;
  ((Arcadia_Engine_Visuals_DisplayModeDispatch*)self)->getColorDepth = (Arcadia_Integer32Value(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayMode*)) & Arcadia_Engine_Visuals_Windows_DisplayMode_getColorDepthImpl;
  ((Arcadia_Engine_Visuals_DisplayModeDispatch*)self)->getFrequency = (Arcadia_Integer32Value(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayMode*)) & Arcadia_Engine_Visuals_Windows_DisplayMode_getFrequencyImpl;
  ((Arcadia_Engine_Visuals_DisplayModeDispatch*)self)->apply = (void(*)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayMode*)) & Arcadia_Engine_Visuals_Windows_DisplayMode_applyImpl;
}

static void
Arcadia_Engine_Visuals_Windows_DisplayMode_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  )
{
  if (self->device) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->device);
  }
}

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_Windows_DisplayMode_getHorizontalResolutionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  )
{ return self->horizontalResolution; }

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_Windows_DisplayMode_getVerticalResolutionImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  )
{ return self->verticalResolution; }

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_Windows_DisplayMode_getColorDepthImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  )
{ return self->colorDepth; }

static Arcadia_Integer32Value
Arcadia_Engine_Visuals_Windows_DisplayMode_getFrequencyImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  )
{ return self->frequency; }

static void
Arcadia_Engine_Visuals_Windows_DisplayMode_applyImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayMode* self
  )
{
  Arcadia_StringBuilder* deviceName = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBack(thread, deviceName, Arcadia_Value_makeObjectReferenceValue(self->device->id));
  Arcadia_Natural32Value zeroTerminator = 0x0;
  Arcadia_StringBuilder_insertBackCodePoints(thread, deviceName, &zeroTerminator, 1);
  // Find the corresponding display mode.
  DWORD iModeNum = 0;
  while (true) {
    DEVMODEA devMode = { 0 };
    if (!EnumDisplaySettingsA(deviceName->elements, iModeNum, &devMode)) {
      break;
    }
    iModeNum++;
    if (0 == (devMode.dmFields & DM_PELSWIDTH)) {
      continue;
    }
    if (0 == (devMode.dmFields & DM_PELSHEIGHT)) {
      continue;
    }
    if (0 == (devMode.dmFields & DM_BITSPERPEL)) {
      continue;
    }
    if (0 == (devMode.dmFields & DM_DISPLAYFREQUENCY)) {
      continue;
    }
    if (devMode.dmBitsPerPel == self->colorDepth &&
        devMode.dmPelsWidth == self->horizontalResolution &&
        devMode.dmPelsHeight == self->verticalResolution &&
        devMode.dmDisplayFrequency == self->frequency) {
      // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-changedisplaysettingsexa
      LONG lResult = ChangeDisplaySettingsExA(deviceName->elements, &devMode, NULL, CDS_FULLSCREEN, NULL);
      switch (lResult) {
        case DISP_CHANGE_SUCCESSFUL: {

        } break;
        case DISP_CHANGE_BADDUALVIEW: {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        } break;
        case DISP_CHANGE_BADFLAGS: {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        } break;
        case DISP_CHANGE_BADMODE: {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        } break;
        case DISP_CHANGE_BADPARAM: {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        } break;
        case DISP_CHANGE_FAILED: {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        } break;
        case DISP_CHANGE_NOTUPDATED: {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        } break;
        case DISP_CHANGE_RESTART: {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        } break;
        default: {
          Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
          Arcadia_Thread_jump(thread);
        } break;
      };
    }
  }
}

Arcadia_Engine_Visuals_Windows_DisplayMode*
Arcadia_Engine_Visuals_Windows_DisplayMode_create
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* device,
    Arcadia_Integer32Value horizontalResolution,
    Arcadia_Integer32Value verticalResolution,
    Arcadia_Integer32Value colorDepth,
    Arcadia_Integer32Value frequency
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Windows_DisplayMode);
  if (device) { Arcadia_ValueStack_pushObjectReferenceValue(thread, device); } else { Arcadia_ValueStack_pushVoidValue(thread, Arcadia_VoidValue_Void); }
  Arcadia_ValueStack_pushInteger32Value(thread, horizontalResolution);
  Arcadia_ValueStack_pushInteger32Value(thread, verticalResolution);
  Arcadia_ValueStack_pushInteger32Value(thread, colorDepth);
  Arcadia_ValueStack_pushInteger32Value(thread, frequency);
  Arcadia_ValueStack_pushNatural8Value(thread, 5);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Windows_DisplayMode);
}
