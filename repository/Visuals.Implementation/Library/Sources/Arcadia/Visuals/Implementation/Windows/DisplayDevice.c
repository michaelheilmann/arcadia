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

#include "Arcadia/Visuals/Implementation/Windows/DisplayDevice.h"

#include "Arcadia/Visuals/Implementation/Windows/DisplayMode.h"

static void
Arcadia_Engine_Visuals_Windows_DisplayDevice_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self
  );

static void
Arcadia_Engine_Visuals_Windows_DisplayDevice_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDeviceDispatch* self
  );

static void
Arcadia_Engine_Visuals_Windows_DisplayDevice_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self
  );

static Arcadia_Engine_Visuals_Windows_DisplayMode*
Arcadia_Engine_Visuals_Windows_DisplayDevice_getCurrentDisplayModeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self
  );

static Arcadia_List*
Arcadia_Engine_Visuals_Windows_DisplayDevice_getAvailableDisplayModesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self
  );

static Arcadia_String*
Arcadia_Engine_Visuals_Windows_DisplayDevice_getIdImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self
  );

static Arcadia_String*
Arcadia_Engine_Visuals_Windows_DisplayDevice_getNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self
  );

static void
Arcadia_Engine_Visuals_Windows_DisplayDevice_getBoundsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top,
    Arcadia_Integer32Value* right,
    Arcadia_Integer32Value* bottom
  );

static const Arcadia_ObjectType_Operations _objectTypeOperations = {
  Arcadia_ObjectType_Operations_Initializer,
  .construct = (Arcadia_Object_ConstructCallbackFunction*)&Arcadia_Engine_Visuals_Windows_DisplayDevice_constructImpl,
  .visit = (Arcadia_Object_VisitCallbackFunction*)&Arcadia_Engine_Visuals_Windows_DisplayDevice_visitImpl,
  .initializeDispatch = (Arcadia_ObjectDispatch_InitializeCallbackFunction*)&Arcadia_Engine_Visuals_Windows_DisplayDevice_initializeDispatchImpl,
};

static const Arcadia_Type_Operations _typeOperations = {
  Arcadia_Type_Operations_Initializer,
  .objectTypeOperations = &_objectTypeOperations,
};

Arcadia_defineObjectType(u8"Arcadia.Visuals.Windows.DisplayDevice", Arcadia_Engine_Visuals_Windows_DisplayDevice,
                         u8"Arcadia.Engine.Visuals.DisplayDevice", Arcadia_Engine_Visuals_DisplayDevice,
                         &_typeOperations);

static void
Arcadia_Engine_Visuals_Windows_DisplayDevice_constructImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self
  )
{
  Arcadia_EnterConstructor(Arcadia_Engine_Visuals_Windows_DisplayDevice);
  {
    Arcadia_ValueStack_pushNatural8Value(thread, 0);
    Arcadia_superTypeConstructor(thread, _type, self);
  }
  if (3 != _numberOfArguments) {
    Arcadia_Thread_setStatus(thread, Arcadia_Status_NumberOfArgumentsInvalid);
    Arcadia_Thread_jump(thread);
  }
  self->id = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 3, _Arcadia_String_getType(thread));
  self->adapter = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 2, _Arcadia_String_getType(thread));
  self->monitor = Arcadia_ValueStack_getObjectReferenceValueChecked(thread, 1, _Arcadia_String_getType(thread));
  self->availableDisplayModes = NULL;
  self->currentDisplayMode = NULL;
  Arcadia_LeaveConstructor(Arcadia_Engine_Visuals_Windows_DisplayDevice);
}

static void
Arcadia_Engine_Visuals_Windows_DisplayDevice_initializeDispatchImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDeviceDispatch* self
  )
{
  ((Arcadia_Engine_Visuals_DisplayDeviceDispatch*)self)->getCurrentDisplayMode = (Arcadia_Engine_Visuals_DisplayMode * (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayDevice*)) & Arcadia_Engine_Visuals_Windows_DisplayDevice_getCurrentDisplayModeImpl;
  ((Arcadia_Engine_Visuals_DisplayDeviceDispatch*)self)->getAvailableDisplayModes = (Arcadia_List * (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayDevice*)) & Arcadia_Engine_Visuals_Windows_DisplayDevice_getAvailableDisplayModesImpl;
  ((Arcadia_Engine_Visuals_DisplayDeviceDispatch*)self)->getId = (Arcadia_String * (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayDevice*)) & Arcadia_Engine_Visuals_Windows_DisplayDevice_getIdImpl;
  ((Arcadia_Engine_Visuals_DisplayDeviceDispatch*)self)->getName = (Arcadia_String * (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayDevice*)) & Arcadia_Engine_Visuals_Windows_DisplayDevice_getNameImpl;
  ((Arcadia_Engine_Visuals_DisplayDeviceDispatch*)self)->getBounds = (void (*)(Arcadia_Thread*, Arcadia_Engine_Visuals_DisplayDevice*, Arcadia_Integer32Value*, Arcadia_Integer32Value*, Arcadia_Integer32Value*, Arcadia_Integer32Value*)) & Arcadia_Engine_Visuals_Windows_DisplayDevice_getBoundsImpl;
}

static void
Arcadia_Engine_Visuals_Windows_DisplayDevice_visitImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self
  )
{
  if (self->id) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->id);
  }
  if (self->adapter) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->adapter);
  }
  if (self->monitor) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->monitor);
  }
  if (self->currentDisplayMode) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->currentDisplayMode);
  }
  if (self->availableDisplayModes) {
    Arcadia_Object_visit(thread, (Arcadia_Object*)self->availableDisplayModes);
  }
}

static Arcadia_Engine_Visuals_Windows_DisplayMode*
Arcadia_Engine_Visuals_Windows_DisplayDevice_getCurrentDisplayModeImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self
  )
{
  if (!self->currentDisplayMode) {
    Arcadia_StringBuilder* deviceModeName = Arcadia_StringBuilder_create(thread);
    Arcadia_StringBuilder_insertBack(thread, deviceModeName, Arcadia_Value_makeObjectReferenceValue(self->id));
    Arcadia_Natural32Value zeroTerminator = 0x0;
    Arcadia_StringBuilder_insertBackCodePoints(thread, deviceModeName, &zeroTerminator, 1);

    DEVMODEA devMode = { 0 };
    if (!EnumDisplaySettingsA(deviceModeName->elements, ENUM_CURRENT_SETTINGS, &devMode)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    if (0 == (devMode.dmFields & DM_PELSWIDTH)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    if (0 == (devMode.dmFields & DM_PELSHEIGHT)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    if (0 == (devMode.dmFields & DM_BITSPERPEL)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    if (0 == (devMode.dmFields & DM_DISPLAYFREQUENCY)) {
      Arcadia_Thread_setStatus(thread, Arcadia_Status_EnvironmentFailed);
      Arcadia_Thread_jump(thread);
    }
    self->currentDisplayMode =
      Arcadia_Engine_Visuals_Windows_DisplayMode_create
        (
          thread,
          self,
          devMode.dmPelsWidth,
          devMode.dmPelsHeight,
          devMode.dmBitsPerPel,
          devMode.dmDisplayFrequency
        );
  }
  return self->currentDisplayMode;
}

static Arcadia_List*
Arcadia_Engine_Visuals_Windows_DisplayDevice_getAvailableDisplayModesImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self
  )
{
  if (!self->availableDisplayModes) {
    Arcadia_List* availableDisplayModes = (Arcadia_List*)Arcadia_ArrayList_create(thread);

    Arcadia_StringBuilder* deviceModeName = Arcadia_StringBuilder_create(thread);
    Arcadia_StringBuilder_insertBack(thread, deviceModeName, Arcadia_Value_makeObjectReferenceValue(self->id));
    Arcadia_Natural32Value zeroTerminator = 0x0;
    Arcadia_StringBuilder_insertBackCodePoints(thread, deviceModeName, &zeroTerminator, 1);

    DWORD iModeNum = 0;
    while (true) {
      DEVMODEA devMode = { 0 };
      if (!EnumDisplaySettingsA(deviceModeName->elements, iModeNum, &devMode)) {
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
      Arcadia_Engine_Visuals_Windows_DisplayMode* displayMode =
        Arcadia_Engine_Visuals_Windows_DisplayMode_create
          (
            thread,
            self,
            devMode.dmPelsWidth,
            devMode.dmPelsHeight,
            devMode.dmBitsPerPel,
            devMode.dmDisplayFrequency
          );
      Arcadia_List_insertBackObjectReferenceValue(thread, availableDisplayModes, (Arcadia_ObjectReferenceValue)displayMode);
      iModeNum++;
      self->availableDisplayModes = availableDisplayModes;
    }
  }
  Arcadia_List * availableDisplayModes = (Arcadia_List*)Arcadia_ArrayList_create(thread);
  for (Arcadia_SizeValue i = 0; i < Arcadia_Collection_getSize(thread, (Arcadia_Collection*)self->availableDisplayModes); ++i) {
    Arcadia_List_insertBack(thread, availableDisplayModes, Arcadia_List_getAt(thread, self->availableDisplayModes, i));
  }
  return availableDisplayModes;
}

static Arcadia_String*
Arcadia_Engine_Visuals_Windows_DisplayDevice_getIdImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self
  )
{ return self->id; }

static Arcadia_String*
Arcadia_Engine_Visuals_Windows_DisplayDevice_getNameImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self
  )
{
  Arcadia_StringBuilder* stringBuffer = Arcadia_StringBuilder_create(thread);
  Arcadia_StringBuilder_insertBack(thread, stringBuffer, Arcadia_Value_makeObjectReferenceValue(self->adapter));
  Arcadia_StringBuilder_insertBackCxxString(thread, stringBuffer, u8"/");
  Arcadia_StringBuilder_insertBack(thread, stringBuffer, Arcadia_Value_makeObjectReferenceValue(self->monitor));
  return Arcadia_String_create(thread, Arcadia_Value_makeObjectReferenceValue(stringBuffer));
}

static void
Arcadia_Engine_Visuals_Windows_DisplayDevice_getBoundsImpl
  (
    Arcadia_Thread* thread,
    Arcadia_Engine_Visuals_Windows_DisplayDevice* self,
    Arcadia_Integer32Value* left,
    Arcadia_Integer32Value* top,
    Arcadia_Integer32Value* right,
    Arcadia_Integer32Value* bottom
  )
{
  *left = self->left;
  *top = self->top;
  *right = self->right;
  *bottom = self->bottom;
}

Arcadia_Engine_Visuals_Windows_DisplayDevice*
Arcadia_Engine_Visuals_Windows_DisplayDevice_create
  (
    Arcadia_Thread* thread,
    Arcadia_String* id,
    Arcadia_String* adapter,
    Arcadia_String* monitor
  )
{
  _Arcadia_BeginCreate(Arcadia_Engine_Visuals_Windows_DisplayDevice);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, id);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, adapter);
  Arcadia_ValueStack_pushObjectReferenceValue(thread, monitor);
  Arcadia_ValueStack_pushNatural8Value(thread, 3);
  _Arcadia_EndCreate(Arcadia_Engine_Visuals_Windows_DisplayDevice);
}

